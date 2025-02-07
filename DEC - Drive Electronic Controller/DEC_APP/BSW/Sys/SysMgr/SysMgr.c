#include "SysMgr.h"
#include "McuSm.h"
#include "Nvm.h"
#include "IfxCpu.h"
#include "Ain_Filtering.h"
#include "Can.h"
#include "Dma.h"
#include "Eru.h"
#include "Gtm_Atom.h"
#include "Gtm_Pwm.h"
#include "Crc.h"
#include "task.h"
#include "FreeRTOSConfig.h"
#include "SCR.h"
#include "Wdg.h"

static uint32 SysMgr_MainCounter = 0u;
SysMgr_EcuState_t SysMgr_EcuState = SYSMGR_INIT;
uint8 SysMgr_NoBusActivity = 0u;
uint8 SysMgr_NoOvercurrentOnDcMotor = 0u;
uint8 SysMgr_NoOverheatOnDcMotor = 0u;
uint8 SysMgr_Core1OnHalt = 0u;
uint8 SysMgr_Core2OnHalt = 0u;

void SysMgr_ProcessResetDtc(void);
void SysMgr_EcuStateMachine(void);
void SysMgr_GoSleepSequence(void);
void SysMgr_WakeupInitScr(void);
void SysMgr_PreSleepInitScr(void);
void SysMgr_LoadScr(void);
void SysMgr_MainFunction(void);

void SysMgr_ProcessResetDtc(void)
{
    if(0u == SysMgr_MainCounter)
    {
        if(NO_ERR != McuSm_LastResetReason)
        {
            //Dem_SetDtc(MCUSM_DTC_ID_SW_ERROR, 1u);
        }
        else
        {
            //Dem_SetDtc(MCUSM_DTC_ID_SW_ERROR, 0u);
        }
    }
    else
    {
        /* Do nothing. */
    }
}

void SysMgr_EcuStateMachine(void)
{
    switch(SysMgr_EcuState)
    {
        case SYSMGR_INIT:
        case SYSMGR_STARTUP:
            if(0u == SysMgr_MainCounter)
            {
                SysMgr_EcuState = SYSMGR_RUN;
            }
            else
            {
                /* Do nothing. */
            }
            break;
        case SYSMGR_RUN:
            if(0u == SysMgr_NoBusActivity &&
                    0u == SysMgr_NoOvercurrentOnDcMotor &&
                    0u == SysMgr_NoOverheatOnDcMotor)
            {
                SysMgr_EcuState = SYSMGR_POSTRUN;
                Nvm_WriteAllFinished = 0u;
            }
            else
            {
                /* Do nothing. */
            }
            break;
        case SYSMGR_POSTRUN:
            Nvm_WriteAll();

            if(((0u != SysMgr_NoBusActivity) ||
                    (0u != SysMgr_NoOvercurrentOnDcMotor) ||
                    (0u != SysMgr_NoOverheatOnDcMotor)) &&
                    (2u == Nvm_WriteAllFinished))
            {
                SysMgr_EcuState = SYSMGR_SLEEP;
            }
            else
            {
                /* Do nothing. */
            }
            break;
        case SYSMGR_SLEEP:
            if(1u == SysMgr_Core1OnHalt &&
                    1u == SysMgr_Core2OnHalt)
            {
                IfxCpu_setAllIdleExceptMasterCpu(0u);
                IfxCpu_disableInterrupts();
                SysMgr_LoadScr();
                SysMgr_PreSleepInitScr();
                SysMgr_GoSleepSequence();
            }
            else
            {
                /* Do nothing.*/
            }
            break;
        default:
            break;
    }
}

void SysMgr_GoSleepSequence(void)
{
    Wdg_ReloadCpu0Watchdog();
    Wdg_DeInitializeCpu0Watchdog();
    Wdg_DeInitializeSafetyWatchdog();
    Can_Sleep();
    IfxEvadc_disableModule(g_evadc.evadc);
    IfxDma_Dma_deInitChannel(&dma,g_DMAchn.channelId);
    IfxScuEru_clearAllEventFlags();
    IfxGtm_Atom_Timer_disableUpdate(&g_timerDriver);
    IfxGtm_Atom_Timer_stop(&g_timerDriver);
    IfxGtm_Atom_Pwm_stop(&g_atomDriver, 1u);
    IfxGtm_Atom_Pwm_stop(&g_atomDriver2, 1u);
    IfxGtm_Atom_Pwm_stop(&g_atomDriver3, 1u);
    IfxGtm_Atom_Pwm_stop(&g_atomDriver4, 1u);
    IfxGtm_Atom_Pwm_stop(&g_atomDriver5, 1u);
    IfxFce_Crc_deInitModule(&g_fceCrc.fceCrc);
    vTaskSuspendAll();
    vTaskEndScheduler();
    SCU_CCUCON0.B.CLKSEL = 0;
    while(SCU_SYSPLLSTAT.B.LOCK == 0);
    SCU_CCUCON0.B.CLKSEL = 1;
    IfxCpu_setCoreMode(&MODULE_CPU0, IfxCpu_CoreMode_idle);
}

void SysMgr_WakeupInitScr(void)
{
    IfxScr_disableSCR();
    IfxScr_init(0u);
}

void SysMgr_PreSleepInitScr(void)
{
    IfxScuWdt_clearSafetyEndinit(IfxScuWdt_getSafetyWatchdogPassword());
    IfxScr_init(1u);
    IfxScuWdt_setSafetyEndinit(IfxScuWdt_getSafetyWatchdogPassword());

    if(SCU_RSTSTAT.B.STBYR)
    {
        IfxScuWdt_clearCpuEndinit(IfxScuWdt_getCpuWatchdogPassword());
        SCU_RSTCON2.B.CLRC = 1u;    /* A write access to this register is Endinit protected; the protection has to be removed and set again afterward. */
        IfxScuWdt_setCpuEndinit(IfxScuWdt_getCpuWatchdogPassword());
    }
    else
    {
        /* Do nothing. */
    }

    while(SCU_RSTSTAT.B.STBYR);/* Wait until Reset status is finally reset */
    /* Set P33.4 under control of SCR (SCR_P00.4) */
    IfxScuWdt_clearSafetyEndinit(IfxScuWdt_getSafetyWatchdogPassword());
    while(P33_PCSR.B.LCK);    /* Wait while any previous update of PCSR is done */
    P33_PCSR.B.SEL10 = 1u;
    IfxScuWdt_setSafetyEndinit(IfxScuWdt_getSafetyWatchdogPassword());
}

void SysMgr_LoadScr(void)
{
    IfxScr_copyProgram();
}

void SysMgr_MainFunction(void)
{
    SysMgr_EcuStateMachine();
    SysMgr_MainCounter++;
}
