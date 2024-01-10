#include "include/global.h"

#include "include/test.h"
#include <z80.h>

// Dec these here Def them down the bottom.
void bankTesting(void);
void mouseTesting(void);
void espatTesting(void);

int main(int argc, char const *argv[])
{

    zx_border(PAPER_WHITE);
    zx_cls_attr(PAPER_WHITE);
    zx_cls_pix(INK_BLACK);

    //TERMINAL_FUNC_PAUSEONSCROLL_DISABLE;

    espatTesting();

    // bankTesting();

    // test();
    return 0;
}

void espatTesting(void)
{
    espat_DriverInstall();
    // espat_OpenConnection();

    z80_delay_ms(250);
    // printf("\nnethandle?: 0x%02x, %c",espat_drvapi.call.function,espat_drvapi.call.function);
    while (1)
    {
        z80_delay_ms(250);
    }

return;
}

void bankTesting(void)
{
    printf("\ntotal banks: %u ", totalBanks(RAM_BANK));
    printf("\navailable banks: %u", availableBanks(RAM_BANK));
    printf("\nallocateBank %u ", allocateBank(RAM_BANK));
    printf("\navailable banks: %u", availableBanks(RAM_BANK));

    allocateManagedBank();
    allocateManagedBank();
    allocateManagedBank();
    allocateManagedBank();
    allocateManagedBank();
    allocateManagedBank();
    allocateManagedBank();
    allocateManagedBank();
    allocateManagedBank();

    while (1)
        ;
}

void mouseTesting(void)
{
    mouseInstallDriver();
    mouseEnableAttributeCursor(1);

    uint16_t i = 0;
    while (1)
    {
        printf("\n%d", i++);
        z80_delay_ms(250);
        mouseStateToConsole(mouseGetState());
        z80_delay_ms(250);
    }
}