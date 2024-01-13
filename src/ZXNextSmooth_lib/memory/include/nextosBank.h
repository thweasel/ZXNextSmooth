#ifndef nextos_bank_h
#define nextos_bank_h

#include "../../general/Z88dkDeps.h"

typedef uint8_t bankType; // VALID type values defined below
#define RAM_BANK 0
#define DIVMMC_BANK 1

// There are 2 bank ID schemes, 8K banks and 16K BASIC banks.
typedef uint8_t bankID;
typedef uint8_t basicBankID;

extern int16_t totalBanks(bankType type);
extern int16_t availableBanks(bankType type);
extern bankID allocateBank(bankType type);
extern uint8_t freeBank(bankType type, bankID page);
extern uint8_t reserveBank(bankType type, bankID page);

extern void memory_MMUtoConsole(void);
extern void memory_MMUbankSwap(uint8_t mmuREG, uint8_t BankNumber);

extern uint8_t reserveBasicBank(basicBankID id);
extern uint8_t allocateBasicBank(void);

extern void bankAllocationsToConsole(uint8_t *banksToShow, uint8_t allocatedBanks ,uint8_t maxBanks, bool isBASICbanks);
#endif