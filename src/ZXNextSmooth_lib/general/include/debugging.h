#ifndef debugging_h
#define debugging_h

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

#endif