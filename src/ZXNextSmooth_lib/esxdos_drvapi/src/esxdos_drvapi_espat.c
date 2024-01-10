#define DEBUG_ON
#include "../../general/include/debugging.h"
#include "../../general/Z88dkDeps.h"

#include "../include/esxdos_drvapi.h"
#include "../../memory/export_memory.h"

/*

    Driver ID is 'N' which is 78 decimal

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

    https://gitlab.com/thesmog358/tbblue/-/blob/master/docs/extra-hw/wifi/ESPATreadme.TXT

*/

#define espat_DRVID (uint8_t)78 // 'N'
#define espat_DRVFUNC_SETMEMBANK (uint8_t)1

static uint8_t memoryBank = 0;
static struct esx_drvapi espat_drvapi;

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

static char connectionString[22] = "TCP,172.16.1.112,1234";
static char errorMsg[33] = " ";

static char espat_sysfile[20] = "/nextzxos/espat.sys";

void espat_setBaud(void)
{
    DEBUG_FUNCTIONCALL("\n espat_setBaud(void)");
    espat_drvapi.call.driver = espat_DRVID;
    espat_drvapi.call.function = NEAT_SetBaudRate;
    espat_drvapi.de = (uint16_t) 0; // 
    espat_drvapi.hl = (uint16_t) 0; // 115K
    //callDriverApi(espat_drvapi);

    return;
}

void espat_setTimeouts(void)
{
    DEBUG_FUNCTIONCALL("\n espat_setTimeouts(void)");
    espat_drvapi.call.driver = espat_DRVID;
    espat_drvapi.call.function = NEAT_SetTimeouts;
    espat_drvapi.de = (uint16_t)96;   // receive
    espat_drvapi.hl = (uint16_t)1024; // send
    callDriverApi(espat_drvapi);
    // safe_callDriverApiErrorMsg(espat_drvapi, errorMsg);
    return;
}

// at least one of:

#define ESX_MODE_READ __esx_mode_read
#define ESX_MODE_WRITE __esx_mode_write

#define ESX_MODE_R __esx_mode_read
#define ESX_MODE_W __esx_mode_write
#define ESX_MODE_RW (__esx_mode_read | __esx_mode_write)

// one of:

#define ESX_MODE_OPEN_EXIST __esx_mode_open_exist
#define ESX_MODE_OPEN_CREAT __esx_mode_open_creat
#define ESX_MODE_OPEN_CREAT_NOEXIST __esx_mode_creat_noexist
#define ESX_MODE_OPEN_CREAT_TRUNC __esx_mode_creat_trunc

#define ESX_MODE_USE_HEADER __esx_mode_use_header

// esx_f_open(const char *filename,unsigned char mode);
// esx_f_read(unsigned char handle,void *dst,size_t nbytes);

void espat_loadEspatSys(void)
{
    DEBUG_FUNCTIONCALL("\n espat_loadEspatSys(void)");

    uint8_t memoryBankH = allocateManagedBank();
    uint8_t memoryBankL = allocateManagedBank();

    //memory_MMUtoConsole();
    uint8_t tempMMU6 = ZXN_READ_MMU6();
    uint8_t tempMMU7 = ZXN_READ_MMU7();

    ZXN_WRITE_REG(REG_MMU6, memoryBankL);
    ZXN_WRITE_REG(REG_MMU7, memoryBankH);

    //memory_MMUtoConsole();

    //printf("\n MMU6 Addr = 0x%04x, bank %03u", zxn_addr_from_mmu(REG_MMU6), ZXN_READ_MMU6());
    uint8_t *dst = 0xC000; // zxn_addr_from_mmu(REG_MMU6);
    //printf("\n *dst -> *0x%02x, %04x", *dst, dst);

    uint16_t handle = 0;
    handle = esx_f_open(espat_sysfile, ESX_MODE_OPEN_EXIST);
    esx_m_geterr(handle, errorMsg);
    //printf("\n Handle? %u, errmsg> %s", handle, errorMsg);

    uint8_t result = 0;
    result = esx_f_read(handle, dst, 8192);
    esx_m_geterr(result, errorMsg);
    //printf("\nFile read: %03u, %s", result, errorMsg);
    //printf("\n *dst -> *0x%02x, %04x", *dst, dst);

    esx_f_close(handle);

    ZXN_WRITE_REG(REG_MMU6, tempMMU6);
    ZXN_WRITE_REG(REG_MMU7, tempMMU7);

    //memory_MMUtoConsole();

    espat_drvapi.call.driver = espat_DRVID;
    espat_drvapi.call.function = NEAT_Deprecated; // (1)
    espat_drvapi.de = 0;
    espat_drvapi.hl = memoryBankL;

    //callDriverApi(espat_drvapi);
    safe_callDriverApiErrorMsg(espat_drvapi, errorMsg);

    return;
}

void espat_addBufferMemoryBank(void)
{ // This is going to be deprecated in BETA1 / RC1

    DEBUG_FUNCTIONCALL("\n espat_setMemoryBank(void)");

    // We need 2 8K banks consecutively (16K), double grab and store the second bank ID
    uint8_t memoryBankH = allocateManagedBank();
    uint8_t memoryBankL = allocateManagedBank();

    DEBUG_MSG("\nespat memoryBankH=%03u , memoryBankL=%03u", memoryBankH, memoryBankL);

    espat_drvapi.call.driver = espat_DRVID;
    espat_drvapi.call.function = NEAT_AddBank; // (6)
    espat_drvapi.de = 0;
    espat_drvapi.hl = memoryBankL;
    // espat_drvapi.hl = 0;

    // callDriverApi(espat_drvapi);
    safe_callDriverApiErrorMsg(espat_drvapi, errorMsg);

    return;
}

void espat_DriverInstall(void)
{
    DEBUG_FUNCTIONCALL("\n espatDriverInstall(void)");
    installDriver("ESPAT.drv");
    espat_loadEspatSys();
    //espat_addBufferMemoryBank();
    //espat_setBaud();
    //espat_setTimeouts();

    printf("\n\nAOK");

    return;
}

void espat_OpenConnection(void)
{
    DEBUG_FUNCTIONCALL("\n espat_OpenConnection(%s)", connectionString);
    espat_drvapi.call.driver = espat_DRVID;
    espat_drvapi.call.function = NOS_Open;
    espat_drvapi.de = strlen(connectionString);
    espat_drvapi.hl = connectionString;

    safe_callDriverApiErrorMsg(espat_drvapi, errorMsg);

    return;
}