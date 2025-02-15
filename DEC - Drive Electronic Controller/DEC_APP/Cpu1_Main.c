#include "Ifx_Types.h"
#include "IfxCpu.h"
#include "IfxScuWdt.h"
#include "Os.h"
#include "Wdg.h"
#include "task_core1.h"

extern uint8 OsInit_C0;
uint8 OsInit_C1 = 0u;

void core1_main(void)
{
    IfxCpu_enableInterrupts();
    /* Wait for CPU sync event */
    Wdg_InitializeCpu1Watchdog();
    Wdg_ReloadCpu1Watchdog();
    while(OsInit_C0 == 0u)
    {
        Wdg_ReloadCpu1Watchdog();
    }
    Os_Init_C1();
    OsInit_C1 = 1u;
    Wdg_ReloadCpu1Watchdog();
    Wdg_ReloadCpu1Watchdog();
    /* Start the scheduler */
    vTaskStartScheduler_core1();
}
