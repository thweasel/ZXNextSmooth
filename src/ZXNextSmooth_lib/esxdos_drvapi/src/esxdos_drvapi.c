#define DEBUG_ON
#include "../../general/include/debugging.h"
#include "../../general/Z88dkDeps.h"

#include <arch/zxn/esxdos.h>

struct esx_drvapi driverApiMsg;
#define stringBufferSize 33
char stringBuffer[stringBufferSize] = "";

void driverApiToConsole(struct esx_drvapi *showDriverApiCall, bool returnValue)
{
    if (returnValue)
    {
        printf("\nDRVAPI << ");
    }
    else
    {
        printf("\nDRVAPI >> ");
    }
    printf("DRVID %03u, FUNC %03u, bc %05u, de %05u, hl %05u",
           showDriverApiCall->call.driver,
           showDriverApiCall->call.function,
           showDriverApiCall->bc,
           showDriverApiCall->de,
           showDriverApiCall->hl);
    return;
}

/*
    These are Valid .Install .Uninstall commands
    install "C:/nextzxos/name.drv"
    install /nextzxos/name.drv
    install nextzxos/name.drv

    The . can be ommitted
    " " are only needed if C: (drive) is used
    Unless you have moved directory you can cheat and assume the C: is present and use relative pathing
*/

#define OSPATH "/nextzxos/"
#define DRIVERFILEACTION_LOAD (uint8_t)0
#define DRIVERFILEACTION_UNLOAD (uint8_t)1

uint16_t driverFileAction(char *driverName, uint8_t driverFileAction)
{
    DEBUG_FUNCTIONCALL("\ndriverFileAction(driverName *%02x, driverFileAction %u)", driverName, driverFileAction);
    switch (driverFileAction)
    {
    case DRIVERFILEACTION_LOAD:
        safe_appendString(stringBuffer, "install ", stringBufferSize);
        break;
    case DRIVERFILEACTION_UNLOAD:
        safe_appendString(stringBuffer, "uninstall ", stringBufferSize);
        break;
    default:
        break;
    }

    safe_appendString(stringBuffer, OSPATH, stringBufferSize);
    safe_appendString(stringBuffer, driverName, stringBufferSize);

    DEBUG_MSG("\nDriverFileAction> %s ", stringBuffer);

    uint16_t esxdosMsg = esx_m_execcmd(stringBuffer);
    esx_m_geterr(esxdosMsg, stringBuffer);

    DEBUG_MSG("\nesxdosMsg> %u, %s ", esxdosMsg, stringBuffer);
    return esxdosMsg;
}

uint16_t installDriver(char *driverName)
{
    return driverFileAction(driverName, DRIVERFILEACTION_LOAD);
}

uint16_t uninstallDriver(char *driverName)
{
    return driverFileAction(driverName, DRIVERFILEACTION_UNLOAD);
}

uint8_t callDriverApi(struct esx_drvapi *driverApiCall)
{
    DEBUG_FUNCTIONCALL("\ncallDriverApi(*driverApiCall) ");
    DEBUG_DRIVERAPI(driverApiCall, false);
    int result = esx_m_drvapi(driverApiCall);
    DEBUG_DRIVERAPI(driverApiCall, true);
    return result;
}

uint8_t safe_callDriverApi(struct esx_drvapi *driverApiCall)
{
    if (NULL == driverApiCall)
    {
        DEBUG_MSG("\nsafe_callDriverApi() NULL call");
        return 255;
    }
    return callDriverApi(driverApiCall);
}

uint8_t callDriverApiErrorMsg(struct esx_drvapi *driverApiCall, char *errorMsgBuffer)
{
    DEBUG_FUNCTIONCALL("\ncallDriverApi(*driverApiCall, *errorMsgBuffer) ");
    DEBUG_DRIVERAPI(driverApiCall,false);

    uint8_t result = esx_m_drvapi(driverApiCall);
    DEBUG_DRIVERAPI(driverApiCall, true);
    
    esx_m_geterr(result, errorMsgBuffer);
    DEBUG_MSG("\n esx_m_drvapi(driverApi) ,%u %s", result, errorMsgBuffer);
    
    return result;
}

uint8_t safe_callDriverApiErrorMsg(struct esx_drvapi *driverApiCall, char *errorMsgBuffer)
{
    if (NULL == driverApiCall)
    {
        DEBUG_MSG("\nsafe_callDriverApi() NULL call");
        return 255;
    }

    if (NULL == errorMsgBuffer)
    {
        DEBUG_MSG("\nsafe_callDriverApi() NULL errorMsgBuffer");
        return 255;
    }

    return callDriverApiErrorMsg(driverApiCall, errorMsgBuffer);
}