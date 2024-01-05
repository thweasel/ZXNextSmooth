#ifndef NZXOSdrv_mouse_h
#define NZXOSdrv_mouse_h

#include "../../general/Z88dkDeps.h"

//#include "../include/NZXOSdrv_mouse.h"



struct mouseState
{
    uint16_t x;
    uint16_t y;
    bool button1;
    bool button2;
    uint8_t wheelpos;
};

extern void mouseInstallDriver(void);
void mouseEnableAttributeCursor(uint8_t attributeValue);
struct mouseState * mouseGetState(void);
void mouseStateToConsole (struct mouseState * mouseState);

#endif