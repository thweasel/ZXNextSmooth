#ifndef smartStrings_h
#define smartStrings_h
#include "../ZXNextSmooths_Z88dkDeps.h"

extern char * byteAsBinaryString(uint8_t byte);
extern void binaryToConsole(uint8_t byte);


extern char * safe_appendString(char * dest , char *src, uint16_t destSize);

#endif