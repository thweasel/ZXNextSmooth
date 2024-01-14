#ifndef nextos_bank_h
#define nextos_bank_h
#include "../../general/ZXNextSmooths_Z88dkDeps.h"

// There are 2 bank ID schemes, 8K banks and 16K BASIC banks.
typedef uint8_t bankID;
typedef uint8_t basicBankID;

typedef uint8_t bankType; // VALID type values defined below
#define RAM_BANK 0
#define DIVMMC_BANK 1


extern int16_t nextos_totalBanks(bankType type);
extern int16_t nextos_availableBanks(bankType type);

extern uint8_t nextos_reserveBank(bankType type, bankID page);
extern bankID nextos_allocateBank(bankType type);

extern uint8_t nextos_reserveBasicBank(basicBankID id);
extern uint8_t nextos_allocateBasicBank(void);

extern uint8_t nextos_freeBank(bankType type, bankID page);

extern void nextos_bankAllocationsToConsole(uint8_t *banksToShow, uint8_t allocatedBanks ,uint8_t maxBanks, bool isBASICbanks);



#endif