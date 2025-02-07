#include "Can.h"
#include "Irq.h"
#include "IfxCan.h"

typedef enum
{
    CAN_INIT = 0U,
    CAN_INIT_SUCCESFUL = 1U,
    CAN_PROCESS_RXTX = 2U,
    CAN_SLEEP = 3U,
    CAN_FIRST_TRANSMIT = 4U,
    CAN_ERROR = 5U,
    CAN_WAKEUP = 6U
}Can_Status_t;

McmcanType g_mcmcan; /* Global MCMCAN configuration and control structure    */
Can_RxMsg_t Can_RxMessageBuffer[50u];
Can_TxMsg_t Can_TxMessageBuffer[50u];
uint32 Can_BufferIndex_ReceivedMessages = 0u;
uint32 Can_BufferIndex_TransmittedMessages = 0u;
static uint32 Can_MainCounter = 0u;
uint8 Can_BusOff_Flag = 0u;
uint8 Can_Error_Flag = 0u;
Can_Status_t Can_State = CAN_INIT;

void Can_Init(void);
void Can_Tx(McmcanType message);
void Can_Rx(void);
void Can_Sleep(void);
void Can_TransmitAllMessages(void);
void Can_ProcessReceivedMessages(void);
void Can_TransmitScheduleTable(void);
void Can_MainFunction(void);
void Can_Wakeup(void);
/* Function to initialize MCMCAN module and nodes related for this application use case */
void Can_Init(void)
{
    Can_State = CAN_INIT;
    IfxCan_Can_initModuleConfig(&g_mcmcan.canConfig, &MODULE_CAN0);
    IfxCan_Can_initModule(&g_mcmcan.canModule, &g_mcmcan.canConfig);
    IfxCan_Can_initNodeConfig(&g_mcmcan.canNodeConfig, &g_mcmcan.canModule);
    g_mcmcan.canNodeConfig.nodeId = IfxCan_NodeId_0;
    g_mcmcan.canNodeConfig.clockSource = IfxCan_ClockSource_both;
    g_mcmcan.canNodeConfig.frame.type = IfxCan_FrameType_transmitAndReceive;
    g_mcmcan.canNodeConfig.frame.mode = IfxCan_FrameMode_standard;
    g_mcmcan.canNodeConfig.baudRate.baudrate = 500000u;
    g_mcmcan.canNodeConfig.baudRate.samplePoint = 8000u;
    g_mcmcan.canNodeConfig.baudRate.syncJumpWidth = 3u;
    g_mcmcan.canNodeConfig.baudRate.prescaler = 0u;
    g_mcmcan.canNodeConfig.baudRate.timeSegment1 = 3u;
    g_mcmcan.canNodeConfig.baudRate.timeSegment2 = 10u;
    g_mcmcan.canNodeConfig.txConfig.txMode = IfxCan_TxMode_dedicatedBuffers;
    g_mcmcan.canNodeConfig.txConfig.dedicatedTxBuffersNumber = 40u;
    g_mcmcan.canNodeConfig.txConfig.txFifoQueueSize = 0u;
    g_mcmcan.canNodeConfig.txConfig.txBufferDataFieldSize = IfxCan_DataFieldSize_8;
    g_mcmcan.canNodeConfig.txConfig.txEventFifoSize = 0u;
    g_mcmcan.canNodeConfig.rxConfig.rxMode                = IfxCan_RxMode_dedicatedBuffers;
    g_mcmcan.canNodeConfig.rxConfig.rxBufferDataFieldSize = IfxCan_DataFieldSize_8;
    g_mcmcan.canNodeConfig.rxConfig.rxFifo0DataFieldSize  = IfxCan_DataFieldSize_8;
    g_mcmcan.canNodeConfig.rxConfig.rxFifo1DataFieldSize  = IfxCan_DataFieldSize_8;
    g_mcmcan.canNodeConfig.rxConfig.rxFifo0OperatingMode  = IfxCan_RxFifoMode_blocking;
    g_mcmcan.canNodeConfig.rxConfig.rxFifo1OperatingMode  = IfxCan_RxFifoMode_blocking;
    g_mcmcan.canNodeConfig.rxConfig.rxFifo0WatermarkLevel = 0;
    g_mcmcan.canNodeConfig.rxConfig.rxFifo1WatermarkLevel = 0;
    g_mcmcan.canNodeConfig.rxConfig.rxFifo0Size           = 0;
    g_mcmcan.canNodeConfig.rxConfig.rxFifo1Size           = 0;
    g_mcmcan.canNodeConfig.messageRAM.baseAddress                    = (uint32)(g_mcmcan.canNodeConfig.can);
    g_mcmcan.canNodeConfig.messageRAM.standardFilterListStartAddress = 0u;
    g_mcmcan.canNodeConfig.messageRAM.extendedFilterListStartAddress = 0u;
    g_mcmcan.canNodeConfig.messageRAM.rxFifo0StartAddress            = 0u;
    g_mcmcan.canNodeConfig.messageRAM.rxFifo1StartAddress            = 0u;
    g_mcmcan.canNodeConfig.messageRAM.rxBuffersStartAddress          = 0x100u;
    g_mcmcan.canNodeConfig.messageRAM.txEventFifoStartAddress        = 0u;
    g_mcmcan.canNodeConfig.messageRAM.txBuffersStartAddress          = 0x420u;
    g_mcmcan.canNodeConfig.interruptConfig.messageStoredToDedicatedRxBufferEnabled = TRUE;
    g_mcmcan.canNodeConfig.interruptConfig.reint.priority = IRQ_CANRX_CHANNEL;
    g_mcmcan.canNodeConfig.interruptConfig.reint.interruptLine = IfxCan_InterruptLine_1;
    g_mcmcan.canNodeConfig.interruptConfig.reint.typeOfService = IfxSrc_Tos_cpu0;
    g_mcmcan.canNodeConfig.interruptConfig.transmissionCompletedEnabled = TRUE;
    g_mcmcan.canNodeConfig.interruptConfig.traco.priority = IRQ_CANTX_CHANNEL;
    g_mcmcan.canNodeConfig.interruptConfig.traco.interruptLine = IfxCan_InterruptLine_0;
    g_mcmcan.canNodeConfig.interruptConfig.traco.typeOfService = IfxSrc_Tos_cpu0;
    g_mcmcan.canNodeConfig.interruptConfig.messageRAMAccessFailureEnabled = TRUE;
    g_mcmcan.canNodeConfig.interruptConfig.timeoutOccurredEnabled = TRUE;
    g_mcmcan.canNodeConfig.interruptConfig.errorLoggingOverflowEnabled = TRUE;
    g_mcmcan.canNodeConfig.interruptConfig.errorPassiveEnabled = TRUE;
    g_mcmcan.canNodeConfig.interruptConfig.busOffStatusEnabled = TRUE;
    g_mcmcan.canNodeConfig.interruptConfig.alrt.interruptLine = IfxCan_InterruptLine_5;
    g_mcmcan.canNodeConfig.interruptConfig.alrt.typeOfService = IfxSrc_Tos_cpu0;
    g_mcmcan.canNodeConfig.interruptConfig.alrt.priority = ISR_PRIORITY_CAN_ALRT;
    g_mcmcan.canNodeConfig.interruptConfig.moer.interruptLine = IfxCan_InterruptLine_2;
    g_mcmcan.canNodeConfig.interruptConfig.moer.typeOfService = IfxSrc_Tos_cpu0;
    g_mcmcan.canNodeConfig.interruptConfig.moer.priority = ISR_PRIORITY_CAN_MOER;
    g_mcmcan.canNodeConfig.interruptConfig.boff.interruptLine = IfxCan_InterruptLine_3;
    g_mcmcan.canNodeConfig.interruptConfig.boff.typeOfService = IfxSrc_Tos_cpu0;
    g_mcmcan.canNodeConfig.interruptConfig.boff.priority = ISR_PRIORITY_CAN_BOFF;
    g_mcmcan.canNodeConfig.interruptConfig.loi.interruptLine = IfxCan_InterruptLine_4;
    g_mcmcan.canNodeConfig.interruptConfig.loi.typeOfService = IfxSrc_Tos_cpu0;
    g_mcmcan.canNodeConfig.interruptConfig.loi.priority = ISR_PRIORITY_CAN_LOI;
    IfxCan_Can_initNode(&g_mcmcan.canSrcNode, &g_mcmcan.canNodeConfig);
    g_mcmcan.canFilter.number = 0;
    g_mcmcan.canFilter.elementConfiguration = IfxCan_FilterElementConfiguration_storeInRxBuffer;
    g_mcmcan.canFilter.id1 = 0;
    g_mcmcan.canFilter.rxBufferOffset = IfxCan_RxBufferId_0;
    IfxCan_Can_setStandardFilter(&g_mcmcan.canDstNode, &g_mcmcan.canFilter);
    IfxPort_setPinLow(&MODULE_P20, 6);
    Can_State = CAN_INIT_SUCCESFUL;
}
/* Function to initialize both TX and RX messages with the default data values.
 * After initialization of the messages, the TX message is transmitted.
 */
