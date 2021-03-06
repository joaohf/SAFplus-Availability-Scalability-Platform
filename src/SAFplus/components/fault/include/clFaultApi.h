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
 * ModuleName  : fault
 * File        : clFaultApi.h
 *******************************************************************************/

/*******************************************************************************
 * Description :
 *
 * This file provides the signature of clFaultNotifyRmd and the
 * default escalation function clFaultEscalate along with the
 * fault client library initialization and finalization
 * function prototypes.
 *
 *****************************************************************************/

/*********************************************************************************/
/************************** Fault Manager functions ***********************************/
/*********************************************************************************/
/*                                                                               */
/* clFaultReport                                                     */
/* clFaultRepairAction                                               */
/* clFaultSvcLibInitialize                                           */
/* clFaultSvcLibFinalize                                             */
/* clFaultVersionVerify                                              */
/*                                                                               */
/*********************************************************************************/

#if 0 /* DEPRECATED */

/**
 *  \file
 *  \brief Header file of Fault Related APIs
 *  \ingroup fault_apis
 */

/**
 ************************************
 *  \addtogroup fault_apis
 *  \{
 */

#ifndef _CL_FAULT_API_H_
#define _CL_FAULT_API_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <clCommon.h>
#include <clCorMetaData.h>
#include <clFaultDefinitions.h>
#include <clAmsClientNotification.h>

/******************************************************************************
 *  Constant and Macro Definitions
 *****************************************************************************/


/******************************************************************************
 *   Function IDs for RMD calls
 ******************************************************************************/


/*****************************************************************************
 *  Functions
 *****************************************************************************/

/**
 ************************************
 *  \brief Reports a fault to the Fault service.
 *
 *  \par Header File:
 *  clFaultApi.h
 *
 *  \param compName Name of the component on which the fault occurred.
 *  \param hMoId Handler of the \e MoId of the object on which the fault occurred.
 *  \param alarmState  State of the alarm, either assert or clear.
 *  \param category Category of the fault.
 *  \param specificProblem Specific-Problem of the fault.
 *  The specific problem is a means of segregating the duplicate
 *  probable causes which fall under the same probable cause list
 *  but are different with respect to the application. This information lies
 *  with the user/application and the interpretation solely rests
 *  in its hands.
 *  \param severity Severity of the fault.
 *  \param cause Probable cause of the fault.
 *  \param pData Some additional information about the fault being notified.
 *  Additional information may be some message about the fault which
 *  the application notifies to FM.
 *  \param len Length of \e pData, the additional information.
 *
 *  \retval CL_OK The API executed successfully.
 *  \retval CL_FAULT_ERR_MOID_NULL \e MoId passed to the fault is NULL.
 *  \retval CL_FAULT_ERR_COMPNAME_NULL Component name passed to fault is NULL.
 *  \retval CL_FAULT_ERR_INVALID_CATEGORY Fault is of invalid category. These
 *  categories are mentioned in ITUX.733.
 *  \retval CL_FAULT_ERR_INVALID_SEVERITY Fault is of invalid severity. These
 *  severity levels are mentioned in ITUX.733.
 *  \retval CL_ERR_NO_MEMORY On memory allocation failure.
 *
 *  \par Description:
 *  This function is used to report non-service impacting
 *  alarms that occurred in the system. 
 *  This is a blocking call and will result in the handlers being called.
 *  The handlers are configured during modeling time and corresponds to a combination of
 *  category and severity. Five(5) handlers are configured for each combination
 *  of category and severity. These handlers are assigned sequence nos 1 to 5.
 *  When identical faults corresponding to the same \e category, \e cause, 
 *  \e severity and \e specificProblem are reported within a probation period, 
 *  the faults get escalated. This escalation is provided by invoking the next 
 *  handler in the sequence.
 *  The implementation of the handlers is left to the user.
 *
 *  \par Library File:
 *  ClFaultClient
 *
 *
 */
