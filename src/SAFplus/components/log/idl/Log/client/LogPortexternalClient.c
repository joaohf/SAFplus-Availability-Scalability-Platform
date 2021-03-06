
/*********************************************************************
* ModuleName  : idl
*********************************************************************/
/*********************************************************************
* Description :ClientSide Stub routines
*
* THIS FILE IS AUTO-GENERATED BY OPENCLOVIS IDE. EDIT THIS FILE AT
* YOUR OWN RISK. ANY CHANGE TO THIS FILE WILL BE OVERWRITTEN ON
* RE-GENERATION.
*
*********************************************************************/

#include <netinet/in.h>
#include <string.h>
#include <clBufferApi.h>
#include <clRmdApi.h>
#include <clIdlApi.h>
#include <clEoApi.h>
#include <clXdrApi.h>
#include <clHandleApi.h>
#include "LogPortexternalClient.h"
extern ClIdlClntT gIdlClnt;


ClRcT clLogExternalSendClientSync_4_0_0(CL_IN ClIdlHandleT handle, CL_IN ClUint16T recsize, CL_IN ClUint8T* pRecord, CL_IN SaNameT* pStreamName, CL_IN SaNameT* pStreamScopeNode)
{
    ClRcT rc = CL_OK;
    ClVersionT funcVer = {4, 0, 0};
    ClUint32T funcNo = CL_EO_GET_FULL_FN_NUM(CL_EO_NATIVE_COMPONENT_TABLE_ID, 21);
    ClBufferHandleT inMsgHdl = 0;
    ClBufferHandleT outMsgHdl = 0;
    ClIocAddressT address = {{0}};
    ClIdlHandleObjT* pHandleObj = NULL;
    ClUint32T tempFlags = 0;

    rc = clHandleCheckout(gIdlClnt.idlDbHdl,handle,(void **)&pHandleObj);
    if( rc != CL_OK )
    {
        return rc ;
    }
    if (CL_IDL_ADDRESSTYPE_IOC == pHandleObj->address.addressType)
    {
        address = pHandleObj->address.address.iocAddress;
    }
    else if (CL_IDL_ADDRESSTYPE_NAME == pHandleObj->address.addressType)
    {
        rc = clNameToObjectReferenceGet(&(pHandleObj->address.address.nameAddress.name),
                                        pHandleObj->address.address.nameAddress.attrCount,
                                        pHandleObj->address.address.nameAddress.attr,
                                        pHandleObj->address.address.nameAddress.contextCookie,
                                        (ClUint64T*)&address);
        if (CL_OK != rc)
        {
            return rc;
        }
    }
    else
    {
        return CL_IDL_RC(CL_ERR_INVALID_PARAMETER);
    }

    rc = clBufferCreate(&inMsgHdl);
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrMarshallClUint16T(&(recsize), inMsgHdl, 0);
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrMarshallSaNameT(pStreamName, inMsgHdl, 0);
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrMarshallSaNameT(pStreamScopeNode, inMsgHdl, 0);
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrMarshallArrayClUint8T(pRecord, recsize, inMsgHdl, 0);
    if (CL_OK != rc)
    {
        return rc;
    }


    tempFlags |= pHandleObj->flags |
                 (CL_RMD_CALL_NON_PERSISTENT);
    tempFlags &= ~CL_RMD_CALL_ASYNC;

    rc = clRmdWithMsgVer(address, &funcVer, funcNo, inMsgHdl, outMsgHdl, tempFlags, &(pHandleObj->options), NULL);
    if(CL_OK != rc)
    {
        return rc;
    }


    
    rc = clHandleCheckin(gIdlClnt.idlDbHdl,handle);
    return rc;
}


static void clLogExternalSendAsyncCallback_4_0_0(ClRcT rc, void *pIdlCookie, ClBufferHandleT inMsgHdl, ClBufferHandleT outMsgHdl)
{
    ClIdlCookieT* pCookie = (ClIdlCookieT*)pIdlCookie;
    ClRcT retVal = CL_OK;
    ClUint16T  recsize;
    ClUint8T* pRecord;
    SaNameT  pStreamName;
    SaNameT  pStreamScopeNode;

    memset(&(recsize), 0, sizeof(ClUint16T));
    memset(&(pRecord), 0, sizeof(ClUint8T*));
    memset(&(pStreamName), 0, sizeof(SaNameT));
    memset(&(pStreamScopeNode), 0, sizeof(SaNameT));


    retVal = clXdrUnmarshallClUint16T(inMsgHdl, &(recsize));
    if (CL_OK != retVal)
    {
        goto L0;
    }

    retVal = clXdrUnmarshallSaNameT(inMsgHdl, &(pStreamName));
    if (CL_OK != retVal)
    {
        goto L1;
    }

    retVal = clXdrUnmarshallSaNameT(inMsgHdl, &(pStreamScopeNode));
    if (CL_OK != retVal)
    {
        goto L2;
    }

    retVal = clXdrUnmarshallPtrClUint8T(inMsgHdl, (void**)&(pRecord), recsize);
    if (CL_OK != retVal)
    {
        goto L3;
    }

    if (rc != CL_OK)
    {
        retVal = rc;
    }

    ((LogClLogExternalSendAsyncCallbackT_4_0_0)(pCookie->actualCallback))(pCookie->handle, recsize, pRecord, &(pStreamName), &(pStreamScopeNode), retVal, pCookie->pCookie);
    goto L4;

L4: 
L3: 
L2: 
L1: 

L0:  clHeapFree(pCookie);
     clBufferDelete(&outMsgHdl);
     return;
}


