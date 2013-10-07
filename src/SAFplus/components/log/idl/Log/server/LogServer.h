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

#ifndef _LOG_SERVER_H_
#define _LOG_SERVER_H_
#ifdef __cplusplus
extern "C" {
#endif
#include <clXdrApi.h>
#include <clIdlApi.h>
#include <ipi/clRmdIpi.h>
#include "../clientIDDefinitions.h"
#include "xdrClLogStreamMapT.h"
#include "xdrClLogFilterT.h"
#include "xdrClLogStreamAttrIDLT.h"
#include "xdrClLogCompDataT.h"



ClRcT clLogIdlSyncDefer(ClIdlHandleT *pIdlHdl);
        
ClRcT clLogClientInstall(void);

ClRcT clLogClientUninstall(void);

extern ClRcT clLogMasterAttrVerifyNGetServer_4_0_0(ClEoDataT eoData, ClBufferHandleT inmSgHdl , ClBufferHandleT outMsgHdl);
extern ClRcT clLogMasterStreamCloseNotifyServer_4_0_0(ClEoDataT eoData, ClBufferHandleT inmSgHdl , ClBufferHandleT outMsgHdl);
extern ClRcT clLogMasterStreamListGetServer_4_0_0(ClEoDataT eoData, ClBufferHandleT inmSgHdl , ClBufferHandleT outMsgHdl);
extern ClRcT clLogMasterCompIdChkNGetServer_4_0_0(ClEoDataT eoData, ClBufferHandleT inmSgHdl , ClBufferHandleT outMsgHdl);
extern ClRcT clLogMasterCompListGetServer_4_0_0(ClEoDataT eoData, ClBufferHandleT inmSgHdl , ClBufferHandleT outMsgHdl);
extern ClRcT clLogMasterCompListNotifyServer_4_0_0(ClEoDataT eoData, ClBufferHandleT inmSgHdl , ClBufferHandleT outMsgHdl);
extern ClRcT clLogStreamOwnerStreamOpenServer_4_0_0(ClEoDataT eoData, ClBufferHandleT inmSgHdl , ClBufferHandleT outMsgHdl);
extern ClRcT clLogStreamOwnerStreamCloseServer_4_0_0(ClEoDataT eoData, ClBufferHandleT inmSgHdl , ClBufferHandleT outMsgHdl);
extern ClRcT clLogStreamOwnerFilterSetServer_4_0_0(ClEoDataT eoData, ClBufferHandleT inmSgHdl , ClBufferHandleT outMsgHdl);
extern ClRcT clLogStreamOwnerHandlerRegisterServer_4_0_0(ClEoDataT eoData, ClBufferHandleT inmSgHdl , ClBufferHandleT outMsgHdl);
extern ClRcT clLogStreamOwnerStreamMcastGetServer_4_0_0(ClEoDataT eoData, ClBufferHandleT inmSgHdl , ClBufferHandleT outMsgHdl);
extern ClRcT clLogStreamOwnerHandlerDeregisterServer_4_0_0(ClEoDataT eoData, ClBufferHandleT inmSgHdl , ClBufferHandleT outMsgHdl);
extern ClRcT clLogStreamOwnerFilterGetServer_4_0_0(ClEoDataT eoData, ClBufferHandleT inmSgHdl , ClBufferHandleT outMsgHdl);
extern ClRcT clLogSvrStreamOpenServer_4_0_0(ClEoDataT eoData, ClBufferHandleT inmSgHdl , ClBufferHandleT outMsgHdl);
extern ClRcT clLogSvrStreamCloseServer_4_0_0(ClEoDataT eoData, ClBufferHandleT inmSgHdl , ClBufferHandleT outMsgHdl);
extern ClRcT clLogSvrFilterSetServer_4_0_0(ClEoDataT eoData, ClBufferHandleT inmSgHdl , ClBufferHandleT outMsgHdl);
extern ClRcT clLogSvrStreamHandleFlagsUpdateServer_4_0_0(ClEoDataT eoData, ClBufferHandleT inmSgHdl , ClBufferHandleT outMsgHdl);
extern ClRcT clLogHandlerSvrAckSendServer_4_0_0(ClEoDataT eoData, ClBufferHandleT inmSgHdl , ClBufferHandleT outMsgHdl);
extern ClRcT clLogFileHdlrFileOpenServer_4_0_0(ClEoDataT eoData, ClBufferHandleT inmSgHdl , ClBufferHandleT outMsgHdl);
extern ClRcT clLogFileHdlrFileRecordsGetServer_4_0_0(ClEoDataT eoData, ClBufferHandleT inmSgHdl , ClBufferHandleT outMsgHdl);
extern ClRcT clLogFileHdlrFileMetaDataGetServer_4_0_0(ClEoDataT eoData, ClBufferHandleT inmSgHdl , ClBufferHandleT outMsgHdl);
extern ClRcT clLogExternalSendServer_4_0_0(ClEoDataT eoData, ClBufferHandleT inmSgHdl , ClBufferHandleT outMsgHdl);


#ifdef __cplusplus
}
#endif
#endif /*_LOG_SERVER_H_*/
