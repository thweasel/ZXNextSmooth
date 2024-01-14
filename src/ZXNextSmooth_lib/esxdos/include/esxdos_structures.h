#ifndef esxdos_structures_h
#define esxdos_structures_h
#include "../../general/ZXNextSmooths_Z88dkDeps.h"

#define ESXDOS_ERRORMSG_SIZE (uint8_t) 33

typedef uint16_t fileHandle;
/* When files are opened, we need to keep a handle on them to so they do not get lost
    Yep, really that simple.
*/

typedef uint8_t esxdosFileAccessMask;
/* esxdosFileAccessMask -- FILE ACCESS TYPES ARE MADE FROM COMBINING MASKS FROM ESXDOS.H

// START WITH ONE OF THESE

#define ESX_MODE_READ __esx_mode_read
#define ESX_MODE_WRITE __esx_mode_write
#define ESX_MODE_RW (__esx_mode_read | __esx_mode_write)

    Optional Short hand style
    #define ESX_MODE_R __esx_mode_read
    #define ESX_MODE_W __esx_mode_write
    

// | (OR) WITH SOME OF THESE

#define ESX_MODE_OPEN_EXIST __esx_mode_open_exist
#define ESX_MODE_OPEN_CREAT __esx_mode_open_creat
#define ESX_MODE_OPEN_CREAT_NOEXIST __esx_mode_creat_noexist
#define ESX_MODE_OPEN_CREAT_TRUNC __esx_mode_creat_trunc

#define ESX_MODE_USE_HEADER __esx_mode_use_header

*/

typedef uint8_t esxdosErrorCode;
/* esxdos reports error codes that can be converted to error messages, 
    Codes can come from:
        - errno 
        - function return
        - driver API message (CPU Registers)
*/

typedef uint16_t bytesRead ;
typedef uint16_t bytesWritten ;

extern esxdosErrorCode esxdos_errorCode;
extern char esxdos_errorMsg[ESXDOS_ERRORMSG_SIZE];


void esxdos_clearErrorCodes (void);
void esxdos_setErrorCodes(esxdosErrorCode errorCode);

#endif