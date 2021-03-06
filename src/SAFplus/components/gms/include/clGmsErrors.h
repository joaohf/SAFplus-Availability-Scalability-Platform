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
 * ModuleName  : gms
 * File        : clGmsErrors.h
 *******************************************************************************/

/*******************************************************************************
 * Description :
 * Error codes returned by the GMS library.
 *
 *
 *****************************************************************************/

/**
 *  \file
 *  \brief Header file of Error Codes returned by the Group Membership Service.
 *  \ingroup gms_apis
 */

/**
 *  \addtogroup gms_apis
 *  \{
 */

#ifndef _CL_GMS_ERRORS_H_
#define _CL_GMS_ERRORS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <clCommon.h>
#include <clCommonErrors.h>

/******************************************************************************
 * ERROR CODES
 *****************************************************************************/
/**
 * Unmarshalling RMD Reply failed.
 */
#define CL_GMS_ERR_UNMARSHALING_FAILED          0x100

/**
 * An error found while performing operation on openais interface
 */
#define CL_GMS_ERR_TOTEM_PROTOCOL_ERROR         0x101

/**
 * The config file paramer is invalid(NOT used)
 */
#define CL_GMS_ERR_CONFIG_FILE_INVALID          0x102

/**
 * Requested group doesnt exist.
 */
#define CL_GMS_ERR_GROUP_DOESNT_EXIST           0x103

/**
 * The group is empty (INCOMPLETE Usage)
 */
#define CL_GMS_ERR_EMPTY_GROUP                  0x104

/**
 * IOC registration of the group member failed.
 */
#define CL_GMS_ERR_IOC_REGISTRATION             0x105

/**
 * IOC Deregistration of the group member failed
 */
#define CL_GMS_ERR_IOC_DEREGISTRATION           0x106

/**
 * If the joining node is version incompatible with that of the other
 * nodes in the cluster . 
 */
#define CL_GMS_ERR_CLUSTER_VERSION_MISMATCH      0x107

/******************************************************************************
 * ERROR/RETRUN CODE HANDLING MACROS
 *****************************************************************************/
#define CL_GMS_RC(ERROR_CODE)  CL_RC(CL_CID_GMS, (ERROR_CODE))

#ifdef __cplusplus
}
#endif

#endif /*  _CL_GMS_ERRORS_H_ */

/** \} */
