#define DEBUG_ON
#include "../../general/include/debugging.h"
#include "../../general/ZXNextSmooths_Z88dkDeps.h"

#include "../../nextREG/export_nextREG.h"
#include "../../memoryBank/export_memoryBank.h"

#include "../include/esxdos_fileIO.h"

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

void espat_setBaud(void) // 0x09 NEAT_SetBaudRate
{
    DEBUG_FUNCTIONCALL("\n espat_setBaud(void)");
    callDriver(DRIVERID_espat, NEAT_SetBaudRate, 0, 0);
    return;
}

void espat_setTimeouts(void) // 0x05 NEAT_SetTimeouts
{
    DEBUG_FUNCTIONCALL("\n espat_setTimeouts(void)");
    callDriver(DRIVERID_espat, NEAT_SetTimeouts, 96, 1024);
    return;
}

void espat_loadEspatSys(void) // 0x01 NEAT_Deprecated
{
    DEBUG_FUNCTIONCALL("\n espat_loadEspatSys(void) \n FILENAME>> %s", espat_sysfile);

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

    callDriver(DRIVERID_espat, NEAT_Deprecated, sysBank, 0);

    nextos_restoreBasicBankSlotMMU(4);

    return;
}

// This should have a counter function to remove buffers (0x07)
void espat_addBufferMemoryBank(void) // 0x06 NEAT_AddBank
{                                    // This is going to be deprecated in BETA1 / RC1
    DEBUG_FUNCTIONCALL("\n espat_setMemoryBank(void)");

    // We need 2 8K banks consecutively (16K), double grab and store the second bank ID
    basicBankID dataBuffer = manage8Banks_allocateBasicBank(); // TODO -- MMU operations
    callDriver(DRIVERID_espat, NEAT_AddBank, dataBuffer, 0);

    return;
}

void espat_DriverInstall(void)
{
    DEBUG_FUNCTIONCALL("\n espatDriverInstall(void)");
    installDriver("ESPAT.drv");

    espat_loadEspatSys();

    espat_addBufferMemoryBank();
    espat_setBaud();
    espat_setTimeouts();

    DEBUG_MSG("\nDRIVER INSTALLED A-OK\n");

    return;
}

//
//  OPEN CONNECTIONS
//
static char connectionString[128] = "";

void espat_GetChannel(void) // 0x03 NEAT_GetChanVals
{
    DEBUG_FUNCTIONCALL("\n\n espat_GetChannel()");
    callDriver(DRIVERID_espat, NEAT_GetChanVals, 0, 0);

    return;
}

nethandle espat_OpenConnection(char *connectionString) // 0xF9 NOS_Open
{                                                      //  Connection string format from ESP-AT "<TCP/UDP>,<IP OR DNS ADDRESS>,<PORT>" e.g."TCP,127.0.0.1,80"
    DEBUG_FUNCTIONCALL("\n espat_OpenConnection( %s )", connectionString);

    espat_GetChannel(); // Do this first ...
    callDriver(DRIVERID_espat, NOS_Open, strlen(connectionString), connectionString);

    return esxdrvApiMsg.call.function; // Driver API returns myNetHandle in Reg C (function);
}

nethandle espat_OpenTCPConnection(char *addr, networkPort port)
{
    strcat(connectionString, "TCP,");

    strcat(connectionString, addr);
    strcat(connectionString, ",");

    char portBuffer[6] = "00000";
    itoa(port, portBuffer, 10);
    strcat(connectionString, portBuffer);

    return espat_OpenConnection(connectionString);
}

//
//  SEND RECEIVE DATA TO NETWORK HANDLE
//

void espat_SendChar(nethandle handle, char c) // 0xFB NOS_OutputChar
{
    DEBUG_FUNCTIONCALL("\n espat_SendChar(handle %03u, char %c)", handle, c);

    nos_OutputChar(DRIVERID_espat, handle, c);

    return;
}

char espat_RecvChar(nethandle handle) // 0xFC NOS_InputChar
{
    callDriver(DRIVERID_espat, NOS_InputChar, (handle << 8), 0);
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

//
//  Other functions
//

void setDriverCurrentChannel(uint16_t channel) // 0x02 NEAT_SetCurChan
{
    callDriver(DRIVERID_espat, NEAT_SetCurChan, channel, 0);
    return;
}

void getChannelValues(channel handle) // 0x03 NEAT_GetChanVals
{
    DEBUG_FUNCTIONCALL("\n getChannelValues(void)");
    // Set the current Channel with 0x02
    // DE channel or 0 for current channel
    callDriver(DRIVERID_espat, NEAT_GetChanVals, handle, 0);
    driverApiToConsole(esxdrvApiMsg,true);
    return;

    // DE and HL are input and output positions on the esp IPD read buffer (@49152)
}

void setCMDIPDvalues(void) // 0x04 NEAT_SetChanVals
{                          
    // Set the current Channel with 0x02
    callDriver(DRIVERID_espat, NEAT_SetChanVals, 0, 0);
    return;
}

void getLinkIDfromHandle(nethandle handle) // 0x08 NEAT_GetESPLink
{
    callDriver(DRIVERID_espat, NEAT_GetESPLink, handle, 0);
    return; // BC contains the linkID
}

void setOutputBufferMode(nethandle channel, uint8_t mode) // 0x0A NEAT_SetBuffMode
{
    DEBUG_FUNCTIONCALL("\n setOutputBufferMode(channel %03u, mode %03u)", channel, mode);
    // DE is channel (0 default) (IPD channels only)
    // HL is mode 2-255 characters 1=immesdiate, 0=wait for CR/256chars
    callDriver(DRIVERID_espat, NEAT_SetBuffMode, channel, mode);
    return; // BC contains the linkID
}

void setIPDdetect(channel handle) // 0x0B NEAT_ProcessCMD
{
    // DE channel 0 default (IPD channel only)
    // HL 0x08 disable (0x00 enable)

    callDriver(DRIVERID_espat, NEAT_ProcessCMD, handle, 0);
    return;
}

bytesWritten netWrite(fileHandle openHandle, void *dataToWrite, size_t numberOfBytesToWrite)
{
    DEBUG_FUNCTIONCALL("\n netWrite(openHandle, dataToWrite, numberOfBytesToWrite )");
    esxdos_clearErrorCodes();
    bytesWritten bytes = esx_f_write(openHandle, dataToWrite, numberOfBytesToWrite);
    DEBUG_MSG("\n esx_f_write() bytesWrite: %03u", bytes);
    if (0 != errno)
    {
        esxdos_setErrorCodes(errno);
        return NULL;
    }
    DEBUG_MSG("\n bytes written: %03u", bytes);
    return bytes;
}

bytesRead netRead(fileHandle openHandle, void *readDataInto, size_t numberOfBytesToRead)
{
}