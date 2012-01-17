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
* Description : This file contains the declarations for marshall 
*               and unmarshall funtions of CkptMasterDBEntryIDLT 
*     
* THIS FILE IS AUTO-GENERATED BY OPENCLOVIS IDE. EDIT THIS FILE AT
* YOUR OWN RISK. ANY CHANGE TO THIS FILE WILL BE OVERWRITTEN ON
* RE-GENERATION.
*     
*********************************************************************/

#ifndef _XDR_CKPT_MASTER_DB_ENTRY_IDLT_H_
#define _XDR_CKPT_MASTER_DB_ENTRY_IDLT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "clXdrApi.h"

#include "clCkptApi.h"
#include "clCkptCommon.h"
#include "xdrClCkptCheckpointCreationAttributesT.h"

struct _CkptMasterDBEntryIDLT_4_0_0;

typedef struct _CkptMasterDBEntryIDLT_4_0_0 {
    ClHandleT    ckptMasterHdl;
    ClNameT    name;
    ClCkptCheckpointCreationAttributesT_4_0_0    attrib;
    ClUint32T    refCount;
    ClInt8T    markedDelete;
    ClHandleT    activeRepHdl;
    ClUint32T    activeRepAddr;
    ClUint32T    prevActiveRepAddr;
    ClUint32T    replicaCount;
    ClUint32T*    replicaListInfo;

}CkptMasterDBEntryIDLT_4_0_0;


ClRcT  clXdrMarshallCkptMasterDBEntryIDLT_4_0_0(void *,ClBufferHandleT , ClUint32T);

ClRcT  clXdrUnmarshallCkptMasterDBEntryIDLT_4_0_0(ClBufferHandleT, void *);

#define clXdrMarshallArrayCkptMasterDBEntryIDLT_4_0_0(pointer, multiplicity, msg, isDelete) \
clXdrMarshallArray((pointer), sizeof(CkptMasterDBEntryIDLT_4_0_0), (multiplicity), clXdrMarshallCkptMasterDBEntryIDLT_4_0_0, (msg), (isDelete))

#define clXdrUnmarshallArrayCkptMasterDBEntryIDLT_4_0_0(msg,pointer, multiplicity) \
clXdrUnmarshallArray((msg),(pointer), sizeof(CkptMasterDBEntryIDLT_4_0_0), (multiplicity), clXdrUnmarshallCkptMasterDBEntryIDLT_4_0_0)

#define clXdrMarshallPointerCkptMasterDBEntryIDLT_4_0_0(pointer, multiplicity, msg, isDelete) \
clXdrMarshallPointer((pointer), sizeof(CkptMasterDBEntryIDLT_4_0_0), (multiplicity), clXdrMarshallCkptMasterDBEntryIDLT_4_0_0, (msg), (isDelete))

#define clXdrUnmarshallPointerCkptMasterDBEntryIDLT_4_0_0(msg,pointer) \
clXdrUnmarshallPointer((msg),(pointer), sizeof(CkptMasterDBEntryIDLT_4_0_0), clXdrUnmarshallCkptMasterDBEntryIDLT_4_0_0)

#define clXdrMarshallPtrCkptMasterDBEntryIDLT_4_0_0(pointer, multiplicity, msg, isDelete) \
clXdrMarshallPtr((pointer), sizeof(CkptMasterDBEntryIDLT_4_0_0), (multiplicity), clXdrMarshallCkptMasterDBEntryIDLT_4_0_0, (msg), (isDelete))

#define clXdrUnmarshallPtrCkptMasterDBEntryIDLT_4_0_0(msg,pointer,multiplicity) \
clXdrUnmarshallPtr((msg),(pointer), sizeof(CkptMasterDBEntryIDLT_4_0_0),multiplicity, clXdrUnmarshallCkptMasterDBEntryIDLT_4_0_0)


typedef CkptMasterDBEntryIDLT_4_0_0 CkptMasterDBEntryIDLT;



#ifdef __cplusplus
}
#endif

#endif /*_XDR_CKPT_MASTER_DB_ENTRY_IDLT_H_*/
