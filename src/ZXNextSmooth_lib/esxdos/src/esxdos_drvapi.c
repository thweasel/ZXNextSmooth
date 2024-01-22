#define DEBUG_OFF
#include "../../general/include/debugging.h"
#include "../../general/ZXNextSmooths_Z88dkDeps.h"

#include "../include/esxdos_drvapi.h"

struct esx_drvapi esxdrvApiMsg = {{(uint16_t)0}, (uint16_t)0, (uint16_t)0};

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

extern int errno; // File errors are reported here comes from errno.h
esxdosErrorCode lastEsxDos_errorCode = 0;
char lastEsxDos_ErrorMessage[ESXDOS_ERRORMSG_SIZE] = "";

void esxdos_clearErrorCodes(void)
{
    errno = 0;
    lastEsxDos_errorCode = 0;
    lastEsxDos_ErrorMessage[0] = '\0';
    return;
}

void esxdos_setErrorCodes(esxdosErrorCode errorCode)
{
    lastEsxDos_errorCode = errno;
    esx_m_geterr(errno, lastEsxDos_ErrorMessage);
    return;
}

/*  .INSTALL information
    These are Valid .Install .Uninstall commands
    install "C:/nextzxos/Filename.drv"
    install /nextzxos/Filename.drv
    install nextzxos/Filename.drv

    The . can be ommitted
    " " are only needed if C: (drive) is used
    Unless you have moved directory you can cheat and assume the C: is present and use relative pathing

    The OS will automatically search for .Dot commands in the C:/DOT.
    However, it will not automatically search for the driver file, so you have to provide a path!
*/

/*  BASIC DRIVER COMMAND explained
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

//
//  DRIVER API MESSAGE SYSTEM
//

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

uint16_t driverFileAction(char *driverName, uint8_t driverFileAction)
{
    DEBUG_FUNCTIONCALL("\ndriverFileAction(driverName *%02x, driverFileAction %u)", driverName, driverFileAction);
    switch (driverFileAction)
    {
    case DRIVERFILEACTION_LOAD:
        safe_appendString(lastEsxDos_ErrorMessage, "install ", ESXDOS_ERRORMSG_SIZE);
        break;
    case DRIVERFILEACTION_UNLOAD:
        safe_appendString(lastEsxDos_ErrorMessage, "uninstall ", ESXDOS_ERRORMSG_SIZE);
        break;
    default:
        break;
    }

    safe_appendString(lastEsxDos_ErrorMessage, OSPATH, ESXDOS_ERRORMSG_SIZE);
    safe_appendString(lastEsxDos_ErrorMessage, driverName, ESXDOS_ERRORMSG_SIZE);

    DEBUG_MSG("\nDriverFileAction> %s ", lastEsxDos_ErrorMessage);

    uint16_t esxdosMsg = esx_m_execcmd(lastEsxDos_ErrorMessage);
    esx_m_geterr(esxdosMsg, lastEsxDos_ErrorMessage);

    DEBUG_MSG("\nesxdosMsg> %u, %s ", esxdosMsg, lastEsxDos_ErrorMessage);
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

void callDriver(driverID driver, driverFunction function, cpuDE de, cpuHL hl)
{
    DEBUG_FUNCTIONCALL("\n callDriver(driver %03u, function %03u, DE %05u, HL %05u) ", driver, function, de, hl);

    esxdrvApiMsg.call.driver = driver;
    esxdrvApiMsg.call.function = function;
    esxdrvApiMsg.de = de;
    esxdrvApiMsg.hl = hl;

    esxdos_clearErrorCodes();

    DEBUG_DRIVERAPI(esxdrvApiMsg, false);
    uint8_t result = esx_m_drvapi(esxdrvApiMsg);
    DEBUG_DRIVERAPI(esxdrvApiMsg, true);

    if (errno > 1)
    {
        esx_m_geterr(errno, lastEsxDos_ErrorMessage);
        printf("\n  ERRNO: %03u, %s", errno, lastEsxDos_ErrorMessage);
    }

    if (result > 0)
    {
        esx_m_geterr(result, lastEsxDos_ErrorMessage);
        DEBUG_MSG("\n  RESULT: %03u, %s", result, lastEsxDos_ErrorMessage);
    }

    return result;
}

//
//  GENERIC CHANNEL FUNCTIONS
//

// OS standard Channel/Stream API functions
#define NOS_Initialise 0x80
#define NOS_Shutdown 0x81
#define NOS_Open 0xF9
#define NOS_Close 0xFA
#define NOS_OutputChar 0xFB
#define NOS_InputChar 0xFC
#define NOS_GetCurStrPtr 0xFD
#define NOS_SetCurStrPtr 0xFE
#define NOS_GetStrExtent 0xFF

/*  Channels access is provided by some installable drivers, we can access them using the driver api

    ; The following calls are used to allow your driver to support
    ; channels for i/o (manipulated with BASIC commands like OPEN #).
    ; Each call is optional - just return with carry set and A=0
    ; for any calls that you don't want to provide.
    ;
    ; B=$f6: copy screen (usually for printer drivers)
    ; B=$f7: return output status
    ; B=$f8: return input status
    ; B=$f9: open channel
    ; B=$fa: close channel
    ; B=$fb: output character
    ; B=$fc: input character
    ; B=$fd: get current stream pointer
    ; B=$fe: set current stream pointer
    ; B=$ff: get stream size/extent
*/

/*  Channels have designations, "Driver ID"

        'k' - keyboard
        's' - screen
        'p' - printer

        'N' - Network (78)

*/

/*  Existing system Streams 0 - 3

    0 - Keyboard
    1 - Keyboard
    2 - Screen
    3 - Printer
*/

/*
    Open a "channel" and assign a "stream"
*/

/*
void openChannelOnStream (char channel, uint8_t streamNumber)
{

    return;
}
*/

void nos_OutputChar(driverID driver, channel handle, char c) // 0xFB
{
    DEBUG_FUNCTIONCALL("\n nos_OutputChar(driver %03u, handle %03u, c %c)", driver, handle, c);
    callDriver(driver, NOS_OutputChar, (handle << 8) + c, 0);
    return;
}

char nos_InputChar(driverID driver, channel handle) // 0xFC
{
    DEBUG_FUNCTIONCALL("\n nos_InputChar(driver %03u, handle %03u)", driver, handle);
    callDriver(driver, NOS_OutputChar, (handle << 8), 0);
    return esxdrvApiMsg.de;
}

uint16_t nos_StreamSize(driverID driver, channel handle) // 0xFF
{
    DEBUG_FUNCTIONCALL("\n nos_StreamSize(driver %03u, handle %03u)", driver, handle);
    // D handle
    callDriver(driver, NOS_GetStrExtent, (handle << 8), 0);
    if (esxdrvApiMsg.hl > 0)
    {
        printf("\n Stream contains: de %05u hl %05u", esxdrvApiMsg.de, esxdrvApiMsg.hl);
    }
    // DE size ?
    // HL extent ?
    return esxdrvApiMsg.hl;
}
