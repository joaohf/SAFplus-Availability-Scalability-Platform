#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <clCommon.h>
#include <clOsalApi.h>
#include <clOsalErrors.h>
#include <clBufferApi.h>
#include <clCntApi.h>
#include <clHeapApi.h>
#include <clEoApi.h>
#include <clIocApi.h>
#include <clIocApiExt.h>
#include <clIocErrors.h>
#include <clIocParseConfig.h>
#include <clIocLogicalAddresses.h>
#include "./alarm_clock_EOServer.h"
#include "rmdExternalDefs.h"
#include "alarm_clock_EOClient.h"
#include "alarm_clock_EOAlarmClockopServer.h"

#define __LOGICAL_ADDRESS(a) CL_IOC_LOGICAL_ADDRESS_FORM(CL_IOC_STATIC_LOGICAL_ADDRESS_START + (a))
#define __RPC_SERVER_ADDRESS __LOGICAL_ADDRESS(4)
#define  LOCAL_ADDRESS 4
extern ClRcT clRmdLibInitialize(ClPtrT pConfig);
extern ClRcT clAlarm_clock_EOClientInstall(void);
ClRcT eoTableInitialize(ClIocLogicalAddressT addr, ClIocPortT port)
{
    ClRcT clrc = CL_OK;
    ClIocTLInfoT tlInfo = {0};
    printf("Enter RmdInitialize\n");
    if ( (clrc = clAlarm_clock_EOClientInstall()) != CL_OK)  // Actually ignore the name, it starts the server
    {
        printf ("RPC server initialization failed. error: 0x%x", clrc);
    }

    if ( (clrc = clAlarm_clock_EOClientTableRegister(port) ) != CL_OK)
    {
        printf ("RPC server client initialization failed. error: 0x%x", clrc);
    }

    tlInfo.physicalAddr.nodeAddress = addr;
    printf ("RPC server nodeAddress: %d \n", tlInfo.physicalAddr.nodeAddress);
    tlInfo.physicalAddr.portId = port;
    tlInfo.haState = CL_IOC_TL_ACTIVE;
    tlInfo.logicalAddr = addr;
    printf ("RPC server logicalAddr: %d \n", (int)addr);
    printf ("RPC server portId: %d \n", (int)port);
    if ( (clrc = clIocTransparencyRegister(&tlInfo) ) != CL_OK )
    {
        printf("RPC server transparency layer init failed with [%#x]", clrc);
    }
    printf("finish Initialize");
    return clrc;
}


int
main(int argc, char **argv)
{
    ClEoConfigT eoConfig =
    {
        CL_OSAL_THREAD_PRI_MEDIUM,    /* EO Thread Priority                       */
        2,                            /* No of EO thread needed                   */
        0,                            /* Required Ioc Port                        */
        (CL_EO_USER_CLIENT_ID_START + 0), 
        CL_EO_USE_THREAD_FOR_APP,     /* Thread Model                             */
        NULL,                         /* Application Initialize Callback          */
        NULL,                         /* Application Terminate Callback           */
        NULL,                         /* Application State Change Callback        */
        NULL                          /* Application Health Check Callback        */
    };    
    int ioc_port = DEF_IOC_PORT; /* IOC port number, default is DEF_IOC_PORT */
    int ioc_address_local = LOCAL_ADDRESS;
    ClRcT rc = CL_OK;
    rc = clExtInitialize( ioc_address_local );
    if (rc != CL_OK)
    {
        printf("Error: failed to Initialize ASP libraries\n");
        exit(1);
    }
    if ((rc = clRmdLibInitialize(NULL)) != CL_OK)
    {
        printf("Error: RMD initialization failed with rc = 0x%x\n", rc);
        exit(1);
    }
    printf("Info: start rmd server\n");

    rc = clExtRmdServerInit(&eoConfig);

    if(rc != CL_OK)
    {
        printf("Info: start rmd server ok\n");
    }
    else
    {
        eoTableInitialize(__RPC_SERVER_ADDRESS,ioc_port);
    }
  
    printf("Info: start rmd server ok\n");

    do
    {
        sleep(20);
        printf("Info : running .... \n");
    }while(1);

}
    