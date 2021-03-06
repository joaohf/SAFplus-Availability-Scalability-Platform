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
 * ModuleName  : name
 * File        : clNameEvent.c
 *******************************************************************************/

/*******************************************************************************
 * Description :
 * This file contains Name Service Server Side functionality
 * related to events.  
 *****************************************************************************/
                                                                                                                             
                                                                                                                             

#include "stdio.h"
#include "string.h"
#include "clCommon.h"
#include "clEoApi.h"
#include "../common/clNameCommon.h"
#include "clNameErrors.h"
#include "clCommonErrors.h"
#include "clNameIpi.h"
#include "clRmdApi.h"
#include "clDebugApi.h"
#include "clNameEventIpi.h"
#include "clCpmApi.h"
#include "clEventExtApi.h"
#include "clCpmExtApi.h"

#define CL_NM_EVT_COMP_DEATH_SUBSCRIPTION_ID 1
#define CL_NM_EVT_NODE_DEATH_SUBSCRIPTION_ID 2
#define NAME_LOG_AREA_EVENT		"EVT"
#define NAME_LOG_CTX_EVENT_INI		"INI"
#define NAME_LOG_CTX_EVENT_CALLBACK	"CALLBACK"

ClEventChannelHandleT gEvtChannelHdl;
ClEventChannelHandleT gEvtChannelSubHdl;
ClEventChannelHandleT gEvtNodeChannelSubHdl;
ClEventInitHandleT    gNameEvtHdl;
ClEventHandleT        gEventHdl;

void nameSvcEventCallback( ClEventSubscriptionIdT subscriptionId,
        ClEventHandleT eventHandle, ClSizeT eventDataSize );

extern void invokeWalkForDelete(ClNameSvcDeregisInfoT *walkData);

SaNameT nameSvcPubChannelName = {
    sizeof(CL_NAME_PUB_CHANNEL)-1,
    CL_NAME_PUB_CHANNEL
};
                                                                                                                             
SaNameT nameSvcNodeSubChannelName = {
    sizeof(CL_CPM_NODE_EVENT_CHANNEL_NAME)-1,
    CL_CPM_NODE_EVENT_CHANNEL_NAME
};

SaNameT nameSvcSubChannelName = {
    sizeof(CL_CPM_EVENT_CHANNEL_NAME)-1,
    CL_CPM_EVENT_CHANNEL_NAME
};

ClEventCallbacksT evtCallbacks =
{
    NULL,
    nameSvcEventCallback,
};



/**
 *  Name: nameSvcEventInitialize
 *
 *  Function for initializing EM library, creating a channel and setting 
 *  the channel attributes and creating the event needed for updating 
 *  the NS users whenever threr is a change in NS DB.
 *
 *  @param  none 
 *
 *  @returns
 *    CL_OK                    - everything is ok <br>
 *
 */

ClRcT nameSvcEventInitialize()
{
    ClVersionT version = CL_EVENT_VERSION;
    ClRcT rc;
    SaNameT publisherName = {sizeof(CL_NAME_PUB_NAME)-1, CL_NAME_PUB_NAME}; 
    ClUint32T                 deathPattern   = htonl(CL_CPM_COMP_DEATH_PATTERN);
    ClUint32T                 nodeDeparturePattern = htonl(CL_CPM_NODE_DEATH_PATTERN);
    ClEventFilterT            compDeathFilter[]  = {{CL_EVENT_EXACT_FILTER, 
                                                {0, (ClSizeT)sizeof(deathPattern), (ClUint8T*)&deathPattern}}
    };
    ClEventFilterArrayT      compDeathFilterArray = {sizeof(compDeathFilter)/sizeof(compDeathFilter[0]), 
                                                     compDeathFilter
    };
    ClEventFilterT            nodeDepartureFilter[]         = { {CL_EVENT_EXACT_FILTER,
                                                                {0, (ClSizeT)sizeof(nodeDeparturePattern),
                                                                (ClUint8T*)&nodeDeparturePattern}}
    };
    ClEventFilterArrayT       nodeDepartureFilterArray = {sizeof(nodeDepartureFilter)/sizeof(nodeDepartureFilter[0]),
                                                          nodeDepartureFilter 
    };
   
    /* Initialize the event lib */
    rc = clEventInitialize(&gNameEvtHdl, &evtCallbacks, &version);
    if(CL_OK != rc)
    {
        clLogError(NAME_LOG_AREA_EVENT,NAME_LOG_CTX_EVENT_INI,"\n NS: Init Failed [%x]\n\r",rc);
        return rc;
    }

    /* Open a channel for publishing */
    rc = clEventChannelOpen(gNameEvtHdl, &nameSvcPubChannelName, 
                     CL_EVENT_CHANNEL_PUBLISHER, (ClTimeT)-1, &gEvtChannelHdl);
    if(CL_OK != rc)
    {
        clLogError(NAME_LOG_AREA_EVENT,NAME_LOG_CTX_EVENT_INI,"\n NS: Channel Open Failed [%x]\n\r",rc);
        /* Do necessary cleanup */
        goto label3;
    }

    /* Allocate the channel */
    rc = clEventAllocate(gEvtChannelHdl, &gEventHdl);
    if(CL_OK != rc)
    {
        clLogError(NAME_LOG_AREA_EVENT,NAME_LOG_CTX_EVENT_INI,"\n NS: Event Allocate Failed [%x]\n\r",rc);
        /* Do necessary cleanup */
        goto label2;
    }

    /* Set the attributes */
    rc = clEventExtAttributesSet(gEventHdl, CL_NAME_EVENT_TYPE, 
                                 1, 0, &publisherName);
    if(CL_OK != rc)
    {
        clLogError(NAME_LOG_AREA_EVENT,NAME_LOG_CTX_EVENT_INI,"Event Attribute Set Failed [%x]\n\r",rc);
        /* Do necessary cleanup */
        goto label1;
    }

    /* Open a channel for subscribing */
    rc = clEventChannelOpen(gNameEvtHdl, &nameSvcSubChannelName, 
                     CL_EVENT_GLOBAL_CHANNEL | CL_EVENT_CHANNEL_SUBSCRIBER,
                     (ClTimeT)-1, &gEvtChannelSubHdl);
    if(CL_OK != rc)
    {
        clLogError(NAME_LOG_AREA_EVENT,NAME_LOG_CTX_EVENT_INI,"\n NS: Channel Open Failed [%x]\n\r",rc);
        /* Do necessary cleanup */
        goto label3;
    }

    rc = clEventSubscribe(gEvtChannelSubHdl, &compDeathFilterArray, CL_NM_EVT_COMP_DEATH_SUBSCRIPTION_ID, NULL);
    if(CL_OK != rc)
    {
        clLogError(NAME_LOG_AREA_EVENT,NAME_LOG_CTX_EVENT_INI,"Subscriptions Failed [%x]\n\r",rc);
        return rc;
    }

    /* Open a channel for subscribing */
    rc = clEventChannelOpen(gNameEvtHdl, &nameSvcNodeSubChannelName, 
                     CL_EVENT_GLOBAL_CHANNEL | CL_EVENT_CHANNEL_SUBSCRIBER,
                     (ClTimeT)-1, &gEvtNodeChannelSubHdl);
    if(CL_OK != rc)
    {
        clLogError(NAME_LOG_AREA_EVENT,NAME_LOG_CTX_EVENT_INI,"\n NS: Channel Open Failed [%x]\n\r",rc);
        /* Do necessary cleanup */
        goto label3;
    }

    rc = clEventSubscribe(gEvtNodeChannelSubHdl, &nodeDepartureFilterArray, CL_NM_EVT_NODE_DEATH_SUBSCRIPTION_ID, NULL);
    if(CL_OK != rc)
    {
        clLogError(NAME_LOG_AREA_EVENT,NAME_LOG_CTX_EVENT_INI,"Subscriptions Failed [%x]\n\r",rc);
        return rc;
    }
    goto label4;

label1:
    clEventFree(gEventHdl);
label2:
    clEventChannelClose(gEvtChannelHdl);
    clEventChannelClose(gEvtChannelSubHdl);
label3:
    clEventFinalize(gNameEvtHdl);
label4:
    return rc;
}


