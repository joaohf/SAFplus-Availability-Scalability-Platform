/*********************************************************************
* ModuleName  : idl
*********************************************************************/
/*********************************************************************
* Description :Server Stub routines
*     
* THIS FILE IS AUTO-GENERATED BY OPENCLOVIS IDE. EDIT THIS FILE AT
* YOUR OWN RISK. ANY CHANGE TO THIS FILE WILL BE OVERWRITTEN ON
* RE-GENERATION.
*     
*********************************************************************/
#include <netinet/in.h>
#include <clBufferApi.h>
#include <clRmdApi.h>
#include <clEoApi.h>
#include <ipi/clRmdIpi.h>
#include <string.h>
#include "LogPortSvrServer.h"
#include "LogServer.h"

extern ClUint32T  LogidlSyncKey;
extern ClHandleDatabaseHandleT  LogidlDatabaseHdl;



ClRcT clLogSvrStreamOpenServer_4_0_0(ClEoDataT eoData, ClBufferHandleT inMsgHdl, ClBufferHandleT outMsgHdl)
{
    ClIdlContextInfoT *pIdlCtxInfo = NULL;
    ClRcT rc = CL_OK;
    SaNameT  streamName;
    ClUint32T  streamScope;
    SaNameT  streamScopeNode;
    ClLogStreamAttrIDLT_4_0_0  pStreamAttr;
    ClUint8T  streamOpenFlags;
    ClUint32T  compId;
    ClUint32T  portId;
    ClUint32T  isExternal;
    ClUint32T  recordSize;
    ClStringT  pShmName;
    ClUint32T  pShmSize;

    memset(&(streamName), 0, sizeof(SaNameT));
    memset(&(streamScope), 0, sizeof(ClUint32T));
    memset(&(streamScopeNode), 0, sizeof(SaNameT));
    memset(&(pStreamAttr), 0, sizeof(ClLogStreamAttrIDLT_4_0_0));
    memset(&(streamOpenFlags), 0, sizeof(ClUint8T));
    memset(&(compId), 0, sizeof(ClUint32T));
    memset(&(portId), 0, sizeof(ClUint32T));
    memset(&(isExternal), 0, sizeof(ClUint32T));
    memset(&(recordSize), 0, sizeof(ClUint32T));
    memset(&(pShmName), 0, sizeof(ClStringT));
    memset(&(pShmSize), 0, sizeof(ClUint32T));


    rc = clXdrUnmarshallSaNameT( inMsgHdl,&(streamName));
    if (CL_OK != rc)
    {
        goto LL0;
    }

    rc = clXdrUnmarshallClUint32T( inMsgHdl,&(streamScope));
    if (CL_OK != rc)
    {
        goto LL1;
    }

    rc = clXdrUnmarshallSaNameT( inMsgHdl,&(streamScopeNode));
    if (CL_OK != rc)
    {
        goto LL2;
    }

    rc = clXdrUnmarshallClLogStreamAttrIDLT_4_0_0( inMsgHdl,&(pStreamAttr));
    if (CL_OK != rc)
    {
        goto LL3;
    }

    rc = clXdrUnmarshallClUint8T( inMsgHdl,&(streamOpenFlags));
    if (CL_OK != rc)
    {
        goto LL4;
    }

    rc = clXdrUnmarshallClUint32T( inMsgHdl,&(compId));
    if (CL_OK != rc)
    {
        goto LL5;
    }

    rc = clXdrUnmarshallClUint32T( inMsgHdl,&(portId));
    if (CL_OK != rc)
    {
        goto LL6;
    }

    rc = clXdrUnmarshallClUint32T( inMsgHdl,&(isExternal));
    if (CL_OK != rc)
    {
        goto LL7;
    }

    pIdlCtxInfo = (ClIdlContextInfoT *)clHeapAllocate(sizeof(ClIdlContextInfoT));
    if(pIdlCtxInfo == NULL)
    {
       return CL_IDL_RC(CL_ERR_NO_MEMORY);
    }
    memset(pIdlCtxInfo, 0, sizeof(ClIdlContextInfoT));
    pIdlCtxInfo->idlDeferMsg = outMsgHdl; 
    pIdlCtxInfo->inProgress  = CL_FALSE;
    rc = clIdlSyncPrivateInfoSet(LogidlSyncKey, (void *)pIdlCtxInfo);
    if (CL_OK != rc)
    {
        clHeapFree(pIdlCtxInfo);
        goto L0;
    }
    rc = clLogSvrStreamOpen_4_0_0(&(streamName), streamScope, &(streamScopeNode), &(pStreamAttr), streamOpenFlags, compId, portId, isExternal, &(recordSize), &(pShmName), &(pShmSize));
    if(pIdlCtxInfo->inProgress == CL_FALSE)
    {
      clHeapFree(pIdlCtxInfo);
      pIdlCtxInfo = NULL;
    }
    if (CL_OK != rc)
    {
       goto L0;
    }
    
    rc = clXdrMarshallSaNameT(&(streamName), 0, 1);
    if (CL_OK != rc)
    {
        goto L1;
    }

    rc = clXdrMarshallClUint32T(&(streamScope), 0, 1);
    if (CL_OK != rc)
    {
        goto L2;
    }

    rc = clXdrMarshallSaNameT(&(streamScopeNode), 0, 1);
    if (CL_OK != rc)
    {
        goto L3;
    }

    rc = clXdrMarshallClLogStreamAttrIDLT_4_0_0(&(pStreamAttr), 0, 1);
    if (CL_OK != rc)
    {
        goto L4;
    }

    rc = clXdrMarshallClUint8T(&(streamOpenFlags), 0, 1);
    if (CL_OK != rc)
    {
        goto L5;
    }

    rc = clXdrMarshallClUint32T(&(compId), 0, 1);
    if (CL_OK != rc)
    {
        goto L6;
    }

    rc = clXdrMarshallClUint32T(&(portId), 0, 1);
    if (CL_OK != rc)
    {
        goto L7;
    }

    rc = clXdrMarshallClUint32T(&(isExternal), 0, 1);
    if (CL_OK != rc)
    {
        goto L8;
    }

    if(pIdlCtxInfo != NULL)
    {
      clHeapFree(pIdlCtxInfo);
      return rc;
    }
    
    rc = clXdrMarshallClUint32T(&(recordSize), outMsgHdl, 1);
    if (CL_OK != rc)
    {
        goto L9;
    }

    rc = clXdrMarshallClStringT(&(pShmName), outMsgHdl, 1);
    if (CL_OK != rc)
    {
        goto L10;
    }

    rc = clXdrMarshallClUint32T(&(pShmSize), outMsgHdl, 1);
    if (CL_OK != rc)
    {
        goto L11;
    }

L11:    return rc;

LL7:  clXdrMarshallClUint32T(&(isExternal), 0, 1);
LL6:  clXdrMarshallClUint32T(&(portId), 0, 1);
LL5:  clXdrMarshallClUint32T(&(compId), 0, 1);
LL4:  clXdrMarshallClUint8T(&(streamOpenFlags), 0, 1);
LL3:  clXdrMarshallClLogStreamAttrIDLT_4_0_0(&(pStreamAttr), 0, 1);
LL2:  clXdrMarshallSaNameT(&(streamScopeNode), 0, 1);
LL1:  clXdrMarshallClUint32T(&(streamScope), 0, 1);
LL0:  clXdrMarshallSaNameT(&(streamName), 0, 1);

    return rc;

L0:  clXdrMarshallSaNameT(&(streamName), 0, 1);
L1:  clXdrMarshallClUint32T(&(streamScope), 0, 1);
L2:  clXdrMarshallSaNameT(&(streamScopeNode), 0, 1);
L3:  clXdrMarshallClLogStreamAttrIDLT_4_0_0(&(pStreamAttr), 0, 1);
L4:  clXdrMarshallClUint8T(&(streamOpenFlags), 0, 1);
L5:  clXdrMarshallClUint32T(&(compId), 0, 1);
L6:  clXdrMarshallClUint32T(&(portId), 0, 1);
L7:  clXdrMarshallClUint32T(&(isExternal), 0, 1);

L8:  clXdrMarshallClUint32T(&(recordSize), 0, 1);
L9:  clXdrMarshallClStringT(&(pShmName), 0, 1);
L10:  clXdrMarshallClUint32T(&(pShmSize), 0, 1);

    return rc;
}

