#define DEBUG_ON
#include "../../general/include/debugging.h"
#include "../../general/Z88dkDeps.h"

#include "../include/nextos_bank.h"




#define maxBanks (uint8_t) 8
static uint8_t managedBanksAllocated = 0;
static uint8_t managedBanks[maxBanks] = {0,0,0,0,0,0,0,0};

void bankAllocationsToConsole (void)
{

    printf("\nBanks Allocated: %u \n",managedBanksAllocated);
    for (uint8_t i = 0; i < maxBanks; i++)
    {
        printf("[%u]%u",i,managedBanks[i]);
        if(i < (maxBanks-1))
        {
            putchar(',');
        }
    }
    return;
}

uint8_t allocateManagedBank(void)
{
    if (managedBanksAllocated < maxBanks )
    {
        managedBanks[managedBanksAllocated] = allocateBank(RAM_BANK);
        managedBanksAllocated++;
    }
    else
    {
        printf("\nArray full no Bank allocated...");
        return 0;
    }
    bankAllocationsToConsole();
    return managedBanks[(managedBanksAllocated-1)];
}

void allocateAllManagedBanks()
{
    for (uint8_t i = 0; i < maxBanks; i++)
    {
        allocateManagedBank();
    }
    return;
}

uint8_t getPageNumber (uint8_t managedBankIndex)
{
    return managedBanks[managedBankIndex];
}