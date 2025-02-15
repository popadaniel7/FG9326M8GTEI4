#include "McuSm.h"
#include "Irq.h"

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

uint8 g_isMeasureAvailable =            FALSE;                             /* Variable to store availability of new measurements */;

Ifx_CSA McuSm_CSA_capture[IFXCPU_NUM_MODULES][CSA_CAPTURE_LIMIT];
uint32 McuSm_STACK_capture[IFXCPU_NUM_MODULES][STACK_CAPTURE_LIMIT][STACK_CAPTURE_SIZE];
Ifx_CPU_PIETR McuSm_PIETR_capture[IFXCPU_NUM_MODULES];
Ifx_CPU_PIEAR McuSm_PIEAR_capture[IFXCPU_NUM_MODULES];
Ifx_CPU_DIETR McuSm_DIETR_capture[IFXCPU_NUM_MODULES];
Ifx_CPU_DIEAR McuSm_DIEAR_capture[IFXCPU_NUM_MODULES];
Ifx_CPU_DATR McuSm_DATR_capture[IFXCPU_NUM_MODULES];
Ifx_CPU_DEADD McuSm_DEADD_capture[IFXCPU_NUM_MODULES];
Ifx_SMU_AG McuSm_AG_capture[IFXCPU_NUM_MODULES][12];
uint32 McuSm_LastResetReason;
uint32 McuSm_LastResetInformation;
uint32 McuSm_IndexResetHistory;
McuSm_ResetHistory_t McuSm_ResetHistory[20u];
McuSm_ResetReason_t McuSm_ResetReasonListCounter[400u];

void McuSm_InitializeDts(void);
void McuSm_InitializeBusMpu(void);
void McuSm_EnableBusMpu(void);
void McuSm_InitializeDataProtectionRange(uint32 lowerBoundAddress, uint32 upperBoundAddress, uint8 range);
void McuSm_InitializeCodeProtectionRange(uint32 lowerBoundAddress, uint32 upperBoundAddress, uint8 range);
void McuSm_EnableDataRead(uint8 protectionSet, uint8 range);
void McuSm_EnableDataWrite(uint8 protectionSet, uint8 range);
void McuSm_EnableCodeExecution(uint8 protectionSet, uint8 range);
void McuSm_MbistManager(void);
void McuSm_MbistTest(IfxMtu_MbistSel mbistSel);
uint32 McuSm_GetMbistErrors(IfxMtu_MbistSel mbistSel);
void McuSm_ClearMbistErrors(IfxMtu_MbistSel mbistSel);
void McuSm_PerformResetHook(uint32 resetReason, uint32 resetInformation);
void McuSm_CsaCapture(void);
void McuSm_PIEAR_PIETR_Capture(void);
void McuSm_DIEAR_DIETR_Capture(void);
void McuSm_DATR_DEADD_Capture(void);
void McuSm_AG_Capture(void);
void McuSm_TRAP2(IfxCpu_Trap trapInfo);
void McuSm_TRAP3(IfxCpu_Trap trapInfo);
void McuSm_TRAP4(IfxCpu_Trap trapInfo);
void McuSm_TRAP7(IfxCpu_Trap trapInfo);

void McuSm_PerformResetHook(uint32 resetReason, uint32 resetInformation)
{
    for(uint32 i = 0; i < sizeof(McuSm_FullListCompare); i++)
    {
        if(resetReason == McuSm_FullListCompare[i])
        {
            break;
        }
        else
        {
            /* Do nothing. */
        }
    }

    if(resetReason != NO_ERR)
    {
        McuSm_LastResetReason = resetReason;
        McuSm_LastResetInformation = resetInformation;
        McuSm_ResetReasonListCounter[resetReason]++;
        McuSm_ResetHistory[McuSm_IndexResetHistory].reason = resetReason;
        McuSm_ResetHistory[McuSm_IndexResetHistory].information = resetInformation;
        McuSm_IndexResetHistory++;
    }
    else
    {
        /* Do nothing. */
    }

    IfxScuRcu_performReset(IfxScuRcu_ResetType_application, 0);
}
/* This function will trace back as many as CSA_CAPTURE_LIMIT context save areas and dump each into CSA_capture
 * For upper context calls, this function will also dump STACK_CAPTURE_SIZE words from the stack of each context*/
