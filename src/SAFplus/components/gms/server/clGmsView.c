/*
 * Copyright (C) 2002-2012 OpenClovis Solutions Inc.  All Rights Reserved.
 *
 * This file is available  under  a  commercial  license  from  the
 * copyright  holder or the GNU General Public License Version 2.0.
 * 
 * The source code for  this program is not published  or otherwise 
 * divested of  its trade secrets, irrespective  of  what  has been 
 * deposited with the U.S. Copyright office.
 * 
 * This program is distributed in the  hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied  warranty  of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 * 
 * For more  information, see  the file  COPYING provided with this
 * material.
 */
/*******************************************************************************
 * ModuleName  : gms                                                           
 * File        : clGmsView.c
 *******************************************************************************/

/*******************************************************************************
 * Description :                                                                
 *
 * 
 *  This file implements the GMS View internal functions. This file mainly contains
 *  View join/close/addition/deletion function calls. It also contains function
 *  calls to get the view changes and view dump to cli. 
 *
 *
 *

 *****************************************************************************/

#include <string.h>
#include <time.h>
#include <clCommonErrors.h>
#include <clGmsErrors.h>
#include <clGmsCli.h>
#include <clOsalApi.h>
#include <clBufferApi.h>
#include <clCntApi.h>
#include <clGmsView.h>
#include <clGmsMsg.h>
#include <clGmsDb.h>
#include <clGms.h>
#include <clCntErrors.h>
#include <clLogApi.h>
#include <clGmsLog.h>
#include <clHash.h>
#include <clNodeCache.h>

#define GMS_VIEW_CACHE_BITS (0x8)
#define GMS_VIEW_CACHE_ENTRIES ( 1 << GMS_VIEW_CACHE_BITS )
#define GMS_VIEW_CACHE_MASK (GMS_VIEW_CACHE_ENTRIES - 1)
#define GMS_VIEW_CACHE_HASH(node) ( (node) & GMS_VIEW_CACHE_MASK )
/*
 * 2 seconds for a leave+join for a node seems acceptable.
 */
#define GMS_VIEW_CACHE_JOIN_LATENCY (2000000L)

static struct hashStruct *gmsViewCache[GMS_VIEW_CACHE_ENTRIES];

typedef struct ClGmsIocViewCache
{
    ClGmsViewNodeT nodeMember;
    ClIocNodeAddressT nodeAddress;
    ClTimeT leaveTime;
    struct hashStruct hash;
} ClGmsIocViewCacheT;

void ClGmsViewNodeTFree(ClGmsViewNodeT* node)
{
    clLogDebug("VIEW","MEM", "Freeing [%p] Node [%d]", (void*) node, node->viewMember.clusterMember.nodeId);
    bzero(node,sizeof(ClGmsViewNodeT));
    clHeapFree(node);
}

ClGmsViewNodeT* ClGmsViewNodeTAlloc(ClGmsNodeIdT nodeId)
{
    ClGmsViewNodeT* ret = (ClGmsViewNodeT*) clHeapCalloc(1,sizeof(ClGmsViewNodeT));    
    clLogDebug("VIEW","MEM", "Alloc [%p] for Node [%d]", (void*) ret, nodeId);
    return ret;
}

/*
 * Stores the last leader cache node
 */
static ClIocNodeAddressT gClLastLeaderViewNode;

static ClGmsIocViewCacheT *gmsViewCacheFind(ClIocNodeAddressT nodeId)
{
    ClUint32T hash = GMS_VIEW_CACHE_HASH(nodeId);
    struct hashStruct *iter;
    for(iter = gmsViewCache[hash]; iter; iter = iter->pNext)
    {
        ClGmsIocViewCacheT *cache = hashEntry(iter, ClGmsIocViewCacheT, hash);
        if(cache->nodeAddress == nodeId)
            return cache;
    }
    return NULL;
}

/*
 * When the node leaves, add it to the view cache.
 */
static __inline__ void gmsViewCacheLastLeaderUpdate(ClIocNodeAddressT nodeId,ClGmsViewNodeT *node)
{
    if(!gClLastLeaderViewNode 
       && 
       node->viewMember.clusterMember.isCurrentLeader == CL_TRUE)
    {
        gClLastLeaderViewNode = nodeId;
        clLogNotice("LAST", "LEADER", "Last leader view cache set to node [%d]", nodeId);
    }
    else if(gClLastLeaderViewNode 
            && 
            nodeId == gClLastLeaderViewNode
            &&
            !node->viewMember.clusterMember.isCurrentLeader)
    {
        gClLastLeaderViewNode = 0;
        clLogNotice("LAST", "LEADER", "Last leader view cache reset from node [%d]", nodeId);
    }
}

static __inline__ void gmsViewCacheLastLeaderReset(ClIocNodeAddressT nodeId)
{
    if(gClLastLeaderViewNode == nodeId)
    {
        gClLastLeaderViewNode = 0;
    }
}

static void gmsViewCacheAdd(ClIocNodeAddressT nodeId, ClGmsViewNodeT *node)
{
    ClGmsIocViewCacheT *cache;
    if( !(cache = gmsViewCacheFind(nodeId) ) )
    {
        cache = (ClGmsIocViewCacheT*) clHeapCalloc(1, sizeof(*cache));
        CL_ASSERT(cache != NULL);
        cache->nodeAddress = nodeId;
        hashAdd(gmsViewCache, GMS_VIEW_CACHE_HASH(nodeId), &cache->hash);
    }
    memcpy(&cache->nodeMember, node, sizeof(cache->nodeMember));
    cache->leaveTime = clOsalStopWatchTimeGet();
    gmsViewCacheLastLeaderUpdate(nodeId, node);
}

/*
 * On a new node join, check if the node is in the view cache left to see determine faster joins    
 * after splits. If no, fetch the footprint from the node cache for the new node.
 * Called with the view lock held.
 */

