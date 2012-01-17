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
*               and unmarshall funtions of CkptCPInfoT 
*     
* THIS FILE IS AUTO-GENERATED BY OPENCLOVIS IDE. EDIT THIS FILE AT
* YOUR OWN RISK. ANY CHANGE TO THIS FILE WILL BE OVERWRITTEN ON
* RE-GENERATION.
*     
*********************************************************************/

#ifndef _XDR_CKPT_CP_INFO_T_H_
#define _XDR_CKPT_CP_INFO_T_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "clXdrApi.h"

#include "clCkptApi.h"
#include "clCkptCommon.h"
#include "xdrClCkptAppInfoT.h"

struct _CkptCPInfoT_4_0_0;

typedef struct _CkptCPInfoT_4_0_0 {
    ClUint32T    updateOption;
    ClUint32T    size;
    ClUint32T    numApps;
    ClUint32T*    presenceList;
    ClCkptAppInfoT_4_0_0*    pAppInfo;

}CkptCPInfoT_4_0_0;


ClRcT  clXdrMarshallCkptCPInfoT_4_0_0(void *,ClBufferHandleT , ClUint32T);

ClRcT  clXdrUnmarshallCkptCPInfoT_4_0_0(ClBufferHandleT, void *);

#define clXdrMarshallArrayCkptCPInfoT_4_0_0(pointer, multiplicity, msg, isDelete) \
clXdrMarshallArray((pointer), sizeof(CkptCPInfoT_4_0_0), (multiplicity), clXdrMarshallCkptCPInfoT_4_0_0, (msg), (isDelete))

#define clXdrUnmarshallArrayCkptCPInfoT_4_0_0(msg,pointer, multiplicity) \
clXdrUnmarshallArray((msg),(pointer), sizeof(CkptCPInfoT_4_0_0), (multiplicity), clXdrUnmarshallCkptCPInfoT_4_0_0)

#define clXdrMarshallPointerCkptCPInfoT_4_0_0(pointer, multiplicity, msg, isDelete) \
clXdrMarshallPointer((pointer), sizeof(CkptCPInfoT_4_0_0), (multiplicity), clXdrMarshallCkptCPInfoT_4_0_0, (msg), (isDelete))

#define clXdrUnmarshallPointerCkptCPInfoT_4_0_0(msg,pointer) \
clXdrUnmarshallPointer((msg),(pointer), sizeof(CkptCPInfoT_4_0_0), clXdrUnmarshallCkptCPInfoT_4_0_0)

#define clXdrMarshallPtrCkptCPInfoT_4_0_0(pointer, multiplicity, msg, isDelete) \
clXdrMarshallPtr((pointer), sizeof(CkptCPInfoT_4_0_0), (multiplicity), clXdrMarshallCkptCPInfoT_4_0_0, (msg), (isDelete))

#define clXdrUnmarshallPtrCkptCPInfoT_4_0_0(msg,pointer,multiplicity) \
clXdrUnmarshallPtr((msg),(pointer), sizeof(CkptCPInfoT_4_0_0),multiplicity, clXdrUnmarshallCkptCPInfoT_4_0_0)


typedef CkptCPInfoT_4_0_0 CkptCPInfoT;



#ifdef __cplusplus
}
#endif

#endif /*_XDR_CKPT_CP_INFO_T_H_*/
