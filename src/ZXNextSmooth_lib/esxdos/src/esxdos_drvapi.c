#define DEBUG_ON
#include "../../general/include/debugging.h"
#include "../../general/ZXNextSmooths_Z88dkDeps.h"

struct esx_drvapi driverApiMsg = {{(uint16_t)0}, (uint16_t)0, (uint16_t)0};

struct esxdos_drvapi_REG
{
    union BC
    {
        uint8_t B;
        uint8_t C;
    };
    union DE
    {
        uint8_t D;
        uint8_t E;
    };
    union HL
    {
        uint8_t H;
        uint8_t L;
    };

} driverApiReturnREG = {{(uint16_t)0}, {(uint16_t)0}, {(uint16_t)0}};


#define esxErrorMessageSize 33
char esxErrorMessageString[esxErrorMessageSize] = "";

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

/*
    The documentation that comes with the driver describes how to use it.

    Some drivers may make use of the new DRIVER command.
    This has the following form: DRIVER driverid,callid[,n1[,n2]] [TO var1[,var2[,var3]]]
    where n1 and n2 are optional values to pass to the driver,
    and var1, var2 and var3 are optional variables to receive results from the driver call.
    The documentation for each driver will describe the individual DRIVER commands that you can use.
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
        safe_appendString(esxErrorMessageString, "install ", esxErrorMessageSize);
        break;
    case DRIVERFILEACTION_UNLOAD:
        safe_appendString(esxErrorMessageString, "uninstall ", esxErrorMessageSize);
        break;
    default:
        break;
    }

    safe_appendString(esxErrorMessageString, OSPATH, esxErrorMessageSize);
    safe_appendString(esxErrorMessageString, driverName, esxErrorMessageSize);

    DEBUG_MSG("\nDriverFileAction> %s ", esxErrorMessageString);

    uint16_t esxdosMsg = esx_m_execcmd(esxErrorMessageString);
    esx_m_geterr(esxdosMsg, esxErrorMessageString);

    DEBUG_MSG("\nesxdosMsg> %u, %s ", esxdosMsg, esxErrorMessageString);
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

uint8_t DUMMY_callDriverApi(struct esx_drvapi *driverApiCall)
{
    DEBUG_FUNCTIONCALL("\nDUMMY_callDriverApi(*driverApiCall) ");
    DEBUG_DRIVERAPI(driverApiCall, false);
    int result = 0; // esx_m_drvapi(driverApiCall);
    DEBUG_DRIVERAPI(driverApiCall, true);
    return result;
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
    DEBUG_FUNCTIONCALL("\ncallDriverApiErrorMsg(*driverApiCall, *errorMsgBuffer) ");
    DEBUG_DRIVERAPI(driverApiCall, false);

    uint8_t result = esx_m_drvapi(driverApiCall);
    DEBUG_DRIVERAPI(driverApiCall, true);

    esx_m_geterr(result, errorMsgBuffer);
    DEBUG_MSG("\n esx_m_drvapi(driverApi) << %u, %s", result, errorMsgBuffer);

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