ClRcT clGmsViewCacheCheckAndAdd(ClGmsNodeIdT currentLeader, ClIocNodeAddressT nodeAddress, ClGmsViewNodeT **pNode)
{
    ClGmsViewNodeT *node = NULL;
    //ClGmsIocViewCacheT *cacheE = NULL;
    ClNodeCacheMemberT member = {0};
    ClRcT rc = CL_OK;

    CL_ASSERT(nodeAddress);  // Slot 0 is invalid...
    CL_ASSERT(pNode);        // programming error, you need to hand me a pointer to fill in
    if(!nodeAddress || !pNode)
    {
        return CL_GMS_RC(CL_ERR_INVALID_PARAMETER);
    }

    /* GMS triggers on the link up event but we must wait for the node hello message to update the node cache (AMF interpretes and processes it) before we
       can run an election.  Its actually ok to just abort here as the election will be handled later but of course it best to handle it now.
     */

    /* Must sync with the node cache every time b/c the leader flag can change */
    if(clNodeCacheMemberGetExtendedSafe(nodeAddress, &member, 10, 200) != CL_OK)
    {
        clLogError("VIEW", "CHECK", "Node [%d] not yet updated in the node cache", nodeAddress);
        return CL_GMS_RC(CL_ERR_NOT_INITIALIZED);
    }
    
    if(!node)
    {
        node = ClGmsViewNodeTAlloc(member.address);
        CL_ASSERT(node !=  NULL);
        node->viewMember.clusterMember.nodeId = member.address;
        node->viewMember.clusterMember.nodeAddress.iocPhyAddress.nodeAddress = member.address;
        saNameSet(&node->viewMember.clusterMember.nodeName, member.name);
        node->viewMember.clusterMember.credential = CL_GMS_INELIGIBLE_CREDENTIALS;

        node->viewMember.clusterMember.isPreferredLeader = CL_FALSE;
        node->viewMember.clusterMember.leaderPreferenceSet = CL_FALSE;
        node->viewMember.clusterMember.bootTimestamp = clOsalStopWatchTimeGet();
    }

    /* Always update the capability from the node cache in case it has changed. */
    if(CL_NODE_CACHE_SC_CAPABILITY(member.capability))
        {
            node->viewMember.clusterMember.credential = member.address + CL_IOC_MAX_NODES + 1;
        }
    else if(CL_NODE_CACHE_SC_PROMOTE_CAPABILITY(member.capability))
        {
            node->viewMember.clusterMember.credential = member.address;
        }    
    node->viewMember.clusterMember.isCurrentLeader = CL_NODE_CACHE_LEADER_CAPABILITY(member.capability);
    
    clLogNotice("VIEW", "CHECK", "Node [%s] slot/address [%d] added/updated to the view cache with capability [%#x] credentials [%d].  Is leader?: %d", member.name, member.address, member.capability, node->viewMember.clusterMember.credential, node->viewMember.clusterMember.isCurrentLeader);
    
    *pNode = node;
    return rc;
}

// check for given name is present or not
ClRcT _clGmsViewClusterGroupFind( CL_IN  const char * name)
{
    const ClRcT rc = (CL_ERR_DOESNT_EXIST);
    int index;

    // search in each groups, for matching group name
    for (index = 0; index < (gmsGlobalInfo.config.noOfGroups) ; index++)
    {
        if ((gmsGlobalInfo.db[index].view.isActive == CL_TRUE) )
        {
                // comapring the group names
                if ( strcmp( (char *) gmsGlobalInfo.db[index].view.name.value, (char *) name) == 0 )
                {
                    clLog (DBG,GEN,NA, "Cluster Group Name %s &  group Id %d!", (char *) gmsGlobalInfo.db[index].view.name.value, gmsGlobalInfo.db[index].view.id);
                        return (CL_ERR_ALREADY_EXIST);
                }
        }
    }

    return rc;
}


/* Find the view Db */

ClRcT    _clGmsViewDbFind(CL_IN   const ClGmsGroupIdT   groupId, CL_IN   ClGmsDbT** const      thisViewDb)
{
    const ClRcT rc = CL_GMS_RC(CL_ERR_DOESNT_EXIST);
    ClUint16T   index = 0x0;

    if (gmsGlobalInfo.db == NULL)
    {
        return CL_ERR_NOT_INITIALIZED;
    }

    if (thisViewDb == NULL)
    {
        return CL_ERR_NULL_POINTER;
    }

    for (index = 0; index < gmsGlobalInfo.config.noOfGroups; index++)
    {
        if ((gmsGlobalInfo.db[index].view.isActive == CL_TRUE) && (gmsGlobalInfo.db[index].view.id == groupId))
        {
            *thisViewDb = &gmsGlobalInfo.db[index];
            return CL_OK;
        }

        *thisViewDb  = NULL;
    }

    return rc;
}

/*
 * Creates a view. Initializes the Cluster/Group Information of
 * the view.
 */

ClRcT	_clGmsViewCreate(
        /* Below comment suppresses the warning from coverity */
        // coverity[pass_by_value]
        CL_IN   SaNameT               name,
        CL_IN   const ClGmsGroupIdT   groupId)
{
    ClRcT   		rc = CL_OK;
    ClGmsDbT		*thisViewDb = NULL;
    ClGmsDbViewTypeT     type = CL_GMS_CLUSTER;
    time_t                t= 0x0;

    /* Intializes all cluster/group/track database */

    if (groupId != CL_GMS_CLUSTER_ID)
    {
        type = CL_GMS_GROUP;
    }

    clGmsMutexLock(gmsGlobalInfo.dbMutex);

    rc = _clGmsDbCreate(gmsGlobalInfo.db,&thisViewDb);

    clGmsMutexUnlock(gmsGlobalInfo.dbMutex);
    if (rc != CL_OK)
    {
        return rc;
    }

    CL_ASSERT(thisViewDb != NULL);

    /* Create and hold the lock */
    clGmsMutexLock(thisViewDb->viewMutex);


    if (time(&t) < 0)
    {
        clLog (ERROR,GEN,NA,
                "time() system call failed while creating view. System returned error [%s]",
                strerror(errno));
    }

    thisViewDb->view.bootTime = (ClTimeT)(t*CL_GMS_NANO_SEC);
    thisViewDb->view.lastModifiedTime = (ClTimeT)(t*CL_GMS_NANO_SEC);
    thisViewDb->view.isActive   = CL_TRUE;
    thisViewDb->viewType        = type;

    /* viewName and groupId are passed only for group */ 
    if (groupId) 
    {
        strncpy((ClCharT *)thisViewDb->view.name.value, (const ClCharT *)name.value,
                name.length);
        thisViewDb->view.name.length = name.length;
        thisViewDb->view.id         = groupId;
    }
    else 
    {
        strncpy((ClCharT *)thisViewDb->view.name.value, CL_GMS_CLUSTER_NAME,
                CL_MAX_NAME_LENGTH-1);
        thisViewDb->view.name.length = strlen(CL_GMS_CLUSTER_NAME);
        thisViewDb->view.id  = CL_GMS_CLUSTER_ID;
    }

    clGmsMutexUnlock(thisViewDb->viewMutex);


    clLog(INFO,GEN,NA,
            "View database for groupID [%d] created",groupId);
    return rc;
}

/* 
 * Destroys the view created by _clGmsViewCreate() 
 */

ClRcT	_clGmsViewDestroy(
        CL_IN  const   ClGmsGroupIdT   groupId)
{
    ClRcT  		rc = CL_OK;
    ClGmsDbT            *thisViewDb = NULL;

    rc  = _clGmsViewDbFind(groupId, &thisViewDb);

    if (rc != CL_OK)
    {
        return rc;
    }

    CL_ASSERT(thisViewDb != NULL);

    clGmsMutexLock(thisViewDb->viewMutex);

    /* Inform every node in the cluster database that this node is leaving
       the cluster */

    rc = _clGmsDbDestroy(thisViewDb);

    clGmsMutexUnlock(thisViewDb->viewMutex);
    /* Above destroy function deletes the mutex. So no need to 
     * unlock it here */
    if(rc != CL_OK)
    {
        clLog(INFO,GEN,NA,
                "View database for groupId [%d] deleted",
                groupId);
    }

    return rc;
}


/* Find a node in a particular view.
 */

