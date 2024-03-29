#ifndef nextREG_operations_h
#define nextREG_operations_h
#include "../../general/ZXNextSmooths_Z88dkDeps.h"

extern void nextregToConsole(uint8_t REG, uint8_t DAT);
extern void nextregToConsoleNewLine(uint8_t REG, uint8_t DAT);

extern void set_NEXTREGvalue(uint8_t REG, uint8_t DAT);
extern uint8_t get_NEXTREGvalue(uint8_t REG);

extern void setBytes_NEXTREG(uint8_t REG, uint8_t *DATS, uint8_t SIZE);

extern void maskON_NEXTREG(uint8_t REG, uint8_t DAT);
extern void maskOFF_NEXTREG(uint8_t REG, uint8_t DAT);

extern void showNEXTREGonConsole(uint8_t REG);

#endif