#include "Scr_Io_sdcc.h"
#include "Scr_Scu_sdcc.h"
#include "Scr_Rtc_sdcc.h"
#include "Scr_Irq_sdcc.h"

__at(0x1600) uint8 Scr_WakeupReason;
__at(0x1601) uint8 Scr_Counter;
__at(0x1602) uint8 Scr_IsrCounter1;
__at(0x1603) uint8 Scr_IsrCounter2;

void Scr_WCAN_Interrupt(void) __interrupt (ECAN);
void Scr_NMIECC_Interrupt(void) __interrupt (NMIRAMECC);

void main()
{
    SCR_SCU_PAGE = 0x01u;                   // switch to page 1
    SCR_PMCON1 = 0x49u;                     // switch OCDS
    SCR_SCU_PAGE = 0x00u;                   // switch to page 0
    SCR_IO_PAGE = 0x02u;                    // switch to IO Page 2
    SCR_P01_PDISC = 0x00;                   // enable all P01 pins
    SCR_MODPISEL0 = 0x00;                   // select WCANRXDA
    SCR_IEN1 = 0x01u;                       // select EWCAN interrupt
    SCR_NMICON = 0x01u;                     // select NMI ECC interrupt
    SCR_IO_PAGE = 0x01u;                    // switch to IO Page 1
    SCR_P01_IOCR2 = 0x20u;                  // configure P33.10 / P01.2 as input pull-up
    SCR_WCAN_PAGE = 0x00u;                  // switch to WCAN Page 0
    SCR_WCAN_CFG = 0x09u;                   // set WCAN_EN to 1, SELWK_EN to 0, CCE to 1
    SCR_WCAN_CFG = 0x01u;                   // set WCAN_EN to 1, SELWK_EN to 0, CCE to 0
    SCR_WCAN_INTMRSLT = 0x0CU;              // set WUFMASK to 1, WUPMASK to 1
    SCR_WCAN_CFG = 0x07u;                   // set WCAN_EN to 1, SELWK_EN to 3, CCE to 0
    SCR_WCAN_PAGE = 0x01u;                  // switch to WCAN Page 1
    while(SCR_WCAN_INTESTAT0 != 0x01u);     // check if configuration is fine
    while(SCR_WCAN_INTESTAT1 != 0x04u);     // check if configuration is fine

    while(1)
    {
        Scr_Counter++;
    }
}

void Scr_WCAN_Interrupt(void) __interrupt (ECAN)
{
    Scr_IsrCounter1++;
    Scr_WakeupReason = 0x01u;
    SCR_SCU_PAGE = 0x01u;           // set SCU PAGE to 1
    SCR_SCU_STDBYWKP = 0x11u;       // request SCRWKP and ECCWKSEL
}

void Scr_NMIECC_Interrupt(void) __interrupt (NMIRAMECC)
{
    Scr_IsrCounter2++;
    Scr_WakeupReason = 0x02u;
    SCR_SCU_PAGE = 0x01u;           // set SCU PAGE to 1
    SCR_SCU_STDBYWKP = 0x05u;       // request SCRWKP and WCANWKSEL
}
