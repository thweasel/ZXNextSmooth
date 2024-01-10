#ifndef esxdos_drv_h
#define esxdos_drv_h
#include "../../general/Z88dkDeps.h"

extern struct esx_drvapi driverApiMsg;

extern void driverApiToConsole(struct esx_drvapi * showDriverApiCall);

extern uint16_t installDriver(char * driverName);
extern uint16_t uninstallDriver(char *driverName);

extern uint8_t DUMMY_callDriverApi(struct esx_drvapi *driverApiCall);

extern uint8_t callDriverApi(struct esx_drvapi * driverApiCall);
extern uint8_t safe_callDriverApi(struct esx_drvapi * driverApiCall);

extern uint8_t callDriverApiErrorMsg (struct esx_drvapi * driverApiCall, char * errorMsgBuffer);
extern uint8_t safe_callDriverApiErrorMsg(struct esx_drvapi * driverApiCall, char * errorMsgBuffer);

#endif