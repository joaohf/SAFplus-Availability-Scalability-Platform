/*
 * Copyright (C) 2002-2013 OpenClovis Solutions Inc.  All Rights Reserved.
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
 * This file is autogenerated by OpenClovis IDE, 
 */

/******************************************************************************
 * Include files needed to compile this file
 *****************************************************************************/

/* POSIX Includes */
#include <assert.h>
#include <errno.h>

/* Basic SAFplus Includes */
#include <clCommon.h>

/* SAFplus Client Includes */
#include <clLogApi.h>
#include <clCpmApi.h>
#include <saAmf.h>
#include <saAis.h>
#include <saEvt.h>
#include <clEventApi.h>
#include <clEventExtApi.h>
#include <clHandleApi.h>



/* Local function declarations */

/* The application should fill these functions */
void safTerminate(SaInvocationT invocation, const SaNameT *compName);
void safAssignWork(SaInvocationT       invocation,
                   const SaNameT       *compName,
                   SaAmfHAStateT       haState,
                   SaAmfCSIDescriptorT csiDescriptor);
void safRemoveWork(SaInvocationT  invocation,
                   const SaNameT  *compName,
                   const SaNameT  *csiName,
                   SaAmfCSIFlagsT csiFlags);

/* Utility functions */
void initializeAmf(void);
void dispatchLoop(void);
void printCSI(SaAmfCSIDescriptorT csiDescriptor, SaAmfHAStateT haState);
int  errorExit(SaAisErrorT rc);

void* logLoop(void*);

/******************************************************************************
 * Optional Features
 *****************************************************************************/

/* A wrapper for our logging. You can change "MAI" and CL_LOG_CONTEXT_UNSPECIFIED
   to another 3 letter acronym meaningful more meaningful to this module.
 */

#define clprintf(severity, ...)   clAppLog(CL_LOG_HANDLE_APP, severity, 10, "MAI", CL_LOG_CONTEXT_UNSPECIFIED,__VA_ARGS__)

/******************************************************************************
 * Global Variables.
 *****************************************************************************/

ClAmsHAStateT           ha_state = CL_AMS_HA_STATE_NONE;


/* This process's SAF name */
SaNameT      appName = {0};

#define EVENT_CHANNEL_PUB_NAME "TestEventChannel"
#define PUBLISHER_NAME "TestEventPublisher"
#define EVENT_CHANNEL_SUB_NAME "TestEventChannel1"


static char     appname[80];
static void appEventCallback(SaEvtSubscriptionIdT	subscriptionId,SaEvtEventHandleT eventHandle,SaSizeT eventDataSize);

/*
 * ---END_APPLICATION_CODE---
 */

/******************************************************************************
 * Global Variables.
 *****************************************************************************/

pid_t mypid;
SaAmfHandleT amfHandle;
SaEvtHandleT      evtHandle;
SaNameT                 evtChannelName;
SaEvtChannelHandleT   evtChannelHandle = 0;
ClBoolT unblockNow = CL_FALSE;
ClRcT alarmClockLogInitialize( void );
/*
 * Declare other global variables here.
 */


// Poor man's namespace.  Gather together some globals that don't
// really need to be global.
typedef struct
{
    ClEoExecutionObjT             *tstRegEoObj;
    ClCpmHandleT                  cpmHandle;
    SaEvtHandleT		          evtInitHandle;
    SaEvtEventHandleT		      eventHandle;
    SaAmfCallbacksT               callbacks;
    ClVersionT                    version;
    SaVersionT                    evtVersion;
    SaNameT                       evtChannelName;
    SaNameT                       publisherName;
    int                           running;
    int                           exiting;
    ClAmsHAStateT                 haState;
} gTestInfoT;

static gTestInfoT gTestInfo;





/* Declare other global variables here. */
static inline void
logrc(ClLogSeverityT severity, char *buffer, char *appname, ClRcT rc)
{
    clLogWrite(CL_LOG_HANDLE_APP, severity, NULL, buffer, appname, rc);
}

static inline void
logmsg(ClLogSeverityT severity, char *buffer, char *appname)
{
    clLogWrite(CL_LOG_HANDLE_APP, severity, NULL, buffer, appname);
}

static ClRcT appPublishEvent();

static void* testEvtMainLoop(void *thdParam);

/******************************************************************************
 * Application Life Cycle Management Functions
 *****************************************************************************/


