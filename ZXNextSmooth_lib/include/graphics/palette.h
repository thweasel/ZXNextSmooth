#ifndef PALETTE_H
#define PALETTE_H

#include <arch/zxn.h>


#include "colour8arrays.h"

#include "../hardware/NextHardware.h"
#include "../smartStrings.h"

// NEXT registers are accessed via IO ports
// IO_NEXTREG_REG & IO_NEXTREG_DAT

/* Palette Index 0x40

    7-0 Reads or writes palette colour atIndex to be manipulated

Classic ULA (palette memory map)
    Index Colours 0-7 Ink
    8-15 Bright ink
    16-23 Paper
    24-31 Bright paper Border is taken from paper colours.

ULA+
    Index Colours 0-64
    Ink Paper and border are taken from Transparency Colour Fallback 0x4A

ULANext normal mode
    Index Colours 0-127 Ink (only a subset)
    128-255 Paper (only a subset)
    Border is taken from paper colours. The number of active indices depends on the number of attribute bits assigned to ink and paper out of the attribute byte by Enhanced ULA Ink Colour Mask $42.

ULANext full-ink mode
    Index Colours 0-255 Ink Paper and border are taken from Transparency Colour Fallback 0x4A

 - *Least significant bit of blue is set to OR between B2 and B1.
 - *Writing the value will automatically increment atIndex in Palette Index $40, if auto-increment is enabled in Enhanced ULA Control $43 (page 65). Read doesn’t auto-increment atIndex.

*/
#define NEXTREG_PaletteIndex 0x40

#define COLOURARRAYSIZE_ClassicULA 8
struct paletteDef_ClassicULA
{
    // A ULA paletteDef contains 32 RGB bytes for Inks, BrightInks, Papers and BrightPapers
    // Border uses the paper colours
    uint8_t Inks[COLOURARRAYSIZE_ClassicULA];
    uint8_t BrightInks[COLOURARRAYSIZE_ClassicULA];
    uint8_t Papers[COLOURARRAYSIZE_ClassicULA];
    uint8_t BrightPapers[COLOURARRAYSIZE_ClassicULA];
};

void classicULApaletteToConsole(struct paletteDef_ClassicULA *paletteDef)
{
    printf("\nInks   : bInks  : Paper  : bPaper\n");
    for (uint8_t i = 0; i < COLOURARRAYSIZE_ClassicULA; i++)
    {
        printf("%03u %02X : %03u %02X : %03u %02X : %03u %02X \n",
               paletteDef->Inks[i], paletteDef->Inks[i],
               paletteDef->BrightInks[i], paletteDef->BrightInks[i],
               paletteDef->Papers[i], paletteDef->Papers[i],
               paletteDef->BrightPapers[i], paletteDef->BrightPapers[i]);
    }
    return;
}

#define COLOURARRAYSIZE_ULAplus 64
struct paletteDef_ULAplus
{
    uint8_t Inks[COLOURARRAYSIZE_ULAplus];
    uint8_t Papers[COLOURARRAYSIZE_ULAplus]; // Placed in Transparency Colour Fallback 0x4A
};

/* Palatte Value 0x41
    8 bit colours RRRGGGBB
    B3 is OR of B1 and B0
*/
#define NEXTREG_PaletteValue 0x41 // Colour data value (8-bit colour)

/* Enhanced ULA Ink Colour Mask 0x42
    Bit Effect 7-0 The number for last ink colour entry in the palette.
    Only used when ULANext mode is enabled (see Enhanced ULA Control 0x43).

    Only the following values are allowed, harware behavior is unpredictable for other values:
        1 Ink and paper only use 1 colour each on indices 0 and 128 respectively
        3 Ink and paper use 4 colours each, on indices 0-3 and 128-131
        7 Ink and paper use 8 colours each, on indices 0-7 and 128-135
        15 Ink and paper use 16 colours each, on indices 0-15 and 128-143
        31 Ink and paper use 32 colours each, on indices 0-31 and 128-159
        63 Ink and paper use 64 colours each, on indices 0-63 and 128-191
        127 Ink and paper use 128 colours each, on indices 0-127 and 128-255
        255 Enables full-ink colour mode where all indices are ink. In this mode paper and border are taken from Transparency Colour Fallback 0x4A
*/
#define NEXTREG_EnhancedULAInkColourMask 0x42

