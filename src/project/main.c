#include "include/global.h"

#include "include/test.h"
#include <sys/ioctl.h>
#include <z80.h>

int main(int argc, char const *argv[])
{

    zx_border(PAPER_WHITE);
    zx_cls_attr(PAPER_WHITE);
    zx_cls_pix(INK_BLACK);

    /*
    puts("\x1B[?4h");
    puts("\x1Bs");
    puts("\x4\x1");
    */
   
    //ioctl(1,IOCTL_OTERM_PAUSE,0); // Scroll like a normal console
    TERMINAL_FUNC_PAUSEONSCROLL_DISABLE;
   



    mouseInstallDriver();
    mouseEnableAttributeCursor(1);

    uint16_t i = 0;
    while (1)
    {
        
        z80_delay_ms(250);
        mouseStateToConsole(mouseGetState());       
        //mouseGetState();
        z80_delay_ms(250);

        printf("\n%d",i++);
        //z80_delay_ms(50);
        if (i > 100)
        {
            TERMINAL_FUNC_PAUSEONSCROLL_ENABLE;
        }
    }

    // test();
    return 0;
}
