#include "Nvm.h"
#include "Ifx_Types.h"
#include "IfxCpu.h"
#include "IfxScuWdt.h"
#include "Os.h"
#include "Ain_Filtering.h"
#include "Can.h"
#include "Dem.h"
#include "Dcm.h"
#include "McuSm.h"
#include "EncCal.h"
#include "SysMgr.h"
#include "Wdg.h"
#include "Smu.h"
#include "Crc.h"
#include "task_core0.h"
#include "SafetyKit_SSW.h"
#include "SafetyKit_Main.h"
#include "aurix_pin_mappings.h"
#include "SCR.h"
uint8 OsInit_C0 = 0u;

extern void Ssw_StartCores(void);

void core0_main(void)
{
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
    IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());
    IfxScuWdt_clearCpuEndinit(IfxScuWdt_getCpuWatchdogPassword());
    IfxScuWdt_clearSafetyEndinit(IfxScuWdt_getSafetyWatchdogPassword());
    IfxScr_init(0u);
    IfxScr_copyProgram();
    IfxScr_disableSCR();
    IfxMtu_clearSram(IfxMtu_MbistSel_scrXram);
    IfxMtu_clearSram(IfxMtu_MbistSel_scrIram);
    IfxScuWdt_setCpuEndinit(IfxScuWdt_getCpuWatchdogPassword());
    IfxScuWdt_setSafetyEndinit(IfxScuWdt_getSafetyWatchdogPassword());
    runSafeAppSwStartup();
    /* Start core 1 and core 2. */
    Ssw_StartCores();
    SysMgr_EcuState = SYSMGR_STARTUP;
    McuSm_InitializeBusMpu();
    Ain_FilteringInit();
    Crc_Init();
    gpio_init_pins();
    can0_node0_init_pins();
    Can_Init();
    Dem_PreInit();
    Nvm_ReadAll();
    Dem_Init();
    EncCal_MainFunction();
    SysMgr_ProcessResetDtc();
    Os_Init_C0();
    IfxCpu_enableInterrupts();
    OsInit_C0 = 1u;
    initCpuWatchdog(0u);
    initSafetyWatchdog();
    serviceCpuWatchdog();
    serviceSafetyWatchdog();
    initSafetyKit();
    vTaskStartScheduler_core0();
}
