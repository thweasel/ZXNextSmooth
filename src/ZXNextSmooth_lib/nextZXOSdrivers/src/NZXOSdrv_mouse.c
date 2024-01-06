#define DEBUG_ON
#include "../../general/include/debugging.h"

#include "../../general/Z88dkDeps.h"

#include "../include/NZXOSdrv_mouse.h"

// This struct is used to send driver calls and returns with driver results
struct esx_drvapi driverApi;

#define MOUSEDRV_DRIVERID 126
#define MOUSEDRV_FUNCTION_GETMOUSESTATE 1
#define MOUSEDRV_FUNCTION_SETCURSORSPRITE 2
#define MOUSEDRV_FUNCTION_DISABLECURSORSPRITE 3
#define MOUSEDRV_FUNCTION_SETCURSORATTRIBUTE 4
#define MOUSEDRV_FUNCTION_DISABLECURSORATTRIBUTE 5
#define MOUSEDRV_FUNCTION_XACCELLERATION 6

extern static struct mouseState currentMouseState = {
    .x = 0,
    .y = 0,
    .button1 = false,
    .button2 = false,
    .wheelpos = 0,
};

void driverApiToConsole(struct esx_drvapi * showDriverApi)
{
    printf("\nDRIVERAPI: \n DRVID %03u , FUNC %03u , bc %u , de %u , hl %u",
           showDriverApi->call.driver,
           showDriverApi->call.function,
           showDriverApi->bc,
           showDriverApi->de,
           showDriverApi->hl);
}

void mouseStateToConsole(struct mouseState * theMouseState)
{
    DEBUG_FUNCTIONCALL("\nmouseStateToConsole(struct mouseState * theMouseState)");

    printf("\nMOUSE STATE\n x %03d , y %03d , b1 %01u , b2 %01u , wheelpos %02u ",
           theMouseState->x,
           theMouseState->y,
           theMouseState->button1,
           theMouseState->button2,
           theMouseState->wheelpos);
    return;
}

// The .INSTALL command requires either a relative path or absolute path in "C:/nextzxos/driver.drv"
static char CMDLINE_installmouse[33] = "install \"c:/nextzxos/mouse.drv\"";

void mouseInstallDriver(void)
{
    DEBUG_FUNCTIONCALL("\nmouseInstallDriver(void)");
    DEBUG_MSG("\ninstall mouse> %s  ", &CMDLINE_installmouse);
    uint16_t result = esx_m_execcmd(CMDLINE_installmouse);
    esx_m_geterr(result, CMDLINE_installmouse);
    printf("\nresult> %u, %s", result, &CMDLINE_installmouse);

    // Driver replies in the structure, so you need to re-populate before calling again
    driverApi.call.driver = (uint8_t)MOUSEDRV_DRIVERID;
    driverApi.call.function = 0;
    driverApi.de = 0;
    driverApi.hl = 0;

    driverApiToConsole(driverApi);
}

void mouseEnableAttributeCursor(uint8_t attributeValue)
{
    DEBUG_FUNCTIONCALL("\nmouseEnableAttributeCursor(uint8_t attributeValue)");
    
    // Driver replies in the structure, so you need to re-populate before calling again
    driverApi.call.driver = MOUSEDRV_DRIVERID;
    driverApi.call.function = (uint8_t)MOUSEDRV_FUNCTION_SETCURSORATTRIBUTE;
    driverApi.hl = 0b11100111; // Attribute;
    driverApi.de = 0;

    uint8_t result = esx_m_drvapi(&driverApi);
    esx_m_geterr(result, CMDLINE_installmouse);
    DEBUG_MSG("\n esx_m_drvapi(driverApi) ,%u %s", result, &CMDLINE_installmouse);
    driverApiToConsole(&driverApi);

    return;
}

struct mouseState * mouseGetState(void)
{
    DEBUG_FUNCTIONCALL("\nmouseGetState(void)");
    DEBUG_MSG("\nDRIVER ID> %u", MOUSEDRV_DRIVERID);

    // Driver replies in the structure, so you need to re-populate before calling again
    driverApi.call.driver = MOUSEDRV_DRIVERID;
    driverApi.call.function = (uint8_t)MOUSEDRV_FUNCTION_GETMOUSESTATE;
    driverApi.de = 0;
    driverApi.hl = 0;

    uint8_t result = esx_m_drvapi(&driverApi);
    esx_m_geterr(result, CMDLINE_installmouse);
    DEBUG_MSG("\n esx_m_drvapi(driverApi) ,%u %s", result, &CMDLINE_installmouse);
    driverApiToConsole(&driverApi);

    
    currentMouseState.x = driverApi.de;
    currentMouseState.y = driverApi.hl;
    currentMouseState.button1 = true;
    currentMouseState.button2 = true;
    currentMouseState.wheelpos = 15;
    

    return currentMouseState;
}
