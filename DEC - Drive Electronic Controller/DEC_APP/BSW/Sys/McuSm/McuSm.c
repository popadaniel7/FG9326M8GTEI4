#include "McuSm.h"
#include "IfxScuLbist.h"
#include "Irq.h"
#include "IfxCpu_IntrinsicsTasking.h"
#include "IfxCpu_reg.h"
#include "Iven.h"

const McuSm_ResetReason_t McuSm_FullListCompare[] =
{
        NO_ERR,
        McuSm_IfxSmu_Alarm_CPU0_Lockstep_ComparatorError                             ,
        McuSm_IfxSmu_Alarm_CPU0_Buslevel_MpuViolation                                ,
        McuSm_IfxSmu_Alarm_CPU0_PFI0_PFLASH0_ReadPathError                           ,
        McuSm_IfxSmu_Alarm_CPU0_PCACHETAG_UncorrectableErrorDetection                ,
        McuSm_IfxSmu_Alarm_CPU0_PCACHETAG_MiscellaneousErrorDetection                ,
        McuSm_IfxSmu_Alarm_CPU0_PSPRPCACHE_SingleBitErrorCorrection                  ,
        McuSm_IfxSmu_Alarm_CPU0_PSPRPCACHE_UncorrectableErrorDetection               ,
        McuSm_IfxSmu_Alarm_CPU0_PSPRPCACHE_MiscellaneousErrorDetection               ,
        McuSm_IfxSmu_Alarm_CPU0_DSPRDCACHEDLMU_SingleBitErrorCorrection              ,
        McuSm_IfxSmu_Alarm_CPU0_DSPRDCACHEDLMU_UncorrectableErrorDetection           ,
        McuSm_IfxSmu_Alarm_CPU0_DSPRDCACHEDLMU_MiscelleniousErrorDetection           ,
        McuSm_IfxSmu_Alarm_CPU0_DCACHETAG_SingleBitErrorCorrection                   ,
        McuSm_IfxSmu_Alarm_CPU0_DCACHETAG_UncorrectableErrorDetection                ,
        McuSm_IfxSmu_Alarm_CPU0_DCACHETAG_MiscellaneousErrorDetection                ,
        McuSm_IfxSmu_Alarm_CPU0_CPU_InstructionFetchSriInterfaceEdcError             ,
        McuSm_IfxSmu_Alarm_CPU0_CPU_DataSriInterfaceLoadStoreEdcError                ,
        McuSm_IfxSmu_Alarm_CPU0_CPU_ExceptionInterruptTrap                           ,
        McuSm_IfxSmu_Alarm_CPU1_Lockstep_ComparatorError                             ,
        McuSm_IfxSmu_Alarm_CPU1_Buslevel_MpuViolation                                ,
        McuSm_IfxSmu_Alarm_CPU1_PFI1_PFLASH1_ReadPathError                           ,
        McuSm_IfxSmu_Alarm_CPU1_PCACHETAG_UncorrectableErrorDetection                ,
        McuSm_IfxSmu_Alarm_CPU1_PCACHETAG_MiscellaneousErrorDetection                ,
        McuSm_IfxSmu_Alarm_CPU1_PSPRPCACHE_SingleBitErrorCorrection                  ,
        McuSm_IfxSmu_Alarm_CPU1_PSPRPCACHE_UncorrectableErrorDetection               ,
        McuSm_IfxSmu_Alarm_CPU1_PSPRPCACHE_MiscellaneousErrorDetection               ,
        McuSm_IfxSmu_Alarm_CPU1_DSPRDCACHEDLMU_SingleBitErrorCorrection              ,
        McuSm_IfxSmu_Alarm_CPU1_DSPRDCACHEDLMU_UncorrectableErrorDetection           ,
        McuSm_IfxSmu_Alarm_CPU1_DSPRDCACHEDLMU_MiscellaneousErrorDetection           ,
        McuSm_IfxSmu_Alarm_CPU1_DCACHETAG_SingleBitErrorCorrection                   ,
        McuSm_IfxSmu_Alarm_CPU1_DCACHETAG_UncorrectableErrorDetection                ,
        McuSm_IfxSmu_Alarm_CPU1_DCACHETAG_MiscellaneousErrorDetection                ,
        McuSm_IfxSmu_Alarm_CPU1_CPU_InstructionFetchSriInterfaceEdcError             ,
        McuSm_IfxSmu_Alarm_CPU1_CPU_DataSriInterfaceLoadStoreEdcError                ,
        McuSm_IfxSmu_Alarm_CPU1_CPU_ExceptionInterruptTrap                           ,
        McuSm_IfxSmu_Alarm_CPU2_Lockstep_ComparatorError                             ,
        McuSm_IfxSmu_Alarm_CPU2_Buslevel_MpuViolation                                ,
        McuSm_IfxSmu_Alarm_CPU2_PFI2_PFLASH2_ReadPathError                           ,
        McuSm_IfxSmu_Alarm_CPU2_PCACHETAG_UncorrectableErrorDetection                ,
        McuSm_IfxSmu_Alarm_CPU2_PCACHETAG_MiscellaneousErrorDetection                ,
        McuSm_IfxSmu_Alarm_CPU2_PSPRPCACHE_SingleBitErrorCorrection                  ,
        McuSm_IfxSmu_Alarm_CPU2_PSPRPCACHE_UncorrectableErrorDetection               ,
        McuSm_IfxSmu_Alarm_CPU2_PSPRPCACHE_MiscellaneousErrorDetection               ,
        McuSm_IfxSmu_Alarm_CPU2_DSPRDCACHEDLMU_SingleBitErrorCorrection              ,
        McuSm_IfxSmu_Alarm_CPU2_DSPRDCACHEDLMU_UncorrectableErrorDetection           ,
        McuSm_IfxSmu_Alarm_CPU2_DSPRDCACHEDLMU_MiscellaneousErrorDetection           ,
        McuSm_IfxSmu_Alarm_CPU2_DCACHETAG_SingleBitErrorCorrection                   ,
        McuSm_IfxSmu_Alarm_CPU2_DCACHETAG_UncorrectableErrorDetection                ,
        McuSm_IfxSmu_Alarm_CPU2_DCACHETAG_MiscellaneousErrorDetection                ,
        McuSm_IfxSmu_Alarm_CPU2_CPU_InstructionFetchSriInterfaceEdcError             ,
        McuSm_IfxSmu_Alarm_CPU2_CPU_DataSriInterfaceLoadStoreEdcError                ,
        McuSm_IfxSmu_Alarm_CPU2_CPU_ExceptionInterruptTrap                           ,
        McuSm_IfxSmu_Alarm_CPU3_Lockstep_ComparatorError                             ,
        McuSm_IfxSmu_Alarm_CPU3_Buslevel_MpuViolation                                ,
        McuSm_IfxSmu_Alarm_CPU3_PFI3_PFLASH3_ReadPathError                           ,
        McuSm_IfxSmu_Alarm_CPU3_PCACHETAG_UncorrectableErrorDetection                ,
        McuSm_IfxSmu_Alarm_CPU3_PCACHETAG_MiscellaneousErrorDetection                ,
        McuSm_IfxSmu_Alarm_CPU3_PSPRPCACHE_SingleBitErrorCorrection                  ,
        McuSm_IfxSmu_Alarm_CPU3_PSPRPCACHE_UncorrectableErrorDetection               ,
        McuSm_IfxSmu_Alarm_CPU3_PSPRPCACHE_MiscellaneousErrorDetection               ,
        McuSm_IfxSmu_Alarm_CPU3_DSPRDCACHEDLMU_SingleBitErrorCorrection              ,
        McuSm_IfxSmu_Alarm_CPU3_DSPRDCACHEDLMU_UncorrectableErrorDetection           ,
        McuSm_IfxSmu_Alarm_CPU3_DSPRDCACHEDLMU_MiscellaneousErrorDetection           ,
        McuSm_IfxSmu_Alarm_CPU3_DCACHETAG_SingleBitErrorCorrection                   ,
        McuSm_IfxSmu_Alarm_CPU3_DCACHETAG_UncorrectableErrorDetection                ,
        McuSm_IfxSmu_Alarm_CPU3_DCACHETAG_MiscellaneousErrorDetection                ,
        McuSm_IfxSmu_Alarm_CPU3_CPU_InstructionFetchSriInterfaceEdcError             ,
        McuSm_IfxSmu_Alarm_CPU3_CPU_DataSriInterfaceLoadStoreEdcError                ,
        McuSm_IfxSmu_Alarm_CPU3_CPU_ExceptionInterruptTrap                           ,
        McuSm_IfxSmu_Alarm_MTU_Safety_FfUncorrectableErrorDetected                   ,
        McuSm_IfxSmu_Alarm_IOM_Safety_FfUncorrectableErrorDetected                   ,
        McuSm_IfxSmu_Alarm_IR_Safety_FfUncorrectableErrorDetected                    ,
        McuSm_IfxSmu_Alarm_EMEM_Safety_FfUncorrectableErrorDetected                  ,
        McuSm_IfxSmu_Alarm_SCU_Safety_FfUncorrectableErrorDetected                   ,
        McuSm_IfxSmu_Alarm_PMS_Safety_FfUncorrectableErrorDetected                   ,
        McuSm_IfxSmu_Alarm_DMA_Safety_FfUncorrectableErrorDetected                   ,
        McuSm_IfxSmu_Alarm_SMU_core_Safety_FfUncorrectableErrorDetected              ,
        McuSm_IfxSmu_Alarm_SYS_PLL_PER_PLL_Safety_FfUncorrectableErrorDetected       ,
        McuSm_IfxSmu_Alarm_GTM_GTM_SingleBitErrorCorrection                          ,
        McuSm_IfxSmu_Alarm_GTM_GTM_UncorrectableErrorDetection                       ,
        McuSm_IfxSmu_Alarm_GTM_GTM_MiscellaneousErrorDetection                       ,
        McuSm_IfxSmu_Alarm_ERAY_ERAY_SingleBitErrorCorrection                        ,
        McuSm_IfxSmu_Alarm_ERAY_ERAY_UncorrectableErrorDetection                     ,
        McuSm_IfxSmu_Alarm_ERAY_ERAY_MiscellaneousErrorDetection                     ,
        McuSm_IfxSmu_Alarm_CAN_CAN_SingleBitErrorCorrection                          ,
        McuSm_IfxSmu_Alarm_CAN_CAN_UncorrectableErrorDetection                       ,
        McuSm_IfxSmu_Alarm_CAN_CAN_MiscellaneousErrorDetection                       ,
        McuSm_IfxSmu_Alarm_Misc_MISC_SingleBitErrorCorrection                        ,
        McuSm_IfxSmu_Alarm_Misc_MISC_UncorrectableErrorDetection                     ,
        McuSm_IfxSmu_Alarm_Misc_MISC_MiscellaneousErrorDetection                     ,
        McuSm_IfxSmu_Alarm_CERBERUS_Safety_FfUncorrectableErrorDetected              ,
        McuSm_IfxSmu_Alarm_CCU_Safety_FfCorrectableErrorDetected                     ,
        McuSm_IfxSmu_Alarm_CCU_Safety_FfUncorrectableErrorDetected                   ,
        McuSm_IfxSmu_Alarm_LMU_FSI_RAM_LMUDMU_SingleBitErrorCorrection               ,
        McuSm_IfxSmu_Alarm_LMU_FSI_RAM_LMUDMU_UncorrectableErrorDetection            ,
        McuSm_IfxSmu_Alarm_LMU_FSI_RAM_LMUDMU_MiscellaneousErrorDetection            ,
        McuSm_IfxSmu_Alarm_LMU_Lockstep_ComparatorError                              ,
        McuSm_IfxSmu_Alarm_LMU_Lockstep_ControlError                                 ,
        McuSm_IfxSmu_Alarm_LMU_ECC_Error                                             ,
        McuSm_IfxSmu_Alarm_LMU_Buslevel_MpuError                                     ,
        McuSm_IfxSmu_Alarm_LMU_EDC_ReadPhaseError                                    ,
        McuSm_IfxSmu_Alarm_XBAR0_SRI_BusErrorEvent                                   ,
        McuSm_IfxSmu_Alarm_XBAR1_SRI_BusErrorEvent                                   ,
        McuSm_IfxSmu_Alarm_XBAR2_SRI_BusErrorEvent                                   ,
        McuSm_IfxSmu_Alarm_SPB_BusErrorEvent                                         ,
        McuSm_IfxSmu_Alarm_BBB_BusErrorEvent                                         ,
        McuSm_IfxSmu_Alarm_FSI_PFlash_SingleBitError                                 ,
        McuSm_IfxSmu_Alarm_FSI_PFlash_DoubleBitError                                 ,
        McuSm_IfxSmu_Alarm_FSI_Single_BitCorrectionTrackingBufferFull                ,
        McuSm_IfxSmu_Alarm_FSI_Double_BitCorrectionTrackingBufferFull                ,
        McuSm_IfxSmu_Alarm_FSI_Multiple_BitErrorDetectionTrackingBufferFull          ,
        McuSm_IfxSmu_Alarm_FSI_Zero_BitErrorTrackingBufferFull                       ,
        McuSm_IfxSmu_Alarm_FSI_PFlash_EccError                                       ,
        McuSm_IfxSmu_Alarm_FSI_PFlash_EdcError                                       ,
        McuSm_IfxSmu_Alarm_FSI_CPUFLASHCON_ConfigurationError                        ,
        McuSm_IfxSmu_Alarm_FSI_Flash_StoredConfigurationError                        ,
        McuSm_IfxSmu_Alarm_SCU_OS_CClockFrequencyOutOfRange                          ,
        McuSm_IfxSmu_Alarm_CCU_Backup_ClockOutOfRangeAlarm                           ,
        McuSm_IfxSmu_Alarm_CCU_Backup_ClockAliveAlarm                                ,
        McuSm_IfxSmu_Alarm_SCU_System_PllDcoLossOfLockEvent                          ,
        McuSm_IfxSmu_Alarm_SCU_Peripheral_PllDcoLossOfLockEvent                      ,
        McuSm_IfxSmu_Alarm_SCU_LBIST_Alarm                                           ,
        McuSm_IfxSmu_Alarm_SCU_External_RequestUnitAlarm0                            ,
        McuSm_IfxSmu_Alarm_SCU_External_RequestUnitAlarm1                            ,
        McuSm_IfxSmu_Alarm_SCU_External_RequestUnitAlarm2                            ,
        McuSm_IfxSmu_Alarm_SCU_External_RequestUnitAlarm3                            ,
        McuSm_IfxSmu_Alarm_SCU_CPU0_WatchdogTimeOut                                  ,
        McuSm_IfxSmu_Alarm_SCU_CPU1_WatchdogTimeOut                                  ,
        McuSm_IfxSmu_Alarm_SCU_CPU2_WatchdogTimeOut                                  ,
        McuSm_IfxSmu_Alarm_SCU_CPU3_WatchdogTimeOut                                  ,
        McuSm_IfxSmu_Alarm_SCU_Safety_WatchdogTimeOut                                ,
        McuSm_IfxSmu_Alarm_SCU_Watchdog_TimeOut                                      ,
        McuSm_IfxSmu_Alarm_SCU_Lockstep_DualRailError                                ,
        McuSm_IfxSmu_Alarm_SCU_External_EmergencyStopSignalEvent                     ,
        McuSm_IfxSmu_Alarm_SCU_Pad_HeatingAlarm                                      ,
        McuSm_IfxSmu_Alarm_SCU_LBIST_TestModeAlarm                                   ,
        McuSm_IfxSmu_Alarm_IR_EDC_ConfigurationDataPathError                         ,
        McuSm_IfxSmu_Alarm_DMA_DMASRI_EccError                                       ,
        McuSm_IfxSmu_Alarm_IOM_Pin_MismatchIndication                                ,
        McuSm_IfxSmu_Alarm_SCU_External_RequestUnitAlarm4                            ,
        McuSm_IfxSmu_Alarm_SCU_External_RequestUnitAlarm5                            ,
        McuSm_IfxSmu_Alarm_SCU_External_RequestUnitAlarm6                            ,
        McuSm_IfxSmu_Alarm_SCU_External_RequestUnitAlarm7                            ,
        McuSm_IfxSmu_Alarm_DTS_core_Under_TemperatureAlarm                           ,
        McuSm_IfxSmu_Alarm_DTS_core_Over_TemperatureAlarm                            ,
        McuSm_IfxSmu_Alarm_DTS_Temperature_Overflow                                  ,
        McuSm_IfxSmu_Alarm_DTS_Temperature_Underflow                                 ,
        McuSm_IfxSmu_Alarm_EVR_Overvoltage_Alarm                                     ,
        McuSm_IfxSmu_Alarm_EVR_Undervoltage_Alarm                                    ,
        McuSm_IfxSmu_Alarm_EVR_Short_ToLowHightAlarm                                 ,
        McuSm_IfxSmu_Alarm_HSM_Overvoltage_Alarm                                     ,
        McuSm_IfxSmu_Alarm_HSM_Undervoltage_Alarm                                    ,
        McuSm_IfxSmu_Alarm_EMEM_Unexpected_WriteToEmemAlarm                          ,
        McuSm_IfxSmu_Alarm_EMEM_SEP_ControlLogicAlarm                                ,
        McuSm_IfxSmu_Alarm_EMEM_Lockstep_ControlLogicInputAlarm                      ,
        McuSm_IfxSmu_Alarm_Software_Alarm0                                           ,
        McuSm_IfxSmu_Alarm_Software_Alarm1                                           ,
        McuSm_IfxSmu_Alarm_Software_Alarm2                                           ,
        McuSm_IfxSmu_Alarm_Software_Alarm3                                           ,
        McuSm_IfxSmu_Alarm_Software_Alarm4                                           ,
        McuSm_IfxSmu_Alarm_Software_Alarm5                                           ,
        McuSm_IfxSmu_Alarm_Software_Alarm6                                           ,
        McuSm_IfxSmu_Alarm_Software_Alarm7                                           ,
        McuSm_IfxSmu_Alarm_Software_Alarm8                                           ,
        McuSm_IfxSmu_Alarm_Software_Alarm9                                           ,
        McuSm_IfxSmu_Alarm_Software_Alarm10                                          ,
        McuSm_IfxSmu_Alarm_Software_Alarm11                                          ,
        McuSm_IfxSmu_Alarm_Software_Alarm12                                          ,
        McuSm_IfxSmu_Alarm_Software_Alarm13                                          ,
        McuSm_IfxSmu_Alarm_Software_Alarm14                                          ,
        McuSm_IfxSmu_Alarm_Software_Alarm15                                          ,
        McuSm_IfxSmu_Alarm_SMU_Timer0_TimeOut                                        ,
        McuSm_IfxSmu_Alarm_SMU_Timer1_TimeOut                                        ,
        McuSm_IfxSmu_Alarm_SMU_Error_PinFaultStateActivation                         ,
        McuSm_IfxSmu_Alarm_SMU_Safety_FfCorrectableErrorDetected                     ,
        McuSm_IfxSmu_Alarm_SMU_Safety_FfUncorrectableErrorDetected                   ,
        McuSm_IfxSmu_Alarm_SMU_Access_EnableErrorDetected                            ,
        McuSm_IfxSmu_Alarm_LMU_EDC_AddressPhaseError                                 ,
        McuSm_IfxSmu_Alarm_LMU_EDC_WritePhaseError                                   ,
        McuSm_IfxSmu_Alarm_XBAR_EDC_AddressPhaseError                                ,
        McuSm_IfxSmu_Alarm_XBAR_EDC_WritePhaseError                                  ,
        McuSm_IfxSmu_Alarm_DMU_EDC_AddressPhaseError                                 ,
        McuSm_IfxSmu_Alarm_DMU_EDC_WritePhaseError                                   ,
        McuSm_IfxSmu_Alarm_SFI_BBB_EDC_AddressPhaseError                             ,
        McuSm_IfxSmu_Alarm_SFI_BBB_EDC_WritePhaseError                               ,
        McuSm_IfxSmu_Alarm_SRI_EDC_ReadPhaseError                                    ,
        McuSm_IfxSmu_Alarm_SFI_SPB_EDC_ReadPhaseError                                ,
        McuSm_IfxSmu_Alarm_HSSL_HSSL0EDC_ReadPhaseError                              ,
        McuSm_IfxSmu_Alarm_CONVERTER_Phase_SyncronizerError                          ,
        McuSm_IfxSmu_Alarm_XBAR_SOTA_SwapError                                       ,
        TRAP0 ,
        TRAP1 ,
        TRAP2 ,
        TRAP3 ,
        TRAP4 ,
        TRAP5 ,
        TRAP6 ,
        TRAP7 ,
        PMSEVER_INITFAIL ,
        PMSEVER_INITCHECKFAIL ,
        LBIST_FAIL ,
        PLL_INITFAIL ,
        MONBIST_FAIL ,
        STACKOVERFLOW ,
        MALLOCERROR ,
        MBIST_FAIL ,
        DIAG_RESET,
        0xFFFFU
};
IfxMtu_MbistSel localMcuArr[21] =
{
        IfxMtu_MbistSel_cpu0Dtag   ,
        IfxMtu_MbistSel_cpu1Dtag   ,
        IfxMtu_MbistSel_cpu1Dlmu   ,
        IfxMtu_MbistSel_cpu2Dtag   ,
        IfxMtu_MbistSel_cpu2Dlmu   ,
        IfxMtu_MbistSel_gtmFifo    ,
        IfxMtu_MbistSel_gtmMcs0Fast,
        IfxMtu_MbistSel_gtmMcs1Fast,
        IfxMtu_MbistSel_gtmDpll1a  ,
        IfxMtu_MbistSel_gtmDpll1b  ,
        IfxMtu_MbistSel_gtmDpll2   ,
        IfxMtu_MbistSel_mcan0      ,
        IfxMtu_MbistSel_mcan1      ,
        IfxMtu_MbistSel_psi5       ,
        IfxMtu_MbistSel_eray0Obf   ,
        IfxMtu_MbistSel_eray0IbfTbf,
        IfxMtu_MbistSel_eray0Mbf   ,
        IfxMtu_MbistSel_scrXram    ,
        IfxMtu_MbistSel_scrIram    ,
        IfxMtu_MbistSel_ethermacRx ,
        IfxMtu_MbistSel_ethermacTx
};

