#include "include/global.h"

#include "include/test.h"
#include <z80.h>

// Dec these here Def them down the bottom.
void bankTesting(void);
void basicBankTesting(void);
void mouseTesting(void);
void espatTesting(void);
void espatTimeTest(void);

// int main(int argc, char const *argv[])
int main(void)
{
    ZXN_NEXTREG(REG_TURBO_MODE, 3);
    zx_border(PAPER_WHITE);
    zx_cls_attr(PAPER_WHITE);
    zx_cls_pix(INK_BLACK);

    TERMINAL_FUNC_PAUSEONSCROLL_DISABLE;
    printf("\n=START=\n");

    // basicBankTesting();

    // espatTesting();
    espatTimeTest();

    // bankTesting();

    // test();

    printf("\n=END=");
    while (1)
    {
        zx_border(INK_CYAN);
        z80_delay_ms(250);
        // zx_border(INK_RED);
        z80_delay_ms(250);
    }

    return 0;
}

static nethandle myNetHandle = 0;
static char connectionString[] = "TCP,172.16.1.112,1234";
void espatTesting(void)
{
    espat_DriverInstall();
    // setOutputBufferMode(myNetHandle, 1);
    // myNetHandle = espat_OpenConnection(connectionString);
    myNetHandle = espat_OpenTCPConnection("172.16.1.112", 1234);

    // getChannelValues();
    // netWrite(myNetHandle,connectionString,sizeof(connectionString));
    nos_StreamSize(DRIVERID_espat, myNetHandle);

    espat_testSend(myNetHandle);

    z80_delay_ms(250);
    // printf("\nnethandle?: 0x%02x, %c",espat_drvapi.call.function,espat_drvapi.call.function);

    return;
}

void espatTimeTest(void)
{
    espat_DriverInstall();

    // myNetHandle = espat_OpenTCPConnection("time.nxtel.org",12300);
    // myNetHandle = espat_OpenTCPConnection("172.16.1.112",1234);
    myNetHandle = espat_OpenTCPConnection("tcpbin.com", 4242);
    setIPDdetect(myNetHandle);
    z80_delay_ms(50);
    setOutputBufferMode(myNetHandle, 4); // Send on 4 bytes or 0 = '\n'  1 = per byte
    espat_SendChar(myNetHandle, 't');
    espat_SendChar(myNetHandle, 'e');
    espat_SendChar(myNetHandle, 's');
    espat_SendChar(myNetHandle, 't');

    // z80_delay_ms(10000);
    // espat_SendChar(myNetHandle,'\n');

    while (1)
    {
        z80_delay_ms(100);
        
        uint16_t bytes = nos_StreamSize(DRIVERID_espat, myNetHandle);
        if (bytes > 0)
        {
            //getChannelValues(myNetHandle);
            printf("\n RX >> ");
            for (uint8_t i = 0; i < bytes; i++)
            {
                putchar(espat_RecvChar(myNetHandle));
            }
        }
    }
    return;
}

//
//  TCP TIME.NXTEL.ORG 12300
//

/*
AT+CIPMUX=0
AT+CIPSTART="TCP","time.nxtel.org",12300
AT+CIPSEND=4 TEST

reply 22 dd/mm/yyyyhh:mm:ss
*/

void bankTesting(void)
{
    printf("\ntotal banks: %u ", nextos_totalBanks(RAM_BANK));
    printf("\navailable banks: %u", nextos_availableBanks(RAM_BANK));
    printf("\nallocateBank %u ", nextos_allocateBank(RAM_BANK));
    printf("\navailable banks: %u", nextos_availableBanks(RAM_BANK));

    manage8Banks_allocateAllBanks();

    // manage8Banks_allocateBank(); // Errors
    // manage8Banks_allocateBasicBank(); // Errors

    manage8Banks_showOnConsole();

    while (1)
    {
        ;
    }
}

void basicBankTesting(void)
{
    nextos_reserveBasicBank(110);
    nextos_allocateBasicBank();
    return;
}

void mouseTesting(void)
{
    mouseInstallDriver();
    mouseEnableAttributeCursor(1);

    uint16_t i = 0;
    while (1)
    {
        printf("\n%d", i++);
        z80_delay_ms(250);
        mouseStateToConsole(mouseGetState());
        z80_delay_ms(250);
    }
}