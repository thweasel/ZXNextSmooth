#define DEBUG_OFF
#include "../../general/include/debugging.h"
#include "../../general/ZXNextSmooths_Z88dkDeps.h"

#include "../include/esxdos_drvapi.h"

extern int errno; // File errors are reported here comes from errno.h
/* errno codes
#define EOK            __EOK
#define EACCES         __EACCES
#define EBADF          __EBADF
#define EBDFD          __EBDFD
#define EDOM           __EDOM
#define EFBIG          __EFBIG
#define EINVAL         __EINVAL
#define EIO            __EIO
#define EMFILE         __EMFILE
#define ENFILE         __ENFILE
#define ENOLCK         __ENOLCK
#define ENOMEM         __ENOMEM
#define ENOTSUP        __ENOTSUP
#define EOVERFLOW      __EOVERFLOW
#define ERANGE         __ERANGE
#define ESTAT          __ESTAT
#define EWOULDBLOCK    __EWOULDBLOCK
#define EAGAIN         __EAGAIN
*/

// esx_f_open(const char *filename,unsigned char mode);
// esx_f_read(unsigned char handle,void *dst,size_t nbytes);



fileHandle openFileHandle(char *pathFileName, esxdosFileAccessMask fileAccessType)
{
    esxdos_clearErrorCodes();
    fileHandle handle = esx_f_open(pathFileName, fileAccessType);
    DEBUG_MSG ("\n esx_f_open() fileHandle: %03u",handle);
    if (0 != errno)
    {
        esxdos_setErrorCodes(errno);
        return NULL;
    }
    return handle;
}

bytesRead readFile(fileHandle openHandle, void *readDataInto, size_t numberOfBytesToRead)
{
    esxdos_clearErrorCodes();
    bytesRead bytes = esx_f_read(openHandle, readDataInto, numberOfBytesToRead);
    DEBUG_MSG ("\n esx_f_read() bytesRead: %03u",bytes);
    if (0 != errno)
    {
        esxdos_setErrorCodes(errno);
        return NULL;
    }
    return bytes;
}

uint8_t closeFileHandle (fileHandle openFileHandle)
{
    esxdos_clearErrorCodes();
    uint8_t result = esx_f_close(openFileHandle);
    DEBUG_MSG ("\n esx_f_close() result: %03u",result);
    if (0 != errno)
    {
        esxdos_setErrorCodes(errno);
        return NULL;
    }

    return result;
}