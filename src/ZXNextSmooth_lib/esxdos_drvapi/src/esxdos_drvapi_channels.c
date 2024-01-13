#define DEBUG_OFF
#include "../../general/include/debugging.h"
#include "../../general/Z88dkDeps.h"

#include "../include/esxdos_drvapi.h"


/*  Channels access is provided by some installable drivers, we can access them using the driver api

    ; The following calls are used to allow your driver to support 
    ; channels for i/o (manipulated with BASIC commands like OPEN #). 
    ; Each call is optional - just return with carry set and A=0 
    ; for any calls that you don't want to provide. 
    ;
    ; B=$f6: copy screen (usually for printer drivers) 
    ; B=$f7: return output status 
    ; B=$f8: return input status 
    ; B=$f9: open channel 
    ; B=$fa: close channel 
    ; B=$fb: output character 
    ; B=$fc: input character 
    ; B=$fd: get current stream pointer 
    ; B=$fe: set current stream pointer 
    ; B=$ff: get stream size/extent
*/

/*
    Channels have designations, "Driver ID"

        'k' - keyboard
        's' - screen
        'p' - printer

        'N' - Network (78)

*/

/*
    Existing system Streams 0 - 3

    0 - Keyboard
    1 - Keyboard
    2 - Screen
    3 - Printer
*/

/*
    Open a "channel" and assign a "stream"
*/


char streams [15] = "kksp";

/*
void openChannelOnStream (char channel, uint8_t streamNumber)
{

    return;
}
*/