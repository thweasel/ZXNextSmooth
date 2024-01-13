#define DEBUG_ON
#include "../../general/include/debugging.h"
#include "../../general/ZXNextSmooths_Z88dkDeps.h"

#include "../include/nextosBank.h"

#define MAXBANKS (uint8_t)8

static uint8_t BanksAllocated = 0;
static uint8_t Banks[MAXBANKS] = {0, 0, 0, 0, 0, 0, 0, 0};

static uint8_t BASIC_BanksAllocated = 0;
static uint8_t BASIC_Banks[MAXBANKS] = {0, 0, 0, 0, 0, 0, 0, 0};

basicBankID manage8Banks_allocateBasicBank(void)
{ // Returns a BasicBankID between 111 and 0, or error @ 255
    DEBUG_FUNCTIONCALL("\n allocateManaged_BasicBank()");
    uint8_t result = 255; // Start in error state, if we successfully get a bank it will change
    if (BASIC_BanksAllocated < MAXBANKS)
    {
        result = nextos_allocateBasicBank();
        if (255 != result)
        {
            BASIC_Banks[BASIC_BanksAllocated] = result;
            BASIC_BanksAllocated++;
        }
    }
    // All errors return 255, which i not a valid bank
    DEBUG_AFTERFUNCTIONCALL("\n allocateManaged_BasicBank() << %03u", result);
    return result;
}

bankID manage8Banks_allocateBank(void)
{ // Returns a BankID between 222 and 0, or error @ 255
    DEBUG_FUNCTIONCALL("\n manage8Banks_allocateBank()");
    uint8_t result = 255; // Start in error state, if we successfully get a bank it will change
    if (BanksAllocated < MAXBANKS)
    {
        result = nextos_allocateBank(RAM_BANK);
        if (255 != result)
        {
            Banks[BanksAllocated] = result;
            BanksAllocated++;
        }
    }
    DEBUG_AFTERFUNCTIONCALL("\n manage8Banks_allocateBank() << %03u", result);
    return result;
}

void manage8Banks_allocateAllBanks(void)
{
    DEBUG_FUNCTIONCALL("\n manage8Banks_allocateAllBanks()");
    for (uint8_t i = 0; i < MAXBANKS; i++)
    {
        manage8Banks_allocateBasicBank();
    }
    for (uint8_t i = 0; i < MAXBANKS; i++)
    {
        manage8Banks_allocateBank();   
    }


    return;
}

basicBankID manage8Banks_getBasicBankID(uint8_t BASIC_BanksIndex)
{
    DEBUG_FUNCTIONCALL("\n manage8Banks_getBankID(BASIC_BanksIndex %03u)", BASIC_BanksIndex);
    return BASIC_Banks[BASIC_BanksIndex];
}

bankID manage8Banks_getBankID(uint8_t BankIndex)
{
    DEBUG_FUNCTIONCALL("\n manage8Banks_getBankID(BankIndex %03u)", BankIndex);
    return Banks[BankIndex];
}

void manage8Banks_showOnConsole(void)
{
    DEBUG_FUNCTIONCALL("\n managed8Banks_showOnConsole()");
    nextos_bankAllocationsToConsole(Banks, BanksAllocated, MAXBANKS, false);
    nextos_bankAllocationsToConsole(BASIC_Banks, BASIC_BanksAllocated, MAXBANKS, true);
    return;
}