#include "Smu.h"
#include "Irq.h"

void Smu_Init(void);

static const IfxSmu_Alarm g_smuAlarms_TC375[] =
{
        IfxSmu_Alarm_CPU0_Lockstep_ComparatorError                              ,
        IfxSmu_Alarm_CPU0_Buslevel_MpuViolation                                 ,
        IfxSmu_Alarm_CPU0_PFI0_PFLASH0_ReadPathError                            ,
        IfxSmu_Alarm_CPU0_PCACHETAG_UncorrectableErrorDetection                 ,
        IfxSmu_Alarm_CPU0_PCACHETAG_MiscellaneousErrorDetection                 ,
        IfxSmu_Alarm_CPU0_PSPRPCACHE_SingleBitErrorCorrection                   ,
        IfxSmu_Alarm_CPU0_PSPRPCACHE_UncorrectableErrorDetection                ,
        IfxSmu_Alarm_CPU0_PSPRPCACHE_MiscellaneousErrorDetection                ,
        IfxSmu_Alarm_CPU0_DSPRDCACHEDLMU_SingleBitErrorCorrection               ,
        IfxSmu_Alarm_CPU0_DSPRDCACHEDLMU_UncorrectableErrorDetection            ,
        IfxSmu_Alarm_CPU0_DSPRDCACHEDLMU_MiscelleniousErrorDetection            ,
        IfxSmu_Alarm_CPU0_DCACHETAG_SingleBitErrorCorrection                    ,
        IfxSmu_Alarm_CPU0_DCACHETAG_UncorrectableErrorDetection                 ,
        IfxSmu_Alarm_CPU0_DCACHETAG_MiscellaneousErrorDetection                 ,
        IfxSmu_Alarm_CPU0_CPU_InstructionFetchSriInterfaceEdcError              ,
        IfxSmu_Alarm_CPU0_CPU_DataSriInterfaceLoadStoreEdcError                 ,
        IfxSmu_Alarm_CPU0_CPU_ExceptionInterruptTrap                            ,
        IfxSmu_Alarm_CPU1_Lockstep_ComparatorError                              ,
        IfxSmu_Alarm_CPU1_Buslevel_MpuViolation                                 ,
        IfxSmu_Alarm_CPU1_PFI1_PFLASH1_ReadPathError                            ,
        IfxSmu_Alarm_CPU1_PCACHETAG_UncorrectableErrorDetection                 ,
        IfxSmu_Alarm_CPU1_PCACHETAG_MiscellaneousErrorDetection                 ,
        IfxSmu_Alarm_CPU1_PSPRPCACHE_SingleBitErrorCorrection                   ,
        IfxSmu_Alarm_CPU1_PSPRPCACHE_UncorrectableErrorDetection                ,
        IfxSmu_Alarm_CPU1_PSPRPCACHE_MiscellaneousErrorDetection                ,
        IfxSmu_Alarm_CPU1_DSPRDCACHEDLMU_SingleBitErrorCorrection               ,
        IfxSmu_Alarm_CPU1_DSPRDCACHEDLMU_UncorrectableErrorDetection            ,
        IfxSmu_Alarm_CPU1_DSPRDCACHEDLMU_MiscellaneousErrorDetection            ,
        IfxSmu_Alarm_CPU1_DCACHETAG_SingleBitErrorCorrection                    ,
        IfxSmu_Alarm_CPU1_DCACHETAG_UncorrectableErrorDetection                 ,
        IfxSmu_Alarm_CPU1_DCACHETAG_MiscellaneousErrorDetection                 ,
        IfxSmu_Alarm_CPU1_CPU_InstructionFetchSriInterfaceEdcError              ,
        IfxSmu_Alarm_CPU1_CPU_DataSriInterfaceLoadStoreEdcError                 ,
        IfxSmu_Alarm_CPU1_CPU_ExceptionInterruptTrap                            ,
        IfxSmu_Alarm_CPU2_Lockstep_ComparatorError                              ,
        IfxSmu_Alarm_CPU2_Buslevel_MpuViolation                                 ,
        IfxSmu_Alarm_CPU2_PFI2_PFLASH2_ReadPathError                            ,
        IfxSmu_Alarm_CPU2_PCACHETAG_UncorrectableErrorDetection                 ,
        IfxSmu_Alarm_CPU2_PCACHETAG_MiscellaneousErrorDetection                 ,
        IfxSmu_Alarm_CPU2_PSPRPCACHE_SingleBitErrorCorrection                   ,
        IfxSmu_Alarm_CPU2_PSPRPCACHE_UncorrectableErrorDetection                ,
        IfxSmu_Alarm_CPU2_PSPRPCACHE_MiscellaneousErrorDetection                ,
        IfxSmu_Alarm_CPU2_DSPRDCACHEDLMU_SingleBitErrorCorrection               ,
        IfxSmu_Alarm_CPU2_DSPRDCACHEDLMU_UncorrectableErrorDetection            ,
        IfxSmu_Alarm_CPU2_DSPRDCACHEDLMU_MiscellaneousErrorDetection            ,
        IfxSmu_Alarm_CPU2_DCACHETAG_SingleBitErrorCorrection                    ,
        IfxSmu_Alarm_CPU2_DCACHETAG_UncorrectableErrorDetection                 ,
        IfxSmu_Alarm_CPU2_DCACHETAG_MiscellaneousErrorDetection                 ,
        IfxSmu_Alarm_CPU2_CPU_InstructionFetchSriInterfaceEdcError              ,
        IfxSmu_Alarm_CPU2_CPU_DataSriInterfaceLoadStoreEdcError                 ,
        IfxSmu_Alarm_CPU2_CPU_ExceptionInterruptTrap                            ,
        IfxSmu_Alarm_CPU3_Lockstep_ComparatorError                              ,
        IfxSmu_Alarm_CPU3_Buslevel_MpuViolation                                 ,
        IfxSmu_Alarm_CPU3_PFI3_PFLASH3_ReadPathError                            ,
        IfxSmu_Alarm_CPU3_PCACHETAG_UncorrectableErrorDetection                 ,
        IfxSmu_Alarm_CPU3_PCACHETAG_MiscellaneousErrorDetection                 ,
        IfxSmu_Alarm_CPU3_PSPRPCACHE_SingleBitErrorCorrection                   ,
        IfxSmu_Alarm_CPU3_PSPRPCACHE_UncorrectableErrorDetection                ,
        IfxSmu_Alarm_CPU3_PSPRPCACHE_MiscellaneousErrorDetection                ,
        IfxSmu_Alarm_CPU3_DSPRDCACHEDLMU_SingleBitErrorCorrection               ,
        IfxSmu_Alarm_CPU3_DSPRDCACHEDLMU_UncorrectableErrorDetection            ,
        IfxSmu_Alarm_CPU3_DSPRDCACHEDLMU_MiscellaneousErrorDetection            ,
        IfxSmu_Alarm_CPU3_DCACHETAG_SingleBitErrorCorrection                    ,
        IfxSmu_Alarm_CPU3_DCACHETAG_UncorrectableErrorDetection                 ,
        IfxSmu_Alarm_CPU3_DCACHETAG_MiscellaneousErrorDetection                 ,
        IfxSmu_Alarm_CPU3_CPU_InstructionFetchSriInterfaceEdcError              ,
        IfxSmu_Alarm_CPU3_CPU_DataSriInterfaceLoadStoreEdcError                 ,
        IfxSmu_Alarm_CPU3_CPU_ExceptionInterruptTrap                            ,
        IfxSmu_Alarm_MTU_Safety_FfUncorrectableErrorDetected                    ,
        IfxSmu_Alarm_IOM_Safety_FfUncorrectableErrorDetected                    ,
        IfxSmu_Alarm_IR_Safety_FfUncorrectableErrorDetected                     ,
        IfxSmu_Alarm_EMEM_Safety_FfUncorrectableErrorDetected                   ,
        IfxSmu_Alarm_SCU_Safety_FfUncorrectableErrorDetected                    ,
        IfxSmu_Alarm_PMS_Safety_FfUncorrectableErrorDetected                    ,
        IfxSmu_Alarm_DMA_Safety_FfUncorrectableErrorDetected                    ,
        IfxSmu_Alarm_SMU_core_Safety_FfUncorrectableErrorDetected               ,
        IfxSmu_Alarm_SYS_PLL_PER_PLL_Safety_FfUncorrectableErrorDetected        ,
        IfxSmu_Alarm_GTM_GTM_SingleBitErrorCorrection                           ,
        IfxSmu_Alarm_GTM_GTM_UncorrectableErrorDetection                        ,
        IfxSmu_Alarm_GTM_GTM_MiscellaneousErrorDetection                        ,
        IfxSmu_Alarm_ERAY_ERAY_SingleBitErrorCorrection                         ,
        IfxSmu_Alarm_ERAY_ERAY_UncorrectableErrorDetection                      ,
        IfxSmu_Alarm_ERAY_ERAY_MiscellaneousErrorDetection                      ,
        IfxSmu_Alarm_CAN_CAN_SingleBitErrorCorrection                           ,
        IfxSmu_Alarm_CAN_CAN_UncorrectableErrorDetection                        ,
        IfxSmu_Alarm_CAN_CAN_MiscellaneousErrorDetection                        ,
        IfxSmu_Alarm_Misc_MISC_SingleBitErrorCorrection                         ,
        IfxSmu_Alarm_Misc_MISC_UncorrectableErrorDetection                      ,
        IfxSmu_Alarm_Misc_MISC_MiscellaneousErrorDetection                      ,
        IfxSmu_Alarm_CERBERUS_Safety_FfUncorrectableErrorDetected               ,
        IfxSmu_Alarm_CCU_Safety_FfCorrectableErrorDetected                      ,
        IfxSmu_Alarm_CCU_Safety_FfUncorrectableErrorDetected                    ,
        IfxSmu_Alarm_LMU_FSI_RAM_LMUDMU_SingleBitErrorCorrection                ,
        IfxSmu_Alarm_LMU_FSI_RAM_LMUDMU_UncorrectableErrorDetection             ,
        IfxSmu_Alarm_LMU_FSI_RAM_LMUDMU_MiscellaneousErrorDetection             ,
        IfxSmu_Alarm_LMU_Lockstep_ComparatorError                               ,
        IfxSmu_Alarm_LMU_Lockstep_ControlError                                  ,
        IfxSmu_Alarm_LMU_ECC_Error                                              ,
        IfxSmu_Alarm_LMU_Buslevel_MpuError                                      ,
        IfxSmu_Alarm_LMU_EDC_ReadPhaseError                                     ,
        IfxSmu_Alarm_XBAR0_SRI_BusErrorEvent                                    ,
        IfxSmu_Alarm_XBAR1_SRI_BusErrorEvent                                    ,
        IfxSmu_Alarm_XBAR2_SRI_BusErrorEvent                                    ,
        IfxSmu_Alarm_SPB_BusErrorEvent                                          ,
        IfxSmu_Alarm_BBB_BusErrorEvent                                          ,
        IfxSmu_Alarm_FSI_PFlash_SingleBitError                                  ,
        IfxSmu_Alarm_FSI_PFlash_DoubleBitError                                  ,
        IfxSmu_Alarm_FSI_Single_BitCorrectionTrackingBufferFull                 ,
        IfxSmu_Alarm_FSI_Double_BitCorrectionTrackingBufferFull                 ,
        IfxSmu_Alarm_FSI_Multiple_BitErrorDetectionTrackingBufferFull           ,
        IfxSmu_Alarm_FSI_Zero_BitErrorTrackingBufferFull                        ,
        IfxSmu_Alarm_FSI_PFlash_EccError                                        ,
        IfxSmu_Alarm_FSI_PFlash_EdcError                                        ,
        IfxSmu_Alarm_FSI_CPUFLASHCON_ConfigurationError                         ,
        IfxSmu_Alarm_FSI_Flash_StoredConfigurationError                         ,
        IfxSmu_Alarm_SCU_OS_CClockFrequencyOutOfRange                           ,
        IfxSmu_Alarm_CCU_Backup_ClockOutOfRangeAlarm                            ,
        IfxSmu_Alarm_CCU_Backup_ClockAliveAlarm                                 ,
        IfxSmu_Alarm_SCU_System_PllDcoLossOfLockEvent                           ,
        IfxSmu_Alarm_SCU_Peripheral_PllDcoLossOfLockEvent                       ,
        IfxSmu_Alarm_SCU_LBIST_Alarm                                            ,
        IfxSmu_Alarm_SCU_External_RequestUnitAlarm0                             ,
        IfxSmu_Alarm_SCU_External_RequestUnitAlarm1                             ,
        IfxSmu_Alarm_SCU_External_RequestUnitAlarm2                             ,
        IfxSmu_Alarm_SCU_External_RequestUnitAlarm3                             ,
        IfxSmu_Alarm_SCU_CPU0_WatchdogTimeOut                                   ,
        IfxSmu_Alarm_SCU_CPU1_WatchdogTimeOut                                   ,
        IfxSmu_Alarm_SCU_CPU2_WatchdogTimeOut                                   ,
        IfxSmu_Alarm_SCU_CPU3_WatchdogTimeOut                                   ,
        IfxSmu_Alarm_SCU_Safety_WatchdogTimeOut                                 ,
        IfxSmu_Alarm_SCU_Watchdog_TimeOut                                       ,
        IfxSmu_Alarm_SCU_Lockstep_DualRailError                                 ,
        IfxSmu_Alarm_SCU_External_EmergencyStopSignalEvent                      ,
        IfxSmu_Alarm_SCU_Pad_HeatingAlarm                                       ,
        IfxSmu_Alarm_SCU_LBIST_TestModeAlarm                                    ,
        IfxSmu_Alarm_IR_EDC_ConfigurationDataPathError                          ,
        IfxSmu_Alarm_DMA_DMASRI_EccError                                        ,
        IfxSmu_Alarm_IOM_Pin_MismatchIndication                                 ,
        IfxSmu_Alarm_SCU_External_RequestUnitAlarm4                             ,
        IfxSmu_Alarm_SCU_External_RequestUnitAlarm5                             ,
        IfxSmu_Alarm_SCU_External_RequestUnitAlarm6                             ,
        IfxSmu_Alarm_SCU_External_RequestUnitAlarm7                             ,
        IfxSmu_Alarm_DTS_core_Under_TemperatureAlarm                            ,
        IfxSmu_Alarm_DTS_core_Over_TemperatureAlarm                             ,
        IfxSmu_Alarm_DTS_Temperature_Overflow                                   ,
        IfxSmu_Alarm_DTS_Temperature_Underflow                                  ,
        IfxSmu_Alarm_EVR_Overvoltage_Alarm                                      ,
        IfxSmu_Alarm_EVR_Undervoltage_Alarm                                     ,
        IfxSmu_Alarm_EVR_Short_ToLowHightAlarm                                  ,
        IfxSmu_Alarm_HSM_Overvoltage_Alarm                                      ,
        IfxSmu_Alarm_HSM_Undervoltage_Alarm                                     ,
        IfxSmu_Alarm_EMEM_Unexpected_WriteToEmemAlarm                           ,
        IfxSmu_Alarm_EMEM_SEP_ControlLogicAlarm                                 ,
        IfxSmu_Alarm_EMEM_Lockstep_ControlLogicInputAlarm                       ,
        IfxSmu_Alarm_Software_Alarm0                                            ,
        IfxSmu_Alarm_Software_Alarm1                                            ,
        IfxSmu_Alarm_Software_Alarm2                                            ,
        IfxSmu_Alarm_Software_Alarm3                                            ,
        IfxSmu_Alarm_Software_Alarm4                                            ,
        IfxSmu_Alarm_Software_Alarm5                                            ,
        IfxSmu_Alarm_Software_Alarm6                                            ,
        IfxSmu_Alarm_Software_Alarm7                                            ,
        IfxSmu_Alarm_Software_Alarm8                                            ,
        IfxSmu_Alarm_Software_Alarm9                                            ,
        IfxSmu_Alarm_Software_Alarm10                                           ,
        IfxSmu_Alarm_Software_Alarm11                                           ,
        IfxSmu_Alarm_Software_Alarm12                                           ,
        IfxSmu_Alarm_Software_Alarm13                                           ,
        IfxSmu_Alarm_Software_Alarm14                                           ,
        IfxSmu_Alarm_Software_Alarm15                                           ,
        IfxSmu_Alarm_SMU_Timer0_TimeOut                                         ,
        IfxSmu_Alarm_SMU_Timer1_TimeOut                                         ,
        IfxSmu_Alarm_SMU_Error_PinFaultStateActivation                          ,
        IfxSmu_Alarm_SMU_Safety_FfCorrectableErrorDetected                      ,
        IfxSmu_Alarm_SMU_Safety_FfUncorrectableErrorDetected                    ,
        IfxSmu_Alarm_SMU_Access_EnableErrorDetected                             ,
        IfxSmu_Alarm_LMU_EDC_AddressPhaseError                                  ,
        IfxSmu_Alarm_LMU_EDC_WritePhaseError                                    ,
        IfxSmu_Alarm_XBAR_EDC_AddressPhaseError                                 ,
        IfxSmu_Alarm_XBAR_EDC_WritePhaseError                                   ,
        IfxSmu_Alarm_DMU_EDC_AddressPhaseError                                  ,
        IfxSmu_Alarm_DMU_EDC_WritePhaseError                                    ,
        IfxSmu_Alarm_SFI_BBB_EDC_AddressPhaseError                              ,
        IfxSmu_Alarm_SFI_BBB_EDC_WritePhaseError                                ,
        IfxSmu_Alarm_SRI_EDC_ReadPhaseError                                     ,
        IfxSmu_Alarm_SFI_SPB_EDC_ReadPhaseError                                 ,
        IfxSmu_Alarm_HSSL_HSSL0EDC_ReadPhaseError                               ,
        IfxSmu_Alarm_CONVERTER_Phase_SyncronizerError                           ,
        IfxSmu_Alarm_XBAR_SOTA_SwapError
};