ClRcT clLogSvrStreamOpenResponseSend_4_0_0(ClIdlHandleT idlHdl,ClRcT retCode,CL_OUT  ClUint32T  recordSize,CL_OUT  ClStringT  pShmName,CL_OUT  ClUint32T  pShmSize)
{
    ClIdlSyncInfoT    *pIdlSyncDeferInfo = NULL;
    ClRcT              rc                = CL_OK;
    ClBufferHandleT outMsgHdl     = 0;
    
    rc = clHandleCheckout(LogidlDatabaseHdl,idlHdl,(void **)&pIdlSyncDeferInfo);
    if( rc != CL_OK)
    {
      goto Label0; 
    }
    outMsgHdl = pIdlSyncDeferInfo->idlRmdDeferMsg;
    
    rc = clXdrMarshallClUint32T(&(recordSize), outMsgHdl, 1);
    if (CL_OK != rc)
    {
        goto L9;
    }

    rc = clXdrMarshallClStringT(&(pShmName), outMsgHdl, 1);
    if (CL_OK != rc)
    {
        goto L10;
    }

    rc = clXdrMarshallClUint32T(&(pShmSize), outMsgHdl, 1);
    if (CL_OK != rc)
    {
        goto L11;
    }

    rc = clIdlSyncResponseSend(pIdlSyncDeferInfo->idlRmdDeferHdl,outMsgHdl,
                                retCode);
    goto Label1; 
L9:  clXdrMarshallClUint32T(&(recordSize), 0, 1);
L10:  clXdrMarshallClStringT(&(pShmName), 0, 1);
L11:  clXdrMarshallClUint32T(&(pShmSize), 0, 1);

    clHandleCheckin(LogidlDatabaseHdl, idlHdl);
    clHandleDestroy(LogidlDatabaseHdl, idlHdl);
    return rc;
Label1:
    clHandleCheckin(LogidlDatabaseHdl, idlHdl);
    clHandleDestroy(LogidlDatabaseHdl, idlHdl);
Label0:
    return rc;
}