extern ClRcT clFaultReport(CL_IN SaNameT *compName,
                           CL_IN ClCorMOIdPtrT hMoId,
                           CL_IN ClAlarmStateT alarmState,
                           CL_IN ClAlarmCategoryTypeT category,
                           CL_IN ClAlarmSpecificProblemT specificProblem,
                           CL_IN ClAlarmSeverityTypeT severity,
                           CL_IN ClAlarmProbableCauseT cause,
                           CL_IN void *pData,
                           CL_IN ClUint32T len);

/**
 ************************************
 *  \brief Notifies fault to the "Fault service" for taking repair action.
 *
 *  \par Header File:
 *  clFaultApi.h
 *
 *  \param iocAddress \e IocAddress of the node on which the fault occurred.
 *  \param alarmHandle Alarm Handle which is used to get the corresponding alarm information
 *   and execute the repair handler for the matching \e moclass type
 *  \param recoveryActionTaken Recovery action taken by AMF.
 *
 *  \retval CL_OK The API executed successfully.
 *  \retval CL_ERR_NO_MEMORY On memory allocation failure.
 *  \retval CL_ERR_INVALID_HANDLE The alarmHandle passed is invalid
 *
 *  \par Description:
 *  This function is used by AMF to inform Fault Manager to take repair actions
 *  corresponding to the fault.
 *  These kind of faults are initially raised as service-impacting alarm.
 *  The AMF does service recovery and informs Fault using this API for
 *  subsequent repair action. 
 *  The role played by FM is to execute the repair
 *  handler configured by the user. The repair handlers are configured per
 *  \e MoClassType.
 *
 */
extern ClRcT clFaultRepairAction(CL_IN ClIocAddressT iocAddress,
                                 CL_IN ClAlarmHandleT alarmHandle,
                                 CL_IN ClUint32T recoveryActionTaken);


extern ClRcT clFaultRepairNotification(ClAmsNotificationDescriptorT *notification,
                                       ClIocAddressT iocAddress,
                                       ClAlarmHandleT alarmHandle,
                                       ClUint32T recovery);

/**
 ************************************
 *  \brief Initializes the Fault Manager Library.
 *
 *  \par Header File:
 *  clFaultApi.h
 *
 *  \par Parameters:
 *  None
 *
 *  \retval CL_OK The API executed successfully.
 *
 *  \par Description:
 *  This function is used to initialize the Fault Manager Library.
 *
 *  \par Library File(s):
 *  ClFaultClient
 *
 *  \sa clFaultSvcLibFinalize()
 *
 */

extern ClRcT clFaultSvcLibInitialize(void);


/**
 ************************************
 *  \brief Cleans up the Fault Manager library and frees resources allocated to it.
 *
 *  \par Header File:
 *  clFaultApi.h
 *
 *  \par Parameters:
 *  None
 *
 *  \retval CL_OK The API executed successfully.
 *
 *  \par Description:
 *  This function is used to clean up any resources allocated during the initialization of Fault Service.
 *  This must be called when the services are no longer required. 
 *
 *  \par Library File:
 *  ClFaultClient
 *
 *  \sa clFaultSvcLibInitialize()
 */

extern ClRcT clFaultSvcLibFinalize(void);


/**
 ************************************************
 *  \brief Verifies the version of the FM library supported by the FM.
 *
 *  \par Header File:
 *  clFaultApi.h
 *
 *  \param version (in/out) As an input parameter, this contains the version queried by you.
 *  In the output parameter, the supported version is returned by this function.
 *
 *  \retval CL_OK The API executed successfully.
 *  \retval CL_FAULT_ERR_VERSION_UNSUPPORTED The version supplied is incompatible. 
 *
 *  \par Description:
 *  This function is used to check for version compatibility with the Fault
 *  client.
 *  When the version supplied by the Application is not compatible, the
 *  supported version is returned in the out parameter \e version
 *
 *  \par Library File:
 *  ClFaultClient
 *
 */
extern ClRcT clFaultVersionVerify( CL_INOUT ClVersionT *version);

/***************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* _CL_FAULT_API_H_ */

/**
 *  \}
 */

#endif
