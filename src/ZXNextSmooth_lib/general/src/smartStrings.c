#define DEBUG_OFF
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


char * safe_appendString(char * dest , char *src, uint16_t destSize)
{
    DEBUG_FUNCTIONCALL("\nsafe_appendString(dest *%02x, src *%02x, destSize %u)", dest,src,destSize );

    uint16_t destLength = strlen(dest);
    uint16_t srcLength = strlen(src);
    DEBUG_MSG("\n IF?  ( %u + %u ) %u < %u ",destLength,srcLength,(destLength + srcLength),destSize);

    if ( (destLength + srcLength) < destSize )
    {        
        strcat(dest,src);
    }
    else
    {
        DEBUG_MSG("\n [!] AppendString() failed ( %u + %u ) %u < %u ",destLength,srcLength,(destLength + srcLength),destSize);
        return NULL;
    }
    return dest;

}