uint32 McuSm_AGs[12];
uint32 McuSm_LastResetReason;
uint32 McuSm_LastResetInformation;
uint32 McuSm_IndexResetHistory;
McuSm_ResetHistory_t McuSm_ResetHistory[20u];
McuSm_ResetReason_t McuSm_ResetReasonListCounter[400u];
Iven_IcmTable_t Iven_IcmLookupTable[IVEN_ICM_NUMBER_OF_MESSAGES] = {{0u,0u}};
uint32 DiagMaster_AliveTime;
uint8 DiagMaster_ActiveSessionState;

void McuSm_InitializeBusMpu(void);
void McuSm_PerformResetHook(uint32 resetReason, uint32 resetInformation);
void McuSm_TRAP2(IfxCpu_Trap trapInfo);
void McuSm_TRAP3(IfxCpu_Trap trapInfo);
void McuSm_TRAP4(IfxCpu_Trap trapInfo);
void McuSm_TRAP7(IfxCpu_Trap trapInfo);

void McuSm_PerformResetHook(uint32 resetReason, uint32 resetInformation)
{
    if(resetReason != 0u)
    {
        IfxCpu_disableInterrupts();
        McuSm_LastResetReason = resetReason;
        McuSm_LastResetInformation = resetInformation;
        McuSm_ResetReasonListCounter[resetReason]++;
        McuSm_ResetHistory[McuSm_IndexResetHistory].reason = resetReason;
        McuSm_ResetHistory[McuSm_IndexResetHistory].information = resetInformation;
        McuSm_IndexResetHistory++;

        if(McuSm_IndexResetHistory > 19)
        {
            McuSm_IndexResetHistory = 0;
        }
        else
        {
            /* Do nothing. */
        }

        IfxScuRcu_performReset(IfxScuRcu_ResetType_application, 0);
    }
    else
    {
        /* Do nothing. */
    }
}