ClRcT clLogSvrStreamCloseServer_4_0_0(ClEoDataT eoData, ClBufferHandleT inMsgHdl, ClBufferHandleT outMsgHdl)
{
    ClIdlContextInfoT *pIdlCtxInfo = NULL;
    ClRcT rc = CL_OK;
    SaNameT  streamName;
    ClUint32T  streamScope;
    SaNameT  streamScopeNode;
    ClUint32T  compId;

    memset(&(streamName), 0, sizeof(SaNameT));
    memset(&(streamScope), 0, sizeof(ClUint32T));
    memset(&(streamScopeNode), 0, sizeof(SaNameT));
    memset(&(compId), 0, sizeof(ClUint32T));


    rc = clXdrUnmarshallSaNameT( inMsgHdl,&(streamName));
    if (CL_OK != rc)
    {
        goto LL0;
    }

    rc = clXdrUnmarshallClUint32T( inMsgHdl,&(streamScope));
    if (CL_OK != rc)
    {
        goto LL1;
    }

    rc = clXdrUnmarshallSaNameT( inMsgHdl,&(streamScopeNode));
    if (CL_OK != rc)
    {
        goto LL2;
    }

    rc = clXdrUnmarshallClUint32T( inMsgHdl,&(compId));
    if (CL_OK != rc)
    {
        goto LL3;
    }

    pIdlCtxInfo = (ClIdlContextInfoT *)clHeapAllocate(sizeof(ClIdlContextInfoT));
    if(pIdlCtxInfo == NULL)
    {
       return CL_IDL_RC(CL_ERR_NO_MEMORY);
    }
    memset(pIdlCtxInfo, 0, sizeof(ClIdlContextInfoT));
    pIdlCtxInfo->idlDeferMsg = outMsgHdl; 
    pIdlCtxInfo->inProgress  = CL_FALSE;
    rc = clIdlSyncPrivateInfoSet(LogidlSyncKey, (void *)pIdlCtxInfo);
    if (CL_OK != rc)
    {
        clHeapFree(pIdlCtxInfo);
        goto L0;
    }
    rc = clLogSvrStreamClose_4_0_0(&(streamName), streamScope, &(streamScopeNode), compId);
    if(pIdlCtxInfo->inProgress == CL_FALSE)
    {
      clHeapFree(pIdlCtxInfo);
      pIdlCtxInfo = NULL;
    }
    if (CL_OK != rc)
    {
       goto L0;
    }
    
    rc = clXdrMarshallSaNameT(&(streamName), 0, 1);
    if (CL_OK != rc)
    {
        goto L1;
    }

    rc = clXdrMarshallClUint32T(&(streamScope), 0, 1);
    if (CL_OK != rc)
    {
        goto L2;
    }

    rc = clXdrMarshallSaNameT(&(streamScopeNode), 0, 1);
    if (CL_OK != rc)
    {
        goto L3;
    }

    rc = clXdrMarshallClUint32T(&(compId), 0, 1);
    if (CL_OK != rc)
    {
        goto L4;
    }

    if(pIdlCtxInfo != NULL)
    {
      clHeapFree(pIdlCtxInfo);
      return rc;
    }
    
L4:    return rc;

LL3:  clXdrMarshallClUint32T(&(compId), 0, 1);
LL2:  clXdrMarshallSaNameT(&(streamScopeNode), 0, 1);
LL1:  clXdrMarshallClUint32T(&(streamScope), 0, 1);
LL0:  clXdrMarshallSaNameT(&(streamName), 0, 1);

    return rc;

L0:  clXdrMarshallSaNameT(&(streamName), 0, 1);
L1:  clXdrMarshallClUint32T(&(streamScope), 0, 1);
L2:  clXdrMarshallSaNameT(&(streamScopeNode), 0, 1);
L3:  clXdrMarshallClUint32T(&(compId), 0, 1);


    return rc;
}

