#define DEBUG_ON
#include "../../general/include/debugging.h"
#include "../../general/ZXNextSmooths_Z88dkDeps.h"

#include "../../nextREG/export_nextREG.h"
#include "../../memoryBank/export_memoryBank.h"

#include "../include/esxdos_fileIO.h"
#include "../include/esxdos_structures.h"
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

    On entry, use B=call id with HL,DE other parameters.
        (NOTE: HL will contain the value that was either provided in HL (when called from dot commands) or IX (when called from a standard program).
    When called from the ESX_DRVAPI() (DRIVER command), DE is the first input and HL is the second.
    When returning values, the ESX_DRVAPI() (DRIVER command) will place the contents of BC into the first return variable, then DE and then HL.

    Remember you have a BASIC stream <> channel(handle) {<> ESP LinkID}

    https://gitlab.com/thesmog358/tbblue/-/blob/master/docs/extra-hw/wifi/ESPATreadme.TXT

*/

#define espat_DRVID (uint8_t)78 // 'N'
#define espat_DRVFUNC_SETMEMBANK (uint8_t)1

static bankID memoryBank = 0;
static struct esx_drvapi espat_drvapi = {{0}, 0, 0};

#define NOS_Initialise 0x80
#define NOS_Shutdown 0x81
#define NOS_Open 0xF9
#define NOS_Close 0xFA
#define NOS_OutputChar 0xFB
#define NOS_InputChar 0xFC
#define NOS_GetCurStrPtr 0xFD
#define NOS_SetCurStrPtr 0xFE
#define NOS_GetStrExtent 0xFF
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

static char espat_sysfile[] = "/nextzxos/espat.sys";

void espat_setBaud(void)
{
    DEBUG_FUNCTIONCALL("\n\n espat_setBaud(void)");
    espat_drvapi.call.driver = espat_DRVID;
    espat_drvapi.call.function = NEAT_SetBaudRate;
    espat_drvapi.de = (uint16_t)0; //
    espat_drvapi.hl = (uint16_t)0; // 115K
    // callDriverApi(espat_drvapi);
    safe_callDriverApiErrorMsg(espat_drvapi, esxdos_errorMsg);
    return;
}

void espat_setTimeouts(void)
{
    DEBUG_FUNCTIONCALL("\n\n espat_setTimeouts(void)");
    espat_drvapi.call.driver = espat_DRVID;
    espat_drvapi.call.function = NEAT_SetTimeouts;
    espat_drvapi.de = (uint16_t)96;   // receive
    espat_drvapi.hl = (uint16_t)1024; // send
    // callDriverApi(espat_drvapi);
    safe_callDriverApiErrorMsg(espat_drvapi, esxdos_errorMsg);
    return;
}

void espat_loadEspatSys(void)
{
    DEBUG_FUNCTIONCALL("\n\n espat_loadEspatSys(void) \n FILENAME>> %s", espat_sysfile);

    // uint8_t memoryBankH = allocateManaged_Bank();
    // uint8_t memoryBankL = allocateManaged_Bank();

    uint8_t memoryBankH = 221;
    uint8_t memoryBankL = 220;

    uint8_t tempMMU7 = ZXN_READ_MMU7();
    uint8_t tempMMU6 = ZXN_READ_MMU6();

    ZXN_WRITE_REG(REG_MMU7, memoryBankH);
    ZXN_WRITE_REG(REG_MMU6, memoryBankL);

    memory_MMUtoConsole();

    uint8_t *dst = (uint8_t *)0xC000; // zxn_addr_from_mmu(REG_MMU6);

    fileHandle handle = openFileHandle(espat_sysfile, ESXDOS_MODE_R | ESXDOS_MODE_OPEN_EXIST);
    if (NULL != handle)
    {
        readFile(handle, dst, 8192);
        closeFileHandle(handle);
    }


    ZXN_WRITE_REG(REG_MMU7, tempMMU7);
    ZXN_WRITE_REG(REG_MMU6, tempMMU6);

    memory_MMUtoConsole();

    espat_drvapi.call.driver = espat_DRVID;
    espat_drvapi.call.function = NEAT_Deprecated; // (1)
    espat_drvapi.de = 0;
    espat_drvapi.de = (uint16_t)(memoryBankL / 2); // BASIC 16KBank ID
    espat_drvapi.hl = 0;
    // espat_drvapi.hl = (uint16_t) (memoryBankL/2);
    // callDriverApi(espat_drvapi);
    safe_callDriverApiErrorMsg(espat_drvapi, esxdos_errorMsg);

    return;
}

void espat_addBufferMemoryBank(void)
{ // This is going to be deprecated in BETA1 / RC1

    DEBUG_FUNCTIONCALL("\n\n espat_setMemoryBank(void)");

    // We need 2 8K banks consecutively (16K), double grab and store the second bank ID
    // uint8_t memoryBankH = allocateManaged_Bank();
    // uint8_t memoryBankL = allocateManaged_Bank();

    uint8_t memoryBankH = 219;
    uint8_t memoryBankL = 218;

    DEBUG_MSG("\nespat memoryBankH=%03u , memoryBankL=%03u", memoryBankH, memoryBankL);

    espat_drvapi.call.driver = espat_DRVID;
    espat_drvapi.call.function = NEAT_AddBank; // (6)
    espat_drvapi.de = 0;
    espat_drvapi.de = (uint16_t)(memoryBankL / 2); // BASIC 16KBank ID
    safe_callDriverApiErrorMsg(espat_drvapi, esxdos_errorMsg);

    printf("\nBANK ADDED");

    return;
}

static uint8_t nethandle = 0;
static char connectionString[] = "TCP,172.16.1.112,1234";
void espat_OpenConnection(void)
{ // YOU MUST CALL espat_GetChannel BEFORE THIS
    DEBUG_FUNCTIONCALL("\n\n espat_OpenConnection() \n%s", connectionString);
    espat_drvapi.call.driver = espat_DRVID;
    espat_drvapi.call.function = NOS_Open;
    espat_drvapi.de = strlen(connectionString);
    espat_drvapi.hl = connectionString;

    safe_callDriverApiErrorMsg(espat_drvapi, esxdos_errorMsg); // calls > esx_m_drvapi(espat_drvapi);

    nethandle = espat_drvapi.call.function; // Driver API returns nethandle in Reg C (function)
    printf("\n\n NETHANDLE : %03u", nethandle);
    return;
}

void espat_GetChannel(void)
{
    DEBUG_FUNCTIONCALL("\n\n espat_GetChannel()");
    espat_drvapi.call.driver = espat_DRVID;
    espat_drvapi.call.function = NEAT_GetChanVals;
    espat_drvapi.de = 0;
    espat_drvapi.hl = 0;
    safe_callDriverApiErrorMsg(espat_drvapi, esxdos_errorMsg);
    return;
}

void espat_SendChar(uint8_t channel, char c)
{
    DEBUG_FUNCTIONCALL("\n\n espat_SendChar(channel %03u, char %c)", channel, c);
    espat_drvapi.call.driver = espat_DRVID;
    espat_drvapi.call.function = NOS_OutputChar;
    espat_drvapi.de = (channel << 8) + c;
    espat_drvapi.hl = 0;
    safe_callDriverApiErrorMsg(espat_drvapi, esxdos_errorMsg);
    return;
}

void espat_DriverInstall(void)
{
    DEBUG_FUNCTIONCALL("\n\n espatDriverInstall(void)");
    installDriver("ESPAT.drv");
    espat_loadEspatSys();
    /*
    espat_addBufferMemoryBank();
    espat_setBaud();
    espat_setTimeouts();

    espat_GetChannel(); // Do this first ...

    espat_OpenConnection(); // Now you get a nethandle

    espat_SendChar(nethandle, 'X');
    espat_SendChar(nethandle, 'Y');
    espat_SendChar(nethandle, 'Z');

    // espat_SendChar(nethandle, '\r');
    // espat_SendChar(nethandle, '\n');
    */
    printf("\n\nAOK\r\n");

    return;
}
