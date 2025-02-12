#include "Irq.h"
#include "Dma.h"
#include "Can.h"
#include "Gtm_Atom.h"
#include "Crc.h"
#include "McuSm.h"
#include "Eru.h"
#include "Ain.h"
#include "IfxStm.h"

void ISR_CanAlrt(void)
{
    IfxCan_Node_clearInterruptFlag(g_mcmcan.canSrcNode.node, IfxCan_Interrupt_errorPassive);
    Can_Error_Flag++;
}

void ISR_CanMoer(void)
{
    IfxCan_Node_clearInterruptFlag(g_mcmcan.canSrcNode.node, IfxCan_Interrupt_errorPassive);
    Can_Error_Flag++;
}

void ISR_CanBoff(void)
{
    IfxCan_Node_clearInterruptFlag(g_mcmcan.canSrcNode.node, IfxCan_Interrupt_busOffStatus);
    Can_BusOff_Flag++;
}

void ISR_CanLoi(void)
{
    IfxCan_Node_clearInterruptFlag(g_mcmcan.canSrcNode.node, IfxCan_Interrupt_errorLoggingOverflow);
    Can_Error_Flag++;
}

void SCUERU_Int2_Handler(void)
{
    Eru_SpeedSen();
}

void SCUERU_Int0_Handler(void)
{
    Eru_EncoderA();
}

void SCUERU_Int1_Handler(void)
{
    Eru_EncoderB();
}

void ISR_DMA1_Transfer(void)
{
    IfxDma_clearChannelInterrupt(g_DMAchn.dma, IRQ_DMA_CHANNEL);/* Acknowledge DMA interrupt */
    IfxDma_Dma_startChannelTransaction(&g_DMAchn);
}
/* Interrupt Service Routine of the ATOM */
void ISR_Gtm_Atom_InterruptHandler(void)
{
    IfxGtm_Atom_Timer_acknowledgeTimerIrq(&g_timerDriver);/* Reset the timer event            */
    Ain_MainFunction();
}
/* Interrupt Service Routine (ISR) called once the TX interrupt has been generated. */
void ISR_CanTx(void)
{
    /* Clear the "Transmission Completed" interrupt flag */
    IfxCan_Node_clearInterruptFlag(g_mcmcan.canSrcNode.node, IfxCan_Interrupt_transmissionCompleted);
}
/* Interrupt Service Routine (ISR) called once the RX interrupt has been generated.
 * Compares the content of the received CAN message with the content of the transmitted CAN message. */
void ISR_CanRx(void)
{
    Can_Rx();
}
/* FCE ISR check length & CRC mismatch error, Bus & Configuration Error */
void ISR_FCE_CRC(void)
{
    Ifx_FCE_IN_STS interruptStatus;
    /* Get FCE CRC interrupt status */
    interruptStatus = IfxFce_Crc_getInterruptStatus(&g_fceCrc.fceCrc);
    /* Check if error flags are set */
    if(interruptStatus.U & FCE_CRC_ERROR_MASK != 0x0U)
    {
        /* Clear Error flags */
        IfxFce_Crc_clearErrorFlags(&g_fceCrc.fceCrc);
    }
}

void ISR_DTS(void)
{
    g_isMeasureAvailable = TRUE; /* Notify the system that a new measurement is ready */
}
