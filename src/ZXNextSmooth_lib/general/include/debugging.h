#ifndef debugging_h
#define debugging_h

#include "../Z88dkDeps.h"

#ifdef DEBUG_ON
#define DEBUG_MSG printf
#define DEBUG_FUNCTIONCALL printf
#define DEBUG_NEXTREG debugNEXTREGtoConsole
#define DEBUG_DRIVERAPI driverApiToConsole
#else
#define DEBUG_MSG // printf 
#define DEBUG_FUNCTIONCALL // printf
#define DEBUG_NEXTREG // debugNEXTREGtoConsole  
#define DEBUG_DRIVERAPI // driverApiCallToConsole
#endif


void debug_showU16consoleDEC (uint16_t byte);
void debug_showU16consoleHEX (uint16_t byte);

#endif