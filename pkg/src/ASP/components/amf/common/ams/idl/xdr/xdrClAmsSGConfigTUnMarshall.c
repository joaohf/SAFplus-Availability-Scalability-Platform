/*
 * Copyright (C) 2002-2009 by OpenClovis Inc. All  Rights Reserved.
 * 
 * The source code for  this program is not published  or otherwise 
 * divested of  its trade secrets, irrespective  of  what  has been 
 * deposited with the U.S. Copyright office.
 * 
 * This program is  free software; you can redistribute it and / or
 * modify  it under  the  terms  of  the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 * 
 * This program is distributed in the  hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied  warranty  of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 * 
 * You  should  have  received  a  copy of  the  GNU General Public
 * License along  with  this program. If  not,  write  to  the 
 * Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 */


/*********************************************************************
* ModuleName  : idl
*********************************************************************/
/*********************************************************************
* Description : Unmarshall routine for ClAmsSGConfigT
*     
* THIS FILE IS AUTO-GENERATED BY OPENCLOVIS IDE. EDIT THIS FILE AT
* YOUR OWN RISK. ANY CHANGE TO THIS FILE WILL BE OVERWRITTEN ON
* RE-GENERATION.
*     
*********************************************************************/
#include "xdrClAmsSGConfigT.h"

ClRcT clXdrUnmarshallClAmsSGConfigT_4_0_0(ClBufferHandleT msg , void* pGenVar)
{
    ClAmsSGConfigT_4_0_0* pVar = (ClAmsSGConfigT_4_0_0*)pGenVar;
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

    rc = clXdrUnmarshallClAmsEntityConfigT_4_0_0(msg,&(pVar->entity));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClAmsAdminStateT_4_0_0(msg,&(pVar->adminState));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClAmsSGRedundancyModelT_4_0_0(msg,&(pVar->redundancyModel));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClAmsSGLoadingStrategyT_4_0_0(msg,&(pVar->loadingStrategy));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClUint16T(msg,&(pVar->failbackOption));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClUint16T(msg,&(pVar->autoRepair));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClInt64T(msg,&(pVar->instantiateDuration));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClUint32T(msg,&(pVar->numPrefActiveSUs));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClUint32T(msg,&(pVar->numPrefStandbySUs));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClUint32T(msg,&(pVar->numPrefInserviceSUs));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClUint32T(msg,&(pVar->numPrefAssignedSUs));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClUint32T(msg,&(pVar->numPrefActiveSUsPerSI));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClUint32T(msg,&(pVar->maxActiveSIsPerSU));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClUint32T(msg,&(pVar->maxStandbySIsPerSU));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClInt64T(msg,&(pVar->compRestartDuration));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClUint32T(msg,&(pVar->compRestartCountMax));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClInt64T(msg,&(pVar->suRestartDuration));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClUint32T(msg,&(pVar->suRestartCountMax));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClUint16T(msg,&(pVar->isCollocationAllowed));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClUint32T(msg,&(pVar->alpha));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClUint16T(msg,&(pVar->autoAdjust));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClInt64T(msg,&(pVar->autoAdjustProbation));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClUint16T(msg,&(pVar->reductionProcedure));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClAmsEntityConfigT_4_0_0(msg,&(pVar->parentApp));
    if (CL_OK != rc)
    {
        return rc;
    }

    }

    return rc;
}