/* Enhanced ULA Control 0x43
Bit Effect
7   1 to enable palette atIndex auto-increment, 0 to enable
6-4 Selects palette for read or write
        000 ULA first palette
        100 ULA second palette
        001 Layer 2 first palette
        101 Layer 2 second palette
        010 Sprites first palette
        110 Sprites second palette
        011 Tilemap first palette
        111 Tilemap second palette
3   Selects active Sprites palette (0 = first palette, 1 = second palette)
2   Selects active Layer 2 palette (0 = first palette, 1 = second palette)
1   Selects active ULA palette (0 = first palette, 1 = second palette)
0   Enables ULANext mode if 1 (0 after reset)
*/
#define NEXTREG_EnhancedULAControl 0x43 // Select the active pallet for display or editing for ULA, Layer 2 and Sprites

typedef uint8_t palettefirstORsecond;
#define FIRSTPALETTE (palettefirstORsecond) 0
#define SECONDPALETTE (palettefirstORsecond) 1

typedef uint8_t paletteselectmask;
#define MASKOFF_PALETTESELECT_CLEAR (paletteselectmask)0b10001111
#define MASKON_PALETTESELECT_ULAFIRST (paletteselectmask) RPC_SELECT_ULA_PALETTE_0  // 0b00000000
#define MASKON_PALETTESELECT_ULASECOND (paletteselectmask) RPC_SELECT_ULA_PALETTE_1 // 0b01000000
#define MASKON_PALETTESELECT_LAYER2FIRST (paletteselectmask) RPC_SELECT_LAYER_2_PALETTE_0 // 0b00010000
#define MASKON_PALETTESELECT_LAYER2SECOND (paletteselectmask) RPC_SELECT_LAYER_2_PALETTE_1 // 0b01010000
#define MASKON_PALETTESELECT_SPRITEFIRST (paletteselectmask) RPC_SELECT_SPRITES_PALETTE_0 // 0b00100000
#define MASKON_PALETTESELECT_SPRITESECOND (paletteselectmask) RPC_SELECT_SPRITES_PALETTE_1 // 0b01100000
#define MASKON_PALETTESELECT_TILEFIRST (paletteselectmask)0b00110000
#define MASKON_PALETTESELECT_TILESECOND (paletteselectmask)0b01110000

#define NEXTREG_EnhancedULAPaletteExtension 0x44 // 9-bit colour extension, requires 2 write (byte 1 & 2)
/* Enhanced ULA Palette Extension 0x44
    Bit Effect 7-0 Reads or writes 9-bit colour definition

    Byte 1: RRRG GGBB
    Byte 2: P000 000B

    Bit 7 of the second write must be 0 except for Layer 2 palettes where it specifies colour priority.
    If set to 1, then the colour will always be on top, above all other layers, regardless of priority set with Sprite and Layers System $15.

*/

#define NEXTREG_TransparencyColourFallback 0x4A // Also used in combination with other registers
/* Transparency Colour Fallback 0x4A
    7-0 8-bit colour to be used when all layers contain transparent pixel. Format is RRRGGGBB
*/

#define NEXTREG_TilemapControl 0x6b // Tilemap (or text-tile mode)


