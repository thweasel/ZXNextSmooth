#define DEBUG_ON
#include "../../general/include/debugging.h"

#include "../../general/Z88dkDeps.h"

char byteBinaryString[10]= "b00000000";

char * byteAsBinaryString(uint8_t byte)
{
    for (uint8_t i = 1; i < 9; i++)
    {
        if (byte & 0b10000000)
        {
            byteBinaryString[i]=(char)'1';
        }
        else
        {
            byteBinaryString[i]=(char)'0';
        }
        byte <<= 1;
    }
    return byteBinaryString;
}

void binaryToConsole(uint8_t byte)
{
    printf("%s", byteAsBinaryString(byte));
}
