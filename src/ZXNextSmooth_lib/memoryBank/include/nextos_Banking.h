#ifndef nextos_banking_h
#define nextos_banking_h
#include "../../general/ZXNextSmooths_Z88dkDeps.h"

// Next BASIC and original Spectrum memory map is managed in 16K blocks.
#define BASIC16KBankSlotAddr_1 0x0000 // ROM 16K - MMU 0 & 1
#define BASIC16KBankSlotAddr_2 0x4000 // RAM 16K - MMU 2 & 3
#define BASIC16KBankSlotAddr_3 0x8000 // RAM 16K - MMU 4 & 5
#define BASIC16KBankSlotAddr_4 0xC000 // RAM 16K - MMU 6 & 7

// Native NEXT software can make use of the advanced memory management 8K blocks
#define NATIVE8KBankSlotAddr_1 0x0000 // MMU0
#define NATIVE8KBankSlotAddr_2 0x2000 // MMU1
#define NATIVE8KBankSlotAddr_3 0x4000 // MMU2
#define NATIVE8KBankSlotAddr_4 0x6000 // MMU3
#define NATIVE8KBankSlotAddr_5 0x8000 // MMU4
#define NATIVE8KBankSlotAddr_6 0xA000 // MMU5
#define NATIVE8KBankSlotAddr_7 0xC000 // MMU6
#define NATIVE8KBankSlotAddr_8 0xD000 // MMU7

typedef uint8_t bankSlot;  // 1 - 8
typedef uint8_t basicBankSlot;  // 1 - 4

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

extern void nextos_bankAllocationsToConsole(uint8_t *banksToShow, uint8_t allocatedBanks, uint8_t maxBanks, bool isBASICbanks);

extern void nextos_currentNEXTREG_MMUtoConsole(void);

extern void nextos_saveBasicBankSlotMMU(basicBankSlot slot);
extern void nextos_restoreBasicBankSlotMMU(basicBankSlot slot);

extern void nextos_setBasicBankSlot(basicBankSlot slot, basicBankID basicbank);
extern void nextos_setBankSlot(bankSlot slot, bankID bank);

#endif