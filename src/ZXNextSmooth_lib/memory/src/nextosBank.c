#define DEBUG_OFF
#include "../../general/include/debugging.h"
#include "../../general/Z88dkDeps.h"

#include "../../hardware/export_hardware.h"

#include "../include/nextosBank.h"

/*  NOTES
    The Next has between 1 and 2MBs of RAM which is natively indexed as 8K banks from 0 to 222.

    In this document the convention is:
        8KBank, Bank = native 8K bank ID
        16Bank, BASIC-Bank = BASIC 16K bank ID

    For BASIC compatability, there is a secondary 16K bank indexing scheme.
    BASIC 16K banks are allocated from an EVEN 8K bank ID and with the bank ID above making the upper 8K.
        e.g. 8K Bank 220 & 8K Bank 221 == 16K BASIC Bank 110

    If you call NEW BANK in BASIC, the first BANK allocated is 110 (8KBank ID 220)

    There is no OS API call to request a BASIC-Bank, if one needs to be allocated you need to find an even bank with one above available.
*/

/*  IDE_BANK (From esxdos.h)
// Call IDE_BANK ($01bd) -- See NEXT OS API Docs
You must ensure allocated pages are returned before program exit

#define ESX_BANKTYPE_RAM     __nextos_rc_banktype_zx
#define ESX_BANKTYPE_DIVMMC  __nextos_rc_banktype_mmc

extern unsigned char __LIB__ esx_ide_bank_total(unsigned char banktype) __smallc __z88dk_fastcall;
extern unsigned char __LIB__ esx_ide_bank_avail(unsigned char banktype) __smallc __z88dk_fastcall;
extern unsigned char __LIB__ esx_ide_bank_alloc(unsigned char banktype) __smallc __z88dk_fastcall;
extern unsigned char __LIB__ esx_ide_bank_reserve(unsigned char banktype,unsigned char page) __smallc;
extern unsigned char __LIB__ esx_ide_bank_free(unsigned char banktype,unsigned char page) __smallc;

*/

int16_t totalBanks(bankType type)
{ // return total number of (8K) banks in system
    return esx_ide_bank_total(type);
}

int16_t availableBanks(bankType type)
{ // return number of available (8K) banks
    return esx_ide_bank_avail(type);
}

bankID allocateBank(bankType type)
{ // allocate a single bankID from ram or divmmc memory
    return esx_ide_bank_alloc(type);
}

uint8_t freeBank(bankType type, bankID id)
{ // deallocate bankID (-1 error)
    return esx_ide_bank_free(type, id);
}

uint8_t reserveBank(bankType type, bankID id)
{ // allocate a specific bankID, 0 = success, 255 = failure
    return esx_ide_bank_reserve(type, id);
}

uint8_t reserveBasicBank(basicBankID id)
{ // allocate a specific BasicBankID, 0 = success, 255 = failure
    DEBUG_FUNCTIONCALL("\n reserveBasicBank(basicBankID %03u)", id);
    if (id < 111)
    {
        uint8_t lowerBank = id * 2; // There will never be an odd numbered ID in the lower bank
        uint8_t upperBank = 0;

        if (0 == (reserveBank(RAM_BANK, lowerBank)))
        {
            upperBank += lowerBank + 1;
            if (0 == (reserveBank(RAM_BANK, upperBank)))
            {
                DEBUG_MSG("\n BASICBank %03u, upperBank %03u , lowerBank %03u", id, upperBank, lowerBank);
                DEBUG_AFTERFUNCTIONCALL("\n reserveBasicBank(basicBankID %03u) << 000");
                return 0;
            }
            freeBank(RAM_BANK, lowerBank);
        }
    }
    DEBUG_AFTERFUNCTIONCALL("\n reserveBasicBank(basicBankID %03u) << 255", id);
    return 255;
}

basicBankID allocateBasicBank(void)
{ // We need to search for an EVEN 8K Bank ID with the ODD 8K Bank above available
    DEBUG_FUNCTIONCALL("\n allocateBasicBank()");
    uint8_t result = 255;
    for (uint8_t basicBankId = 110; basicBankId > 0; basicBankId--)
    {        
        result = reserveBasicBank(basicBankId);
        if (255 != result)
        {
            DEBUG_AFTERFUNCTIONCALL("\n allocateBasicBank() << %03u", basicBankId);
            return basicBankId;
        }
    }
    DEBUG_AFTERFUNCTIONCALL("\n allocateBasicBank() << %03u", result);
    return 255;
}

void memory_MMUtoConsole(void)
{
    printf("\n MMU 0[%03u],1[%03u],2[%03u],3[%03u],4[%03u],5[%03u],6[%03u],7[%03u]",
           ZXN_READ_MMU0(),
           ZXN_READ_MMU1(),
           ZXN_READ_MMU2(),
           ZXN_READ_MMU3(),
           ZXN_READ_MMU4(),
           ZXN_READ_MMU5(),
           ZXN_READ_MMU6(),
           ZXN_READ_MMU7());
    return;
}

void memory_MMUbankSwap(uint8_t mmuREG, bankID ID)
{
    DEBUG_FUNCTIONCALL("\nmemory_swapMMUbank(mmuREG %03u ,BankNumber %03u)", mmuREG, ID);
    set_NEXTREGvalue(mmuREG, ID);
    return;
}

void bankAllocationsToConsole(uint8_t *banksToShow, uint8_t allocatedBanks, uint8_t maxBanks, bool isBASICbanks)
{
    if (isBASICbanks)
    {
        printf("\n BASIC");
    }
    else
    {
        putchar('\n');
    }
    
    printf(" Banks Allocated: %u \n", allocatedBanks);

    for (uint8_t i = 0; i < maxBanks; i++)
    {        
        if (isBASICbanks)
        {
            printf("%u[%u<%u,%u>]",i, banksToShow[i],(banksToShow[i]*2+1) ,(banksToShow[i]*2) );
        }
        else
        {
            printf("%u[%u]", i, banksToShow[i]);
        }
        if (i < (maxBanks - 1))
        {
            putchar(',');
        }
    }
    return;
}