#ifndef esxdos_fileIO_h
#define esxdos_fileIO_h

#include "../../general/ZXNextSmooths_Z88dkDeps.h"

#include "../include/esxdos_structures.h"

extern fileHandle openFileHandle(char *pathFileName, esxdosFileAccessMask fileAccessType);
extern uint16_t readFile(fileHandle openHandle, void *readDataInto, size_t numberOfBytesToRead);
extern uint8_t closeFileHandle (fileHandle openFileHandle);



#endif