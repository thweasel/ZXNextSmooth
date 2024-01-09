#define DEBUG_ON
#include "../../general/include/debugging.h"
#include "../../general/Z88dkDeps.h"

#include "../include/esxdos_drvapi.h"
#include "../../memory/export_memory.h"

/*

    Driver ID is 'N' which is 78 decimal

    ESPAT Driver function numbers

        B=1: Set memory bank for CMD and SYS (Depreciated)
        B=2: Set current channel for DRIVER
        B=3: Get CMD or IPD channel values
        B=4: Set CMD or IPD values
        B=5: Set CMD and IPD timeouts
        B=6: Add 16K bank to IPD receive pool
        B=7: Remove 16K BANK from channel IPD only does not apply to CMD
        B=8: Get ESP Link ID from handle
        B=9: Specific UART BAUD rate to be set from lookup table.
        B=10: Set output buffer mode for channel
        B=11: enable/disable embedded IPD detect

    https://gitlab.com/thesmog358/tbblue/-/blob/master/docs/extra-hw/wifi/ESPATreadme.TXT

*/


# define espat_DRVID (uint8_t) 78 // 'N'
# define espat_DRVFUNC_SETMEMBANK (uint8_t) 1

static uint8_t memoryBank=0;
static struct esx_drvapi espat_drvapi;





void espat_setMemoryBank(void)
{   // This is going to be deprecated in BETA1 / RC1

    DEBUG_FUNCTIONCALL("\nespat_setMemoryBank(void)");

    // We need 2 8K banks consecutively (16K), double grab and store the second bank ID
    allocateManagedBank();
    memoryBank = allocateManagedBank();

    DEBUG_MSG("\nespat memoryBank=%03u",memoryBank);

    espat_drvapi.call.driver = espat_DRVID;
    espat_drvapi.call.function = espat_DRVFUNC_SETMEMBANK;
    espat_drvapi.de = 0;
    espat_drvapi.hl = memoryBank;

    safe_callDriverApi(&espat_drvapi);

    printf("\ndrvapi H,L: 0x%04x",espat_drvapi.hl);

    return;
}

void espat_DriverInstall (void)
{
    DEBUG_FUNCTIONCALL("espatDriverInstall(void)");
    installDriver("ESPAT.drv");
    espat_setMemoryBank();
    return;
}

void espat_OpenConnection (void)
{

}