void Can_Tx(McmcanType message)
{
    /* Send the CAN message with the previously defined TX message content */
    IfxCan_Can_sendMessage(&message.canSrcNode, &message.txMsg, message.txData);
    if(0u != Can_BufferIndex_TransmittedMessages)
    {
        IfxCan_Can_initMessage(&Can_TxMessageBuffer[Can_BufferIndex_TransmittedMessages].txMsg);
        memset(Can_TxMessageBuffer[Can_BufferIndex_TransmittedMessages].txData, 0u, sizeof(Can_TxMessageBuffer[Can_BufferIndex_TransmittedMessages].txData));
        Can_BufferIndex_TransmittedMessages--;
    }
    else
    {
        /* Do nothing. */
    }
}

void Can_Rx(void)
{
    /* Clear the "Message stored to Dedicated RX Buffer" interrupt flag */
    IfxCan_Node_clearInterruptFlag(g_mcmcan.canDstNode.node, IfxCan_Interrupt_messageStoredToDedicatedRxBuffer);
    /* Read the received CAN message */
    IfxCan_Can_readMessage(&g_mcmcan.canDstNode, &g_mcmcan.rxMsg, g_mcmcan.rxData);

    if(0u != g_mcmcan.rxMsg.messageId)
    {
        Can_RxMessageBuffer[Can_BufferIndex_ReceivedMessages].rxMsg = g_mcmcan.rxMsg;
        memcpy(Can_RxMessageBuffer[Can_BufferIndex_ReceivedMessages].rxData, g_mcmcan.rxData, sizeof(g_mcmcan.rxData));
        Can_BufferIndex_ReceivedMessages++;
    }
    else
    {
        /* Do nothing. */
    }
}