void McuSm_TRAP1(IfxCpu_Trap trapInfo)
{
    McuSm_PerformResetHook(TRAP1, trapInfo.tId);
}

void McuSm_TRAP2(IfxCpu_Trap trapInfo)
{
    McuSm_PerformResetHook(TRAP2, trapInfo.tId);
}

void McuSm_TRAP4(IfxCpu_Trap trapInfo)
{
    McuSm_PerformResetHook(TRAP4, trapInfo.tId);
}

void McuSm_TRAP7(IfxCpu_Trap trapInfo)
{
    uint32 const volatile* ag;
    uint32 agRstRsn = 0u;
    uint32 agRstInfo = 0u;

    McuSm_AGs[0] = SMU_AGCF0_0.U & SMU_AGCF0_2.U & ~(SMU_AGCF0_1.U);
    McuSm_AGs[1] = SMU_AGCF1_0.U & SMU_AGCF1_2.U & ~(SMU_AGCF1_1.U);
    McuSm_AGs[2] = SMU_AGCF2_0.U & SMU_AGCF2_2.U & ~(SMU_AGCF2_1.U);
    McuSm_AGs[6] = SMU_AGCF6_0.U & SMU_AGCF6_2.U & ~(SMU_AGCF6_1.U);
    McuSm_AGs[7] = SMU_AGCF7_0.U & SMU_AGCF7_2.U & ~(SMU_AGCF7_1.U);
    McuSm_AGs[8] = SMU_AGCF8_0.U & SMU_AGCF8_2.U & ~(SMU_AGCF8_1.U);
    McuSm_AGs[9] = SMU_AGCF9_0.U & SMU_AGCF9_2.U & ~(SMU_AGCF9_1.U);
    McuSm_AGs[10] = SMU_AGCF10_0.U & SMU_AGCF10_2.U & ~(SMU_AGCF10_1.U);
    McuSm_AGs[11] = SMU_AGCF11_0.U & SMU_AGCF11_2.U & ~(SMU_AGCF11_1.U);

    ag = (uint32 const volatile*)(&SMU_AG0);

    for(sint8 i = 0; i < 12; i++)
    {
        if(0u != (ag[i] & McuSm_AGs[i]))
        {
            agRstRsn = i;
            agRstInfo = (sint8)(31u - (uint8)__clz(ag[i] & McuSm_AGs[i]));
            break;
        }
        else
        {
            /* Do nothing. */
        }
    }

    McuSm_PerformResetHook(agRstRsn, agRstInfo);
}

