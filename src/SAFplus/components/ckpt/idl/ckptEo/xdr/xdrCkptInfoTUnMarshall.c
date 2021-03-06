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


/*********************************************************************
* ModuleName  : idl
*********************************************************************/
/*********************************************************************
* Description : Unmarshall routine for CkptInfoT
*     
* THIS FILE IS AUTO-GENERATED BY OPENCLOVIS IDE. EDIT THIS FILE AT
* YOUR OWN RISK. ANY CHANGE TO THIS FILE WILL BE OVERWRITTEN ON
* RE-GENERATION.
*     
*********************************************************************/
#include "xdrCkptInfoT.h"

ClRcT clXdrUnmarshallCkptInfoT_4_0_0(ClBufferHandleT msg , void* pGenVar)
{
    CkptInfoT_4_0_0* pVar = (CkptInfoT_4_0_0*)pGenVar;
    ClRcT     rc     = CL_OK;
    ClUint32T length = 0;

    if ((void*)0 == pVar)
    {
        return CL_XDR_RC(CL_ERR_NULL_POINTER);
    }

    clXdrUnmarshallClUint32T(msg, &length);
    if( 0 == length)
    {
        pGenVar = NULL;
    }
    else
    {

    rc = clXdrUnmarshallClHandleT(msg,&(pVar->ckptHdl));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallPtrSaNameT(msg,(void**)&(pVar->pName),1);
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallPtrCkptCPInfoT_4_0_0(msg,(void**)&(pVar->pCpInfo),1);
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallPtrCkptDPInfoT_4_0_0(msg,(void**)&(pVar->pDpInfo),1);
    if (CL_OK != rc)
    {
        return rc;
    }

    }

    return rc;
}


ClRcT clXdrUnmarshallCkptInfoT_5_0_0(ClBufferHandleT msg , void* pGenVar)
{
    CkptInfoT_5_0_0* pVar = (CkptInfoT_5_0_0*)pGenVar;
    ClRcT     rc     = CL_OK;
    ClUint32T length = 0;

    if ((void*)0 == pVar)
    {
        return CL_XDR_RC(CL_ERR_NULL_POINTER);
    }

    clXdrUnmarshallClUint32T(msg, &length);
    if( 0 == length)
    {
        pGenVar = NULL;
    }
    else
    {

    rc = clXdrUnmarshallClHandleT(msg,&(pVar->ckptHdl));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallPtrSaNameT(msg,(void**)&(pVar->pName),1);
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallPtrCkptCPInfoT_5_0_0(msg,(void**)&(pVar->pCpInfo),1);
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallPtrCkptDPInfoT_4_0_0(msg,(void**)&(pVar->pDpInfo),1);
    if (CL_OK != rc)
    {
        return rc;
    }

    }

    return rc;
}