ClRcT   _clGmsViewFindNode(
        CL_IN   const ClGmsGroupIdT       groupId,
        CL_IN   const ClGmsNodeIdT        nodeId,
        CL_OUT  ClGmsViewNodeT** const    node)
{
    ClRcT   rc = CL_OK;
    ClGmsDbT    *thisViewDb = NULL;

    rc  = _clGmsViewDbFind(groupId, &thisViewDb);

    if (rc != CL_OK) 
    {
        return rc;
    }
    CL_ASSERT(thisViewDb != NULL);

    clGmsMutexLock(thisViewDb->viewMutex);



    rc = _clGmsViewFindNodePrivate(thisViewDb, nodeId, CL_GMS_CURRENT_VIEW, 
            node);

    clGmsMutexUnlock(thisViewDb->viewMutex);

    return rc;
}

/* This function is used internally within view. 
 */

ClRcT   _clGmsViewFindNodePrivate(
        CL_IN   const ClGmsDbT*  const  thisViewDb,
        CL_IN   const ClGmsNodeIdT      nodeId,
        CL_IN   const ClGmsDbTypeT      type,
        CL_OUT  ClGmsViewNodeT** const  node)
{
    ClRcT       rc = CL_OK;
    ClGmsDbKeyT dbKey = {{0}};

    dbKey.nodeId = nodeId;

    rc = _clGmsDbFind(thisViewDb, type, dbKey, (void **)node);

    return rc;
}

/* Get the view associated with the groupId */

ClRcT	_clGmsViewFind(
        CL_IN 	const ClGmsGroupIdT     groupId, 
        CL_OUT 	ClGmsViewT** const      thisView)
{
    ClRcT       rc = CL_GMS_RC(CL_ERR_DOESNT_EXIST);
    ClGmsDbT    *thisViewDb= NULL;

    if (thisView == NULL)
    {
        return CL_ERR_NULL_POINTER;
    }

    rc = _clGmsViewDbFind(groupId, &thisViewDb);

    if (rc != CL_OK)
    {
        return rc;
    }
    CL_ASSERT(thisViewDb != NULL);
    *thisView = &thisViewDb->view;

    return rc;

}

/* Get the view associated with the groupId  and
 * lock it */

ClRcT	_clGmsViewFindAndLock(
        CL_IN 	const ClGmsGroupIdT     groupId, 
        CL_OUT 	ClGmsViewT** const      thisView)
{
    ClRcT       rc = CL_GMS_RC(CL_ERR_DOESNT_EXIST);
    ClGmsDbT    *thisViewDb= NULL;


    if (thisView == NULL)
    {
        return CL_ERR_NULL_POINTER;
    }

    *thisView = NULL;

    rc = _clGmsViewDbFind(groupId, &thisViewDb);

    if (rc != CL_OK)
    {
        return rc;
    }
    CL_ASSERT(thisViewDb != NULL);

    clGmsMutexLock(thisViewDb->viewMutex);


    *thisView = &thisViewDb->view;

    return rc;

}

/* Unlock the given group */

ClRcT   _clGmsViewUnlock(
        CL_IN   const ClGmsGroupIdT   groupId)
{
    ClRcT   rc = CL_OK;
    ClGmsDbT    *thisViewDb = NULL;

    rc = _clGmsViewDbFind(groupId, &thisViewDb);
    if (rc != CL_OK)
    {
        return rc;
    }
    CL_ASSERT(thisViewDb != NULL);

    clGmsMutexUnlock(thisViewDb->viewMutex);

    return rc;
}

/* Used to get notification for changeOnly tracflag.
 * This fn. does not lock the database. The calling
 * function should lock it.
 */

static ClRcT   _clGmsViewGetChangeOnlyViewNotification(
        CL_IN   const ClGmsDbT* const thisViewDb,
        CL_OUT  void**  const         noti,
        CL_OUT  ClUint32T* const      noOfItems)
{
    ClRcT                           rc = CL_OK;
    ClCntNodeHandleT                *opaque = NULL;
    ClGmsViewNodeT                  *node = NULL;
    ClGmsGroupNotificationT         *groupBuf=NULL;
    ClGmsClusterNotificationT       *clusterBuf=NULL;
    ClUint32T                       index=0;

    if ((thisViewDb == (const void*)NULL) || (noti == NULL) || (noOfItems == NULL))
    {
        return CL_ERR_NULL_POINTER;
    }

    rc = _clGmsDbGetFirst(thisViewDb, CL_GMS_JOIN_LEFT_VIEW, &opaque, (void **)
            &node);
    if (rc != CL_OK)
    {
        return rc;
    }

    while((rc == CL_OK) && (node != NULL)) 
    {

        /* Do a realloc here, as we dont know how many entries have
         * changed since last track.So we increment the size
         * one buffer at a time. */

        if (thisViewDb->viewType == CL_GMS_CLUSTER)
        {
            clusterBuf = (ClGmsClusterNotificationT *)realloc(
                    clusterBuf,
                    sizeof(ClGmsClusterNotificationT)*
                    (index+1)); 
            if (clusterBuf == NULL)
            {
                return CL_GMS_RC(CL_ERR_NO_MEMORY);
            }
        }
        else
        {
            groupBuf = (ClGmsGroupNotificationT *)realloc(
                    groupBuf,
                    sizeof(ClGmsGroupNotificationT)*
                    (index+1)); 

            if (groupBuf == NULL)
            {
                return CL_GMS_RC(CL_ERR_NO_MEMORY);
            }
        }


        if (thisViewDb->viewType == CL_GMS_CLUSTER)
        {
            clusterBuf[index].clusterChange  = (ClGmsClusterChangesT) node->trackFlags;
            clusterBuf[index++].clusterNode = node->viewMember.clusterMember;
        }
        else
        {
            groupBuf[index].groupChange  = (ClGmsGroupChangesT) node->trackFlags;
            groupBuf[index++].groupMember = node->viewMember.groupMember;
        }

        rc = _clGmsDbGetNext(thisViewDb, CL_GMS_JOIN_LEFT_VIEW, &opaque, 
                (void **)&node);
    }

    rc = _clGmsDbDeleteAll(thisViewDb, CL_GMS_JOIN_LEFT_VIEW);

    /* Copy & return */

    if (thisViewDb->viewType == CL_GMS_CLUSTER)
        *noti = (void *)clusterBuf;
    else
        *noti = (void *)groupBuf;

    *noOfItems = index;

    return rc; 
}



/* Gets the view list for the trackflag change/current.
 * This function does not lock the database. The calling
 * function should lock it.
 */