ClRcT clLogSvrStreamCloseResponseSend_4_0_0(ClIdlHandleT idlHdl,ClRcT retCode)
{
    ClIdlSyncInfoT    *pIdlSyncDeferInfo = NULL;
    ClRcT              rc                = CL_OK;
    ClBufferHandleT outMsgHdl     = 0;
    
    rc = clHandleCheckout(LogidlDatabaseHdl,idlHdl,(void **)&pIdlSyncDeferInfo);
    if( rc != CL_OK)
    {
      goto Label0; 
    }
    outMsgHdl = pIdlSyncDeferInfo->idlRmdDeferMsg;
    
    rc = clIdlSyncResponseSend(pIdlSyncDeferInfo->idlRmdDeferHdl,outMsgHdl,
                                retCode);
    

    

    clHandleCheckin(LogidlDatabaseHdl, idlHdl);
    clHandleDestroy(LogidlDatabaseHdl, idlHdl);
Label0:
    return rc;
}

ClRcT clLogSvrFilterSetServer_4_0_0(ClEoDataT eoData, ClBufferHandleT inMsgHdl, ClBufferHandleT outMsgHdl)
{
    ClIdlContextInfoT *pIdlCtxInfo = NULL;
    ClRcT rc = CL_OK;
    SaNameT  pStreamName;
    ClUint32T  streamScope;
    SaNameT  pStreamScopeNode;
    ClLogFilterT_4_0_0  pFilter;

    memset(&(pStreamName), 0, sizeof(SaNameT));
    memset(&(streamScope), 0, sizeof(ClUint32T));
    memset(&(pStreamScopeNode), 0, sizeof(SaNameT));
    memset(&(pFilter), 0, sizeof(ClLogFilterT_4_0_0));


    rc = clXdrUnmarshallSaNameT( inMsgHdl,&(pStreamName));
    if (CL_OK != rc)
    {
        goto LL0;
    }

    rc = clXdrUnmarshallClUint32T( inMsgHdl,&(streamScope));
    if (CL_OK != rc)
    {
        goto LL1;
    }

    rc = clXdrUnmarshallSaNameT( inMsgHdl,&(pStreamScopeNode));
    if (CL_OK != rc)
    {
        goto LL2;
    }

    rc = clXdrUnmarshallClLogFilterT_4_0_0( inMsgHdl,&(pFilter));
    if (CL_OK != rc)
    {
        goto LL3;
    }

    pIdlCtxInfo = (ClIdlContextInfoT *)clHeapAllocate(sizeof(ClIdlContextInfoT));
    if(pIdlCtxInfo == NULL)
    {
       return CL_IDL_RC(CL_ERR_NO_MEMORY);
    }
    memset(pIdlCtxInfo, 0, sizeof(ClIdlContextInfoT));
    pIdlCtxInfo->idlDeferMsg = outMsgHdl; 
    pIdlCtxInfo->inProgress  = CL_FALSE;
    rc = clIdlSyncPrivateInfoSet(LogidlSyncKey, (void *)pIdlCtxInfo);
    if (CL_OK != rc)
    {
        clHeapFree(pIdlCtxInfo);
        goto L0;
    }
    rc = clLogSvrFilterSet_4_0_0(&(pStreamName), streamScope, &(pStreamScopeNode), &(pFilter));
    if(pIdlCtxInfo->inProgress == CL_FALSE)
    {
      clHeapFree(pIdlCtxInfo);
      pIdlCtxInfo = NULL;
    }
    if (CL_OK != rc)
    {
       goto L0;
    }
    
    rc = clXdrMarshallSaNameT(&(pStreamName), 0, 1);
    if (CL_OK != rc)
    {
        goto L1;
    }

    rc = clXdrMarshallClUint32T(&(streamScope), 0, 1);
    if (CL_OK != rc)
    {
        goto L2;
    }

    rc = clXdrMarshallSaNameT(&(pStreamScopeNode), 0, 1);
    if (CL_OK != rc)
    {
        goto L3;
    }

    rc = clXdrMarshallClLogFilterT_4_0_0(&(pFilter), 0, 1);
    if (CL_OK != rc)
    {
        goto L4;
    }

    if(pIdlCtxInfo != NULL)
    {
      clHeapFree(pIdlCtxInfo);
      return rc;
    }
    
L4:    return rc;

LL3:  clXdrMarshallClLogFilterT_4_0_0(&(pFilter), 0, 1);
LL2:  clXdrMarshallSaNameT(&(pStreamScopeNode), 0, 1);
LL1:  clXdrMarshallClUint32T(&(streamScope), 0, 1);
LL0:  clXdrMarshallSaNameT(&(pStreamName), 0, 1);

    return rc;

L0:  clXdrMarshallSaNameT(&(pStreamName), 0, 1);
L1:  clXdrMarshallClUint32T(&(streamScope), 0, 1);
L2:  clXdrMarshallSaNameT(&(pStreamScopeNode), 0, 1);
L3:  clXdrMarshallClLogFilterT_4_0_0(&(pFilter), 0, 1);


    return rc;
}

