#define DEBUG_ON
#include "../../general/include/debugging.h"
#include "../../general/ZXNextSmooths_Z88dkDeps.h"

#include "../../nextREG/export_nextREG.h"
#include "../../memoryBank/export_memoryBank.h"

#include "../include/esxdos_fileIO.h"
#include "../include/esxdos_structures.h"

#include "../include/esxdos_drvapi_espat.h"
#include "../include/esxdos_drvapi.h"


/*  ESPAT Driver ID is 'N' which is 78 decimal

    ESPAT Driver function numbers

        B=01: Set memory bank for CMD and SYS (Depreciated)
        B=02: Set current channel for DRIVER
        B=03: Get CMD or IPD channel values
        B=04: Set CMD or IPD values
        B=05: Set CMD and IPD timeouts
        B=06: Add 16K bank to IPD receive pool
        B=07: Remove 16K BANK from channel IPD only does not apply to CMD
        B=08: Get ESP Link ID from handle
        B=09: Specific UART BAUD rate to be set from lookup table.
        B=10: Set output buffer mode for channel
        B=11: enable/disable embedded IPD detect

    On entry, use B=call id with HL,DE other parameters. (Tim works DE=Var1 HL=Var2)
        (NOTE: HL will contain the value that was either provided in HL (when called from dot commands) or IX (when called from a standard program).
    When called from the ESX_DRVAPI() (DRIVER command), DE is the first input and HL is the second.
    When returning values, the ESX_DRVAPI() (DRIVER command) will place the contents of BC into the first return variable, then DE and then HL.

    Remember you have a BASIC stream <> channel(handle) {<> ESP LinkID}

    https://gitlab.com/thesmog358/tbblue/-/blob/master/docs/extra-hw/wifi/ESPATreadme.TXT

*/

#define espat_DRVID (uint8_t)78 // 'N'
#define espat_DRVFUNC_SETMEMBANK (uint8_t)1

static bankID memoryBank = 0;

// Network Driver API functions
#define NEAT_Deprecated 0x01
#define NEAT_SetCurChan 0x02
#define NEAT_GetChanVals 0x03
#define NEAT_SetChanVals 0x04
#define NEAT_SetTimeouts 0x05
#define NEAT_AddBank 0x06
#define NEAT_RemoveBank 0x07
#define NEAT_GetESPLink 0x08
#define NEAT_SetBaudRate 0x09
#define NEAT_SetBuffMode 0x0A
#define NEAT_ProcessCMD 0x0B

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

extern struct esx_drvapi esxdrvApiMsg;
static char espat_sysfile[] = "/nextzxos/espat.sys";


//
//  INSTALL AND SETUP DRIVER
//

void espat_setBaud(void)
{
    DEBUG_FUNCTIONCALL("\n\n espat_setBaud(void)");
    esxdrvApiMsg.call.driver = espat_DRVID;
    esxdrvApiMsg.call.function = NEAT_SetBaudRate;
    esxdrvApiMsg.de = (uint16_t)0; // 115K
    esxdrvApiMsg.hl = (uint16_t)0; //
    // callDriverApi(esxdrvApiMsg);
    safe_callDriverApiErrorMsg(esxdrvApiMsg, esxdos_errorMsg);
    return;
}

void espat_setTimeouts(void)
{
    DEBUG_FUNCTIONCALL("\n\n espat_setTimeouts(void)");
    esxdrvApiMsg.call.driver = espat_DRVID;
    esxdrvApiMsg.call.function = NEAT_SetTimeouts;
    esxdrvApiMsg.de = (uint16_t)96;   // receive
    esxdrvApiMsg.hl = (uint16_t)1024; // send
    // callDriverApi(esxdrvApiMsg);
    safe_callDriverApiErrorMsg(esxdrvApiMsg, esxdos_errorMsg);
    return;
}

void espat_loadEspatSys(void)
{
    DEBUG_FUNCTIONCALL("\n\n espat_loadEspatSys(void) \n FILENAME>> %s", espat_sysfile);

    basicBankID sysBank = manage8Banks_allocateBasicBank(); // TODO -- MMU operations
    nextos_saveBasicBankSlotMMU(4);
    nextos_setBasicBankSlot(4, sysBank);

    uint8_t *dst = BASIC16KBankSlotAddr_4;
    fileHandle handle = openFileHandle(espat_sysfile, ESXDOS_MODE_R | ESXDOS_MODE_OPEN_EXIST);
    if (NULL != handle)
    {
        readFile(handle, dst, 8192);
        closeFileHandle(handle);
    }

    nextos_restoreBasicBankSlotMMU(4);

    esxdrvApiMsg.call.driver = espat_DRVID;
    esxdrvApiMsg.call.function = NEAT_Deprecated; // (1)
    esxdrvApiMsg.de = sysBank;                    // BASIC 16KBank ID
    esxdrvApiMsg.hl = 0;
    safe_callDriverApiErrorMsg(esxdrvApiMsg, esxdos_errorMsg);

    return;
}