int main(int argc, char *argv[])
{
    SaAisErrorT rc = SA_AIS_OK;

    /* Connect to the SAF cluster */
    initializeAmf();

    /* Do the application specific initialization here. */

    gTestInfo.tstRegEoObj      = 0;
  	gTestInfo.cpmHandle        = 0;                 // CPM service instance handle
  	gTestInfo.evtInitHandle    = 0;
  	gTestInfo.eventHandle      = 0;
  	gTestInfo.callbacks.saAmfHealthcheckCallback          = NULL; /* rarely necessary because SAFplus monitors the process */
  	gTestInfo.callbacks.saAmfComponentTerminateCallback   = safTerminate;
  	gTestInfo.callbacks.saAmfCSISetCallback               = safAssignWork;
  	gTestInfo.callbacks.saAmfCSIRemoveCallback            = safRemoveWork;
  	gTestInfo.callbacks.saAmfProtectionGroupTrackCallback = NULL;
    gTestInfo.version.releaseCode                    = 'B';
    gTestInfo.version.majorVersion                   = 01;
    gTestInfo.version.minorVersion                   = 01;
    gTestInfo.evtVersion.releaseCode                    = 'B';
    gTestInfo.evtVersion.majorVersion                   = 01;
    gTestInfo.evtVersion.minorVersion                   = 01;
    saNameSet(&gTestInfo.evtChannelName,EVENT_CHANNEL_PUB_NAME);
    saNameSet(&gTestInfo.publisherName,PUBLISHER_NAME);
    gTestInfo.running          = 1;
    gTestInfo.exiting          = 0;
    gTestInfo.haState          = CL_AMS_HA_STATE_STANDBY;
    SaEvtChannelHandleT evtChannelHandle      = 0;
    SaEvtCallbacksT     evtCallbacks          = {NULL, NULL};
    const SaEvtCallbacksT evtSubCallbacks =
    {
        NULL,
        appEventCallback
    };
    SaVersionT  evtVersion = CL_EVENT_VERSION;

#define min(x,y) ((x < y)? x: y)

    strncpy(appname, (char*)appName.value, min(sizeof appname, appName.length));
    appname[min(sizeof appname - 1, appName.length)] = 0;
    printf("EventPublisher : Initializing and registering with CPM...\n");

    //==================Initialize the the event for publish====================================
    rc = saEvtInitialize(&gTestInfo.evtInitHandle,
                           &evtCallbacks,
                           &gTestInfo.evtVersion);
    if (rc != SA_AIS_OK)
    {
        clprintf(CL_LOG_SEV_ERROR, "%s\t:Failed to init event system[0x%x]\n",
                    appname, rc);
        return rc;
    }

    // Open an event channel so we can publish our events.
    rc = saEvtChannelOpen (gTestInfo.evtInitHandle,
                             &gTestInfo.evtChannelName,
                            (SA_EVT_CHANNEL_PUBLISHER |
                             SA_EVT_CHANNEL_CREATE),
                             (ClTimeT)SA_TIME_END,
                             &evtChannelHandle);
    if (rc != SA_AIS_OK)
    {
        clprintf(CL_LOG_SEV_ERROR, "%s\t:Failed to open event channel [0x%x]\n",
                appname, rc);
        return rc;
    }

    rc = saEvtEventAllocate(evtChannelHandle, &gTestInfo.eventHandle);
    if (rc != SA_AIS_OK)
    {
        clprintf(CL_LOG_SEV_ERROR, "%s\t:Failed to allocate event [0x%x]\n",
                appname, rc);
        return rc;
    }

    rc = saEvtEventAttributesSet(gTestInfo.eventHandle,
            NULL,
            1,
            0,
            &gTestInfo.publisherName);
    if (rc != SA_AIS_OK)
    {
        clprintf(CL_LOG_SEV_ERROR, "%s\t:Failed to set event attributes [0x%x]\n",
                appname, rc);
        return rc;
    }

    //==================Initialize the the event for subscriber====================================
    rc = saEvtInitialize(&evtHandle, &evtSubCallbacks, &evtVersion);
    if (rc != SA_AIS_OK)
    {
        clprintf(CL_LOG_SEV_ERROR, "%s: Failed to init event mechanism [0x%x]\n",
                appname, rc);
        return rc;
    }
    // Open an event chanel so that we can subscribe to events on that channel
    saNameSet(&evtChannelName,EVENT_CHANNEL_SUB_NAME);
    rc = saEvtChannelOpen(evtHandle,&evtChannelName,
            (SA_EVT_CHANNEL_SUBSCRIBER |
                SA_EVT_CHANNEL_CREATE),
            (SaTimeT)SA_TIME_END,
            &evtChannelHandle);
    if (rc != SA_AIS_OK)
    {
        clprintf(CL_LOG_SEV_ERROR, "%s\t:Failure opening event channel[0x%x] at %ld\n", appname,
                    rc, time(0L));
        clprintf(CL_LOG_SEV_ERROR, "%s\t:Failure opening event channel[0x%x]\n",
                    appname, rc);
        goto errorexit;
    }

    rc = saEvtEventSubscribe(evtChannelHandle, NULL, 1);
    if (rc != SA_AIS_OK)
    {
        clprintf(CL_LOG_SEV_ERROR, "%s\t: Failed to subscribe to event channel [0x%x]\n",
                    appname, rc);
        clprintf(CL_LOG_SEV_ERROR,
                    "%s\t:Failed to subscribe to event channel [0x%x]\n",
                    appname, rc);
        goto errorexit;
    }



    clOsalTaskCreateDetached("testEvtMainLoop",
                             CL_OSAL_SCHED_OTHER,
                             CL_OSAL_THREAD_PRI_NOT_APPLICABLE,
                             65536,
                             testEvtMainLoop,
                             (void*)appname);

    clprintf(CL_LOG_SEV_INFO, "%s: Instantiated as component instance %s.\n", appname,
                appName.value);
    /* Block on AMF dispatch file descriptor for callbacks.
       When this function returns its time to quit. */
    dispatchLoop();
    
    /* Do the application specific finalization here. */

    /* Now finalize my connection with the SAF cluster */
    if((rc = saAmfFinalize(amfHandle)) != SA_AIS_OK)
      clprintf (CL_LOG_SEV_ERROR, "AMF finalization error[0x%X]", rc);
    else
      clprintf (CL_LOG_SEV_INFO, "AMF Finalized");   

    return 0;
    errorexit:
        clprintf (CL_LOG_SEV_ERROR, "Component [%.*s] : PID [%d]. Initialization error [0x%x]\n",
                  appName.length, appName.value, mypid, rc);

        return -1;
}