ClRcT
_clGmsViewGetCurrentViewNotification(
        CL_IN   const ClGmsDbT* const thisViewDb,
        CL_OUT  void**          const noti,
        CL_OUT  ClUint32T*      const noOfItems)
{
    ClRcT                           rc = CL_OK;
    ClCntNodeHandleT                *opaque = NULL;
    ClGmsViewNodeT                  *node = NULL;
    ClGmsClusterNotificationT       *clusterViewBuf=NULL;
    ClGmsGroupNotificationT         *groupViewBuf=NULL;
    ClUint32T                       index=0;

    if ((thisViewDb == (const void*)NULL) || (noti == NULL) || (noOfItems == NULL))
    {
        return CL_ERR_NULL_POINTER;
    }

    if (thisViewDb->view.noOfViewMembers == 0)
    {
        *noti = NULL;
        *noOfItems = 0;
        return CL_OK;
    }

    if (thisViewDb->viewType == CL_GMS_CLUSTER)
    {
        clusterViewBuf = (ClGmsClusterNotificationT *)clHeapAllocate(
                sizeof(ClGmsClusterNotificationT)*
                thisViewDb->view.noOfViewMembers);

        if (clusterViewBuf == NULL)
        {
            return CL_GMS_RC(CL_ERR_NO_MEMORY); 
        }

        memset(clusterViewBuf, 0, sizeof(*clusterViewBuf));
    }
    else 
    {
        groupViewBuf = (ClGmsGroupNotificationT *)clHeapAllocate(
                sizeof(ClGmsGroupNotificationT)*
                thisViewDb->view.noOfViewMembers);

        if (groupViewBuf == NULL)
        {
            return CL_GMS_RC(CL_ERR_NO_MEMORY);
        }

        memset(groupViewBuf, 0, sizeof(*groupViewBuf));
    }

    /* Do a loop of the entire view list and populate the notification
     * buffer for the trackFlags change and current */

    /* Get first node */

    rc = _clGmsDbGetFirst(thisViewDb, CL_GMS_CURRENT_VIEW, &opaque, (void **) &node);

    if (rc != CL_OK)
    {
        return rc;
    }

    while((rc == CL_OK) && (node != NULL))
    {
        /* If a node has just left then don't include that node
         * in the current list */
        if (index == thisViewDb->view.noOfViewMembers)
        {
            clLogMultiline(ERROR,GEN,NA,
                    "Actual number of view elements exceeds the value of "
                     "thisViewDb->view.noOfViewMembers = %d\n",
                     thisViewDb->view.noOfViewMembers);
            break;
        }

        node->trackFlags = CL_GMS_MEMBER_NO_CHANGE;

        if (thisViewDb->viewType == CL_GMS_CLUSTER)
        {
            clusterViewBuf[index].clusterChange  = (ClGmsClusterChangesT) node->trackFlags;
            clusterViewBuf[index++].clusterNode = node->viewMember.clusterMember;
        }
        else
        {
            groupViewBuf[index].groupChange  = (ClGmsGroupChangesT) node->trackFlags;
            groupViewBuf[index++].groupMember = node->viewMember.groupMember;
        }

        /* Get next node */
        rc = _clGmsDbGetNext(thisViewDb, CL_GMS_CURRENT_VIEW, &opaque, (void **)&node);
    }

    if (thisViewDb->viewType == CL_GMS_CLUSTER)
        *noti = (void *)clusterViewBuf;
    else
        *noti = (void *)groupViewBuf;

    *noOfItems = index;

    return rc;
}

static ClRcT
fill_notification_buffer_from_viewdb(
        const ClGmsDbT* const thisViewDb, 
        void*     const res)
{
    ClRcT       rc = CL_OK;
    void       *buf = NULL;
    ClUint32T   noOfItems=0;

    if ((thisViewDb == NULL) || (res == NULL))
    {
        return CL_ERR_NULL_POINTER;
    }

    rc = _clGmsViewGetCurrentViewNotification(thisViewDb, &buf, &noOfItems);

    if (rc != CL_OK)
    {
        return rc;
    }

    /* Populate the response structure. */

    if (thisViewDb->viewType == CL_GMS_CLUSTER)
    {
        ((ClGmsClusterNotificationBufferT*)res)->notification=
            (ClGmsClusterNotificationT*) buf;

        ((ClGmsClusterNotificationBufferT*)res)->viewNumber = 
            thisViewDb->view.viewNumber; 

        ((ClGmsClusterNotificationBufferT*)res)->numberOfItems = noOfItems;

        /* Cluster Leader */
        ((ClGmsClusterNotificationBufferT*)res)->leader =
            thisViewDb->view.leader; 

        /* Cluster Deputy */
        ((ClGmsClusterNotificationBufferT*)res)->deputy =
            thisViewDb->view.deputy;

        ((ClGmsClusterNotificationBufferT*)res)->leadershipChanged = 
            thisViewDb->view.leadershipChanged;

    }
    else
    {
        ((ClGmsGroupNotificationBufferT*)res)->notification = 
            (ClGmsGroupNotificationT*) buf;

        ((ClGmsGroupNotificationBufferT*)res)->viewNumber   =
            thisViewDb->view.viewNumber;

        ((ClGmsGroupNotificationBufferT*)res)->numberOfItems = noOfItems;

        /* Group Leader */
        ((ClGmsGroupNotificationBufferT*)res)->leader =
            thisViewDb->view.leader;

        /* Group Deputy */
        ((ClGmsGroupNotificationBufferT*)res)->deputy =
            thisViewDb->view.deputy;

        ((ClGmsGroupNotificationBufferT*)res)->leadershipChanged = 
            thisViewDb->view.leadershipChanged;
    }

    return rc;
}

/*
 *  Gets the changed and changed only entries from the view.
 */
ClRcT   _clGmsViewGetCurrentView(CL_IN   const ClGmsGroupIdT    groupId, CL_OUT  void* const            res)
{
    ClRcT       rc = CL_OK ;
    ClGmsDbT    *thisViewDb = NULL;

    rc = _clGmsViewDbFind(groupId, &thisViewDb);

    if (rc != CL_OK)
    {
        return rc;
    }

    CL_ASSERT(thisViewDb != NULL);

    clGmsMutexLock(thisViewDb->viewMutex);

    rc = fill_notification_buffer_from_viewdb(thisViewDb, res);

    clGmsMutexUnlock(thisViewDb->viewMutex);

    return rc;
}

/*
 * Same as above function, but without locking (it assumes that the view is
 * already locked.
 */
ClRcT   _clGmsViewGetCurrentViewLocked(CL_IN   const ClGmsGroupIdT    groupId, CL_OUT  void* const            res)
{
    ClRcT       rc = CL_OK;
    ClGmsDbT    *thisViewDb = NULL;

    rc = _clGmsViewDbFind(groupId, &thisViewDb);

    if (rc != CL_OK)
    {
        return rc;
    }

    rc = fill_notification_buffer_from_viewdb(thisViewDb, res);

    return rc;
}

/*
 * Gets the changed and changed only entries from the view.
 * Used by the trackNotify function in clGmsTrack.c file.
 */

