#define DEBUG_OFF
#include "../../general/include/debugging.h"
#include "../../general/ZXNextSmooths_Z88dkDeps.h"

#include "../include/NextHardware.h"

void memory_MMUtoConsole(void)
{
    printf("\n MMU 0[%03u],1[%03u],2[%03u],3[%03u],4[%03u],5[%03u],6[%03u],7[%03u]",
           ZXN_READ_MMU0(),
           ZXN_READ_MMU1(),
           ZXN_READ_MMU2(),
           ZXN_READ_MMU3(),
           ZXN_READ_MMU4(),
           ZXN_READ_MMU5(),
           ZXN_READ_MMU6(),
           ZXN_READ_MMU7());
    return;
}

void memory_MMUbankSwap(uint8_t mmuREG, uint8_t bankID)
{
    DEBUG_FUNCTIONCALL("\nmemory_swapMMUbank(mmuREG %03u ,BankNumber %03u)", mmuREG, bankID);
    set_NEXTREGvalue(mmuREG, bankID);
    return;
}