/*
 * clCompAppTerminate
 * ------------------
 * This function is invoked when the application is to be terminated.
 */

void safTerminate(SaInvocationT invocation, const SaNameT *compName)
{
    SaAisErrorT rc = SA_AIS_OK;

    clprintf (CL_LOG_SEV_INFO, "Component [%.*s] : PID [%d]. Terminating\n", compName->length, compName->value, mypid);

    
    /*
     * Unregister with AMF and respond to AMF saying whether the
     * termination was successful or not.
     */
    if ( (rc = saAmfComponentUnregister(amfHandle, compName, NULL)) != SA_AIS_OK)
    {
        clprintf (CL_LOG_SEV_ERROR, "Component [%.*s] : PID [%d]. Unregister failed with error [0x%x]\n",
                  compName->length, compName->value, mypid, rc);
        return;        
    }

    /* Ok tell SAFplus that we handled it properly */
    saAmfResponse(amfHandle, invocation, SA_AIS_OK);

    clprintf (CL_LOG_SEV_INFO, "Component [%.*s] : PID [%d]. Terminated\n",
              compName->length, compName->value, mypid);

    unblockNow = CL_TRUE;
}

/******************************************************************************
 * Application Work Assignment Functions
 *****************************************************************************/

/*
 * safAssignWork
 * ------------------
 * This function is invoked when a CSI assignment is made or the state
 * of a CSI is changed.
 */