ClRcT clLogExternalSendClientAsync_4_0_0(CL_IN ClIdlHandleT handle, CL_IN ClUint16T  recsize, CL_IN ClUint8T* pRecord, CL_IN SaNameT* pStreamName, CL_IN SaNameT* pStreamScopeNode,CL_IN LogClLogExternalSendAsyncCallbackT_4_0_0 fpAsyncCallback, CL_IN void *cookie)
{
    ClRcT rc = CL_OK;
    ClVersionT funcVer = {4, 0, 0};
    ClUint32T funcNo = CL_EO_GET_FULL_FN_NUM(CL_EO_NATIVE_COMPONENT_TABLE_ID, 21);
    ClBufferHandleT inMsgHdl = 0;
    ClBufferHandleT outMsgHdl = 0;
    ClIocAddressT address = {{0}};
    ClIdlHandleObjT* pHandleObj = NULL;
    ClRmdAsyncOptionsT asyncOptions;
    ClUint32T tempFlags = 0;
    ClIdlCookieT* pCookie = NULL;

    rc = clHandleCheckout(gIdlClnt.idlDbHdl,handle,(void **)&pHandleObj);
    if(rc != CL_OK)
    {
        return rc;
    }
    if (CL_IDL_ADDRESSTYPE_IOC == pHandleObj->address.addressType)
    {
        address = pHandleObj->address.address.iocAddress;
    }
    else if (CL_IDL_ADDRESSTYPE_NAME == pHandleObj->address.addressType)
    {
        rc = clNameToObjectReferenceGet(&(pHandleObj->address.address.nameAddress.name),
                                        pHandleObj->address.address.nameAddress.attrCount,
                                        pHandleObj->address.address.nameAddress.attr,
                                        pHandleObj->address.address.nameAddress.contextCookie,
                                        (ClUint64T*)&address);
        if (CL_OK != rc)
        {
            goto L;
        }
    }
    else
    {
        rc = CL_IDL_RC(CL_ERR_INVALID_PARAMETER);
        goto L;
    }

    rc = clBufferCreate(&inMsgHdl);
    if (CL_OK != rc)
    {
        goto L;
    }

    rc = clXdrMarshallClUint16T(&(recsize), inMsgHdl, 0);
    if (CL_OK != rc)
    {
        goto L;
    }

    rc = clXdrMarshallSaNameT(pStreamName, inMsgHdl, 0);
    if (CL_OK != rc)
    {
        goto L;
    }

    rc = clXdrMarshallSaNameT(pStreamScopeNode, inMsgHdl, 0);
    if (CL_OK != rc)
    {
        goto L;
    }

    rc = clXdrMarshallArrayClUint8T(pRecord, recsize, inMsgHdl, 0);
    if (CL_OK != rc)
    {
        goto L;
    }

    if(fpAsyncCallback != NULL)
    {
        

        pCookie = (ClIdlCookieT*) clHeapAllocate(sizeof(ClIdlCookieT));
        if (NULL == pCookie)
        {
            return CL_IDL_RC(CL_ERR_NO_MEMORY);
        }
        
        asyncOptions.pCookie = NULL;
        asyncOptions.fpCallback = NULL;
        
        rc = clBufferCreate(&outMsgHdl);
        if (CL_OK != rc)
        {
            goto L2;
        }

        tempFlags |= pHandleObj->flags |
                     (CL_RMD_CALL_ASYNC | CL_RMD_CALL_NON_PERSISTENT | CL_RMD_CALL_NEED_REPLY);
        
        pCookie->pCookie = cookie;
        pCookie->actualCallback = (void(*)())fpAsyncCallback;
        pCookie->handle = handle;
        asyncOptions.pCookie = pCookie;
        asyncOptions.fpCallback = clLogExternalSendAsyncCallback_4_0_0;

        rc = clRmdWithMsgVer(address, &funcVer, funcNo, inMsgHdl, outMsgHdl, tempFlags, &(pHandleObj->options), &asyncOptions);
        if (CL_OK != rc)
        {
            goto LL;
         }
    }
    else
    {
        tempFlags |= pHandleObj->flags |
                         (CL_RMD_CALL_ASYNC | CL_RMD_CALL_NON_PERSISTENT);
        rc = clRmdWithMsgVer(address, &funcVer, funcNo, inMsgHdl, 0, tempFlags, &(pHandleObj->options),NULL);
        if(CL_OK != rc)
        {
               goto L;
        }
    }
    
    
    clHandleCheckin(gIdlClnt.idlDbHdl,handle);
    return rc;

LL: clBufferDelete(&outMsgHdl);
L2:  clHeapFree(pCookie);
L:
     clHandleCheckin(gIdlClnt.idlDbHdl,handle);
    return rc;
}