void McuSm_CsaCapture(void)
{
    /*Fetch the ID of the current CPU*/
    uint8 cpu = __mfcr(CPU_CUS_ID);
    Ifx_CSA *context_ptr;
    Ifx_CPU_PCXI pcxi;
    uint8 csa_count = 0;
    uint8 stack_count = 0;
    /*Fetch the previous context information*/
    pcxi.U = __mfcr(CPU_PCXI);
    /*Run as long as we have space to dump another CSA*/
    while (csa_count < CSA_CAPTURE_LIMIT)
    {
        /*Recover pointer to CSA block*/
        context_ptr = (Ifx_CSA *) ((pcxi.B.PCXS << 28) | ((pcxi.B.PCXO)*(sizeof(Ifx_CSA))));
        /*Once we've reached the end of the context trace, we are done*/
        if (context_ptr == NULL)
        {
            break;
        }
        else
        {
            /* Do nothing. */
        }

        if (pcxi.B.UL == CSA_UPPER_CONTEXT)
        {
            /* UPPER contexts are from function calls
             * Reference:
             * - TriCore_TC162P_core_architecture_volume_1_of_2.pdf
             * - 4.5 Context Switching for Function Callsc
             */
            McuSm_CSA_capture[cpu][csa_count].UPPER.CSA_PCXI   = context_ptr->UPPER.CSA_PCXI;
            McuSm_CSA_capture[cpu][csa_count].UPPER.CSA_PSW    = context_ptr->UPPER.CSA_PSW;
            McuSm_CSA_capture[cpu][csa_count].UPPER.CSA_A10    = context_ptr->UPPER.CSA_A10;
            McuSm_CSA_capture[cpu][csa_count].UPPER.CSA_A11    = context_ptr->UPPER.CSA_A11;
            McuSm_CSA_capture[cpu][csa_count].UPPER.CSA_D8     = context_ptr->UPPER.CSA_D8;
            McuSm_CSA_capture[cpu][csa_count].UPPER.CSA_D9     = context_ptr->UPPER.CSA_D9;
            McuSm_CSA_capture[cpu][csa_count].UPPER.CSA_D10    = context_ptr->UPPER.CSA_D10;
            McuSm_CSA_capture[cpu][csa_count].UPPER.CSA_D11    = context_ptr->UPPER.CSA_D11;
            McuSm_CSA_capture[cpu][csa_count].UPPER.CSA_A12    = context_ptr->UPPER.CSA_A12;
            McuSm_CSA_capture[cpu][csa_count].UPPER.CSA_A13    = context_ptr->UPPER.CSA_A13;
            McuSm_CSA_capture[cpu][csa_count].UPPER.CSA_A14    = context_ptr->UPPER.CSA_A14;
            McuSm_CSA_capture[cpu][csa_count].UPPER.CSA_A15    = context_ptr->UPPER.CSA_A15;
            McuSm_CSA_capture[cpu][csa_count].UPPER.CSA_D12    = context_ptr->UPPER.CSA_D12;
            McuSm_CSA_capture[cpu][csa_count].UPPER.CSA_D13    = context_ptr->UPPER.CSA_D13;
            McuSm_CSA_capture[cpu][csa_count].UPPER.CSA_D14    = context_ptr->UPPER.CSA_D14;
            McuSm_CSA_capture[cpu][csa_count].UPPER.CSA_D15    = context_ptr->UPPER.CSA_D15;
            /*For upper context calls, we also store the stack if there is room left in STACK_capture*/
            if (stack_count < STACK_CAPTURE_LIMIT)
            {
                /*Fetch the stack pointer stored in register A10*/
                uint32 *stack_ptr = (uint32*)context_ptr->UPPER.CSA_A10.U;

                /*store STACK_CAPTURE_SIZE words*/
                for (uint8 i = 0; i < STACK_CAPTURE_SIZE; ++i)
                {
                    McuSm_STACK_capture[cpu][stack_count][i] = stack_ptr[i];
                }

                ++stack_count;
            }
            else
            {
                /* Do nothing. */
            }
            /*Set pcxi to the next previous context*/
            pcxi = context_ptr->UPPER.CSA_PCXI;
        }
        else
        {
            /* LOWER contexts are from interrupts or traps
             * Reference:
             * - TriCore_TC162P_core_architecture_volume_1_of_2.pdf
             * - 4.4 Context Switching with Interrupts and Traps
             */
            McuSm_CSA_capture[cpu][csa_count].LOWER.CSA_PCXI   = context_ptr->LOWER.CSA_PCXI;
            McuSm_CSA_capture[cpu][csa_count].LOWER.CSA_A11    = context_ptr->LOWER.CSA_A11;
            McuSm_CSA_capture[cpu][csa_count].LOWER.CSA_A2     = context_ptr->LOWER.CSA_A2;
            McuSm_CSA_capture[cpu][csa_count].LOWER.CSA_A3     = context_ptr->LOWER.CSA_A3;
            McuSm_CSA_capture[cpu][csa_count].LOWER.CSA_D0     = context_ptr->LOWER.CSA_D0;
            McuSm_CSA_capture[cpu][csa_count].LOWER.CSA_D1     = context_ptr->LOWER.CSA_D1;
            McuSm_CSA_capture[cpu][csa_count].LOWER.CSA_D2     = context_ptr->LOWER.CSA_D2;
            McuSm_CSA_capture[cpu][csa_count].LOWER.CSA_D3     = context_ptr->LOWER.CSA_D3;
            McuSm_CSA_capture[cpu][csa_count].LOWER.CSA_A4     = context_ptr->LOWER.CSA_A4;
            McuSm_CSA_capture[cpu][csa_count].LOWER.CSA_A5     = context_ptr->LOWER.CSA_A5;
            McuSm_CSA_capture[cpu][csa_count].LOWER.CSA_A6     = context_ptr->LOWER.CSA_A6;
            McuSm_CSA_capture[cpu][csa_count].LOWER.CSA_A7     = context_ptr->LOWER.CSA_A7;
            McuSm_CSA_capture[cpu][csa_count].LOWER.CSA_D4     = context_ptr->LOWER.CSA_D4;
            McuSm_CSA_capture[cpu][csa_count].LOWER.CSA_D5     = context_ptr->LOWER.CSA_D5;
            McuSm_CSA_capture[cpu][csa_count].LOWER.CSA_D6     = context_ptr->LOWER.CSA_D6;
            McuSm_CSA_capture[cpu][csa_count].LOWER.CSA_D7     = context_ptr->LOWER.CSA_D7;
            /*Set pcxi to the next previous context*/
            pcxi = context_ptr->LOWER.CSA_PCXI;
        }
        csa_count++;
    }
}