void Can_TransmitAllMessages(void)
{

}

void Can_ProcessReceivedMessages(void)
{

}

void Can_TransmitScheduleTable(void)
{

}

void Can_Sleep(void)
{
    IfxCan_disableModule(g_mcmcan.canModule.can);
    IfxCan_setSleepMode(g_mcmcan.canModule.can, TRUE);
}

void Can_Wakeup(void)
{
    IfxCan_setSleepMode(g_mcmcan.canModule.can, FALSE);
    Can_Init();
}

void Can_MainFunction(void)
{
    static uint32 timestamp = 0u;
    static uint8 localErrorFlag = 0u;

    if((0u != Can_BusOff_Flag) ||
            (0u != Can_Error_Flag))
    {
        timestamp = Can_MainCounter;
    }
    else
    {
        timestamp = 0u;
        localErrorFlag = 0u;
    }

    if((12000u < Can_MainCounter - timestamp) ||
            (6000u < Can_BusOff_Flag) ||
            (6000u < Can_Error_Flag))
    {
        localErrorFlag = 1u;
        timestamp = 0u;
        Can_BusOff_Flag = 0u;
        Can_Error_Flag = 0u;
    }
    else
    {
        /* Do nothing. */
    }

    if(0u == localErrorFlag)
    {
        switch(Can_State)
        {
            case CAN_INIT:
                Can_Init();
                break;
            case CAN_INIT_SUCCESFUL:
                Can_State = CAN_FIRST_TRANSMIT;
                break;
            case CAN_FIRST_TRANSMIT:
                Can_TransmitAllMessages();
                Can_State = CAN_PROCESS_RXTX;
                break;
            case CAN_PROCESS_RXTX:
                Can_ProcessReceivedMessages();
                Can_TransmitScheduleTable();
                break;
            case CAN_SLEEP:
                Can_Sleep();
                break;
            case CAN_WAKEUP:
                Can_Wakeup();
                break;
            default:
                break;
        }
    }
    else
    {
        Can_State = CAN_ERROR;
    }

    Can_MainCounter++;
}
