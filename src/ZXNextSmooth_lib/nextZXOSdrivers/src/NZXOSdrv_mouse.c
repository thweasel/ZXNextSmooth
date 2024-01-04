#define DEBUG_OFF
#include "../../general/include/debugging.h"

#include "../../general/Z88dkDeps.h"



static struct esx_drvapi driverApi;

// The .INSTALL command requires either a relative path or absolute path in "C:/nextzxos/driver.drv"
static char CMDLINE_installmouse[33] = "install \"c:/nextzxos/mouse.drv\"";

void mouseInstallDriver(void)
{
    printf("install mouse> %s  ", CMDLINE_installmouse); 
    uint16_t result = esx_m_execcmd(CMDLINE_installmouse);
    esx_m_geterr(result,CMDLINE_installmouse);
    printf("\nresult> %u, %s",result,CMDLINE_installmouse);
}


