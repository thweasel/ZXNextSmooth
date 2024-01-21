#define DEBUG_ON
#include "../../general/include/debugging.h"
#include "../../general/ZXNextSmooths_Z88dkDeps.h"


#include "../include/esxdos_drvapi.h"

#include "../include/esxdos_drvapi_mouse.h"

/*
    TO DO

    - Sprite mouse enable/disable
    - Attribute cursor disable
    - Change accelleration

*/

// This struct is used to send driver calls and returns with driver results

char mouseStringBuffer[ESXDOS_ERRORMSG_SIZE] = "";
extern struct esx_drvapi esxdrvApiMsg;

#define MOUSEDRV_DRIVERID (uint8_t) 126
#define MOUSEDRV_FUNCTION_GETMOUSESTATE (uint8_t) 1
#define MOUSEDRV_FUNCTION_SETCURSORSPRITE (uint8_t) 2
#define MOUSEDRV_FUNCTION_DISABLECURSORSPRITE (uint8_t) 3
#define MOUSEDRV_FUNCTION_SETCURSORATTRIBUTE (uint8_t) 4
#define MOUSEDRV_FUNCTION_DISABLECURSORATTRIBUTE (uint8_t) 5
#define MOUSEDRV_FUNCTION_ACCELLERATIONX (uint8_t) 6

extern struct mouseState currentMouseState = {
    .errorCode = 255,
    .x = 0,
    .y = 0,
    .button1 = false,
    .button2 = false,
    .wheelpos = 0,
};

void mouseStateToConsole(struct mouseState * theMouseState)
{
    DEBUG_FUNCTIONCALL("\nmouseStateToConsole(theMouseState *%02x)",theMouseState);

    printf("\nMOUSE STATE: errorCode %u \n x %03d , y %03d , b1 %01u , b2 %01u , wheelpos %02u ",
           theMouseState->errorCode,
           theMouseState->x,
           theMouseState->y,
           theMouseState->button1,
           theMouseState->button2,
           theMouseState->wheelpos);
    return;
}

// The .INSTALL command requires either a relative path or absolute path in "C:/nextzxos/driver.drv"
//static char CMDLINE_installmouse[33] = "install \"c:/nextzxos/mouse.drv\"";

void mouseInstallDriver(void)
{
    DEBUG_FUNCTIONCALL("\nmouseInstallDriver()");
    if ( 0 == installDriver("mouse.drv") )
    {
        currentMouseState.errorCode = 0;
        return;
    }
    DEBUG_MSG("\n MOUSE driver install failed %u", currentMouseState.errorCode);
    return;
}

void mouseEnableAttributeCursor(uint8_t attributeValue)
{
    DEBUG_FUNCTIONCALL("\nmouseEnableAttributeCursor(attributeValue %s)",byteAsBinaryString(attributeValue));
    
    // Driver replies in the structure, so you need to re-populate before calling again
    esxdrvApiMsg.call.driver = MOUSEDRV_DRIVERID;
    esxdrvApiMsg.call.function = MOUSEDRV_FUNCTION_SETCURSORATTRIBUTE;
    esxdrvApiMsg.hl = 0b11100111; // Attribute;
    esxdrvApiMsg.de = 0;

    currentMouseState.errorCode = safe_callDriverApiErrorMsg(esxdrvApiMsg);

    return;
}

struct mouseState * mouseGetState(void)
{
    DEBUG_FUNCTIONCALL("\nmouseGetState()");
    
    // Driver replies in the structure, so you need to re-populate before calling again
    esxdrvApiMsg.call.driver = MOUSEDRV_DRIVERID;
    esxdrvApiMsg.call.function = MOUSEDRV_FUNCTION_GETMOUSESTATE;
    esxdrvApiMsg.de = 0;
    esxdrvApiMsg.hl = 0;

    currentMouseState.errorCode = safe_callDriverApiErrorMsg(esxdrvApiMsg);
    
    currentMouseState.x = esxdrvApiMsg.de;
    currentMouseState.y = esxdrvApiMsg.hl;
    currentMouseState.button1 = true;
    currentMouseState.button2 = true;
    currentMouseState.wheelpos = 15;   

    return &currentMouseState;
}