void safAssignWork(SaInvocationT       invocation,
                   const SaNameT       *compName,
                   SaAmfHAStateT       haState,
                   SaAmfCSIDescriptorT csiDescriptor)
{
    /* Print information about the CSI Set */

    clprintf (CL_LOG_SEV_INFO, "Component [%.*s] : PID [%d]. CSI Set Received\n", 
              compName->length, compName->value, mypid);

    printCSI(csiDescriptor, haState);

    /*
     * Take appropriate action based on state
     */

    switch ( haState )
    {
        /* AMF asks this process to take the standby HA state for the work
           described in the csiDescriptor variable */
        case SA_AMF_HA_ACTIVE:
        {
            /* Typically you would spawn a thread here to initiate active 
               processing of the work. */


            /* The AMF times the interval between the assignment and acceptance
               of the work (the time interval is configurable).
               So it is important to call this saAmfResponse function ASAP.
             */
        	alarmClockLogInitialize();
        	gTestInfo.haState          = CL_AMS_HA_STATE_ACTIVE;
        	ha_state = CL_AMS_HA_STATE_ACTIVE;
            saAmfResponse(amfHandle, invocation, SA_AIS_OK);
            ClRcT rc = clOsalTaskCreateDetached("Logging Loop", CL_OSAL_SCHED_OTHER, 0, 0, logLoop, NULL);
            assert(rc==CL_OK);
            break;
        }

        /* AMF asks this process to take the standby HA state for the work
           described in the csiDescriptor variable */
        case SA_AMF_HA_STANDBY:
        {
            /* If your standby has ongoing maintenance, you would spawn a thread
               here to do it. */

            /* The AMF times the interval between the assignment and acceptance
               of the work (the time interval is configurable).
               So it is important to call this saAmfResponse function ASAP.
             */
            saAmfResponse(amfHandle, invocation, SA_AIS_OK);  
            break;
        }

        case SA_AMF_HA_QUIESCED:
        {
            /*
             * AMF has requested application to quiesce the CSI currently
             * assigned the active or quiescing HA state. The application 
             * must stop work associated with the CSI immediately.
             */

            saAmfResponse(amfHandle, invocation, SA_AIS_OK);
            break;
        }

        case SA_AMF_HA_QUIESCING:
        {
            /*
             * AMF has requested application to quiesce the CSI currently
             * assigned the active HA state. The application must stop work
             * associated with the CSI gracefully and not accept any new
             * workloads while the work is being terminated.
             */

            /* There are two typical cases for quiescing.  Chooose one!
               CASE 1: Its possible to quiesce rapidly within this thread context */
            if (1)
              {
              /* App code here: Now finish your work and cleanly stop the work*/
            
              /* Call saAmfCSIQuiescingComplete when stopping the work is done */
              saAmfCSIQuiescingComplete(amfHandle, invocation, SA_AIS_OK);
              }
            else
              {
              /* CASE 2: You can't quiesce within this thread context or quiesce
               rapidly. */

              /* Respond immediately to the quiescing request */
              saAmfResponse(amfHandle, invocation, SA_AIS_OK);

              /* App code here: Signal or spawn a thread to cleanly stop the work*/
              /* When that thread is done, it should call:
                 saAmfCSIQuiescingComplete(amfHandle, invocation, SA_AIS_OK);
              */
              }

            break;
        }

        default:
        {
            assert(0);
            break;
        }
    }

    return;
}

/*
 * safRemoveWork
 * ---------------------
 * This function is invoked when a CSI assignment is to be removed.
 */

void safRemoveWork(SaInvocationT  invocation,
                   const SaNameT  *compName,
                   const SaNameT  *csiName,
                   SaAmfCSIFlagsT csiFlags)
{
    clprintf (CL_LOG_SEV_INFO, "Component [%.*s] : PID [%d]. CSI Remove Received\n", 
              compName->length, compName->value, mypid);

    clprintf (CL_LOG_SEV_INFO, "   CSI                     : %.*s\n", csiName->length, csiName->value);
    clprintf (CL_LOG_SEV_INFO, "   CSI Flags               : 0x%d\n", csiFlags);

    /*
     * Add application specific logic for removing the work for this CSI.
     */

    saAmfResponse(amfHandle, invocation, SA_AIS_OK);
}

/******************************************************************************
 * Utility functions 
 *****************************************************************************/
/* This simple helper function just prints an error and quits */
int errorExit(SaAisErrorT rc)
{        
    clprintf (CL_LOG_SEV_ERROR, "Component [%.*s] : PID [%d]. Initialization error [0x%x]\n",
              appName.length, appName.value, mypid, rc);
    exit(-1);
    return -1;
}