void McuSm_PIEAR_PIETR_Capture(void)
{
    /*Fetch the ID of the current CPU*/
    uint8 cpu = __mfcr(CPU_CUS_ID);
    McuSm_PIEAR_capture[cpu].U = __mfcr(CPU_PIEAR);
    McuSm_PIETR_capture[cpu].U = __mfcr(CPU_PIETR);
}

void McuSm_DIEAR_DIETR_Capture(void)
{
    /*Fetch the ID of the current CPU*/
    uint8 cpu = __mfcr(CPU_CUS_ID);
    McuSm_DIEAR_capture[cpu].U = __mfcr(CPU_DIEAR);
    McuSm_DIETR_capture[cpu].U = __mfcr(CPU_DIETR);
}

void McuSm_DATR_DEADD_Capture(void)
{
    /*Fetch the ID of the current CPU*/
    uint8 cpu = __mfcr(CPU_CUS_ID);
    McuSm_DATR_capture[cpu].U = __mfcr(CPU_DATR);
    McuSm_DEADD_capture[cpu].U = __mfcr(CPU_DEADD);
}

void McuSm_AG_Capture(void)
{
    /*Fetch the ID of the current CPU*/
    uint8 cpu = __mfcr(CPU_CUS_ID);
    for (uint8 i=0; i<12; ++i)
    {
        McuSm_AG_capture[cpu][i] = MODULE_SMU.AG[i];
    }
}

void McuSm_TRAP2(IfxCpu_Trap trapInfo)
{
    /*execute the context save area capture*/
    McuSm_CsaCapture();
    switch (trapInfo.tId)
    {
        case IfxCpu_Trap_InstructionErrors_Id_dataAddressAlignment:
        case IfxCpu_Trap_InstructionErrors_Id_invalidMemoryAddress:
            /*Capture CPUx_DATR and CPUx_DEADD*/
            McuSm_DATR_DEADD_Capture();
            break;
        default:
            break;
    }
    McuSm_PerformResetHook(TRAP2, trapInfo.tId);
}

void McuSm_TRAP4(IfxCpu_Trap trapInfo)
{
    /*execute the context save area capture*/
    McuSm_CsaCapture();
    switch (trapInfo.tId)
    {
        case IfxCpu_Trap_Bus_Id_programMemoryIntegrityError:
            /*Capture CPUx_PIEAR and CPUx_PIETR*/
            McuSm_PIEAR_PIETR_Capture();
            break;
        case IfxCpu_Trap_Bus_Id_dataMemoryIntegrityError:
            /*Capture CPUx_DIEAR and CPUx_DIETR*/
            McuSm_DIEAR_DIETR_Capture();
            break;
        default:
            break;
    }
    McuSm_PerformResetHook(TRAP4, trapInfo.tId);
}

void McuSm_TRAP7(IfxCpu_Trap trapInfo)
{
    /*execute the context save area capture*/
    McuSm_CsaCapture();
    /*capture SMU_AGx*/
    McuSm_AG_Capture();
    McuSm_PerformResetHook(TRAP7, trapInfo.tId);
}

void McuSm_TRAP3(IfxCpu_Trap trapInfo)
{
    /*execute the context save area capture*/
    McuSm_CsaCapture();
    /*capture SMU_AGx*/
    McuSm_AG_Capture();
    McuSm_PerformResetHook(TRAP3, trapInfo.tId);
}
/* Function to initialize the Die Temperature Sensor */
void McuSm_InitializeDts(void)
{
    /* Get the default configuration */
    IfxDts_Dts_Config dtsConf;
    IfxDts_Dts_initModuleConfig(&dtsConf);              /* Initialize the structure with default values              */
    dtsConf.lowerTemperatureLimit = MIN_TEMP_LIMIT;     /* Set the lower temperature limit                           */
    dtsConf.upperTemperatureLimit = MAX_TEMP_LIMIT;     /* Set the upper temperature limit                           */
    dtsConf.isrPriority = 8u;             /* Set the interrupt priority for new measurement events     */
    dtsConf.isrTypeOfService = IfxSrc_Tos_cpu0;         /* Set the service provider responsible for handling the interrupts */
    IfxDts_Dts_initModule(&dtsConf);                    /* Initialize the DTS with the given configuration           */
}

