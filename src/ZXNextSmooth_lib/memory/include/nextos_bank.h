#ifndef nextos_bank_h
#define nextos_bank_h

#define RAM_BANK 0
#define DIVMMC_BANK 1

typedef uint8_t bankType;
typedef uint8_t pageNumber;

extern int16_t totalBanks(bankType type);
extern int16_t availableBanks(bankType type);
extern uint16_t allocateBank(bankType type);
extern uint8_t freeBank(bankType type, pageNumber page);
extern uint8_t reserveBank(bankType type, pageNumber page);

#endif