void initializeAmf(void)
{
    SaAmfCallbacksT     callbacks;
    SaVersionT          version;
    ClIocPortT          iocPort;
    SaAisErrorT         rc = SA_AIS_OK;

    /* Get the pid for the process and store it in global variable. */
    mypid = getpid();

    /* SAFplus is fully API compatible with SA-Forum (SAF) definitions.

       This optional call customizes OpenClovis SAFplus Platform extensions
       to the basic SAF services (to use, you would define the parameters as globals).  
       
       If this call is removed, standard SAF services will work just fine. */

    /* clAppConfigure(&clEoConfig,clEoBasicLibs,clEoClientLibs); */

    
    /*
     * Initialize and register with SAFplus AMF. 'version' specifies the
     * version of AMF with which this application would like to
     * interface. 'callbacks' is used to register the callbacks this
     * component expects to receive.
     */
    version.releaseCode  = 'B';
    version.majorVersion = 01;
    version.minorVersion = 01;
    
    callbacks.saAmfHealthcheckCallback          = NULL; /* rarely necessary because SAFplus monitors the process */
    callbacks.saAmfComponentTerminateCallback   = safTerminate;
    callbacks.saAmfCSISetCallback               = safAssignWork;
    callbacks.saAmfCSIRemoveCallback            = safRemoveWork;
    callbacks.saAmfProtectionGroupTrackCallback = NULL;
        
    /* Initialize AMF client library. */
    if ( (rc = saAmfInitialize(&amfHandle, &callbacks, &version)) != SA_AIS_OK)
        errorExit(rc);

    /*
     * Now register the component with AMF. At this point it is
     * ready to provide service, i.e. take work assignments.
     */

    if ( (rc = saAmfComponentNameGet(amfHandle, &appName)) != SA_AIS_OK) 
        errorExit(rc);
    if ( (rc = saAmfComponentRegister(amfHandle, &appName, NULL)) != SA_AIS_OK) 
        errorExit(rc);

    /*
     * Print out standard information for this component.
     */

    clEoMyEoIocPortGet(&iocPort);
    
    clprintf (CL_LOG_SEV_INFO, "Component [%.*s] : PID [%d]. Initializing\n", appName.length, appName.value, mypid);
    clprintf (CL_LOG_SEV_INFO, "   IOC Address             : 0x%x\n", clIocLocalAddressGet());
    clprintf (CL_LOG_SEV_INFO, "   IOC Port                : 0x%x\n", iocPort);
}

void dispatchLoop(void)
{        
  SaAisErrorT         rc = SA_AIS_OK;
  SaSelectionObjectT amf_dispatch_fd;
  int maxFd;
  fd_set read_fds;

  /* This boilerplate code includes an example of how to simultaneously
     dispatch for 2 services (in this case AMF and CKPT).  But since checkpoint
     is not initialized or used, it is commented out */
  /* SaSelectionObjectT ckpt_dispatch_fd; */

  /*
   * Get the AMF dispatch FD for the callbacks
   */
  if ( (rc = saAmfSelectionObjectGet(amfHandle, &amf_dispatch_fd)) != SA_AIS_OK)
    errorExit(rc);
  /* if ( (rc = saCkptSelectionObjectGet(ckptLibraryHandle, &ckpt_dispatch_fd)) != SA_AIS_OK)
       errorExit(rc); */
    
  maxFd = amf_dispatch_fd;  /* maxFd = max(amf_dispatch_fd,ckpt_dispatch_fd); */
  do
    {
      FD_ZERO(&read_fds);
      FD_SET(amf_dispatch_fd, &read_fds);
      /* FD_SET(ckpt_dispatch_fd, &read_fds); */
        
      if( select(maxFd + 1, &read_fds, NULL, NULL, NULL) < 0)
        {
          char errorStr[80];
          int err = errno;
          if (EINTR == err) continue;

          errorStr[0] = 0; /* just in case strerror does not fill it in */
          strerror_r(err, errorStr, 79);
          clprintf (CL_LOG_SEV_ERROR, "Error [%d] during dispatch loop select() call: [%s]",err,errorStr);
          break;
        }
      if (FD_ISSET(amf_dispatch_fd,&read_fds)) saAmfDispatch(amfHandle, SA_DISPATCH_ALL);
      /* if (FD_ISSET(ckpt_dispatch_fd,&read_fds)) saCkptDispatch(ckptLibraryHandle, SA_DISPATCH_ALL); */
    }while(!unblockNow);      
}


#define STRING_HA_STATE(S)                                                  \
(   ((S) == SA_AMF_HA_ACTIVE)             ? "Active" :                \
    ((S) == SA_AMF_HA_STANDBY)            ? "Standby" :               \
    ((S) == SA_AMF_HA_QUIESCED)           ? "Quiesced" :              \
    ((S) == SA_AMF_HA_QUIESCING)          ? "Quiescing" :             \
                                            "Unknown" )

#define STRING_CSI_FLAGS(S)                                                 \
(   ((S) == SA_AMF_CSI_ADD_ONE)            ? "Add One" :               \
    ((S) == SA_AMF_CSI_TARGET_ONE)         ? "Target One" :            \
    ((S) == SA_AMF_CSI_TARGET_ALL)         ? "Target All" :            \
                                                  "Unknown" )