ClRcT   _clGmsViewGetTrackAsync(
        CL_IN   const ClGmsDbT* const   thisViewDb,
        CL_OUT  void*     const   changeList,
        CL_OUT  void*     const   changeOnlyList)
{
    ClRcT       rc = CL_OK;
    ClUint32T   noOfItems = 0;
    void        *buf = NULL;
    ClUint32T   index = 0;

    if ((thisViewDb == NULL) || (changeList == NULL) || (changeOnlyList == NULL))
    {
        return CL_ERR_NULL_POINTER;
    }

    rc = _clGmsViewGetChangeOnlyViewNotification(thisViewDb, &buf, &noOfItems);

    if (rc != CL_OK)
    {
		if (buf != NULL)
		{
			clHeapFree(buf);
		}
        return rc;
    }

    ((ClGmsTrackNotifyT*)changeOnlyList)->buffer = buf;
    ((ClGmsTrackNotifyT*)changeOnlyList)->entries = noOfItems;

    rc = _clGmsViewGetCurrentViewNotification(thisViewDb, &buf, &noOfItems);
    if (rc != CL_OK)
    {
        if (buf != NULL)
        {
            clHeapFree(buf);
        }

        return rc;
    }

    ((ClGmsTrackNotifyT*)changeList)->entries = noOfItems;
    ((ClGmsTrackNotifyT*)changeList)->buffer = buf;

    for (index = 0; index < ((ClGmsTrackNotifyT*)changeOnlyList)->entries; index++)
    {
        if (thisViewDb->viewType == CL_GMS_CLUSTER)
        {
            ClGmsClusterNotificationT   *notification = (ClGmsClusterNotificationT*)((ClGmsTrackNotifyT*)changeList)->buffer;
            ClUint64T                   numEntries = ((ClGmsTrackNotifyT*)changeList)->entries;
            ClGmsClusterNotificationT   *changeOnlyNotif = (ClGmsClusterNotificationT*)((ClGmsTrackNotifyT*)changeOnlyList)->buffer;
            ClUint32T                   nodeSize = sizeof(ClGmsClusterNotificationT);

            if (changeOnlyNotif[index].clusterChange == (ClInt32T)CL_GMS_MEMBER_LEFT)
            {
                notification = (ClGmsClusterNotificationT*) clHeapRealloc(notification, (nodeSize * (numEntries+1)));
                if (notification == NULL)
                {
                    return CL_ERR_NO_MEMORY;
                }
                notification[numEntries].clusterChange = changeOnlyNotif[index].clusterChange;
                notification[numEntries].clusterNode = changeOnlyNotif[index].clusterNode;

                ((ClGmsTrackNotifyT*)changeList)->buffer = notification;
                ((ClGmsTrackNotifyT*)changeList)->entries++;
            }
        } else {
            ClGmsGroupNotificationT   *notification = (ClGmsGroupNotificationT*)((ClGmsTrackNotifyT*)changeList)->buffer;
            ClUint64T                   numEntries = ((ClGmsTrackNotifyT*)changeList)->entries;
            ClGmsGroupNotificationT   *changeOnlyNotif = (ClGmsGroupNotificationT*)((ClGmsTrackNotifyT*)changeOnlyList)->buffer;
            ClUint32T                   nodeSize = sizeof(ClGmsGroupNotificationT);

            if (changeOnlyNotif[index].groupChange == CL_GMS_MEMBER_LEFT)
            {
                notification = (ClGmsGroupNotificationT*) clHeapRealloc(notification, (nodeSize * (numEntries+1)));
                if (notification == NULL)
                {
                    return CL_ERR_NO_MEMORY;
                }
                notification[numEntries].groupChange = changeOnlyNotif[index].groupChange;
                notification[numEntries].groupMember = changeOnlyNotif[index].groupMember;

                ((ClGmsTrackNotifyT*)changeList)->buffer = notification;
                ((ClGmsTrackNotifyT*)changeList)->entries++;
            }
        }
    }
    return rc;
}


/* Sending all the nodes in the given view to the CLI. 
 * This function is called from clGmsCli.c. BufferMessageHandle
 * is used to just make the string.
 */
ClRcT   _clGmsViewCliPrint(
        CL_IN   const ClGmsGroupIdT    groupId, 
        CL_OUT  ClCharT**  const       ret) 
{
    ClRcT                     rc = CL_OK;
    ClGmsViewNodeT            *node = NULL;
    ClBufferHandleT    msg;
    ClCntNodeHandleT         *hdl = NULL;
    ClCharT                   str[CL_GMS_PRINT_STR_LENGTH] = {0};
    ClUint32T                 len = 0;
    ClGmsDbT                  *thisViewDb = NULL;
    ClTimeT                   ti = 0;
    char                      timeBuffer[256] ={0};


    if (ret == NULL)
    {
        return CL_ERR_NULL_POINTER;
    }

    rc = clDebugPrintInitialize(&msg);

    if (rc != CL_OK)
    {
        return rc;
    }

    rc = _clGmsViewDbFind(groupId, &thisViewDb);

    if (rc != CL_OK)
    {
        return rc;
    }

    CL_ASSERT(thisViewDb != NULL);

    clGmsMutexLock(thisViewDb->viewMutex);

    memset(str, 0, sizeof(str));

    rc = clDebugPrint(msg,
            "--------------------------------------------------------------------------------------------------------\n");

    if (rc != CL_OK)
    {
        goto ERROR_EXIT;
    }

    ti = thisViewDb->view.bootTime/CL_GMS_NANO_SEC;


    rc = clDebugPrint(msg,"\n%s Name : %s\nbootTime           : %s"
                          "View Number        : %llu\n", thisViewDb->viewType == CL_GMS_CLUSTER?"Cluster":"Group",
            thisViewDb->view.name.value, ctime_r((const time_t*)&ti,timeBuffer),
            thisViewDb->view.viewNumber);
    if (rc != CL_OK)
    {
        goto ERROR_EXIT;
    }

    rc = clDebugPrint(msg, "%s",
            "--------------------------------------------------------------------------------------------------------\n");
    
    if (rc != CL_OK)
    {
        goto ERROR_EXIT;
    }

    if (thisViewDb->viewType == CL_GMS_CLUSTER)
    {
        rc = clDebugPrint(msg, "NodeId NodeName        HostAddr Port Leader     Credentials PrefLead LeadshipSet BootTime\n"
                "--------------------------------------------------------------------------------------------------------\n");
    }
    else
    {
        rc = clDebugPrint(msg, "NodeId NodeName        HostAddr Port Credentials BootTime\n"
                "--------------------------------------------------------------------------------------------------------\n");
    }
   
    if (rc != CL_OK)
    {
        goto ERROR_EXIT;
    }

    len = 0;

    rc = _clGmsDbGetFirst(thisViewDb, CL_GMS_CURRENT_VIEW, &hdl, (void **)&node);

    while ((rc == CL_OK) && (node != NULL))
    {
        if (thisViewDb->viewType == CL_GMS_CLUSTER) 
        {
            ti = node->viewMember.clusterMember.bootTimestamp/CL_GMS_NANO_SEC;

            rc = clDebugPrint(msg, "%-6d %-15s %-8d %-4d %-10s %-11d %-8s %-11s %s", 
                    node->viewMember.clusterMember.nodeId, 
                    node->viewMember.clusterMember.nodeName.value, 
                    node->viewMember.clusterMember.nodeAddress.
                    iocPhyAddress.nodeAddress,
                    node->viewMember.clusterMember.nodeAddress.iocPhyAddress.portId,
                    (node->viewMember.clusterMember.isCurrentLeader == CL_TRUE) ? "Yes":
                    CL_NODE_CACHE_SC_PROMOTE_CAPABILITY(node->viewMember.clusterMember.isCurrentLeader) ? 
                    "(Pending)": "No",
                    node->viewMember.clusterMember.credential,
                    node->viewMember.clusterMember.isPreferredLeader ? "Yes":"No",
                    node->viewMember.clusterMember.leaderPreferenceSet ? "Yes":"No",
                    ctime_r((const time_t*)&ti,timeBuffer));
        }
        else
        {
            ti = node->viewMember.groupMember.joinTimestamp/CL_GMS_NANO_SEC;

            rc = clDebugPrint(msg, "%6d  %-15s  %-8d  %-4d  %-4d  %s", 
                    node->viewMember.groupMember.memberId, 
                    node->viewMember.groupMember.memberName.value, 
                    node->viewMember.groupMember.memberAddress.iocPhyAddress.nodeAddress, 
                    node->viewMember.groupMember.memberAddress.iocPhyAddress.portId,
                    node->viewMember.groupMember.credential,
                    ctime_r((const time_t*)&ti,timeBuffer));
        }

        if (rc != CL_OK)
        {
            goto ERROR_EXIT;
        }

        rc = _clGmsDbGetNext(thisViewDb, CL_GMS_CURRENT_VIEW, &hdl, (void **)&node);
    }
    rc = clBufferLengthGet(msg, &len);

    if (rc != CL_OK)
    {
        goto ERROR_EXIT;
    }

    *ret = (ClCharT *) clHeapAllocate(len);

    if (*ret == NULL)
    {
        rc = CL_GMS_RC(CL_ERR_NO_MEMORY);
        goto ERROR_EXIT;
    }

    rc = clDebugPrintFinalize( &msg,ret);
    if (rc != CL_OK)
    {
        goto ERROR_EXIT;
    }

ERROR_EXIT:
    clGmsMutexUnlock(thisViewDb->viewMutex);

    return rc;
}

