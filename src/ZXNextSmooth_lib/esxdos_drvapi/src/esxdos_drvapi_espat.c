#define DEBUG_ON
#include "../../general/include/debugging.h"
#include "../../general/Z88dkDeps.h"

#include "../include/esxdos_drvapi.h"

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

    https://gitlab.com/thesmog358/tbblue/-/blob/master/docs/extra-hw/wifi/ESPATreadme.TXT

*/

void espatDriverInstall (void)
{
    DEBUG_FUNCTIONCALL("espatDriverInstall(void)");
    installDriver("ESPAT.drv");
    return;
}