#ifndef manage8Banks_h
#define manage8Banks_h
#include "../../general/Z88dkDeps.h"

basicBankID manage8Banks_allocateBasicBank (void);
bankID manage8Banks_allocateBank(void);
void manage8Banks_allocateAllBanks(void);

basicBankID manage8Banks_getBasicBankID(uint8_t BASIC_BanksIndex);
bankID manage8Banks_getBankID(uint8_t BankIndex);

void manage8Banks_showOnConsole(void);

#endif