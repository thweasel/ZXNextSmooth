#ifndef terminalFunctionMacros_h
#define terminalFunctionMacros_h

#include <sys/ioctl.h>

#define TERMINAL_FUNC_PAUSEONSCROLL_DISABLE ioctl(1,IOCTL_OTERM_PAUSE,0)
#define TERMINAL_FUNC_PAUSEONSCROLL_ENABLE  ioctl(1,IOCTL_OTERM_PAUSE,1)

#endif