/* Adds a node to the view of 'type'. */

static ClRcT   
_clGmsViewAddNodePrivate(
        CL_IN   ClGmsDbT*  const       thisViewDb,
        CL_IN   const ClGmsNodeIdT     nodeId,
        CL_IN   const ClGmsDbTypeT     type,
        CL_IN   ClGmsViewNodeT*  const node)
{
    ClRcT               rc = CL_OK;
    ClGmsDbKeyT         dbKey = {{0}};
    ClGmsViewNodeT      *joinNode =NULL;
    ClGmsViewNodeT      *foundNode =NULL;

    if ((node == NULL) || (thisViewDb == NULL))
    {
        return CL_ERR_NULL_POINTER;
    }

    if (node->trackFlags != CL_GMS_MEMBER_LEFT)
    {
        if (thisViewDb->viewType == CL_GMS_CLUSTER)
        {
            node->viewMember.clusterMember.memberActive = CL_TRUE;
        }
        else
        {
            node->viewMember.groupMember.memberActive = CL_TRUE;
        }
    }

    /* Add the node to the database  */

    dbKey.nodeId = nodeId;

    rc = _clGmsDbAdd(thisViewDb, type, dbKey, (void *)node);

    if (rc != CL_OK) 
    {
        return rc;
    }

    if (type == CL_GMS_CURRENT_VIEW)
    {
        /* Incr the no. of active members */
        thisViewDb->view.noOfViewMembers++;

        /* View number should be incremented everytime there is
         * a change to a node in the cluster/group */

        thisViewDb->view.viewNumber++;
    }

    /* Do only if called from AddNode  */

    if (node->trackFlags == CL_GMS_MEMBER_JOINED)
    {
        joinNode = ClGmsViewNodeTAlloc(node->viewMember.clusterMember.nodeId);

        if (joinNode == NULL)
        {
            return CL_GMS_RC(CL_ERR_NO_MEMORY);
        }

        memcpy((void*)joinNode, (void*)node, sizeof(ClGmsViewNodeT));
        rc =  _clGmsDbAdd(thisViewDb, CL_GMS_JOIN_LEFT_VIEW, dbKey, joinNode);
        if ((CL_GET_ERROR_CODE(rc) == CL_ERR_ALREADY_EXIST) ||
                (CL_GET_ERROR_CODE(rc) == CL_ERR_DUPLICATE))
        {
            /* Node does not exist in CURRENT view but exists in JOIN_LEFT.
             *  Hence it would have left. So we dont notify both the
             *  events. and Hence we will delete the node from the JOIN_LEFT
             *  View.
             */
            clLogMultiline(ERROR,GEN,NA,
                    "NodeID:%d already exists in CL_GMS_JOIN_LEFT_VIEW, with "
                     "track flag : %d\n", nodeId,joinNode->trackFlags);
            rc = _clGmsViewFindNodePrivate(thisViewDb, nodeId,
                    CL_GMS_JOIN_LEFT_VIEW, &foundNode);
            if (rc != CL_OK)
            {
                clLogMultiline(ERROR,GEN,NA,
                        "Finding node Id = %d in CL_GMS_JOIN_LEFT_VIEW "
                         "failed. RC = 0x%x\n",nodeId, rc);
                return rc;
            }
            CL_ASSERT(foundNode != NULL);

            if (foundNode->trackFlags == CL_GMS_MEMBER_LEFT)
            {
                dbKey.nodeId = nodeId;
                rc = _clGmsDbDelete(thisViewDb, CL_GMS_JOIN_LEFT_VIEW, dbKey);
                ClGmsViewNodeTFree(joinNode);
            }
        }
    }

    return rc;
}

static ClRcT
_clGmsViewNodeCompare(
        CL_IN   const ClGmsDbT*       const  thisViewDb,
        CL_IN   const ClGmsViewNodeT* const  node1,
        CL_IN   const ClGmsViewNodeT* const  node2,
        CL_OUT  ClGmsViewCompareT*    const  result)
{
    ClRcT   rc = CL_OK;

    if ((node1 == (const void*)NULL) || (node2 == (const void*)NULL) || 
            (result == NULL) || (thisViewDb == (const void*)NULL))
    {
        return CL_ERR_NULL_POINTER;
    }

    if (thisViewDb->viewType == CL_GMS_CLUSTER)
    {
        if (strncmp((const ClCharT *)node1->viewMember.clusterMember.nodeName.value,
                        (const ClCharT *)node2->viewMember.clusterMember.nodeName.value,
                    node1->viewMember.clusterMember.nodeName.length))
        {
            *result = CL_GMS_VIEW_NOT_EQUAL;
            return rc;
        }

        if (node1->viewMember.clusterMember.credential != 
                node2->viewMember.clusterMember.credential)
        {
            *result = CL_GMS_VIEW_NOT_EQUAL;
            return rc;
        }

        if (node1->viewMember.clusterMember.nodeAddress.iocPhyAddress.nodeAddress != 
                node2->viewMember.clusterMember.nodeAddress.iocPhyAddress.nodeAddress)
        {
            *result = CL_GMS_VIEW_NOT_EQUAL;
            return rc;
        }
    }
    else
    {
        if (strncmp((const ClCharT *)node1->viewMember.groupMember.memberName.value,
                        (const ClCharT *)node2->viewMember.groupMember.memberName.value,
                    node1->viewMember.groupMember.memberName.length))
        {
            *result = CL_GMS_VIEW_NOT_EQUAL;
            return rc;
        }

        if (node1->viewMember.groupMember.credential != 
                node2->viewMember.groupMember.credential)
        {
            *result = CL_GMS_VIEW_NOT_EQUAL;
            return rc;
        }

        if (node1->viewMember.groupMember.memberAddress.iocPhyAddress.nodeAddress != 
                node2->viewMember.groupMember.memberAddress.iocPhyAddress.nodeAddress)
        {
            *result = CL_GMS_VIEW_NOT_EQUAL;
            return rc;
        }
    }

    *result = CL_GMS_VIEW_EQUAL;

    return rc;
}


