#ifndef paletteStructures_h
#define paletteStructures_h
#include "../../general/ZXNextSmooths_Z88dkDeps.h"

//
//  Palette structures and sizes
//


typedef uint8_t FIRST_SECOND_PALETTE;
#define FIRSTPALETTE 0
#define SECONDPALETTE 1


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

#define COLOURARRAYSIZE_ULAplus 64
struct paletteDef_ULAplus
{
    uint8_t Inks[COLOURARRAYSIZE_ULAplus];
    uint8_t Papers[COLOURARRAYSIZE_ULAplus]; // Placed in Transparency Colour Fallback 0x4A
};

#endif