void Smu_Init(void)
{
    /* Configure FSP output port pin */
    IfxSmu_unlockConfigRegisters();
    uint16 passwd = IfxScuWdt_getSafetyWatchdogPassword();
    /* disable the write-protection for registers */
    IfxScuWdt_clearSafetyEndinit(passwd);
    /* Restore back the write-protection for registers */
    IfxScuWdt_setSafetyEndinit(passwd);
    /* Enable the transition from fault state to run state */
    IfxSmu_enableFaultToRunState(TRUE);
    /* Clear Safety ENDINIT protection for modification of the configuration registers */
    IfxScuWdt_clearSafetyEndinit(IfxScuWdt_getSafetyWatchdogPasswordInline());
    /* Set Safety ENDINIT protection after modification of the configuration registers */
    IfxScuWdt_setSafetyEndinit(IfxScuWdt_getSafetyWatchdogPasswordInline());

    for(uint32 i = 0; i < (sizeof(g_smuAlarms_TC375)/sizeof(g_smuAlarms_TC375[0])); i++)
    {
        IfxSmu_setAlarmAction(g_smuAlarms_TC375[i], IfxSmu_InternalAlarmAction_nmi);
    }
    /* Afterwards, lock the SMU configuration */
    IfxSmu_lockConfigRegisters();
    /* Enter the fault free state "Run" (after the Power-on Reset (PORST) the FSP is in fault state) */
    IfxSmu_releaseFSP();
    IfxSmu_activateRunState();                                          /* Start the SMU state machine (SSM)        */
}
