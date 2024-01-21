#ifndef esxdos_drv_h
#define esxdos_drv_h
#include "../../general/ZXNextSmooths_Z88dkDeps.h"


typedef uint8_t channel;
typedef uint8_t driverID;
typedef uint8_t driverFunction;
typedef uint16_t cpuDE;
typedef uint16_t cpuHL;
typedef uint8_t cpuD;
typedef uint8_t cpuE;
typedef uint8_t cpuH;
typedef uint8_t cpuL;

extern struct esx_drvapi esxdrvApiMsg;
extern struct esxdos_drvapi_REG driverApiReturnREG;


//
//  ERROR HANDLING
//


typedef uint8_t esxdosErrorCode;
/* esxdos reports error codes that can be converted to error messages, 
    Codes can come from:
        - errno 
        - function return
        - driver API message (CPU Registers)
*/

#define ESXDOS_ERRORMSG_SIZE (uint8_t) 33
extern esxdosErrorCode lastEsxDos_errorCode;
extern char lastEsxDos_ErrorMessage[ESXDOS_ERRORMSG_SIZE];

void esxdos_clearErrorCodes (void);
void esxdos_setErrorCodes(esxdosErrorCode errorCode);

//
// FILE IO
//


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

typedef uint16_t bytesRead ;
typedef uint16_t bytesWritten ;


//
//  DRIVER API MESSAGE SYSTEM
//

void driverApiToConsole(struct esx_drvapi *showDriverApiCall, bool returnValue);

extern uint16_t installDriver(char * driverName);
extern uint16_t uninstallDriver(char *driverName);

extern void callDriver(driverID driver, driverFunction function, cpuDE de, cpuHL hl);

//
//  Generic channel functions
//

void nos_OutputChar(driverID driver, channel handle, char c);
char nos_InputChar(driverID driver, channel handle);

uint16_t nos_StreamSize(driverID driver , channel handle);

#endif