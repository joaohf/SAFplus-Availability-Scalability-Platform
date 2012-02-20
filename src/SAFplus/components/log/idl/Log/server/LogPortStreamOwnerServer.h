
/*********************************************************************
* File: 
*********************************************************************/
/*********************************************************************
* Description : This file contains the declartions for server stub
*               routines
*     
* THIS FILE IS AUTO-GENERATED BY OPENCLOVIS IDE. EDIT THIS FILE AT
* YOUR OWN RISK. ANY CHANGE TO THIS FILE WILL BE OVERWRITTEN ON
* RE-GENERATION.
*     
*********************************************************************/

#ifndef _LOGPORTSTREAMOWNER_SERVER_H_
#define _LOGPORTSTREAMOWNER_SERVER_H_
#ifdef __cplusplus
extern "C" {
#endif
#include <clXdrApi.h>
#include <clIdlApi.h>
#include <ipi/clRmdIpi.h>
#include "../clientIDDefinitions.h"
#include "xdrClLogFilterT.h"
#include "xdrClLogStreamAttrIDLT.h"
#include "xdrClLogCompDataT.h"



ClRcT clLogStreamOwnerStreamOpen_4_0_0(CL_IN ClUint8T  logOpenFlags, CL_IN ClUint32T  nodeAddr, CL_INOUT ClNameT*  pStreamName, CL_INOUT ClUint32T*  pStreamScope, CL_INOUT ClNameT*  pStreamScopeNode, CL_INOUT ClUint32T*  compId, CL_INOUT ClLogStreamAttrIDLT_4_0_0*  pStreamAttr, CL_OUT ClUint64T*  pStreamMastAddr, CL_OUT ClLogFilterT_4_0_0*  pStreamFilter, CL_OUT ClUint32T*  pAckerCnt, CL_OUT ClUint32T*  pNonAckerCnt, CL_INOUT ClUint16T*  pStreamId);

ClRcT clLogStreamOwnerStreamOpenResponseSend_4_0_0(CL_IN ClIdlHandleT idlHdl,CL_IN ClRcT retCode,CL_INOUT  ClNameT  pStreamName,CL_INOUT  ClUint32T  pStreamScope,CL_INOUT  ClNameT  pStreamScopeNode,CL_INOUT  ClUint32T  compId,CL_INOUT  ClLogStreamAttrIDLT_4_0_0  pStreamAttr,CL_OUT  ClUint64T  pStreamMastAddr,CL_OUT  ClLogFilterT_4_0_0  pStreamFilter,CL_OUT  ClUint32T  pAckerCnt,CL_OUT  ClUint32T  pNonAckerCnt,CL_INOUT  ClUint16T  pStreamId);

ClRcT clLogStreamOwnerStreamClose_4_0_0(CL_IN ClNameT*  pStreamName, CL_IN ClUint32T  pStreamScope, CL_IN ClNameT*  pStreamScopeNode, CL_IN ClUint32T  nodeAddress, CL_IN ClUint32T  compId);

ClRcT clLogStreamOwnerStreamCloseResponseSend_4_0_0(CL_IN ClIdlHandleT idlHdl,CL_IN ClRcT retCode);

ClRcT clLogStreamOwnerFilterSet_4_0_0(CL_IN ClNameT*  pStreamName, CL_IN ClUint32T  pStreamScope, CL_IN ClNameT*  pStreamScopeNode, CL_IN ClUint8T  filterFlags, CL_IN ClLogFilterT_4_0_0*  pFilter);

ClRcT clLogStreamOwnerFilterSetResponseSend_4_0_0(CL_IN ClIdlHandleT idlHdl,CL_IN ClRcT retCode);

ClRcT clLogStreamOwnerHandlerRegister_4_0_0(CL_IN ClNameT*  pStreamName, CL_IN ClUint32T  pStreamScope, CL_IN ClNameT*  pStreamScopeNode, CL_IN ClUint8T  handlerFlags, CL_IN ClUint32T  localAddr, CL_IN ClUint32T  compId);

ClRcT clLogStreamOwnerHandlerRegisterResponseSend_4_0_0(CL_IN ClIdlHandleT idlHdl,CL_IN ClRcT retCode);

ClRcT clLogStreamOwnerStreamMcastGet_4_0_0(CL_IN ClNameT*  pStreamName, CL_IN ClUint32T  pStreamScope, CL_IN ClNameT*  pStreamScopeNode, CL_OUT ClUint64T*  mcastAddr);

ClRcT clLogStreamOwnerStreamMcastGetResponseSend_4_0_0(CL_IN ClIdlHandleT idlHdl,CL_IN ClRcT retCode,CL_OUT  ClUint64T  mcastAddr);

ClRcT clLogStreamOwnerHandlerDeregister_4_0_0(CL_IN ClNameT*  pStreamName, CL_IN ClUint32T  pStreamScope, CL_IN ClNameT*  pStreamScopeNode, CL_IN ClUint8T  handlerFlags, CL_IN ClUint32T  localAddr, CL_IN ClUint32T  compId);

ClRcT clLogStreamOwnerHandlerDeregisterResponseSend_4_0_0(CL_IN ClIdlHandleT idlHdl,CL_IN ClRcT retCode);

ClRcT clLogStreamOwnerFilterGet_4_0_0(CL_IN ClNameT*  pStreamName, CL_IN ClUint32T  pStreamScope, CL_IN ClNameT*  pStreamScopeNode, CL_OUT ClLogFilterT_4_0_0*  pFilter);

ClRcT clLogStreamOwnerFilterGetResponseSend_4_0_0(CL_IN ClIdlHandleT idlHdl,CL_IN ClRcT retCode,CL_OUT  ClLogFilterT_4_0_0  pFilter);


#ifdef __cplusplus
}
#endif
#endif /*_LOGPORTSTREAMOWNER_SERVER_H_*/