ClRcT clLogSvrFilterSetResponseSend_4_0_0(ClIdlHandleT idlHdl,ClRcT retCode)
{
    ClIdlSyncInfoT    *pIdlSyncDeferInfo = NULL;
    ClRcT              rc                = CL_OK;
    ClBufferHandleT outMsgHdl     = 0;
    
    rc = clHandleCheckout(LogidlDatabaseHdl,idlHdl,(void **)&pIdlSyncDeferInfo);
    if( rc != CL_OK)
    {
      goto Label0; 
    }
    outMsgHdl = pIdlSyncDeferInfo->idlRmdDeferMsg;
    
    rc = clIdlSyncResponseSend(pIdlSyncDeferInfo->idlRmdDeferHdl,outMsgHdl,
                                retCode);
    

    

    clHandleCheckin(LogidlDatabaseHdl, idlHdl);
    clHandleDestroy(LogidlDatabaseHdl, idlHdl);
Label0:
    return rc;
}

ClRcT clLogSvrStreamHandleFlagsUpdateServer_4_0_0(ClEoDataT eoData, ClBufferHandleT inMsgHdl, ClBufferHandleT outMsgHdl)
{
    ClIdlContextInfoT *pIdlCtxInfo = NULL;
    ClRcT rc = CL_OK;
    SaNameT  pStreamName;
    ClUint32T  streamScope;
    SaNameT  pStreamScopeNode;
    ClUint8T  handleFlags;
    ClUint16T  setFlags;

    memset(&(pStreamName), 0, sizeof(SaNameT));
    memset(&(streamScope), 0, sizeof(ClUint32T));
    memset(&(pStreamScopeNode), 0, sizeof(SaNameT));
    memset(&(handleFlags), 0, sizeof(ClUint8T));
    memset(&(setFlags), 0, sizeof(ClUint16T));


    rc = clXdrUnmarshallSaNameT( inMsgHdl,&(pStreamName));
    if (CL_OK != rc)
    {
        goto LL0;
    }

    rc = clXdrUnmarshallClUint32T( inMsgHdl,&(streamScope));
    if (CL_OK != rc)
    {
        goto LL1;
    }

    rc = clXdrUnmarshallSaNameT( inMsgHdl,&(pStreamScopeNode));
    if (CL_OK != rc)
    {
        goto LL2;
    }

    rc = clXdrUnmarshallClUint8T( inMsgHdl,&(handleFlags));
    if (CL_OK != rc)
    {
        goto LL3;
    }

    rc = clXdrUnmarshallClUint16T( inMsgHdl,&(setFlags));
    if (CL_OK != rc)
    {
        goto LL4;
    }

    pIdlCtxInfo = (ClIdlContextInfoT *)clHeapAllocate(sizeof(ClIdlContextInfoT));
    if(pIdlCtxInfo == NULL)
    {
       return CL_IDL_RC(CL_ERR_NO_MEMORY);
    }
    memset(pIdlCtxInfo, 0, sizeof(ClIdlContextInfoT));
    pIdlCtxInfo->idlDeferMsg = outMsgHdl; 
    pIdlCtxInfo->inProgress  = CL_FALSE;
    rc = clIdlSyncPrivateInfoSet(LogidlSyncKey, (void *)pIdlCtxInfo);
    if (CL_OK != rc)
    {
        clHeapFree(pIdlCtxInfo);
        goto L0;
    }
    rc = clLogSvrStreamHandleFlagsUpdate_4_0_0(&(pStreamName), streamScope, &(pStreamScopeNode), handleFlags, setFlags);
    if(pIdlCtxInfo->inProgress == CL_FALSE)
    {
      clHeapFree(pIdlCtxInfo);
      pIdlCtxInfo = NULL;
    }
    if (CL_OK != rc)
    {
       goto L0;
    }
    
    rc = clXdrMarshallSaNameT(&(pStreamName), 0, 1);
    if (CL_OK != rc)
    {
        goto L1;
    }

    rc = clXdrMarshallClUint32T(&(streamScope), 0, 1);
    if (CL_OK != rc)
    {
        goto L2;
    }

    rc = clXdrMarshallSaNameT(&(pStreamScopeNode), 0, 1);
    if (CL_OK != rc)
    {
        goto L3;
    }

    rc = clXdrMarshallClUint8T(&(handleFlags), 0, 1);
    if (CL_OK != rc)
    {
        goto L4;
    }

    rc = clXdrMarshallClUint16T(&(setFlags), 0, 1);
    if (CL_OK != rc)
    {
        goto L5;
    }

    if(pIdlCtxInfo != NULL)
    {
      clHeapFree(pIdlCtxInfo);
      return rc;
    }
    
L5:    return rc;

LL4:  clXdrMarshallClUint16T(&(setFlags), 0, 1);
LL3:  clXdrMarshallClUint8T(&(handleFlags), 0, 1);
LL2:  clXdrMarshallSaNameT(&(pStreamScopeNode), 0, 1);
LL1:  clXdrMarshallClUint32T(&(streamScope), 0, 1);
LL0:  clXdrMarshallSaNameT(&(pStreamName), 0, 1);

    return rc;

L0:  clXdrMarshallSaNameT(&(pStreamName), 0, 1);
L1:  clXdrMarshallClUint32T(&(streamScope), 0, 1);
L2:  clXdrMarshallSaNameT(&(pStreamScopeNode), 0, 1);
L3:  clXdrMarshallClUint8T(&(handleFlags), 0, 1);
L4:  clXdrMarshallClUint16T(&(setFlags), 0, 1);


    return rc;
}