ClRcT clXdrUnmarshallClAmsSGConfigT_4_1_0(ClBufferHandleT msg , void* pGenVar)
{
    ClAmsSGConfigT_4_1_0* pVar = (ClAmsSGConfigT_4_1_0*)pGenVar;
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

    rc = clXdrUnmarshallClAmsEntityConfigT_4_0_0(msg,&(pVar->entity));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClAmsAdminStateT_4_0_0(msg,&(pVar->adminState));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClAmsSGRedundancyModelT_4_0_0(msg,&(pVar->redundancyModel));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClAmsSGLoadingStrategyT_4_0_0(msg,&(pVar->loadingStrategy));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClUint16T(msg,&(pVar->failbackOption));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClUint16T(msg,&(pVar->autoRepair));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClInt64T(msg,&(pVar->instantiateDuration));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClUint32T(msg,&(pVar->numPrefActiveSUs));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClUint32T(msg,&(pVar->numPrefStandbySUs));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClUint32T(msg,&(pVar->numPrefInserviceSUs));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClUint32T(msg,&(pVar->numPrefAssignedSUs));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClUint32T(msg,&(pVar->numPrefActiveSUsPerSI));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClUint32T(msg,&(pVar->maxActiveSIsPerSU));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClUint32T(msg,&(pVar->maxStandbySIsPerSU));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClInt64T(msg,&(pVar->compRestartDuration));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClUint32T(msg,&(pVar->compRestartCountMax));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClInt64T(msg,&(pVar->suRestartDuration));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClUint32T(msg,&(pVar->suRestartCountMax));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClUint16T(msg,&(pVar->isCollocationAllowed));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClUint32T(msg,&(pVar->alpha));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClUint16T(msg,&(pVar->autoAdjust));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClInt64T(msg,&(pVar->autoAdjustProbation));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClUint16T(msg,&(pVar->reductionProcedure));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClAmsEntityConfigT_4_0_0(msg,&(pVar->parentApp));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClUint32T(msg,&(pVar->maxFailovers));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClInt64T(msg,&(pVar->failoverDuration));
    if (CL_OK != rc)
    {
        return rc;
    }

    }

    return rc;
}


ClRcT clXdrUnmarshallClAmsSGConfigT_5_0_0(ClBufferHandleT msg , void* pGenVar)
{
    ClAmsSGConfigT_5_0_0* pVar = (ClAmsSGConfigT_5_0_0*)pGenVar;
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

    rc = clXdrUnmarshallClAmsEntityConfigT_4_0_0(msg,&(pVar->entity));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClAmsAdminStateT_4_0_0(msg,&(pVar->adminState));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClAmsSGRedundancyModelT_4_0_0(msg,&(pVar->redundancyModel));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClAmsSGLoadingStrategyT_4_0_0(msg,&(pVar->loadingStrategy));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClUint16T(msg,&(pVar->failbackOption));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClUint16T(msg,&(pVar->autoRepair));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClInt64T(msg,&(pVar->instantiateDuration));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClUint32T(msg,&(pVar->numPrefActiveSUs));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClUint32T(msg,&(pVar->numPrefStandbySUs));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClUint32T(msg,&(pVar->numPrefInserviceSUs));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClUint32T(msg,&(pVar->numPrefAssignedSUs));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClUint32T(msg,&(pVar->numPrefActiveSUsPerSI));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClUint32T(msg,&(pVar->maxActiveSIsPerSU));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClUint32T(msg,&(pVar->maxStandbySIsPerSU));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClInt64T(msg,&(pVar->compRestartDuration));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClUint32T(msg,&(pVar->compRestartCountMax));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClInt64T(msg,&(pVar->suRestartDuration));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClUint32T(msg,&(pVar->suRestartCountMax));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClUint16T(msg,&(pVar->isCollocationAllowed));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClUint32T(msg,&(pVar->alpha));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClUint16T(msg,&(pVar->autoAdjust));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClInt64T(msg,&(pVar->autoAdjustProbation));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClUint16T(msg,&(pVar->reductionProcedure));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClAmsEntityConfigT_4_0_0(msg,&(pVar->parentApp));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClUint32T(msg,&(pVar->maxFailovers));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClInt64T(msg,&(pVar->failoverDuration));
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrUnmarshallClUint32T(msg,&(pVar->beta));
    if (CL_OK != rc)
    {
        return rc;
    }

    }

    return rc;
}

