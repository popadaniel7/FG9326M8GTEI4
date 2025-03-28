#include "McuSm.h"
#include "IfxScuLbist.h"
#include "Irq.h"
#include "IfxCpu_IntrinsicsTasking.h"
#include "IfxCpu_reg.h"
#include "Iven.h"

uint32 McuSm_AGs[12u];
uint32 McuSm_LastResetReason;
uint32 McuSm_LastResetInformation;
uint32 McuSm_IndexResetHistory;
McuSm_ResetHistory_t McuSm_ResetHistory[20u];
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
        McuSm_LastResetReason = resetReason;
        McuSm_LastResetInformation = resetInformation;
        McuSm_ResetHistory[McuSm_IndexResetHistory].reason = resetReason;
        McuSm_ResetHistory[McuSm_IndexResetHistory].information = resetInformation;
        McuSm_IndexResetHistory++;
        DiagMaster_AliveTime = 0u;

        if(19u < McuSm_IndexResetHistory)
        {
            McuSm_IndexResetHistory = 0u;
        }
        else
        {
            /* Do nothing. */
        }

        IfxScuRcu_performReset(IfxScuRcu_ResetType_application, 0u);
    }
    else
    {
        /* Do nothing. */
    }
}

void McuSm_TRAP1(IfxCpu_Trap trapInfo)
{
    McuSm_PerformResetHook(371u, trapInfo.tId);
}

void McuSm_TRAP2(IfxCpu_Trap trapInfo)
{
    McuSm_PerformResetHook(372u, trapInfo.tId);
}

void McuSm_TRAP4(IfxCpu_Trap trapInfo)
{
    McuSm_PerformResetHook(374u, trapInfo.tId);
}

