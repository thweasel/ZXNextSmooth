#ifndef esxdos_drvapi_espat_h
#define esxdos_drvapi_espat_h
#include "../../general/ZXNextSmooths_Z88dkDeps.h"

#define DRIVERID_espat (uint8_t)78 // 'N'

typedef uint8_t nethandle;
typedef uint16_t networkPort;

void espat_DriverInstall(void);

nethandle espat_OpenConnection(char *connectionString);
nethandle espat_OpenTCPConnection(char *addr, networkPort port);

void espat_SendChar(nethandle handle, char c);
char espat_RecvChar(nethandle handle);

bytesWritten netWrite(fileHandle openHandle, void *dataToWrite, size_t numberOfBytesToWrite);

void espat_testSend(nethandle myNetHandle);

void setDriverCurrentChannel(uint16_t channel);
void getChannelValues(channel handle);
void setCMDIPDvalues(void);

void getLinkIDfromHandle(nethandle handle);
void setOutputBufferMode(nethandle channel, uint8_t mode);
void setIPDdetect(channel handle);
#endif