typedef uint8_t autoinc;
#define AUTOINC_ON  (autoinc) 1
#define AUTOINC_OFF (autoinc) 0
#define MASKOFF_PaletteIndexAutoInc !RPC_DISABLE_AUTOINC // 0b01111111
#define MASKON_PaletteIndexAutoInc RPC_DISABLE_AUTOINC // 0b10000000
void setPaletteIndexAutoInc(autoinc enable)
{ // If enabled the palette index is automatically incremented on each write to palette value register
    DEBUG_FUNCTIONCALL("\nsetPaletteIndexAutoInc(enable %u)", enable);
    if (enable == AUTOINC_ON)
    {
        maskOFF_NEXTREG(NEXTREG_EnhancedULAControl, MASKOFF_PaletteIndexAutoInc);
        return;
    }
    else
    {
        maskON_NEXTREG(NEXTREG_EnhancedULAControl, MASKON_PaletteIndexAutoInc);
        return;
    }
}

void selectPalette(paletteselectmask paletteSelectMask)
{
    DEBUG_FUNCTIONCALL("\nselectPalette(paletteSelectMask %s)", byteAsBinaryString(paletteSelectMask));

    maskOFF_NEXTREG(NEXTREG_EnhancedULAControl, MASKOFF_PALETTESELECT_CLEAR);
    maskON_NEXTREG(NEXTREG_EnhancedULAControl, paletteSelectMask);
    return;
}

void setPaletteValues(uint8_t *values, uint8_t size, uint8_t startIndex)
{
    DEBUG_FUNCTIONCALL("\nsetPaletteValues(*values, size %u, startIndex%u)", size, startIndex);

    setPaletteIndexAutoInc(AUTOINC_ON);
    set_NEXTREGvalue(NEXTREG_PaletteIndex, startIndex);
    setBytes_NEXTREG(NEXTREG_PaletteValue, values, size);
    return;
}

uint8_t getPaletteValue(uint8_t atIndex)
{
    DEBUG_FUNCTIONCALL("\n\ngetPaletteValue(atIndex %u)", atIndex);

    set_NEXTREGvalue(NEXTREG_PaletteIndex, atIndex);
    return get_NEXTREGvalue(NEXTREG_PaletteValue);
}

void setClassicULAPalette(struct paletteDef_ClassicULA *paletteDef, palettefirstORsecond firstORsecond)
{
    /* Enhanced ULA Control 0x43
        6-4 Selects palette for read or write
            000 ULA first palette
    */

    if (firstORsecond == FIRSTPALETTE)
    {
        selectPalette(MASKON_PALETTESELECT_ULAFIRST);
    }
    else
    {
        selectPalette(MASKON_PALETTESELECT_ULASECOND);
    }
    setPaletteValues(paletteDef->Inks, COLOURARRAYSIZE_ClassicULA, 0);
    setPaletteValues(paletteDef->BrightInks, COLOURARRAYSIZE_ClassicULA, 8);
    setPaletteValues(paletteDef->Papers, COLOURARRAYSIZE_ClassicULA, 16);
    setPaletteValues(paletteDef->BrightPapers, COLOURARRAYSIZE_ClassicULA, 24);
    return;
}

struct paletteDef_ClassicULA *getClassicULAPalette(uint8_t firstORsecond, struct paletteDef_ClassicULA *returnPaletteDef)
{
    if (firstORsecond == FIRSTPALETTE)
    {
        selectPalette(MASKON_PALETTESELECT_ULAFIRST);
    }
    else
    {
        selectPalette(MASKON_PALETTESELECT_ULASECOND);
    }

    setPaletteIndexAutoInc(AUTOINC_OFF);
    for (uint8_t i = 0; i < 8; i++)
    {
        returnPaletteDef->Inks[i] = getPaletteValue(i);
        returnPaletteDef->BrightInks[i] = getPaletteValue(i + 8);
        returnPaletteDef->Papers[i] = getPaletteValue(i + 16);
        returnPaletteDef->BrightPapers[i] = getPaletteValue(i + 24);
    }

    return returnPaletteDef;
}

void setULAplusPalette(struct paletteDef_ULAplus *paletteDef, uint8_t firstORsecond)
{
    
}

#endif