void McuSm_MbistManager(void)
{
    /* Initialize the selected SRAM */
    IfxMtu_clearSram(IfxMtu_MbistSel_dma);
    IfxMtu_clearSram(IfxMtu_MbistSel_mcan0);

    if((SCU_RSTSTAT.U & POWERONRESET_MASK) > 0)
    {
        //IfxMtu_clearSram(IfxMtu_MbistSel_cpu0Dlmu);
        IfxMtu_clearSram(IfxMtu_MbistSel_cpu1Dlmu);
        IfxMtu_clearSram(IfxMtu_MbistSel_cpu2Dlmu);
    }
    else
    {
        /* Do nothing. */
    }

    McuSm_MbistTest(IfxMtu_MbistSel_dma);
    McuSm_MbistTest(IfxMtu_MbistSel_mcan0);
    //McuSm_MbistTest(IfxMtu_MbistSel_cpu0Dlmu);
    McuSm_MbistTest(IfxMtu_MbistSel_cpu1Dlmu);
    McuSm_MbistTest(IfxMtu_MbistSel_cpu2Dlmu);
}
/* This function initializes and tests an SRAM memory using MBIST */
void McuSm_MbistTest(IfxMtu_MbistSel mbistSel)
{
    uint16 errAddr = 0u;
    uint8 result = 0u;
    uint8 returnedError = 0u;
    uint32 resultedInformation = 0u;
    /* Check if any error flag is set.
     * After any System Reset: For each and every SSH in the system, the UCE alarm status in the SMU, the ECCD.UCERR
     * (Consequently also SERR) and the FAULTSTS.OPERR[0] are set.
     */
    if(McuSm_GetMbistErrors(mbistSel))
    {
        /* Clear the alarm status flags to enable a correct error tracking */
        McuSm_ClearMbistErrors(mbistSel);
    }
    else
    {
        /* Do nothing. */
    }
    /* Run the Non-Destructive Test (NDT) */
    result = IfxMtu_runNonDestructiveInversionTest((IfxMtu_MbistSel)mbistSel,
            MBIST_TEST_RANGE_ENABLE,
            MBIST_TEST_HIGHER_RANGE_LIMIT,
            MBIST_TEST_LOWER_RANGE_LIMIT,
            &errAddr);
    if(0u != result)
    {
        returnedError = returnedError | (1u << 0u);
        result = 0u;
    }
    else
    {
        /* Do nothing. */
    }
    resultedInformation = errAddr | (returnedError << 16u);
    /* Check the test result */
    /* No error has occurred */
    if (result == 0)
    {
        /* Do nothing. */
    }
    else /* An error is detected */
    {
        McuSm_PerformResetHook(MBIST_FAIL, resultedInformation);
    }
}
/* Get MBIST error flags of a specific memory */
uint32 McuSm_GetMbistErrors(IfxMtu_MbistSel mbistSel)
{
    Ifx_MTU_MC *mc = (Ifx_MTU_MC *)(IFXMTU_MC_ADDRESS_BASE + 0x100 * mbistSel);
    return (uint32)(mc->ECCD.U & (IFXMTU_ERROR_FLAGS_MASK));
}
/* Clear MCi_ECCD and MCi_FAULTSTS (i=0-95) error flags of a specific memory raised by a System Reset */
void McuSm_ClearMbistErrors(IfxMtu_MbistSel mbistSel)
{
    Ifx_MTU_MC *mc = &MODULE_MTU.MC[mbistSel];
    uint16  password = IfxScuWdt_getSafetyWatchdogPassword();
    uint8   isEndInitEnabled = 0;
    /* Check if the EndInit is already cleared:
     * if not, clear it to enable writes on MCi_FAULTST (i=0-95) register of a specific memory
     */
    if (IfxScuWdt_getSafetyWatchdogEndInit() == 1U)
    {
        /* Clear EndInit */
        IfxScuWdt_clearSafetyEndinit(password);
        isEndInitEnabled = 1;
    }
    else
    {
        /* Do nothing. */
    }

    mc->ECCD.B.SERR = 0;                                    /* Error Detected flag                          */
    mc->ECCD.B.UCERR = 0;                                   /* UnCorrectable Error (UCE) alarm flag         */
    mc->FAULTSTS.B.OPERR &= MTU_CLEAR_FAULTSTS_OPERR0_MSK;  /* SSH Critical Operation Error flags: clear
     * bit 0, set by any System Reset               */
    /* Restore the EndInit state */
    if (isEndInitEnabled == 1)
    {
        /* Set EndInit Watchdog (to prevent Watchdog Time Out) */
        IfxScuWdt_setSafetyEndinit(password);
    }
    else
    {
        /* Do nothing. */
    }

    IfxMtu_clearErrorTracking(mbistSel);                    /* Clear the error tracking registers (ETRR)    */
}