/* Adds a node to the view of 'type'. */

static ClRcT
_clGmsViewUpdateNodePrivate(
        CL_IN   const ClGmsDbT*        const thisViewDb,
        CL_IN   const ClGmsNodeIdT     nodeId,
        CL_IN   ClGmsViewNodeT*  const node,
        CL_IN   ClGmsViewNodeT*  const foundNode)
{
    ClRcT            rc = CL_OK;
    ClGmsDbKeyT      dbKey = {{0}};
    ClGmsViewNodeT   *joinNode =NULL;
    ClGmsViewCompareT result = CL_GMS_VIEW_EQUAL;
    ClGmsViewNodeT *joinLeftViewNode=NULL;

    if ((thisViewDb == (const void*)NULL) || (node == NULL) || (foundNode == NULL))
    {
        return CL_ERR_NULL_POINTER;
    }

    rc = _clGmsViewNodeCompare(thisViewDb, foundNode, node, &result);    
    if (rc != CL_OK)
    {
        return rc;
    }

    if (result == CL_GMS_VIEW_EQUAL)
    {
        return CL_GMS_RC(CL_ERR_ALREADY_EXIST);
    }

    rc = _clGmsViewFindNodePrivate(thisViewDb, nodeId, CL_GMS_JOIN_LEFT_VIEW, &joinLeftViewNode);
    if (rc == CL_OK)
    {
        /*Node has been found in JOIN_LEFT_VIEW so update the same. No need to
         * create a new one
         */
        CL_ASSERT(joinLeftViewNode != NULL);
        *joinLeftViewNode = *node;
        joinLeftViewNode->trackFlags = CL_GMS_MEMBER_RECONFIGURED;
    } 
    else if (CL_GET_ERROR_CODE(rc) == CL_ERR_NOT_EXIST)
    {
        joinNode = ClGmsViewNodeTAlloc(node->viewMember.clusterMember.nodeId);

        if (joinNode == NULL)
        {
            return CL_GMS_RC(CL_ERR_NO_MEMORY);
        }

        memcpy((void*)joinNode, (void*)node, sizeof(ClGmsViewNodeT));

        joinNode->trackFlags = CL_GMS_MEMBER_RECONFIGURED;

        dbKey.nodeId = nodeId;

        rc =  _clGmsDbAdd(thisViewDb, CL_GMS_JOIN_LEFT_VIEW, dbKey, joinNode);
        if (rc != CL_OK)
        {
            clLogMultiline(ERROR,GEN,NA, "Adding an entry into CL_GMS_JOIN_LEFT_VIEW when node is reconfigured, failed with rc = 0x%x\n",rc);
        }
    }
    else
    {
        clLogMultiline(ERROR,GEN,NA, "Failed to find the node Id %d in JOIN_LEFT_VIEW. RC = 0x%x\n",nodeId, rc);
    }

    *foundNode = *node;

    foundNode->trackFlags = CL_GMS_MEMBER_RECONFIGURED;

    return rc;
}


/* Add a node to the given view.  I take the *node memory
 * This function does not lock the view. The caller
 * is supposed to lock the view first before calling this
 * function.
 */

ClRcT   _clGmsViewAddNodeExtended(
        CL_IN   const ClGmsGroupIdT    groupId,
        CL_IN   const ClGmsNodeIdT     nodeId,
        CL_IN   ClBoolT          reElection,
        CL_IN   ClGmsViewNodeT**       ppNode)
{
    ClRcT               rc = CL_OK;
    ClGmsDbT            *thisViewDb = NULL;
    ClGmsViewNodeT      *foundNode = NULL;
    ClGmsViewNodeT      *node = NULL;

    if (!ppNode) 
    {
        return CL_ERR_NULL_POINTER;
    }

    rc = _clGmsViewDbFind(groupId, &thisViewDb);

    if (rc != CL_OK)
    {
        return rc;
    }

    CL_ASSERT(thisViewDb != NULL);


    node = *ppNode;

    if (!node)
    {
        clLogCritical("VIEW", "CACHE", "node not passed (group %d, nodeId %d)", groupId,nodeId);
        CL_ASSERT(node != 0);
    }
    
    rc = _clGmsViewFindNodePrivate(thisViewDb, nodeId, CL_GMS_CURRENT_VIEW, &foundNode);

    switch (rc)
    {
        case CL_CNT_RC(CL_ERR_NOT_EXIST):
            node->trackFlags = CL_GMS_MEMBER_JOINED;
            rc = _clGmsViewAddNodePrivate(thisViewDb, nodeId, CL_GMS_CURRENT_VIEW, node);
            if (rc != CL_OK)
            {
                ClGmsViewNodeTFree(node);
                return rc;
            }
            break;
        case CL_OK:
            /*  
             * If not a reelection, then clear the current leader states
             * since it could be a join trigger from amf up notification indicating
             * that its not a temporary split
             */
            if(!reElection)
            {
                foundNode->viewMember.clusterMember.isCurrentLeader = node->viewMember.clusterMember.isCurrentLeader;
            }
            if(!foundNode->viewMember.clusterMember.isCurrentLeader
               &&
               gClLastLeaderViewNode == nodeId)
            {
                clLogNotice("LAST", "LEADER", "Resetting last leader view cache for node [%d]", nodeId);
                gmsViewCacheLastLeaderReset(nodeId);
            }
            if(!foundNode->viewMember.clusterMember.bootTimestamp)
            {
                foundNode->viewMember.clusterMember.bootTimestamp = node->viewMember.clusterMember.bootTimestamp;
            }
            rc = _clGmsViewUpdateNodePrivate(thisViewDb, nodeId, node, foundNode);
            ClGmsViewNodeTFree(node);
            *ppNode = NULL;
            break;
        default:
            break; 
    }

    return rc;
}

ClRcT   _clGmsViewAddNode(
        CL_IN   const ClGmsGroupIdT    groupId,
        CL_IN   const ClGmsNodeIdT     nodeId,
        CL_IN   ClGmsViewNodeT* node)
{
    return _clGmsViewAddNodeExtended(groupId, nodeId, CL_FALSE, &node);
}

/* Deletes a node from the given view.
 */