/**
 *  Name: nameSvcEventFinalize 
 *
 *  This function deletes th eevevnt, event channel and finalizes the  
 *  EM library
 *
 *  @param  none 
 *
 *  @returns none
 *
 */

void nameSvcEventFinalize()
{
   clEventFree(gEventHdl);
   clEventChannelClose(gEvtChannelHdl);
   clEventChannelClose(gEvtChannelSubHdl);
   clEventFinalize(gNameEvtHdl);
   return;
}


void nameSvcEventCallback( ClEventSubscriptionIdT subscriptionId,
                           ClEventHandleT eventHandle, ClSizeT eventDataSize )
{
    ClRcT rc = CL_OK;
    ClCpmEventPayLoadT compPayload;
    ClCpmEventNodePayLoadT nodePayload;
    ClNameSvcDeregisInfoT    walkData = {0};

    switch (subscriptionId)
    {
    case CL_NM_EVT_NODE_DEATH_SUBSCRIPTION_ID:
        rc = clCpmEventPayLoadExtract(eventHandle, eventDataSize, 
                                      CL_CPM_NODE_EVENT, &nodePayload);
        if (rc != CL_OK)
        {
            clLogError(NAME_LOG_AREA_EVENT,NAME_LOG_CTX_EVENT_CALLBACK,"Could not extract payload info from "
                       "CPM event. rc 0x%x",rc);
            break;
        }

        if (nodePayload.operation != CL_CPM_NODE_DEATH)
            goto out_free;

        clLogTrace(NAME_LOG_AREA_EVENT,NAME_LOG_CTX_EVENT_CALLBACK,"Node with IOC address %d is going down"
                   " Removing the name entries from that node",
                   nodePayload.nodeIocAddress);

        walkData.nodeAddress = nodePayload.nodeIocAddress;
        walkData.operation   = CL_NS_NODE_DEREGISTER_OP;

        invokeWalkForDelete(&walkData);
        break;

    case CL_NM_EVT_COMP_DEATH_SUBSCRIPTION_ID:
        rc = clCpmEventPayLoadExtract(eventHandle, eventDataSize, 
                                      CL_CPM_COMP_EVENT, &compPayload);
        if (rc != CL_OK)
        {
            clLogError(NAME_LOG_AREA_EVENT,NAME_LOG_CTX_EVENT_CALLBACK,"Could not extract payload info from "
                        "CPM event. rc 0x%x",rc);
            break;
        }

        if (compPayload.operation != CL_CPM_COMP_DEATH)
            goto out_free;

        clLogTrace(NAME_LOG_AREA_EVENT,NAME_LOG_CTX_EVENT_CALLBACK,"Component with ID %d going down"
                   " Removing the name entries from this component",
                   compPayload.compId);
        walkData.compId    = compPayload.compId;
        walkData.eoID      = compPayload.eoId;
        walkData.operation = CL_NS_COMP_DEATH_DEREGISTER_OP;

        invokeWalkForDelete(&walkData);
            
        break;
    default:
        clLogError(NAME_LOG_AREA_EVENT,NAME_LOG_CTX_EVENT_CALLBACK,"Received an event with subID %d",
                   subscriptionId);
    }

    out_free:
    clEventFree(eventHandle);
}