void McuSm_InitializeBusMpu(void)
{
    /* Configure memory protection */
    /* Rules for memory protection range configuration:
     * The lower and upper boundary addresses must be 8 byte aligned.
     * After enabling the Memory Protection, access to an address 'x' will be allowed only if:
     * lowerBoundAddress <= x < upperBoundAddress
     */
    /* Define the Data Protection Range 0: from 0x00000000 up to the address of the 48th index of the dummyData array */
    McuSm_InitializeDataProtectionRange(FIRST_ADDR, LAST_ADDR, DATA_PROTECTION_RANGE_0);
    /* Enable read access to the Data Protection Range 0 in the Protection Set 1 */
    McuSm_EnableDataRead(PROTECTION_SET_1, DATA_PROTECTION_RANGE_0);
    /* Enable write access to the Data Protection Range 0 in the Protection Set 1 */
    McuSm_EnableDataWrite(PROTECTION_SET_1, DATA_PROTECTION_RANGE_0);
    /* Define the Data Protection Range 1: from the address of the last element of dummyData array to 0xFFFFFFFF*/
    McuSm_InitializeDataProtectionRange(FIRST_ADDR, LAST_ADDR, DATA_PROTECTION_RANGE_1);
    /* Enable read access to the Data Protection Range 1 in the Protection Set 1 */
    McuSm_EnableDataRead(PROTECTION_SET_1, DATA_PROTECTION_RANGE_1);
    /* Enable write access to the Data Protection Range 1 in the Protection Set 1 */
    McuSm_EnableDataWrite(PROTECTION_SET_1, DATA_PROTECTION_RANGE_1);
    /* Define the Code Protection Range 0: from 0x00000000 to 0xFFFFFFFF (every address of the microcontroller) */
    McuSm_InitializeCodeProtectionRange(FIRST_ADDR, LAST_ADDR, CODE_PROTECTION_RANGE_0);
    /* Enable code execution on every address for Protection Set 1 */
    McuSm_EnableCodeExecution(PROTECTION_SET_1, CODE_PROTECTION_RANGE_0);
    /* When a trap occurs (i.e. in case of an access violation) the active Protection Set is automatically switched to 0.
     * The following code enables the write/read access and code execution to all the memory addresses on protection set 0.
     * This ensures that the trap vector table is still executable and readable.
     */
    /* Define the Code Protection Range 9: from 0x00000000 to 0xFFFFFFFF (every address of the microcontroller) */
    McuSm_InitializeCodeProtectionRange(FIRST_ADDR, LAST_ADDR, CODE_PROTECTION_RANGE_9);
    /* Define the Data Protection Range 17: from 0x00000000 to 0xFFFFFFFF (every address of the microcontroller) */
    McuSm_InitializeDataProtectionRange(FIRST_ADDR, LAST_ADDR, DATA_PROTECTION_RANGE_17);
    /* Enable code execution for Protection Set 0 (the default Protection Set) */
    McuSm_EnableCodeExecution(PROTECTION_SET_0, CODE_PROTECTION_RANGE_9);
    /* Enable read access to the Data Protection Range 17 in the Protection Set 0 */
    McuSm_EnableDataRead(PROTECTION_SET_0, DATA_PROTECTION_RANGE_17);
    /* Enable write access to the Data Protection Range 17 in the Protection Set 0 */
    McuSm_EnableDataWrite(PROTECTION_SET_0, DATA_PROTECTION_RANGE_17);
    /* Activate Protection Set 1.
     * The active protection set determines the read, write, and execute permissions of the CPU.
     */
    McuSm_SetActiveProtectionSet(PROTECTION_SET_1);
    /* Enable the Memory Protection */
    McuSm_EnableBusMpu();
}
/* Function that enables the memory protection
 * Make sure to define code and data protection ranges before calling this function.
 *
 * In this function two intrinsic functions, __mfcr() and __mtcr() are called, to access control registers.
 * The intrinsic function __mfcr(int cfsr) moves contents of the addressed Core Special Function Register (CSFR)
 * into a data register.
 * The intrinsic function __mtcr(int csfr, int val) moves contents of a data register (second int) to the addressed
 * CSFR (first int).
 * The ISYNC instruction ensures that the effects of the CSFR update are correctly seen by all following instructions.
 *
 * The Tasking intrinsic function for MTCR automatically includes an ISYNC instruction.
 * ISYNC for other compiler vendors is ensured by use of a preprocessor macro.
 */
void McuSm_EnableBusMpu(void)
{
    Ifx_CPU_SYSCON sysconValue;
    sysconValue.U = __mfcr(CPU_SYSCON);                 /* Get the System Configuration Register (SYSCON) value     */
    sysconValue.B.PROTEN = 1;                           /* Set the PROTEN bitfield to enable the Memory Protection  */
    __mtcr(CPU_SYSCON, sysconValue.U);                  /* Set the System Configuration Register (SYSCON)           */
    __isync();
}
/* Function that defines a data protection range in the corresponding CPU Data Protection Range Register (DPR).
 * Data protection ranges have 8-byte granularity.
 * As a result, the lower 3 bits of any address passed to the define_data_protection_range function will be discarded.
 * After enabling the Memory Protection, access to an address 'x' will be allowed only if:
 * lowerBoundAddress <= x < upperBoundAddress
 */
