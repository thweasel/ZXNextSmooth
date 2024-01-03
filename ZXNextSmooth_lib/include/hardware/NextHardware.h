#ifndef nextHardware_h
#define nextHardware_h

#include <stdint.h>
#include <arch/zxn.h>

#include "../smartStrings.h"

//
// DEBUGGING
//
#define DEBUG_ON
#include "../debugging.h"


void nextregToConsole(uint8_t REG, uint8_t DAT)
{
    printf("NEXT: REG 0x%02X | DAT %03u 0x%02X %s", REG, DAT, DAT, byteAsBinaryString(DAT));
}

void nextregToConsoleNewLine(uint8_t REG, uint8_t DAT)
{
    putchar('\n');
    nextregToConsole(REG, DAT);
    putchar('\n');
}

//
//  DEBUGGING
//

#ifdef DEBUG_ON
void debugNEXTREGtoConsole(char *msg, uint8_t REG, uint8_t DAT)
{
    printf("%s\n = ",msg);
    nextregToConsole(REG, DAT);
}
#endif

void set_NEXTREGvalue(uint8_t REG, uint8_t DAT)
{
    DEBUG_FUNCTIONCALL("\nset_NEXTREGvalue(REG %02X, DAT %u)",REG,DAT);

    IO_NEXTREG_REG = REG;
    IO_NEXTREG_DAT = DAT;
    
    DEBUG_NEXTREG("", IO_NEXTREG_REG, IO_NEXTREG_DAT);
    return;
}

uint8_t get_NEXTREGvalue(uint8_t REG)
{
    DEBUG_FUNCTIONCALL("\nget_NEXTREGvalue(REG %02X)",REG);

    IO_NEXTREG_REG = REG;
    
    DEBUG_NEXTREG("", IO_NEXTREG_REG, IO_NEXTREG_DAT);
    return IO_NEXTREG_DAT;    
}

void setBytes_NEXTREG(uint8_t REG, uint8_t *DATS, uint8_t SIZE)
{
    DEBUG_FUNCTIONCALL("\nsetBytes_NEXTREG(REG %02X, *DATS, SIZE %u)", REG, SIZE);   

    IO_NEXTREG_REG = REG; 
    for (uint8_t i = 0; i < SIZE; i++)
    {       
        IO_NEXTREG_DAT = DATS[i];
        DEBUG_NEXTREG("", IO_NEXTREG_REG, IO_NEXTREG_DAT);
    }
    return;
}

void maskON_NEXTREG(uint8_t REG, uint8_t DAT)
{ // Any bits in DAT at 1 are set 1
    DEBUG_FUNCTIONCALL("\nmaskON_NEXTREG(REG 0x%02x, DAT %u %s)", REG, DAT, byteAsBinaryString(DAT));

    IO_NEXTREG_REG = REG;
    IO_NEXTREG_DAT |= DAT;
    
    DEBUG_NEXTREG("", IO_NEXTREG_REG, IO_NEXTREG_DAT);
    return;
}

void maskOFF_NEXTREG(uint8_t REG, uint8_t DAT)
{ // Any bits in DAT at 0 are set 0
    DEBUG_FUNCTIONCALL("\nmaskOFF_NEXTREG(REG 0x%02x, DAT %u %s)", REG, DAT, byteAsBinaryString(DAT));

    IO_NEXTREG_REG = REG;
    IO_NEXTREG_DAT &= DAT;

    DEBUG_NEXTREG("", IO_NEXTREG_REG, IO_NEXTREG_DAT);
    return;
}

void showNEXTREGonConsole(uint8_t REG)
{
    nextregToConsole(REG, get_NEXTREGvalue(REG));
    putchar('\n');
    return;
}

#undef DEBUG_ON
#endif
