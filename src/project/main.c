#include "include/global.h"

#include "include/test.h"

int main(int argc, char const *argv[])
{

    zx_border(PAPER_WHITE);
    zx_cls_attr(PAPER_WHITE);
    zx_cls_pix(INK_BLACK);
    
    mouseInstallDriver();

    //test();
    return 0;
}