void McuSm_TRAP3(IfxCpu_Trap trapInfo)
{
    uint32 index;
    uint16 password;
    Ifx_SCU_WDTS *watchdog = &MODULE_SCU.WDTS;
    Ifx_CPU_CORE_ID reg;
    uint8 coreId;
    reg.U = __mfcr(CPU_CORE_ID);
    coreId =  (IfxCpu_ResourceCpu)reg.B.CORE_ID;
    Ifx_SCU_WDTCPU *cpuwdg = &MODULE_SCU.WDTCPU[coreId];

    McuSm_LastResetReason = TRAP3;
    McuSm_LastResetInformation = trapInfo.tId;
    McuSm_ResetReasonListCounter[TRAP3]++;
    McuSm_ResetHistory[McuSm_IndexResetHistory].reason = TRAP3;
    McuSm_ResetHistory[McuSm_IndexResetHistory].information = trapInfo.tId;
    McuSm_IndexResetHistory++;

    if(McuSm_IndexResetHistory > 19)
    {
        McuSm_IndexResetHistory = 0;
    }
    else
    {
        /* Do nothing. */
    }

    /* Read Password from Safety WDT CON0 register
     * !!! NOTE: !!! when read bottom six bit of password are inverted so we have
     * to toggle them before returning password */
    password  = watchdog->CON0.B.PW;
    password ^= 0x003F;

    if (SCU_WDTS_CON0.B.LCK)
    {
        /* see Table 1 (Password Access Bit Pattern Requirements) */
        SCU_WDTS_CON0.U = (1 << IFX_SCU_WDTS_CON0_ENDINIT_OFF) |
                (0 << IFX_SCU_WDTS_CON0_LCK_OFF) |
                (password << IFX_SCU_WDTS_CON0_PW_OFF) |
                (SCU_WDTS_CON0.B.REL << IFX_SCU_WDTS_CON0_REL_OFF);
    }
    else
    {
        /* Do nothing. */
    }

    /* Clear ENDINT and set LCK bit in Config_0 register */
    SCU_WDTS_CON0.U = (0 << IFX_SCU_WDTS_CON0_ENDINIT_OFF) |
            (1 << IFX_SCU_WDTS_CON0_LCK_OFF) |
            (password << IFX_SCU_WDTS_CON0_PW_OFF) |
            (SCU_WDTS_CON0.B.REL << IFX_SCU_WDTS_CON0_REL_OFF);
    /* read back ENDINIT and wait until it has been cleared */

    while (SCU_WDTS_CON0.B.ENDINIT == 1)
    {}

    MODULE_SCU.RSTCON.B.SW = 2; /* Application Reset */

    /* Read Password from CON0 register
     * !!! NOTE: !!! when read bottom six bit of password are inverted so we have
     * to toggle them before returning password */

    password  = cpuwdg->CON0.B.PW;
    password ^= 0x003F;

    if (cpuwdg->CON0.B.LCK)
    {
        /* see Table 1 (Password Access Bit Pattern Requirements) */
        cpuwdg->CON0.U = (1 << IFX_SCU_WDTCPU_CON0_ENDINIT_OFF) |
                (0 << IFX_SCU_WDTCPU_CON0_LCK_OFF) |
                (password << IFX_SCU_WDTCPU_CON0_PW_OFF) |
                (cpuwdg->CON0.B.REL << IFX_SCU_WDTCPU_CON0_REL_OFF);
    }
    else
    {
        /* Do nothing. */
    }

    /* Clear ENDINT and set LCK bit in Config_0 register */
    cpuwdg->CON0.U = (0 << IFX_SCU_WDTCPU_CON0_ENDINIT_OFF) |
            (1 << IFX_SCU_WDTCPU_CON0_LCK_OFF) |
            (password << IFX_SCU_WDTCPU_CON0_PW_OFF) |
            (cpuwdg->CON0.B.REL << IFX_SCU_WDTCPU_CON0_REL_OFF);
    /* read back ENDINIT and wait until it has been cleared */
    while (cpuwdg->CON0.B.ENDINIT == 1)
    {}

    if (cpuwdg->CON0.B.LCK)
    {
        /* see Table 1 (Password Access Bit Pattern Requirements) */
        cpuwdg->CON0.U = (1 << IFX_SCU_WDTCPU_CON0_ENDINIT_OFF) |
                           (0 << IFX_SCU_WDTCPU_CON0_LCK_OFF) |
                           (password << IFX_SCU_WDTCPU_CON0_PW_OFF) |
                           (cpuwdg->CON0.B.REL << IFX_SCU_WDTCPU_CON0_REL_OFF);
    }

    /* Clear ENDINT and set LCK bit in Config_0 register */
    cpuwdg->CON0.U = (0 << IFX_SCU_WDTCPU_CON0_ENDINIT_OFF) |
                       (1 << IFX_SCU_WDTCPU_CON0_LCK_OFF) |
                       (password << IFX_SCU_WDTCPU_CON0_PW_OFF) |
                       (cpuwdg->CON0.B.REL << IFX_SCU_WDTCPU_CON0_REL_OFF);

    /* read back ENDINIT and wait until it has been cleared */
    while (cpuwdg->CON0.B.ENDINIT == 1)
    {}

    /* Write the user DATA to reset evaluation */
    MODULE_SCU.RSTCON2.B.USRINFO = 34;
    /* software Reset can be performed by writing to Reset Request register  SWRSTCON */
    MODULE_SCU.SWRSTCON.B.SWRSTREQ = 1U;
    /* Add some delay for HW to reset */
    for (index = 0U; index < (uint32)90000U; index++)
    {}
}

