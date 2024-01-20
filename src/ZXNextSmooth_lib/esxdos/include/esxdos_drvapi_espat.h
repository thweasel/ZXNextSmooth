#ifndef esxdos_drvapi_espat_h
#define esxdos_drvapi_espat_h
#include "../../general/ZXNextSmooths_Z88dkDeps.h"

typedef uint8_t nethandle;
typedef uint16_t networkPort;

void espat_DriverInstall(void);

nethandle espat_OpenConnection(char *connectionString);
nethandle espat_OpenTCPConnection(char *addr, networkPort port);

void espat_SendChar(nethandle handle, char c);

void espat_testSend(nethandle myNetHandle);
char espat_RecvChar(nethandle handle);
#endif