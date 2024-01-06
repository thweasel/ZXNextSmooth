#ifndef esxdos_drvapi_mouse_h
#define esxdos_drvapi_mouse_h

#include "../../general/Z88dkDeps.h"

struct mouseState
{
    uint16_t errorCode;
    uint16_t x;
    uint16_t y;
    bool button1;
    bool button2;
    uint8_t wheelpos;
};

extern struct mouseState currentMouseState;

extern void mouseInstallDriver(void);

extern void mouseEnableAttributeCursor(uint8_t attributeValue);

extern struct mouseState * mouseGetState(void);

extern void mouseStateToConsole (struct mouseState * mouseState);

#endif