void McuSm_InitializeDataProtectionRange(uint32 lowerBoundAddress, uint32 upperBoundAddress, uint8 range)
{
    switch(range)
    {
        case 0: /* Data Protection Range 0 */
            __mtcr(CPU_DPR0_L, lowerBoundAddress);      /* Set the lower bound of CPU Data Protection Range 0       */
            __mtcr(CPU_DPR0_U, upperBoundAddress);      /* Set the upper bound of CPU Data Protection Range 0       */
            break;
        case 1: /* Data Protection Range 1 */
            __mtcr(CPU_DPR1_L, lowerBoundAddress);      /* Set the lower bound of CPU Data Protection Range 1       */
            __mtcr(CPU_DPR1_U, upperBoundAddress);      /* Set the upper bound of CPU Data Protection Range 1       */
            break;
        case 2: /* Data Protection Range 2 */
            __mtcr(CPU_DPR2_L, lowerBoundAddress);      /* Set the lower bound of CPU Data Protection Range 2       */
            __mtcr(CPU_DPR2_U, upperBoundAddress);      /* Set the upper bound of CPU Data Protection Range 2       */
            break;
        case 3: /* Data Protection Range 3 */
            __mtcr(CPU_DPR3_L, lowerBoundAddress);      /* Set the lower bound of CPU Data Protection Range 3       */
            __mtcr(CPU_DPR3_U, upperBoundAddress);      /* Set the upper bound of CPU Data Protection Range 3       */
            break;
        case 4: /* Data Protection Range 4 */
            __mtcr(CPU_DPR4_L, lowerBoundAddress);      /* Set the lower bound of CPU Data Protection Range 4       */
            __mtcr(CPU_DPR4_U, upperBoundAddress);      /* Set the upper bound of CPU Data Protection Range 4       */
            break;
        case 5: /* Data Protection Range 5 */
            __mtcr(CPU_DPR5_L, lowerBoundAddress);      /* Set the lower bound of CPU Data Protection Range 5       */
            __mtcr(CPU_DPR5_U, upperBoundAddress);      /* Set the upper bound of CPU Data Protection Range 5       */
            break;
        case 6: /* Data Protection Range 6 */
            __mtcr(CPU_DPR6_L, lowerBoundAddress);      /* Set the lower bound of CPU Data Protection Range 6       */
            __mtcr(CPU_DPR6_U, upperBoundAddress);      /* Set the upper bound of CPU Data Protection Range 6       */
            break;
        case 7: /* Data Protection Range 7 */
            __mtcr(CPU_DPR7_L, lowerBoundAddress);      /* Set the lower bound of CPU Data Protection Range 7       */
            __mtcr(CPU_DPR7_U, upperBoundAddress);      /* Set the upper bound of CPU Data Protection Range 7       */
            break;
        case 8: /* Data Protection Range 8 */
            __mtcr(CPU_DPR8_L, lowerBoundAddress);      /* Set the lower bound of CPU Data Protection Range 8       */
            __mtcr(CPU_DPR8_U, upperBoundAddress);      /* Set the upper bound of CPU Data Protection Range 8       */
            break;
        case 9: /* Data Protection Range 9 */
            __mtcr(CPU_DPR9_L, lowerBoundAddress);      /* Set the lower bound of CPU Data Protection Range 9       */
            __mtcr(CPU_DPR9_U, upperBoundAddress);      /* Set the upper bound of CPU Data Protection Range 9       */
            break;
        case 10: /* Data Protection Range 10 */
            __mtcr(CPU_DPR10_L, lowerBoundAddress);     /* Set the lower bound of CPU Data Protection Range 10      */
            __mtcr(CPU_DPR10_U, upperBoundAddress);     /* Set the upper bound of CPU Data Protection Range 10      */
            break;
        case 11: /* Data Protection Range 11 */
            __mtcr(CPU_DPR11_L, lowerBoundAddress);     /* Set the lower bound of CPU Data Protection Range 11      */
            __mtcr(CPU_DPR11_U, upperBoundAddress);     /* Set the upper bound of CPU Data Protection Range 11      */
            break;
        case 12: /* Data Protection Range 12 */
            __mtcr(CPU_DPR12_L, lowerBoundAddress);     /* Set the lower bound of CPU Data Protection Range 12      */
            __mtcr(CPU_DPR12_U, upperBoundAddress);     /* Set the upper bound of CPU Data Protection Range 12      */
            break;
        case 13: /* Data Protection Range 13 */
            __mtcr(CPU_DPR13_L, lowerBoundAddress);     /* Set the lower bound of CPU Data Protection Range 13      */
            __mtcr(CPU_DPR13_U, upperBoundAddress);     /* Set the upper bound of CPU Data Protection Range 13      */
            break;
        case 14: /* Data Protection Range 14 */
            __mtcr(CPU_DPR14_L, lowerBoundAddress);     /* Set the lower bound of CPU Data Protection Range 14      */
            __mtcr(CPU_DPR14_U, upperBoundAddress);     /* Set the upper bound of CPU Data Protection Range 14      */
            break;
        case 15: /* Data Protection Range 15 */
            __mtcr(CPU_DPR15_L, lowerBoundAddress);     /* Set the lower bound of CPU Data Protection Range 15      */
            __mtcr(CPU_DPR15_U, upperBoundAddress);     /* Set the upper bound of CPU Data Protection Range 15      */
            break;
        case 16: /* Data Protection Range 15 */
            __mtcr(CPU_DPR16_L, lowerBoundAddress);     /* Set the lower bound of CPU Data Protection Range 16      */
            __mtcr(CPU_DPR16_U, upperBoundAddress);     /* Set the upper bound of CPU Data Protection Range 16      */
            break;
        case 17: /* Data Protection Range 15 */
            __mtcr(CPU_DPR17_L, lowerBoundAddress);     /* Set the lower bound of CPU Data Protection Range 17      */
            __mtcr(CPU_DPR17_U, upperBoundAddress);     /* Set the upper bound of CPU Data Protection Range 17      */
            break;
    }
    __isync();
}
/* Function that defines a code protection range in the corresponding CPU Code Protection Range Register (CPR).
 * Code protection ranges have 8-byte granularity.
 * As a result, the lower 3 bits of any address passed to the define_code_protection_range function will be discarded.
 * After enabling the Memory Protection, access to an address 'x' will be allowed only if:
 * lowerBoundAddress <= x < upperBoundAddress
 */