ClRcT   _clGmsViewDeleteNodeExtended(
        CL_IN   const ClGmsGroupIdT       groupId,
        CL_IN   const ClGmsNodeIdT        nodeId,
        CL_IN   ClBoolT viewCache)
{
    ClRcT           rc = CL_OK;
    ClGmsDbKeyT     dbKey  = {{0}}; 
    ClGmsDbT        *thisViewDb = NULL;
    ClGmsViewNodeT  *foundNode = NULL;
    ClGmsViewNodeT  *leftNode = NULL;

    clLogDebug("VIEW", "DEL", "Deleting node [%d] from group [%d]", nodeId,groupId);

    rc = _clGmsViewDbFind(groupId, &thisViewDb);

    if (rc != CL_OK) 
    {
        return rc;
    }

    CL_ASSERT(thisViewDb != NULL);

    rc = _clGmsViewFindNodePrivate(thisViewDb, nodeId, CL_GMS_CURRENT_VIEW, &foundNode);

    if (rc != CL_OK) 
    {
        return rc;
    }
    CL_ASSERT(foundNode != NULL);

    foundNode->viewMember.clusterMember.bootTimestamp = 0;

    if (thisViewDb->viewType == CL_GMS_CLUSTER)
    {
        foundNode->viewMember.clusterMember.memberActive = CL_FALSE;
    }
    else
    {
        foundNode->viewMember.groupMember.memberActive = CL_FALSE;
    }

    /*
     * Add it to the view cache.
     */
    if(viewCache)
    {
        gmsViewCacheAdd(nodeId, foundNode);
    }

    /* Decrement the active view member count */

    thisViewDb->view.noOfViewMembers--;

    leftNode = ClGmsViewNodeTAlloc(foundNode->viewMember.clusterMember.nodeId);

    if (!leftNode)
        goto DEL_ERROR;

    memcpy((void*)leftNode, (void*)foundNode, sizeof(ClGmsViewNodeT));

    leftNode->trackFlags = CL_GMS_MEMBER_LEFT;

    rc = _clGmsViewFindNodePrivate(thisViewDb, nodeId, CL_GMS_JOIN_LEFT_VIEW, &foundNode);

    if (rc == CL_OK)
    {
        CL_ASSERT(foundNode != NULL);

        /* The node entry already exists in the CL_GMS_JOIN_LEFT_VIEW.*/
        if (foundNode->trackFlags == CL_GMS_MEMBER_JOINED)
        {
            dbKey.nodeId = nodeId;
            rc = _clGmsDbDelete(thisViewDb, CL_GMS_JOIN_LEFT_VIEW, dbKey);

            if (rc != CL_OK)
            {
                clLog(CL_LOG_SEV_ERROR,GEN,NA, "Unable delete the node [%d] from CL_GMS_JOIN_LEFT_VIEW. rc [0x%x]",nodeId, rc);
            }

            if (bootTimeElectionDone == CL_TRUE)
            {
                /* Boot time election is done, but still duplicate entry is
                 *  found, hence logging an error
                 */
                clLogMultiline(ERROR,GEN,NA,
                        "Duplicate entry found in CL_GMS_JOIN_LEFT_VIEW, "
                         "for NodeID: %d even when boot time election is "
                         "done\n", nodeId);
            }
            else 
            {
                clLogMultiline(ERROR,GEN,NA,
                        "NodeId %d, is leaving before leader election timer "
                         "expiry\n",nodeId);
            }
        }
        else
        {
            /* No need to delete the entry. Just change the track flag to
             *  CL_GMS_MEMBER_LEFT.
             */
            clLogMultiline(ERROR,GEN,NA,
                    "Duplicate entry found in CL_GMS_JOIN_LEFT_VIEW. "
                     "For NodeId = %d, trackFlag = %d\n",nodeId, foundNode->trackFlags);

            foundNode->trackFlags = CL_GMS_MEMBER_LEFT;
        }
    }
    else
    {
        /*Node is not found in the CL_GMS_JOIN_LEFT_VIEW. So add it */
        rc =  _clGmsViewAddNodePrivate(thisViewDb, nodeId, CL_GMS_JOIN_LEFT_VIEW, leftNode);
        if (rc != CL_OK)
        {
            clLogMultiline(ERROR,GEN,NA,
                    "Adding node with ID=%d into CL_GMS_JOIN_LEFT_VIEW failed "
                     "with rc = %d\n",nodeId, rc);
            goto DEL_ERROR;
        }
    }


DEL_ERROR:

    /* Even if any operations above fail, continue and delete
     * the node from CURRENT VIEW
     */
    dbKey.nodeId   = nodeId;

    rc = _clGmsDbDelete(thisViewDb, CL_GMS_CURRENT_VIEW, dbKey);

    thisViewDb->view.viewNumber--;

    return rc;
}

ClRcT   _clGmsViewDeleteNode(
        CL_IN   const ClGmsGroupIdT       groupId,
        CL_IN   const ClGmsNodeIdT        nodeId)
{
    return _clGmsViewDeleteNodeExtended(groupId, nodeId, CL_FALSE);
}

/*
   _clGmsViewDbCreate
   -------------------
   Creates the view database and creates the database for the cluster.
 */
void
_clGmsViewDbCreate (void)
{
    ClRcT rc = CL_OK ;
    rc = _clGmsViewInitialize();

    if(rc != CL_OK )
    {
        clLog(EMER,GEN,NA,
                "View Database Creation Failed with rc [0x%x]. Booting Aborted",rc);
        exit(0);
    }
}



/* Open the database for action and create the cluster view.
 */

ClRcT   _clGmsViewInitialize(void)
{
    ClRcT                   rc = CL_OK;
    ClGmsDbT                *thisDb = NULL;

    rc = _clGmsDbOpen(gmsGlobalInfo.config.noOfGroups, &thisDb);

    if (rc != CL_OK)
    {
        return rc;
    }

    gmsGlobalInfo.db = thisDb;

    rc = _clGmsViewCreate(gmsGlobalInfo.config.clusterName, 0);

    return rc;
}

/* FIXME:
 */


ClRcT _clGmsViewFinalize(void)
{
    return CL_OK;
}


/*   FIXME:
 */


ClUint32T  _clGmsViewHashCallback(
        CL_IN  const ClCntKeyHandleT userKey) 
{
    return (ClWordT)userKey%CL_GMS_MAX_NUM_OF_BUCKETS;
}


/*   FIXME:
 */


ClInt32T   _clGmsViewKeyCompareCallback(
        CL_IN  const   ClCntKeyHandleT key1,
        CL_IN  const   ClCntKeyHandleT key2)
{
    return ((ClWordT)key1 - (ClWordT)key2);
}


/*  FIXME:
 */


void  _clGmsViewDeleteCallback(
        CL_IN  const   ClCntKeyHandleT userKey, 
        CL_IN  const   ClCntDataHandleT userData)
{

    clHeapFree((void*)userData);

    return;
}


/* FIXME:
 */

void  _clGmsViewDestroyCallback(
        CL_IN  const  ClCntKeyHandleT userKey, 
        CL_IN  const  ClCntDataHandleT userData)
{

    clHeapFree((void*)userData);

    return;
}