/*
 * printCSI
 * --------------------
 * Print information received in a CSI set request.
 */

void printCSI(SaAmfCSIDescriptorT csiDescriptor, SaAmfHAStateT haState)
{
    clprintf (CL_LOG_SEV_INFO, "CSI Flags : [%s]", STRING_CSI_FLAGS(csiDescriptor.csiFlags));

    if (SA_AMF_CSI_TARGET_ALL != csiDescriptor.csiFlags)
    {
        clprintf (CL_LOG_SEV_INFO, "CSI Name : [%s]", csiDescriptor.csiName.value);
    }

    if (SA_AMF_CSI_ADD_ONE == csiDescriptor.csiFlags)
    {
        ClUint32T i = 0;
        
        clprintf (CL_LOG_SEV_INFO, "Name value pairs :");
        for (i = 0; i < csiDescriptor.csiAttr.number; i++)
        {
            clprintf (CL_LOG_SEV_INFO, "Name : [%s]",
                      csiDescriptor.csiAttr.
                      attr[i].attrName);
            clprintf (CL_LOG_SEV_INFO, "Value : [%s]",
                      csiDescriptor.csiAttr.
                      attr[i].attrValue);
        }
    }
    
    clprintf (CL_LOG_SEV_INFO, "HA state : [%s]", STRING_HA_STATE(haState));

    if (SA_AMF_HA_ACTIVE == haState)
    {
        clprintf (CL_LOG_SEV_INFO, "Active Descriptor :");
        clprintf (CL_LOG_SEV_INFO,
                  "Transition Descriptor : [%d]",
                  csiDescriptor.csiStateDescriptor.
                  activeDescriptor.transitionDescriptor);
        clprintf (CL_LOG_SEV_INFO,
                  "Active Component : [%s]",
                  csiDescriptor.csiStateDescriptor.
                  activeDescriptor.activeCompName.value);
    }
    else if (SA_AMF_HA_STANDBY == haState)
    {
        clprintf (CL_LOG_SEV_INFO, "Standby Descriptor :");
        clprintf (CL_LOG_SEV_INFO,
                  "Standby Rank : [%d]",
                  csiDescriptor.csiStateDescriptor.
                  standbyDescriptor.standbyRank);
        clprintf (CL_LOG_SEV_INFO, "Active Component : [%s]",
                  csiDescriptor.csiStateDescriptor.
                  standbyDescriptor.activeCompName.value);
    }
}


static void
generate_time_of_day(char **data, ClSizeT *data_len)
{
    time_t t;

    // minimal error checking
    if (data == 0 || data_len == 0)
    {
        clprintf(CL_LOG_SEV_ERROR,
                "%s\t: generate_time_of_day passed null pointer\n", appname);
        return;
    }

    // magic number, but well, that's what ctime_r needs
    *data_len = 26;
    *data = (char*)clHeapAllocate(*data_len);
    if (*data == 0)
    {
        *data_len = 0;
        return;
    }
    time(&t);
    ctime_r(&t, *data);
    *(*data + 24) = 0;
    (*data_len) -= 1;
    return;
}

static void
generate_load_average(char **data, ClSizeT *data_len)
{
    int fd;
    char *tmp_ptr;
    char buf[500];                  //insane over doing it
    ssize_t num_read;

    // minimal error checking
    if (data == 0 || data_len == 0)
    {
        clprintf(CL_LOG_SEV_ERROR,
                "%s\t: generate_load_average passed null pointer\n ", appname);
        return;
    }

    // Now open the load average file in /proc, read the file into a local
    // buffer, allocate memory to hold the file contents, copy the contents
    // of the file into the newly allocated buffer.
    if ((fd = open("/proc/loadavg", O_RDONLY, 0)) == -1)
    {
        clprintf(CL_LOG_SEV_ERROR, "%s\t: failed to open /proc/loadavg\n", appname);
        return;
    }
    num_read = read(fd, buf, sizeof buf);
    if (num_read == 0 || num_read == -1)
    {
        clprintf(CL_LOG_SEV_ERROR, "%s\t: bogus result from read of loadavg\n",
                appname);
        return;
    }
    close(fd);
    *data_len = num_read + 1;
    *data = (char*)clHeapAllocate(*data_len);
    if (data == 0)
    {
        clprintf(CL_LOG_SEV_ERROR,
                "%s: failed to allocate memory for loadavg contents\n",
                appname);
        *data_len = 0;
        close(fd);
        return;
    }
    *(*data + (*data_len) - 1) = 0;     // preemptively null-terminate the line
    strncpy(*data, buf, *data_len);

    //
    // Do MINIMAL parsing in that we look for the third space in the buffer
    // (which comes after the load average information proper) and we replace
    // the space with a nul character to terminate the string.
    // If there is no third space character, just return the buffer unchanged.
    tmp_ptr = strchr(*data, ' ');
    if (tmp_ptr == 0)
    {
        return;
    }
    tmp_ptr = strchr(tmp_ptr + 1, ' ');
    if (tmp_ptr == 0)
    {
        return;
    }
    tmp_ptr = strchr(tmp_ptr + 1, ' ');
    if (tmp_ptr == 0)
    {
        return;
    }
    *tmp_ptr = 0;
    return;
}

