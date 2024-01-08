#define DEBUG_OFF
#include "../../general/include/debugging.h"
#include "../../general/Z88dkDeps.h"

#include "../include/nextos_bank.h"

// Call IDE_BANK ($01bd) -- See NEXT OS API Docs

/*   IDE_BANK (From esxdos.h)

You must ensure allocated pages are returned before program exit

#define ESX_BANKTYPE_RAM     __nextos_rc_banktype_zx
#define ESX_BANKTYPE_DIVMMC  __nextos_rc_banktype_mmc

extern unsigned char __LIB__ esx_ide_bank_total(unsigned char banktype) __smallc __z88dk_fastcall;


extern unsigned char __LIB__ esx_ide_bank_avail(unsigned char banktype) __smallc __z88dk_fastcall;



extern unsigned char __LIB__ esx_ide_bank_alloc(unsigned char banktype) __smallc __z88dk_fastcall;


extern unsigned char __LIB__ esx_ide_bank_reserve(unsigned char banktype,unsigned char page) __smallc;
extern unsigned char __LIB__ esx_ide_bank_reserve_callee(unsigned char banktype,unsigned char page) __smallc __z88dk_callee;
#define esx_ide_bank_reserve(a,b) esx_ide_bank_reserve_callee(a,b)



extern unsigned char __LIB__ esx_ide_bank_free(unsigned char banktype,unsigned char page) __smallc;
extern unsigned char __LIB__ esx_ide_bank_free_callee(unsigned char banktype,unsigned char page) __smallc __z88dk_callee;
#define esx_ide_bank_free(a,b) esx_ide_bank_free_callee(a,b)

*/



int16_t totalBanks(bankType type)
{ // return total number of banks in system
    return esx_ide_bank_total(type);
}

int16_t availableBanks(bankType type)
{ // return number of available 8k pages
    return esx_ide_bank_avail(type);
}

uint16_t allocateBank(bankType type)
{ // allocate a single page from ram or divmmc memory
    return esx_ide_bank_alloc(type);
}

/*
uint8_t freeBank(bankType type, pageNumber page)
{ // deallocate page (-1 error)
    return esx_ide_bank_free(bankType type, pageNumber page);
}

uint8_t reserveBank(bankType type, pageNumber page)
{ // allocate a specific page
    return esx_ide_bank_reserve(bankType type, pageNumber page);
}
*/