void espat_addBufferMemoryBank(void)
{ // This is going to be deprecated in BETA1 / RC1
    DEBUG_FUNCTIONCALL("\n\n espat_setMemoryBank(void)");

    // We need 2 8K banks consecutively (16K), double grab and store the second bank ID
    basicBankID dataBuffer = manage8Banks_allocateBasicBank(); // TODO -- MMU operations

    esxdrvApiMsg.call.driver = espat_DRVID;
    esxdrvApiMsg.call.function = NEAT_AddBank; // (6)
    esxdrvApiMsg.de = dataBuffer;              // BASIC 16KBank ID
    esxdrvApiMsg.hl = 0;
    safe_callDriverApiErrorMsg(esxdrvApiMsg, esxdos_errorMsg);

    return;
}

void espat_DriverInstall(void)
{
    DEBUG_FUNCTIONCALL("\n\n espatDriverInstall(void)");
    installDriver("ESPAT.drv");
    espat_loadEspatSys();

    espat_addBufferMemoryBank();
    espat_setBaud();
    espat_setTimeouts();

    DEBUG_MSG("\n\nDRIVER INSTALLED A-OK\n\n");

    return;
}

//
//  OPEN CONNECTIONS
//
static char connectionString [128] = "";

void espat_GetChannel(void)
{
    DEBUG_FUNCTIONCALL("\n\n espat_GetChannel()");

    esxdrvApiMsg.call.driver = espat_DRVID;
    esxdrvApiMsg.call.function = NEAT_GetChanVals;
    esxdrvApiMsg.de = 0;
    esxdrvApiMsg.hl = 0;
    safe_callDriverApiErrorMsg(esxdrvApiMsg, esxdos_errorMsg);

    return;
}

nethandle espat_OpenConnection(char *connectionString)
{   //  Connection string format from ESP-AT "<TCP/UDP>,<IP OR DNS ADDRESS>,<PORT>" e.g."TCP,127.0.0.1,80"
    DEBUG_FUNCTIONCALL("\n espat_OpenConnection( %s )", connectionString);

    espat_GetChannel(); // Do this first ...

    esxdrvApiMsg.call.driver = espat_DRVID;
    esxdrvApiMsg.call.function = NOS_Open;
    esxdrvApiMsg.de = strlen(connectionString);
    esxdrvApiMsg.hl = connectionString;
    safe_callDriverApiErrorMsg(esxdrvApiMsg, esxdos_errorMsg); // calls > esx_m_drvapi(esxdrvApiMsg);

    return esxdrvApiMsg.call.function; // Driver API returns myNetHandle in Reg C (function);
}

nethandle espat_OpenTCPConnection(char *addr, networkPort port)
{
    strcat(connectionString,"TCP,");    
    
    strcat(connectionString,addr);
    strcat(connectionString,",");
    
    char portBuffer[6]= "00000";
    itoa(port,portBuffer,10);
    strcat(connectionString, portBuffer);
    
    return espat_OpenConnection(connectionString);
}

//
//  SEND RECEIVE DATA TO NETWORK HANDLE
//

void espat_SendChar(nethandle handle, char c)
{
    DEBUG_FUNCTIONCALL("\n\n espat_SendChar(handle %03u, char %c)", handle, c);

    esxdrvApiMsg.call.driver = espat_DRVID;
    esxdrvApiMsg.call.function = NOS_OutputChar;
    esxdrvApiMsg.de = (handle << 8) + c;
    esxdrvApiMsg.hl = 0;
    safe_callDriverApiErrorMsg(esxdrvApiMsg, esxdos_errorMsg);

    return;
}

char espat_RecvChar(nethandle handle)
{
    esxdrvApiMsg.call.driver = espat_DRVID;
    esxdrvApiMsg.call.function = NOS_InputChar;
    esxdrvApiMsg.de = (handle << 8);
    esxdrvApiMsg.hl = 0;
    safe_callDriverApiErrorMsg(esxdrvApiMsg, esxdos_errorMsg);

    char result = esxdrvApiMsg.de;
    return result;
}



void espat_testSend(nethandle myNetHandle)
{
    espat_SendChar(myNetHandle, 'X');
    espat_SendChar(myNetHandle, 'Y');
    espat_SendChar(myNetHandle, 'Z');
    return;
}



