
/*******************************************************************************
*
* This headerfile is auto-generated by OpenClovis IDE
*
* clCompConfig.h for App2plus1
*
********************************************************************************/

#ifndef CL_COMP_CFG
#define CL_COMP_CFG

#ifdef __cplusplus
extern "C" {
#endif

#include <clCommon.h>   
#include <clEoApi.h>                                                                                                                          
                                                                                                                             
#define COMP_NAME       "App2plus1"
#define COMP_EO_NAME    "App2plus1_EO"

#define HAS_EO_SERVICES 0

#define COMP_EO_THREAD_PRIORITY CL_OSAL_THREAD_PRI_MEDIUM
#define COMP_EO_NUM_THREAD 2
#define COMP_IOC_PORT 0
#define COMP_EO_USER_CLIENT_ID (CL_EO_USER_CLIENT_ID_START + 0)
#define COMP_EO_USE_THREAD_MODEL CL_EO_USE_THREAD_FOR_RECV
                                                                                                                             
/* Component EO Basic Libraries */
#define COMP_EO_BASICLIB_OSAL   CL_TRUE
#define COMP_EO_BASICLIB_TIMER   CL_TRUE
#define COMP_EO_BASICLIB_BUFFER   CL_TRUE
#define COMP_EO_BASICLIB_IOC   CL_TRUE
#define COMP_EO_BASICLIB_RMD   CL_TRUE
#define COMP_EO_BASICLIB_EO   CL_TRUE
#define COMP_EO_BASICLIB_OM   CL_FALSE
#define COMP_EO_BASICLIB_HAL  CL_FALSE
#define COMP_EO_BASICLIB_DBAL  CL_FALSE
                                                                                                                             
/* Component EO Client Libraries */                                                                                                                            

#define COMP_EO_CLIENTLIB_COR   CL_TRUE
#define COMP_EO_CLIENTLIB_CM    CL_FALSE                  
#define COMP_EO_CLIENTLIB_NAME    CL_TRUE                  
#define COMP_EO_CLIENTLIB_LOG    CL_TRUE                  
#define COMP_EO_CLIENTLIB_TRACE    CL_FALSE                 
#define COMP_EO_CLIENTLIB_DIAG    CL_FALSE
#define COMP_EO_CLIENTLIB_TXN    CL_TRUE
#define COMP_EO_CLIENTLIB_MSO    CL_TRUE
#define COMP_EO_CLIENTLIB_PROV    CL_TRUE
#define COMP_EO_CLIENTLIB_ALARM    CL_FALSE
#define COMP_EO_CLIENTLIB_DEBUG    CL_TRUE
#define COMP_EO_CLIENTLIB_GMS    CL_FALSE
#define COMP_EO_CLIENTLIB_PM    CL_FALSE

#ifdef __cplusplus
}
#endif                                                                                                                             

#endif