static ClRcT
appPublishEvent()
{

    ClEventIdT      eventId         = 0;
    static int      index           = 0;
    SaSizeT         data_len        = 0;
    SaAisErrorT	    saRc = SA_AIS_OK;
    char            *data           = 0;
    typedef void (*Generator)(char **, ClSizeT*);
    clprintf(CL_LOG_SEV_INFO, "Publish time event to external application \n");
    //
    // Note: to add a new generator, just define it above and then include
    // the new functions name in the generators list.
    // Next, maybe something that gets disk free info by way of getfsent
    // and statfs?
    static Generator generators[]   =
    {
        generate_time_of_day,
        generate_load_average
    };

    //
    // every time through increment index and then set index to
    // it's value modulo the number of entries in the generators
    // array.  This will cause us to cycle through the list of
    // generators as we're called to publish events.
    (*generators[index++])(&data, &data_len);
    index %= (int)(sizeof generators / sizeof generators[0]);
    if (data == 0 || data_len == 0)
    {
        clprintf(CL_LOG_SEV_ERROR, "%s: no event data generated\n", appname);
        return CL_ERR_NO_MEMORY;
    }
    clOsalPrintf("%s: Publishing Event: %.*s\n", appname, (int)data_len, data);
    saRc = saEvtEventPublish(gTestInfo.eventHandle, (void *)data, data_len, &eventId);
    clHeapFree(data);

    return CL_OK;
}

static void*
testEvtMainLoop(void *thdParam)
{
    char *appname = (char*)thdParam;

    printf("%s: Waiting for CSI assignment...\n", appname);

    /* Letting the world know that we entered the mainloop */
    //clprintf(CL_LOG_SVR_DEBUG, "%s entered main loop", appname);

    /* Main loop: Keep printing and publishing unless we are suspended */
    while (!gTestInfo.exiting)
    {
    	printf("publish Event...\n");
        if (gTestInfo.running && gTestInfo.haState == CL_AMS_HA_STATE_ACTIVE)
        {
            appPublishEvent();
        }
        sleep(1);
    }

    /* Letting the world know that we exited from mainloop */
    //clprintf(CL_LOG_SVR_DEBUG, "%s exited main loop", appname);

    return NULL;
}
static void
appEventCallback( SaEvtSubscriptionIdT	subscriptionId,
                             SaEvtEventHandleT     eventHandle,
			     SaSizeT eventDataSize)
{
    SaAisErrorT  saRc = SA_AIS_OK;
    static ClPtrT   resTest = 0;
    static ClSizeT  resSize = 0;

    if (ha_state != CL_AMS_HA_STATE_ACTIVE)
    {
        return;
    }
    clprintf (CL_LOG_SEV_INFO,"We've got an event to receive\n");
    if (resTest != 0)
    {
        // Maybe try to save the previously allocated buffer if it's big
        // enough to hold the new event message.
        clHeapFree((char *)resTest);
        resTest = 0;
        resSize = 0;
    }
    resTest = clHeapAllocate(eventDataSize + 1);
    if (resTest == 0)
    {
        clprintf(CL_LOG_SEV_ERROR, "%s\t:Failed to allocate space for event\n",
                    appname);
        return;
    }
    *(((char *)resTest) + eventDataSize) = 0;
    resSize = eventDataSize;
    saRc = saEvtEventDataGet(eventHandle, resTest, &resSize);
    if (saRc!= SA_AIS_OK)
    {
        clprintf(CL_LOG_SEV_ERROR, "%s\t:Failed to get event data [0x%x]\n",
                    appname, saRc);
    }

    clprintf (CL_LOG_SEV_INFO,"received event: %s\n", (char *)resTest);
    return;
}
static ClLogHandleT        logSvcHandle = CL_HANDLE_INVALID_VALUE;

