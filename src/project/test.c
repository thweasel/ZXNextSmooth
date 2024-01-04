
#include "include/global.h"


static struct paletteDef_ClassicULA res;
static struct paletteDef_ClassicULA ambersULAPallet = {
    .Inks = COLOURARRAY8_amber,
    .BrightInks = COLOURARRAY8_amber,
    .Papers = COLOURARRAY8_amber,
    .BrightPapers = COLOURARRAY8_amber};

int test(void)
{
    zx_border(PAPER_BLACK);
    zx_cls_attr(PAPER_BLACK);
    zx_cls_pix(INK_WHITE);

    printf("  -- RUNNING -- \n");

    setClassicULAPalette(ambersULAPallet, FIRSTPALETTE);

    printf("\nDOS Version: %u ", esx_m_dosversion());
    printf("\nDrive: %C ", esx_m_getdrv());
    printf("\nfree what?: %u ", esx_f_getfree());

    printf("\nambersULAPallet:\n");
    classicULApaletteToConsole(ambersULAPallet);

    printf("\nFirst ULAPallet:\n");
    classicULApaletteToConsole(getClassicULAPalette(FIRSTPALETTE, res));
    
    // executeDotCommand(CMDLINE_cow);

    printf("\nEND");
    while (1)
    {
        printf("");
    }
    
    return 0;
}