#include "Bsw_Wrapper.h"
#include "SysMgr.h"
#include "Wdg.h"
#include "IfxCpu.h"

void Bsw_Wrapper_MainFunction_C0(void);
void Bsw_Wrapper_MainFunction_C1(void);
void Bsw_Wrapper_MainFunction_C2(void);

void Bsw_Wrapper_MainFunction_C0(void)
{
    Wdg_ReloadCpu0Watchdog();
    Wdg_ReloadSafetyWatchdog();
}
void Bsw_Wrapper_MainFunction_C1(void)
{
    Wdg_ReloadCpu1Watchdog();

    if(SYSMGR_SLEEP == SysMgr_EcuState)
    {
        Wdg_DeInitializeCpu1Watchdog();
        SysMgr_Core1OnHalt = 1u;
        IfxCpu_setCoreMode(&MODULE_CPU1, IfxCpu_CoreMode_idle);
    }
    else
    {
        /* Do nothing. */
    }
}
void Bsw_Wrapper_MainFunction_C2(void)
{
    Wdg_ReloadCpu2Watchdog();

    if(SYSMGR_SLEEP == SysMgr_EcuState)
    {
        Wdg_DeInitializeCpu1Watchdog();
        SysMgr_Core2OnHalt = 1u;
        IfxCpu_setCoreMode(&MODULE_CPU2, IfxCpu_CoreMode_idle);
    }
    else
    {
        /* Do nothing. */
    }
}