ClLogStreamHandleT  streamHandle = CL_HANDLE_INVALID_VALUE;
ClLogStreamAttributesT myStreamAttr;

ClRcT alarmClockLogInitialize( void )
{
    ClRcT      rc     = CL_OK;
    ClVersionT version= {'B', 0x1, 0x1};
    ClLogCallbacksT  logCallbacks = {0};
    SaNameT       streamName;

    logSvcHandle = CL_HANDLE_INVALID_VALUE;
    streamHandle = CL_HANDLE_INVALID_VALUE;

    /*
    Initialize the client log library.

        ClLogHandleT    *phLog,
         - used for subsequent invocation of Log Service APIs
          ClLogCallbacksT *pLogCallbacks,
          ClLogFilterSetCallbackT - Callback after filter update is completed.
          ClLogRecordDeliveryCallbackT - Callback for retrieving records
          ClLogStreamOpenCallbackT     - Callback for clLogStreamOpenAsync()

        ClVersionT         *pVersion`
    */

    rc = clLogInitialize(&logSvcHandle, &logCallbacks, &version);
    if(CL_OK != rc)
    {
        // Error occured. Take appropriate action.
        clprintf(CL_LOG_SEV_ERROR, "Failed to initialze log: %x\n", rc);
        return rc;
    }
    sleep(5);
    clprintf(CL_LOG_SEV_ERROR, "open External log \n");
    myStreamAttr.fileName = (char *)"external.log";
    myStreamAttr.fileLocation=(char *)".:var/log";
    myStreamAttr.recordSize = 300;
    myStreamAttr.fileUnitSize = 1000000;
    myStreamAttr.fileFullAction = CL_LOG_FILE_FULL_ACTION_ROTATE;
    myStreamAttr.maxFilesRotated = 10;
    myStreamAttr.flushFreq = 10;
    myStreamAttr.haProperty = 0;
    myStreamAttr.flushInterval = 1 * (1000L * 1000L * 1000L);
    myStreamAttr.waterMark.lowLimit = 0;
    myStreamAttr.waterMark.highLimit = 0;
    myStreamAttr.syslog = CL_FALSE;
    clprintf(CL_LOG_SEV_ERROR, "open clockStream 1 \n");
    /* Stream Name is defined in the IDE during
     * modeling phase
     */
    saNameSet(&streamName,"externalAppStream");
    //strcpy(streamName.value, "clockStream");
    //streamName.length = strlen("clockStream");


    /* open the clock stream
     * ClLogHandleT  - returned with clLogInitialize()
     * ClNameT       - name of the stream to open
     * ClLogStreamScopeT  - scope can be global (cluster wide) or local
     * ClLogStreamAttributesT * - set to null because this is precreated stream
     * ClLogStreamOpenFlagsT  -  no stream open flags specified
     * ClTimeT   timeout      - timeout set to zero, if failed return immed.
     * ClLogStreamHandleT *   - stream handle returned if successful
    */
    clprintf(CL_LOG_SEV_ERROR, "open external log \n");
    rc = clLogStreamOpen(logSvcHandle,
                         streamName,
                         CL_LOG_STREAM_GLOBAL,
                         &myStreamAttr,
                         CL_LOG_STREAM_CREATE,
                         0,
                         &streamHandle);
    if(CL_OK != rc)
    {
        /* error occurred. Close Log Service handle and
           return error code
         */
        clprintf(CL_LOG_SEV_ERROR, "Failed to open clockStream : %x\n", rc);
        (void)clLogFinalize(logSvcHandle);
        return rc;
    }
    
    rc = clLogWriteAsync(streamHandle, CL_LOG_SEV_NOTICE, 10, CL_LOG_MSGID_PRINTF_FMT, "\n(%s:%d[pid=%d]) External log open and ready to receive logs.\n", __FILE__, __LINE__,getpid());
    return CL_OK;
}


void* logLoop(void* nothing)
{
    int i=0;
    clprintf(CL_LOG_SEV_ERROR, "Starting Log loop");
    while(1)
    {
        i++;        
        clLogWriteAsync(streamHandle, CL_LOG_SEV_NOTICE, 10, CL_LOG_MSGID_PRINTF_FMT, "log loop: %d",i);
        sleep(5);        
    }

    return 0;
}
