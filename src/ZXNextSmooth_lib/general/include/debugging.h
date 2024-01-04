#ifndef debugging_h
#define debugging_h

#ifdef DEBUG_ON
#define DEBUG_MSG printf
#define DEBUG_FUNCTIONCALL printf
#define DEBUG_NEXTREG debugNEXTREGtoConsole
#else
#define DEBUG_MSG //DEBUG_OFF 
#define DEBUG_FUNCTIONCALL //DEBUG_OFF
#define DEBUG_NEXTREG //DEBUG_OFF  
#endif

#endif