void McuSm_InitializeCodeProtectionRange(uint32 lowerBoundAddress, uint32 upperBoundAddress, uint8 range)
{
    switch(range)
    {
        case 0: /* Code Protection Range 0 */
            __mtcr(CPU_CPR0_L, lowerBoundAddress);      /* Set the lower bound of CPU Code Protection Range 0       */
            __mtcr(CPU_CPR0_U, upperBoundAddress);      /* Set the upper bound of CPU Code Protection Range 0       */
            break;
        case 1: /* Code Protection Range 1 */
            __mtcr(CPU_CPR1_L, lowerBoundAddress);      /* Set the lower bound of CPU Code Protection Range 1       */
            __mtcr(CPU_CPR1_U, upperBoundAddress);      /* Set the upper bound of CPU Code Protection Range 1       */
            break;
        case 2: /* Code Protection Range 2 */
            __mtcr(CPU_CPR2_L, lowerBoundAddress);      /* Set the lower bound of CPU Code Protection Range 2       */
            __mtcr(CPU_CPR2_U, upperBoundAddress);      /* Set the upper bound of CPU Code Protection Range 2       */
            break;
        case 3: /* Code Protection Range 3 */
            __mtcr(CPU_CPR3_L, lowerBoundAddress);      /* Set the lower bound of CPU Code Protection Range 3       */
            __mtcr(CPU_CPR3_U, upperBoundAddress);      /* Set the upper bound of CPU Code Protection Range 3       */
            break;
        case 4: /* Code Protection Range 4 */
            __mtcr(CPU_CPR4_L, lowerBoundAddress);      /* Set the lower bound of CPU Code Protection Range 4       */
            __mtcr(CPU_CPR4_U, upperBoundAddress);      /* Set the upper bound of CPU Code Protection Range 4       */
            break;
        case 5: /* Code Protection Range 5 */
            __mtcr(CPU_CPR5_L, lowerBoundAddress);      /* Set the lower bound of CPU Code Protection Range 5       */
            __mtcr(CPU_CPR5_U, upperBoundAddress);      /* Set the upper bound of CPU Code Protection Range 5       */
            break;
        case 6: /* Code Protection Range 6 */
            __mtcr(CPU_CPR6_L, lowerBoundAddress);      /* Set the lower bound of CPU Code Protection Range 6       */
            __mtcr(CPU_CPR6_U, upperBoundAddress);      /* Set the upper bound of CPU Code Protection Range 6       */
            break;
        case 7: /* Code Protection Range 7 */
            __mtcr(CPU_CPR7_L, lowerBoundAddress);      /* Set the lower bound of CPU Code Protection Range 7       */
            __mtcr(CPU_CPR7_U, upperBoundAddress);      /* Set the upper bound of CPU Code Protection Range 7       */
            break;
        case 8: /* Code Protection Range 7 */
            __mtcr(CPU_CPR8_L, lowerBoundAddress);      /* Set the lower bound of CPU Code Protection Range 8       */
            __mtcr(CPU_CPR8_U, upperBoundAddress);      /* Set the upper bound of CPU Code Protection Range 8       */
            break;
        case 9: /* Code Protection Range 7 */
            __mtcr(CPU_CPR9_L, lowerBoundAddress);      /* Set the lower bound of CPU Code Protection Range 9       */
            __mtcr(CPU_CPR9_U, upperBoundAddress);      /* Set the upper bound of CPU Code Protection Range 9       */
            break;
    }
    __isync();
}
/* Function to enable the data read access to a predefined Range in a Protection Set */
void McuSm_EnableDataRead(uint8 protectionSet, uint8 range)
{
    Ifx_CPU_DPRE DPRERegisterValue;
    /* Get the CPU Data Protection Read Enable Register value */
    switch(protectionSet)
    {
        case 0: /* Protection Set 0 */
            DPRERegisterValue.U = __mfcr(CPU_DPRE_0);
            break;
        case 1: /* Protection Set 1 */
            DPRERegisterValue.U = __mfcr(CPU_DPRE_1);
            break;
        case 2: /* Protection Set 2 */
            DPRERegisterValue.U = __mfcr(CPU_DPRE_2);
            break;
        case 3: /* Protection Set 3 */
            DPRERegisterValue.U = __mfcr(CPU_DPRE_3);
            break;
        case 4: /* Protection Set 4 */
            DPRERegisterValue.U = __mfcr(CPU_DPRE_4);
            break;
        case 5: /* Protection Set 5 */
            DPRERegisterValue.U = __mfcr(CPU_DPRE_5);
            break;

    }
    DPRERegisterValue.B.RE_N |= (1 << range); /* Set the bit corresponding to the given Data Protection Range */
    /* Set the CPU Data Protection Read Enable Register value to enable data read access */
    switch(protectionSet)
    {
        case 0: /* Protection Set 0 */
            __mtcr(CPU_DPRE_0, DPRERegisterValue.U);
            break;
        case 1: /* Protection Set 1 */
            __mtcr(CPU_DPRE_1, DPRERegisterValue.U);
            break;
        case 2: /* Protection Set 2 */
            __mtcr(CPU_DPRE_2, DPRERegisterValue.U);
            break;
        case 3: /* Protection Set 3 */
            __mtcr(CPU_DPRE_3, DPRERegisterValue.U);
            break;
        case 4: /* Protection Set 4 */
            __mtcr(CPU_DPRE_4, DPRERegisterValue.U);
            break;
        case 5: /* Protection Set 5 */
            __mtcr(CPU_DPRE_5, DPRERegisterValue.U);
            break;
    }
}
/* Function to enable the data write access to a predefined Range in a Protection Set */
void McuSm_EnableDataWrite(uint8 protectionSet, uint8 range)
{
    Ifx_CPU_DPWE DPWERegisterValue;
    /* Get the CPU Data Protection Write Enable Register value */
    switch(protectionSet)
    {
        case 0: /* Protection Set 0 */
            DPWERegisterValue.U = __mfcr(CPU_DPWE_0);
            break;
        case 1: /* Protection Set 1 */
            DPWERegisterValue.U = __mfcr(CPU_DPWE_1);
            break;
        case 2: /* Protection Set 2 */
            DPWERegisterValue.U = __mfcr(CPU_DPWE_2);
            break;
        case 3: /* Protection Set 3 */
            DPWERegisterValue.U = __mfcr(CPU_DPWE_3);
            break;
        case 4: /* Protection Set 4 */
            DPWERegisterValue.U = __mfcr(CPU_DPWE_4);
            break;
        case 5: /* Protection Set 5 */
            DPWERegisterValue.U = __mfcr(CPU_DPWE_5);
            break;
    }
    /* Set the bit corresponding to the given Data Protection Range */
    DPWERegisterValue.B.WE_N |= (0x1 << range);
    /* Set the CPU Data Protection Write Enable Register value to enable data write access */
    switch(protectionSet)
    {
        case 0: /* Protection Set 0 */
            __mtcr(CPU_DPWE_0, DPWERegisterValue.U);
            break;
        case 1: /* Protection Set 1 */
            __mtcr(CPU_DPWE_1, DPWERegisterValue.U);
            break;
        case 2: /* Protection Set 2 */
            __mtcr(CPU_DPWE_2, DPWERegisterValue.U);
            break;
        case 3: /* Protection Set 3 */
            __mtcr(CPU_DPWE_3, DPWERegisterValue.U);
            break;
        case 4: /* Protection Set 4 */
            __mtcr(CPU_DPWE_4, DPWERegisterValue.U);
            break;
        case 5: /* Protection Set 5 */
            __mtcr(CPU_DPWE_5, DPWERegisterValue.U);
            break;
    }
    __isync();
}
/* Function to enable code execution access to a predefined Range in a Protection Set */
void McuSm_EnableCodeExecution(uint8 protectionSet, uint8 range)
{
    Ifx_CPU_CPXE CPXERegisterValue;
    /* Get the CPU Code Protection Execute Enable Register value */
    switch(protectionSet)
    {
        case 0: /* Protection Set 0 */
            CPXERegisterValue.U = __mfcr(CPU_CPXE_0);
            break;
        case 1: /* Protection Set 1 */
            CPXERegisterValue.U = __mfcr(CPU_CPXE_1);
            break;
        case 2: /* Protection Set 2 */
            CPXERegisterValue.U = __mfcr(CPU_CPXE_2);
            break;
        case 3: /* Protection Set 3 */
            CPXERegisterValue.U = __mfcr(CPU_CPXE_3);
            break;
        case 4: /* Protection Set 4 */
            CPXERegisterValue.U = __mfcr(CPU_CPXE_4);
            break;
        case 5: /* Protection Set 5 */
            CPXERegisterValue.U = __mfcr(CPU_CPXE_5);
            break;
    }
    /* Set the bit corresponding to the given Code Protection Range */
    CPXERegisterValue.B.XE_N |= (0x1 << range);
    /* Set the CPU Code Protection Execute Enable Register value to enable coded execution */
    switch(protectionSet)
    {
        case 0: /* Protection Set 0 */
            __mtcr(CPU_CPXE_0, CPXERegisterValue.U);
            break;
        case 1: /* Protection Set 1 */
            __mtcr(CPU_CPXE_1, CPXERegisterValue.U);
            break;
        case 2: /* Protection Set 2 */
            __mtcr(CPU_CPXE_2, CPXERegisterValue.U);
            break;
        case 3: /* Protection Set 3 */
            __mtcr(CPU_CPXE_3, CPXERegisterValue.U);
            break;
        case 4: /* Protection Set 4 */
            __mtcr(CPU_CPXE_4, CPXERegisterValue.U);
            break;
        case 5: /* Protection Set 5 */
            __mtcr(CPU_CPXE_5, CPXERegisterValue.U);
            break;
    }
    __isync();
}
