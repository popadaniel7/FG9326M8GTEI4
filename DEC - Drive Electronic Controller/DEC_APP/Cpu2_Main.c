#include "Ifx_Types.h"
#include "IfxCpu.h"
#include "IfxScuWdt.h"
#include "Os.h"
#include "Wdg.h"
#include "task_core2.h"

extern uint8 OsInit_C1;
uint8 OsInit_C2 = 0u;

void core2_main(void)
{
    IfxCpu_enableInterrupts();
    Wdg_InitializeCpu2Watchdog();
    Wdg_ReloadCpu2Watchdog();
    while(OsInit_C1 == 0u)
    {
        Wdg_ReloadCpu2Watchdog();
    }
    Os_Init_C2();
    OsInit_C2 = 1u;
    Wdg_ReloadCpu2Watchdog();
    /* Start the scheduler */
    vTaskStartScheduler_core2();
}
