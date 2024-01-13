#ifndef palette_h
#define palette_h
#include "../../general/ZXNextSmooths_Z88dkDeps.h"

#include "../include/paletteStructures.h"


//
//  Palette methods
//

extern void classicULApaletteToConsole(struct paletteDef_ClassicULA *paletteDef);
extern void setClassicULAPalette(struct paletteDef_ClassicULA *paletteDef, FIRST_SECOND_PALETTE firstORsecond);
extern struct paletteDef_ClassicULA *getClassicULAPalette(uint8_t firstORsecond, struct paletteDef_ClassicULA *returnPaletteDef);

#endif