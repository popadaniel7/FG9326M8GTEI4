#include "Ifx_Types.h"
#include "IfxCpu.h"
#include "IfxScuWdt.h"
#include "Os.h"
#include "Ain_Filtering.h"
#include "Can.h"
#include "Nvm.h"
#include "Dem.h"
#include "Dcm.h"
#include "McuSm.h"
#include "EncCal.h"
#include "SysMgr.h"
#include "Wdg.h"
#include "Smu.h"
#include "Crc.h"
#include "task_core0.h"

uint8 OsInit_C0 = 0u;

extern void Ifx_Ssw_Lbist(void);
extern void Ifx_Ssw_Monbist(void);
extern void Ssw_StartCores(void);

void core0_main(void)
{
    unsigned int  tempValue = SCU_RSTSTAT.U;

    if ((tempValue & APPLICATIONRESET_MASK) > 0U)
    {
        tempValue = tempValue & APPLICATIONRESET_MASK;
        tempValue = (SCU_RSTCON.U >> ((31U - __clz(tempValue)) << 1U)) & 3U;
        if (tempValue != 2)
        {
            /* LBIST Tests and evaluation */
            Ifx_Ssw_Lbist();
        }
        else
        {
            /* Do nothing. */
        }
    }
    else
    {
        /* Do nothing. */
    }
    /* MONBIST Tests and evaluation */
    Ifx_Ssw_Monbist();
    /* MBIST Tests and evaluation */
#if DEBUG_CODE == 1
    while(MODULE_SCU.LBISTCTRL0.U == 0x10000400)
    {
        __debug();
    }
#endif
    McuSm_MbistManager();
    /* Start core 1 and core 2. */
    Ssw_StartCores();
    SysMgr_EcuState = SYSMGR_STARTUP;
    IfxCpu_enableInterrupts();
    IfxScuWdt_enableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
    IfxScuWdt_enableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());
    McuSm_InitializeBusMpu();
    McuSm_InitializeDts();
    Ain_FilteringInit();
    Smu_Init();
    Crc_Init();
    Can_Init();
    Nvm_ReadAll();
    Dem_Init();
    Dcm_Init();
    EncCal_MainFunction();
    SysMgr_WakeupInitScr();
    SysMgr_ProcessResetDtc();
    Os_Init_C0();
    OsInit_C0 = 1u;
    vTaskStartScheduler_core0();
}