void McuSm_InitializeBusMpu(void)
{
    IfxScuWdt_clearSafetyEndinit(IfxScuWdt_getSafetyWatchdogPassword());
    CPU0_SPR_SPROT_RGNLA0.U = 0x50000000U;
    CPU0_SPR_SPROT_RGNUA0.U = 0x50017FFFU;
    CPU0_SPR_SPROT_RGNACCENA0_W.U = 0x10001777U;
    CPU0_SPR_SPROT_RGNACCENB0_W.U = 0x00000000U;
    CPU0_SPR_SPROT_RGNACCENA0_R.U = 0xFFFFFFFFU;
    CPU0_SPR_SPROT_RGNACCENB0_R.U = 0xFFFFFFFFU;
    CPU1_SPR_SPROT_RGNLA0.U = 0x60000000U;
    CPU1_SPR_SPROT_RGNUA0.U = 0x6003BFFFU;
    CPU1_SPR_SPROT_RGNACCENA0_W.U = 0x10001777U;
    CPU1_SPR_SPROT_RGNACCENB0_W.U = 0x00000000U;
    CPU1_SPR_SPROT_RGNACCENA0_R.U = 0xFFFFFFFFU;
    CPU1_SPR_SPROT_RGNACCENB0_R.U = 0xFFFFFFFFU;
    CPU2_SPR_SPROT_RGNLA0.U = 0x70000000U;
    CPU2_SPR_SPROT_RGNUA0.U = 0x7003BFFFU;
    CPU2_SPR_SPROT_RGNACCENA0_W.U = 0x10001777U;
    CPU2_SPR_SPROT_RGNACCENB0_W.U = 0x00000000U;
    CPU2_SPR_SPROT_RGNACCENA0_R.U = 0xFFFFFFFFU;
    CPU2_SPR_SPROT_RGNACCENB0_R.U = 0xFFFFFFFFU;
    CPU0_DLMU_SPROT_RGNLA0.U = 0xB0000000U;
    CPU0_DLMU_SPROT_RGNLA0.U = 0xB000FFFFU;
    CPU0_DLMU_SPROT_RGNACCENA0_W.U = 0x10001777U;
    CPU0_DLMU_SPROT_RGNACCENB0_W.U = 0x00000000U;
    CPU0_DLMU_SPROT_RGNACCENA0_R.U = 0xFFFFFFFFU;
    CPU0_DLMU_SPROT_RGNACCENB0_R.U = 0xFFFFFFFFU;
    CPU0_LPB_SPROT_ACCENA_R.U = 0xFFFFFFFFU;
    CPU1_LPB_SPROT_ACCENA_R.U = 0xFFFFFFFFU;
    CPU2_LPB_SPROT_ACCENA_R.U = 0xFFFFFFFFU;
    CPU0_LPB_SPROT_ACCENB_R.U = 0xFFFFFFFFU;
    CPU1_LPB_SPROT_ACCENB_R.U = 0xFFFFFFFFU;
    CPU2_LPB_SPROT_ACCENB_R.U = 0xFFFFFFFFU;
    IfxScuWdt_setSafetyEndinit(IfxScuWdt_getSafetyWatchdogPassword());
}
