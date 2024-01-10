#define DEBUG_OFF
#include "../../general/include/debugging.h"
#include "../../general/Z88dkDeps.h"

#include "../../hardware/export_hardware.h"

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

void memory_MMUtoConsole(void)
{
    /*
     printf("\n MMU REG0 %03u, MMU REG1 %03u,",get_NEXTREGvalue(REG_MMU0),get_NEXTREGvalue(REG_MMU1));
     printf("\n MMU REG2 %03u, MMU REG3 %03u,",get_NEXTREGvalue(REG_MMU2),get_NEXTREGvalue(REG_MMU3));
     printf("\n MMU REG4 %03u, MMU REG5 %03u,",get_NEXTREGvalue(REG_MMU4),get_NEXTREGvalue(REG_MMU5));
     printf("\n MMU REG6 %03u, MMU REG7 %03u,",get_NEXTREGvalue(REG_MMU6),get_NEXTREGvalue(REG_MMU7));
    */
    
    printf("\n MMU 0[%03u],1[%03u],2[%03u],3[%03u],4[%03u],5[%03u],6[%03u],7[%03u]",
    ZXN_READ_MMU0(),
    ZXN_READ_MMU1(),
    ZXN_READ_MMU2(),
    ZXN_READ_MMU3(),
    ZXN_READ_MMU4(),
    ZXN_READ_MMU5(),
    ZXN_READ_MMU6(),
    ZXN_READ_MMU7()
    );
    return;
}

void memory_MMUbankSwap(uint8_t mmuREG, uint8_t BankNumber)
{
    DEBUG_FUNCTIONCALL("\nmemory_swapMMUbank(mmuREG %03u ,BankNumber %03u)",mmuREG, BankNumber);
    set_NEXTREGvalue(mmuREG, BankNumber);
    return;
}