void McuSm_TRAP7(IfxCpu_Trap trapInfo)
{
    uint32 const volatile* ag;
    uint32 agRstRsn = 0u;
    uint32 agRstInfo = 0u;

    McuSm_AGs[0u] = SMU_AGCF0_0.U & SMU_AGCF0_2.U & ~(SMU_AGCF0_1.U);
    McuSm_AGs[1u] = SMU_AGCF1_0.U & SMU_AGCF1_2.U & ~(SMU_AGCF1_1.U);
    McuSm_AGs[2u] = SMU_AGCF2_0.U & SMU_AGCF2_2.U & ~(SMU_AGCF2_1.U);
    McuSm_AGs[6u] = SMU_AGCF6_0.U & SMU_AGCF6_2.U & ~(SMU_AGCF6_1.U);
    McuSm_AGs[7u] = SMU_AGCF7_0.U & SMU_AGCF7_2.U & ~(SMU_AGCF7_1.U);
    McuSm_AGs[8u] = SMU_AGCF8_0.U & SMU_AGCF8_2.U & ~(SMU_AGCF8_1.U);
    McuSm_AGs[9u] = SMU_AGCF9_0.U & SMU_AGCF9_2.U & ~(SMU_AGCF9_1.U);
    McuSm_AGs[10u] = SMU_AGCF10_0.U & SMU_AGCF10_2.U & ~(SMU_AGCF10_1.U);
    McuSm_AGs[11u] = SMU_AGCF11_0.U & SMU_AGCF11_2.U & ~(SMU_AGCF11_1.U);

    ag = (uint32 const volatile*)(&SMU_AG0);

    for(sint8 i = 0u; i < 12u; i++)
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

    __debug();
    
  while(1)
  {
    __debug();
  }

    McuSm_LastResetReason = 373u;
    McuSm_LastResetInformation = trapInfo.tId;
    McuSm_ResetHistory[McuSm_IndexResetHistory].reason = 373u;
    McuSm_ResetHistory[McuSm_IndexResetHistory].information = trapInfo.tId;
    McuSm_IndexResetHistory++;

    if(McuSm_IndexResetHistory > 19u)
    {
        McuSm_IndexResetHistory = 0u;
    }
    else
    {
        /* Do nothing. */
    }

    /* Read Password from Safety WDT CON0 register
     * !!! NOTE: !!! when read bottom six bit of password are inverted so we have
     * to toggle them before returning password */
    password  = watchdog->CON0.B.PW;
    password ^= 0x003Fu;

    if (SCU_WDTS_CON0.B.LCK)
    {
        /* see Table 1 (Password Access Bit Pattern Requirements) */
        SCU_WDTS_CON0.U = (1u << IFX_SCU_WDTS_CON0_ENDINIT_OFF) |
                (0u << IFX_SCU_WDTS_CON0_LCK_OFF) |
                (password << IFX_SCU_WDTS_CON0_PW_OFF) |
                (SCU_WDTS_CON0.B.REL << IFX_SCU_WDTS_CON0_REL_OFF);
    }
    else
    {
        /* Do nothing. */
    }

    /* Clear ENDINT and set LCK bit in Config_0 register */
    SCU_WDTS_CON0.U = (0u << IFX_SCU_WDTS_CON0_ENDINIT_OFF) |
            (1u << IFX_SCU_WDTS_CON0_LCK_OFF) |
            (password << IFX_SCU_WDTS_CON0_PW_OFF) |
            (SCU_WDTS_CON0.B.REL << IFX_SCU_WDTS_CON0_REL_OFF);
    /* read back ENDINIT and wait until it has been cleared */

    while (SCU_WDTS_CON0.B.ENDINIT == 1u)
    {}

    MODULE_SCU.RSTCON.B.SW = 2u; /* Application Reset */

    /* Read Password from CON0 register
     * !!! NOTE: !!! when read bottom six bit of password are inverted so we have
     * to toggle them before returning password */

    password  = cpuwdg->CON0.B.PW;
    password ^= 0x003Fu;

    if (cpuwdg->CON0.B.LCK)
    {
        /* see Table 1 (Password Access Bit Pattern Requirements) */
        cpuwdg->CON0.U = (1u << IFX_SCU_WDTCPU_CON0_ENDINIT_OFF) |
                (0u << IFX_SCU_WDTCPU_CON0_LCK_OFF) |
                (password << IFX_SCU_WDTCPU_CON0_PW_OFF) |
                (cpuwdg->CON0.B.REL << IFX_SCU_WDTCPU_CON0_REL_OFF);
    }
    else
    {
        /* Do nothing. */
    }

    /* Clear ENDINT and set LCK bit in Config_0 register */
    cpuwdg->CON0.U = (0u << IFX_SCU_WDTCPU_CON0_ENDINIT_OFF) |
            (1u << IFX_SCU_WDTCPU_CON0_LCK_OFF) |
            (password << IFX_SCU_WDTCPU_CON0_PW_OFF) |
            (cpuwdg->CON0.B.REL << IFX_SCU_WDTCPU_CON0_REL_OFF);
    /* read back ENDINIT and wait until it has been cleared */
    while (cpuwdg->CON0.B.ENDINIT == 1u)
    {}

    if (cpuwdg->CON0.B.LCK)
    {
        /* see Table 1 (Password Access Bit Pattern Requirements) */
        cpuwdg->CON0.U = (1u << IFX_SCU_WDTCPU_CON0_ENDINIT_OFF) |
                           (0u << IFX_SCU_WDTCPU_CON0_LCK_OFF) |
                           (password << IFX_SCU_WDTCPU_CON0_PW_OFF) |
                           (cpuwdg->CON0.B.REL << IFX_SCU_WDTCPU_CON0_REL_OFF);
    }

    /* Clear ENDINT and set LCK bit in Config_0 register */
    cpuwdg->CON0.U = (0u << IFX_SCU_WDTCPU_CON0_ENDINIT_OFF) |
                       (1u << IFX_SCU_WDTCPU_CON0_LCK_OFF) |
                       (password << IFX_SCU_WDTCPU_CON0_PW_OFF) |
                       (cpuwdg->CON0.B.REL << IFX_SCU_WDTCPU_CON0_REL_OFF);

    /* read back ENDINIT and wait until it has been cleared */
    while (cpuwdg->CON0.B.ENDINIT == 1u)
    {}

    /* Write the user DATA to reset evaluation */
    MODULE_SCU.RSTCON2.B.USRINFO = 34u;
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
    CPU0_SPR_SPROT_RGNUA0.U = 0x50017FE0U;
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
    CPU2_SPR_SPROT_RGNUA0.U = 0x7003BFE0U;
    CPU2_SPR_SPROT_RGNACCENA0_W.U = 0x10001777U;
    CPU2_SPR_SPROT_RGNACCENB0_W.U = 0x00000000U;
    CPU2_SPR_SPROT_RGNACCENA0_R.U = 0xFFFFFFFFU;
    CPU2_SPR_SPROT_RGNACCENB0_R.U = 0xFFFFFFFFU;
    CPU0_DLMU_SPROT_RGNLA0.U = 0xB0000000U;
    CPU0_DLMU_SPROT_RGNLA0.U = 0xB000FFE0U;
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
