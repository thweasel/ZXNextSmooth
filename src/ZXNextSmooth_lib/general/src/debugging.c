#ifndef debugging_h
#define debugging_h

#include "../ZXNextSmooths_Z88dkDeps.h"

void debug_showU16consoleDEC(uint16_t byte)
{
    printf("\n u16 %u = H: %03u , L: %03u", byte, (uint8_t)(byte >> 8), (uint8_t)byte);
    return;
}

void debug_showU16consoleHEX(uint16_t byte)
{
    printf("\n u16 %04x = H: %02x , L: %02x", byte, (uint8_t)(byte >> 8), (uint8_t)byte);
    return;
}

#endif