ClRcT clLogSvrStreamHandleFlagsUpdateResponseSend_4_0_0(ClIdlHandleT idlHdl,ClRcT retCode)
{
    ClIdlSyncInfoT    *pIdlSyncDeferInfo = NULL;
    ClRcT              rc                = CL_OK;
    ClBufferHandleT outMsgHdl     = 0;
    
    rc = clHandleCheckout(LogidlDatabaseHdl,idlHdl,(void **)&pIdlSyncDeferInfo);
    if( rc != CL_OK)
    {
      goto Label0; 
    }
    outMsgHdl = pIdlSyncDeferInfo->idlRmdDeferMsg;
    
    rc = clIdlSyncResponseSend(pIdlSyncDeferInfo->idlRmdDeferHdl,outMsgHdl,
                                retCode);
    

    

    clHandleCheckin(LogidlDatabaseHdl, idlHdl);
    clHandleDestroy(LogidlDatabaseHdl, idlHdl);
Label0:
    return rc;
}

ClRcT clLogHandlerSvrAckSendServer_4_0_0(ClEoDataT eoData, ClBufferHandleT inMsgHdl, ClBufferHandleT outMsgHdl)
{
    ClIdlContextInfoT *pIdlCtxInfo = NULL;
    ClRcT rc = CL_OK;
    SaNameT  pStreamName;
    SaNameT  pNodeName;
    ClUint32T  streamScope;
    ClUint32T  sequenceNumber;
    ClUint32T  numRecords;
    ClHandleT  flushCookie;

    memset(&(pStreamName), 0, sizeof(SaNameT));
    memset(&(pNodeName), 0, sizeof(SaNameT));
    memset(&(streamScope), 0, sizeof(ClUint32T));
    memset(&(sequenceNumber), 0, sizeof(ClUint32T));
    memset(&(numRecords), 0, sizeof(ClUint32T));
    memset(&(flushCookie), 0, sizeof(ClHandleT));


    rc = clXdrUnmarshallSaNameT( inMsgHdl,&(pStreamName));
    if (CL_OK != rc)
    {
        goto LL0;
    }

    rc = clXdrUnmarshallSaNameT( inMsgHdl,&(pNodeName));
    if (CL_OK != rc)
    {
        goto LL1;
    }

    rc = clXdrUnmarshallClUint32T( inMsgHdl,&(streamScope));
    if (CL_OK != rc)
    {
        goto LL2;
    }

    rc = clXdrUnmarshallClUint32T( inMsgHdl,&(sequenceNumber));
    if (CL_OK != rc)
    {
        goto LL3;
    }

    rc = clXdrUnmarshallClUint32T( inMsgHdl,&(numRecords));
    if (CL_OK != rc)
    {
        goto LL4;
    }

    rc = clXdrUnmarshallClHandleT( inMsgHdl,&(flushCookie));
    if (CL_OK != rc)
    {
        goto LL5;
    }

    pIdlCtxInfo = (ClIdlContextInfoT *)clHeapAllocate(sizeof(ClIdlContextInfoT));
    if(pIdlCtxInfo == NULL)
    {
       return CL_IDL_RC(CL_ERR_NO_MEMORY);
    }
    memset(pIdlCtxInfo, 0, sizeof(ClIdlContextInfoT));
    pIdlCtxInfo->idlDeferMsg = outMsgHdl; 
    pIdlCtxInfo->inProgress  = CL_FALSE;
    rc = clIdlSyncPrivateInfoSet(LogidlSyncKey, (void *)pIdlCtxInfo);
    if (CL_OK != rc)
    {
        clHeapFree(pIdlCtxInfo);
        goto L0;
    }
    rc = clLogHandlerSvrAckSend_4_0_0(&(pStreamName), &(pNodeName), streamScope, sequenceNumber, numRecords, flushCookie);
    if(pIdlCtxInfo->inProgress == CL_FALSE)
    {
      clHeapFree(pIdlCtxInfo);
      pIdlCtxInfo = NULL;
    }
    if (CL_OK != rc)
    {
       goto L0;
    }
    
    rc = clXdrMarshallSaNameT(&(pStreamName), 0, 1);
    if (CL_OK != rc)
    {
        goto L1;
    }

    rc = clXdrMarshallSaNameT(&(pNodeName), 0, 1);
    if (CL_OK != rc)
    {
        goto L2;
    }

    rc = clXdrMarshallClUint32T(&(streamScope), 0, 1);
    if (CL_OK != rc)
    {
        goto L3;
    }

    rc = clXdrMarshallClUint32T(&(sequenceNumber), 0, 1);
    if (CL_OK != rc)
    {
        goto L4;
    }

    rc = clXdrMarshallClUint32T(&(numRecords), 0, 1);
    if (CL_OK != rc)
    {
        goto L5;
    }

    rc = clXdrMarshallClHandleT(&(flushCookie), 0, 1);
    if (CL_OK != rc)
    {
        goto L6;
    }

    if(pIdlCtxInfo != NULL)
    {
      clHeapFree(pIdlCtxInfo);
      return rc;
    }
    
L6:    return rc;

LL5:  clXdrMarshallClHandleT(&(flushCookie), 0, 1);
LL4:  clXdrMarshallClUint32T(&(numRecords), 0, 1);
LL3:  clXdrMarshallClUint32T(&(sequenceNumber), 0, 1);
LL2:  clXdrMarshallClUint32T(&(streamScope), 0, 1);
LL1:  clXdrMarshallSaNameT(&(pNodeName), 0, 1);
LL0:  clXdrMarshallSaNameT(&(pStreamName), 0, 1);

    return rc;

L0:  clXdrMarshallSaNameT(&(pStreamName), 0, 1);
L1:  clXdrMarshallSaNameT(&(pNodeName), 0, 1);
L2:  clXdrMarshallClUint32T(&(streamScope), 0, 1);
L3:  clXdrMarshallClUint32T(&(sequenceNumber), 0, 1);
L4:  clXdrMarshallClUint32T(&(numRecords), 0, 1);
L5:  clXdrMarshallClHandleT(&(flushCookie), 0, 1);


    return rc;
}

ClRcT clLogHandlerSvrAckSendResponseSend_4_0_0(ClIdlHandleT idlHdl,ClRcT retCode)
{
    ClIdlSyncInfoT    *pIdlSyncDeferInfo = NULL;
    ClRcT              rc                = CL_OK;
    ClBufferHandleT outMsgHdl     = 0;
    
    rc = clHandleCheckout(LogidlDatabaseHdl,idlHdl,(void **)&pIdlSyncDeferInfo);
    if( rc != CL_OK)
    {
      goto Label0; 
    }
    outMsgHdl = pIdlSyncDeferInfo->idlRmdDeferMsg;
    
    rc = clIdlSyncResponseSend(pIdlSyncDeferInfo->idlRmdDeferHdl,outMsgHdl,
                                retCode);
    

    

    clHandleCheckin(LogidlDatabaseHdl, idlHdl);
    clHandleDestroy(LogidlDatabaseHdl, idlHdl);
Label0:
    return rc;
}

