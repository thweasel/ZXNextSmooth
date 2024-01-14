#define DEBUG_ON
#include "../../general/include/debugging.h"
#include "../../general/ZXNextSmooths_Z88dkDeps.h"

#include "../include/esxdos_structures.h"

extern int errno; // File errors are reported here comes from errno.h

esxdosErrorCode esxdos_errorCode = 0;

char esxdos_errorMsg[ESXDOS_ERRORMSG_SIZE] = " ";

void esxdos_clearErrorCodes(void)
{
    errno = 0;
    esxdos_errorCode = 0;
    esxdos_errorMsg[0] = '\0';
    return;
}

void esxdos_setErrorCodes(esxdosErrorCode errorCode)
{
    esxdos_errorCode = errno;
    esx_m_geterr(errno, esxdos_errorMsg);
    return;
}