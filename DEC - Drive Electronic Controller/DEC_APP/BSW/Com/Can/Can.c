#include "Can.h"
#include "Irq.h"
#include "IfxCan.h"
#include "ComMaster.h"

McmcanType g_mcmcan;
Can_RxMsg_t Can_RxMessageBuffer[50u];
uint32 Can_BufferIndex_ReceivedMessages = 0u;
static uint32 Can_MainCounter = 0u;
uint8 Can_BusOff_Flag = 0u;
uint8 Can_Error_Flag = 0u;
Can_Status_t Can_State = CAN_INIT;
Can_TransmitType_t Can_TransmitTable[7u];
Can_ReceiveType_t Can_ReceiveTable[9u];
uint32 Can_Rx_DiagBufCnt = 0u;
Can_RxMsg_t Can_Rx_DiagnosticBuffer[50u];
IsoTpChannel g_isoTpRx;
IsoTpChannel g_isoTpTx;
uint8 g_rxDataBuffer[4096u] = {0u};
uint8 Can_ActivityOnTheBus = 0u;

void Can_Init(void);
bool Can_Tx(McmcanType message);
void Can_Rx(void);
void Can_Sleep(void);
void Can_TransmitAllMessages(void);
void Can_ProcessReceivedMessages(void);
void Can_TransmitScheduleTable(void);
bool Can_IsoTp_SendFrame(uint16 canId, const uint8 *data, uint8 size);
void Can_IsoTp_Init(void);
bool Can_IsoTp_Transmit(uint16 canId, const uint8 *data, uint16 size);
void Can_IsoTp_OnCanReceive(uint16 canId, const uint8 *data, uint8 size);
void Can_IsoTp_MainFunction(void);
void Can_MainFunction(void);
/* Function to initialize MCMCAN module and nodes related for this application use case */
void Can_Init(void)
{
    IfxScuWdt_clearCpuEndinit(IfxScuWdt_getCpuWatchdogPassword());
    IfxScuWdt_clearSafetyEndinit(IfxScuWdt_getSafetyWatchdogPassword());
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
    IfxPort_setPinLow(&MODULE_P20, 6);
    Can_State = CAN_INIT_SUCCESFUL;
    IfxScuWdt_setCpuEndinit(IfxScuWdt_getCpuWatchdogPassword());
    IfxScuWdt_setSafetyEndinit(IfxScuWdt_getSafetyWatchdogPassword());
}
/* Function to initialize both TX and RX messages with the default data values.
 * After initialization of the messages, the TX message is transmitted.
 */
bool Can_Tx(McmcanType message)
{
    return IfxCan_Can_sendMessage(&message.canSrcNode, &message.txMsg, message.txData);
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
        Can_ActivityOnTheBus = 1u;
    }
    else
    {
        /* Do nothing. */
    }
}

void Can_TransmitAllMessages(void)
{
    IfxCpu_disableInterrupts();
    memcpy(Can_TransmitTable, ComMaster_TransmitTable, sizeof(Can_TransmitType_t));

    IfxCpu_enableInterrupts();
    for(uint8 i = 0; i < 9u; i++)
    {
        if(1u == Can_TransmitTable[i].transmitFlag)
        {
            IfxCpu_disableInterrupts();
            g_mcmcan.txMsg = Can_TransmitTable[i].transmitMessage.txMsg;
            memcpy(g_mcmcan.txData, Can_TransmitTable[i].transmitMessage.txData, sizeof(Can_TransmitTable[i].transmitMessage.txData));
            IfxCpu_enableInterrupts();
            Can_Tx(g_mcmcan);
            Can_TransmitTable[i].transmitFlag = 0u;
        }
        else
        {
            /* Do nothing. */
        }
    }
}

void Can_ProcessReceivedMessages(void)
{
    IfxCpu_disableInterrupts();
    while(Can_BufferIndex_ReceivedMessages != 0u)
    {
        switch(Can_RxMessageBuffer[Can_BufferIndex_ReceivedMessages].rxMsg.messageId)
        {
            case 0x100u:
                Can_ReceiveTable[0].receiveMessage.rxMsg = Can_RxMessageBuffer[Can_BufferIndex_ReceivedMessages].rxMsg;
                break;
            case 0x101u:
                Can_ReceiveTable[1].receiveMessage.rxMsg = Can_RxMessageBuffer[Can_BufferIndex_ReceivedMessages].rxMsg;
                break;
            case 0x103u:
                Can_ReceiveTable[2].receiveMessage.rxMsg = Can_RxMessageBuffer[Can_BufferIndex_ReceivedMessages].rxMsg;
                break;
            case 0x104u:
                Can_ReceiveTable[3].receiveMessage.rxMsg = Can_RxMessageBuffer[Can_BufferIndex_ReceivedMessages].rxMsg;
                break;
            case 0x107u:
                Can_ReceiveTable[4].receiveMessage.rxMsg = Can_RxMessageBuffer[Can_BufferIndex_ReceivedMessages].rxMsg;
                break;
            case 0x108u:
                Can_ReceiveTable[5].receiveMessage.rxMsg = Can_RxMessageBuffer[Can_BufferIndex_ReceivedMessages].rxMsg;
                break;
            case 0x10Au:
                Can_ReceiveTable[6].receiveMessage.rxMsg = Can_RxMessageBuffer[Can_BufferIndex_ReceivedMessages].rxMsg;
                break;
            case 0x10Cu:
                Can_ReceiveTable[7].receiveMessage.rxMsg = Can_RxMessageBuffer[Can_BufferIndex_ReceivedMessages].rxMsg;
                break;
            case 0x10Du:
                Can_ReceiveTable[8].receiveMessage.rxMsg = Can_RxMessageBuffer[Can_BufferIndex_ReceivedMessages].rxMsg;
                break;
            case 0x701u:
                Can_Rx_DiagnosticBuffer[Can_Rx_DiagBufCnt].rxMsg = Can_RxMessageBuffer[Can_BufferIndex_ReceivedMessages].rxMsg;
                Can_Rx_DiagBufCnt++;
                break;
            case 0x703u:
                Can_Rx_DiagnosticBuffer[Can_Rx_DiagBufCnt].rxMsg = Can_RxMessageBuffer[Can_BufferIndex_ReceivedMessages].rxMsg;
                Can_Rx_DiagBufCnt++;
                break;
            case 0x705u:
                Can_Rx_DiagnosticBuffer[Can_Rx_DiagBufCnt].rxMsg = Can_RxMessageBuffer[Can_BufferIndex_ReceivedMessages].rxMsg;
                Can_Rx_DiagBufCnt++;
                break;
            case 0x6FEu:
                Can_Rx_DiagnosticBuffer[Can_Rx_DiagBufCnt].rxMsg = Can_RxMessageBuffer[Can_BufferIndex_ReceivedMessages].rxMsg;
                Can_Rx_DiagBufCnt++;
                break;
            default:
                break;
        }
        memset(&Can_RxMessageBuffer[Can_BufferIndex_ReceivedMessages], 0u, sizeof(Can_RxMessageBuffer[Can_BufferIndex_ReceivedMessages]));
        Can_BufferIndex_ReceivedMessages--;
    }
    IfxCpu_enableInterrupts();
}

void Can_TransmitScheduleTable(void)
{
    IfxCpu_disableInterrupts();
    memcpy(Can_TransmitTable, ComMaster_TransmitTable, sizeof(Can_TransmitType_t));

    for(uint8 i = 0; i < 9u; i++)
    {
        if(1u == Can_TransmitTable[i].transmitFlag)
        {
            g_mcmcan.txMsg = Can_TransmitTable[i].transmitMessage.txMsg;
            memcpy(g_mcmcan.txData, Can_TransmitTable[i].transmitMessage.txData, sizeof(Can_TransmitTable[i].transmitMessage.txData));
            Can_TransmitTable[i].transmitFlag = 0u;
            IfxCpu_enableInterrupts();
            Can_Tx(g_mcmcan);
        }
        else
        {
            /* Do nothing. */
        }
    }
}

void Can_Sleep(void)
{
    IfxCan_disableModule(g_mcmcan.canModule.can);
    IfxCan_setSleepMode(g_mcmcan.canModule.can, TRUE);
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

bool Can_IsoTp_SendFrame(uint16 canId, const uint8 *data, uint8 size)
{
    McmcanType localMcmcan;
    bool status = false;

    IfxCpu_disableInterrupts();

    memcpy(&localMcmcan, &g_mcmcan, sizeof(g_mcmcan));

    IfxCan_Can_initMessage(&localMcmcan.txMsg);

    localMcmcan.txMsg.messageId = canId;
    localMcmcan.txMsg.dataLengthCode = size;

    for(uint8 i = 0; i < size; i++)
    {
        localMcmcan.txData[i] = data[i];
    }

    IfxCpu_enableInterrupts();

    status = Can_Tx(localMcmcan);

    return status;
}

void Can_IsoTp_Init(void)
{
    memset(&g_isoTpRx, 0, sizeof(g_isoTpRx));
    memset(&g_isoTpTx, 0, sizeof(g_isoTpTx));
    g_isoTpRx.state         = ISOTP_STATE_IDLE;
    g_isoTpRx.canId         = 0x6FE;
    g_isoTpRx.rxBufferPtr   = g_rxDataBuffer;
    g_isoTpRx.rxBufferSize  = sizeof(g_rxDataBuffer);
    g_isoTpTx.state         = ISOTP_STATE_IDLE;
    g_isoTpTx.canId         = 0x6FF;
    g_isoTpTx.blockSize     = ISOTP_DEFAULT_BLOCK_SIZE;
    g_isoTpTx.stMin         = ISOTP_DEFAULT_ST_MIN_MS;
}

bool Can_IsoTp_Transmit(uint16 canId, const uint8 *data, uint16 size)
{
    if (g_isoTpTx.state != ISOTP_STATE_IDLE)
    {
        // Tx channel busy
        return false;
    }
    else
    {
        /* Do nothing. */
    }
    // Initialize Tx channel
    g_isoTpTx.state         = ISOTP_STATE_TX_FF; // We'll decide SF vs FF below
    g_isoTpTx.canId         = canId;
    g_isoTpTx.txDataPtr     = data;
    g_isoTpTx.txDataSize    = size;
    g_isoTpTx.txDataOffset  = 0;
    g_isoTpTx.txSequenceNumber = 1; // next CF SN
    g_isoTpTx.blockCounter  = 0;
    g_isoTpTx.timer         = 1; // Not defined above, you can define your own
    // Decide if Single Frame or Multi-Frame
    // Single Frame can carry up to 7 bytes (PCI nibble + up to 7 data bytes)
    if (size <= 7)
    {
        uint8 canPayload[ISOTP_CAN_DL] = {0};
        // PCI nibble: 0x0 | (size)
        canPayload[0] = (uint8_t)(ISOTP_PCI_SF | (uint8_t)size);
        // Copy pay-load
        memcpy(&canPayload[1], data, size);
        // Send single CAN frame
        if (!Can_IsoTp_SendFrame((uint16)canId, canPayload, 1 + (uint8_t)size))
        {
            g_isoTpTx.state = ISOTP_STATE_IDLE;
            return false;
        }
        else
        {
            /* Do nothing. */
        }
        // Done
        g_isoTpTx.state = ISOTP_STATE_IDLE;  // Transmission completed
    }
    else
    {
        // Multi-frame
        // First frame has 2 bytes of length info:
        // Byte0: PCI nibble 0x1 for FF, plus the high nibble of total length
        // Byte1: lower 8 bits of total length
        // Then up to 6 data bytes
        uint8 canPayload[ISOTP_CAN_DL] = {0};
        canPayload[0] = (uint8_t)(ISOTP_PCI_FF | ((size >> 8) & 0x0F));
        canPayload[1] = (uint8_t)(size & 0xFF);
        // We can fit up to 6 bytes of data in the first frame
        uint8 bytesInFF = 6U;
        memcpy(&canPayload[2], data, bytesInFF);
        // Transmit FF
        if (!Can_IsoTp_SendFrame((uint16)canId, canPayload, 8))
        {
            g_isoTpTx.state = ISOTP_STATE_IDLE;
            return false;
        }
        else
        {
            /* Do nothing. */
        }
        // Move offset
        g_isoTpTx.txDataOffset = bytesInFF;
        // Remain in ISOTP_STATE_TX_FF until we get FlowControl from the receiver
    }
    return true;
}

void Can_IsoTp_OnCanReceive(uint16 canId, const uint8 *data, uint8 size)
{
    if (size < 1)
    {
        return; // invalid
    }
    else
    {
        /* Do nothing. */
    }
    uint8 pci = data[0] & 0xF0; // top nibble = PCI type
    // --------------------------------------------------------
    // 1. Check if this frame is for our Rx channel
    // --------------------------------------------------------
    if (canId == g_isoTpRx.canId)
    {
        switch (pci)
        {
            // -----------------------------------------
            // Single Frame
            // -----------------------------------------
            case ISOTP_PCI_SF:
            {
                uint8 len = data[0] & 0x0F; // low nibble = data length
                if (len <= 7 && len <= (size - 1))
                {
                    // Copy the payload
                    memcpy(g_isoTpRx.rxBufferPtr, &data[1], len);
                    g_isoTpRx.payloadLength  = len;
                    g_isoTpRx.receivedBytes  = len;
                    g_isoTpRx.state          = ISOTP_STATE_IDLE; // Rx complete
                }
                else
                {
                    /* Do nothing. */
                }
                break;
            }
            // -----------------------------------------
            // First Frame
            // -----------------------------------------
            case ISOTP_PCI_FF:
            {
                // length is 12 bits across (data[0] & 0x0F) << 8 | data[1]
                uint16 totalLen = ((data[0] & 0x0F) << 8) | data[1];
                if (totalLen > g_isoTpRx.rxBufferSize)
                {
                    // Buffer overflow, discard or handle error
                    g_isoTpRx.state = ISOTP_STATE_IDLE;
                    break;
                }
                else
                {
                    /* Do nothing. */
                }
                // Copy up to 6 data bytes from FF
                uint8 bytesInFF = 6;
                if (bytesInFF > (size - 2))
                {
                    break;
                }
                else
                {
                    /* Do nothing. */
                }

                memcpy(g_isoTpRx.rxBufferPtr, &data[2], bytesInFF);
                g_isoTpRx.payloadLength   = totalLen;
                g_isoTpRx.receivedBytes   = bytesInFF;
                g_isoTpRx.state           = ISOTP_STATE_RX_FF;
                g_isoTpRx.nextCfSequenceNumber = 1; // next CF must have SN=1
                // We must send Flow Control (FC)
                uint8 fcFrame[ISOTP_CAN_DL] = {0};
                fcFrame[0] = (uint8_t)(ISOTP_PCI_FC | ISOTP_FC_STATUS_CTS);
                fcFrame[1] = g_isoTpTx.blockSize;
                fcFrame[2] = g_isoTpTx.stMin;
                // The rest can be 0
                Can_IsoTp_SendFrame((uint16)canId, fcFrame, 3);
                break;
            }
            // -----------------------------------------
            // Consecutive Frame
            // -----------------------------------------
            case ISOTP_PCI_CF:
            {
                if ((g_isoTpRx.state == ISOTP_STATE_RX_FF) ||
                        (g_isoTpRx.state == ISOTP_STATE_RX_CF))
                {
                    uint8 sn = data[0] & 0x0F;
                    if (sn == g_isoTpRx.nextCfSequenceNumber)
                    {
                        // OK, copy data
                        uint8 bytesToCopy = (uint8_t)(size - 1);
                        uint16 remaining = g_isoTpRx.payloadLength - g_isoTpRx.receivedBytes;
                        if (bytesToCopy > remaining)
                        {
                            bytesToCopy = (uint8_t)remaining; // avoid overflow
                        }
                        memcpy(&g_isoTpRx.rxBufferPtr[g_isoTpRx.receivedBytes], &data[1], bytesToCopy);
                        g_isoTpRx.receivedBytes += bytesToCopy;
                        g_isoTpRx.nextCfSequenceNumber = (sn + 1) & 0x0F;
                        g_isoTpRx.state = ISOTP_STATE_RX_CF;
                        // Check if we are done
                        if (g_isoTpRx.receivedBytes >= g_isoTpRx.payloadLength)
                        {
                            // Rx complete
                            g_isoTpRx.state = ISOTP_STATE_IDLE;
                            // Notify upper layer
                            // e.g. Dcm_OnIsoTpRxComplete(...)
                        }
                        else
                        {
                            /* Do nothing. */
                        }
                        // else, we might need to handle block size / flow control timing
                        // This example always returns CTS once at FF,
                        // a real stack might do extended block checks, further FC, etc.
                    }
                    else
                    {
                        // Sequence mismatch -> error
                        g_isoTpRx.state = ISOTP_STATE_IDLE;
                    }
                }
                break;
            }
            // -----------------------------------------
            // Flow Control - Not expected for Rx
            // -----------------------------------------
            case ISOTP_PCI_FC:
            default:
                // ignore or handle error
                break;
        }
    }
    // --------------------------------------------------------
    // 2. Check if this frame is FlowControl for our Tx channel
    // --------------------------------------------------------
    else if (canId == g_isoTpTx.canId)
    {
        if ((pci & 0xF0) == ISOTP_PCI_FC)
        {
            // This is a FlowControl frame from the receiver, relevant only if we are in TX_FF
            if (g_isoTpTx.state == ISOTP_STATE_TX_FF)
            {
                uint8 fs = data[0] & 0x0F; // FlowStatus
                uint8 blockSize = data[1];
                uint8 stMin     = data[2];

                if (fs == ISOTP_FC_STATUS_CTS)
                {
                    // Accept new block size, stMin
                    g_isoTpTx.blockSize = blockSize;
                    g_isoTpTx.stMin     = stMin; // (in ms)
                    g_isoTpTx.blockCounter = 0;

                    // Start sending Consecutive Frames
                    g_isoTpTx.state = ISOTP_STATE_TX_CF;
                }
                else if (fs == ISOTP_FC_STATUS_WAIT)
                {
                    // The receiver requests a pause
                    // Re-send after some time or wait for next FC
                    // This demo does not handle WAIT properly
                }
                else
                {
                    // Overflow or other negative flow status
                    // Abort
                    g_isoTpTx.state = ISOTP_STATE_IDLE;
                }
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
    }
    else
    {
        /* Do nothing. */
    }
}

void Can_IsoTp_MainFunction(void)
{
    // -----------------------------
    // RX Timeout Management
    // -----------------------------
    switch (g_isoTpRx.state)
    {
        case ISOTP_STATE_RX_FF:
        case ISOTP_STATE_RX_CF:
            if (g_isoTpRx.timer > 0)
            {
                g_isoTpRx.timer--;
                if (g_isoTpRx.timer == 0)
                {
                    // Rx timeout
                    g_isoTpRx.state = ISOTP_STATE_IDLE;
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
            break;
        default:
            // Idle or no action
            break;
    }
    // -----------------------------
    // TX Timeout Management & CF Sending
    // -----------------------------
    switch (g_isoTpTx.state)
    {
        case ISOTP_STATE_TX_FF:
            // We are waiting for FlowControl
            if (g_isoTpTx.timer > 0)
            {
                g_isoTpTx.timer--;
                if (g_isoTpTx.timer == 0)
                {
                    // Timeout waiting for FC
                    g_isoTpTx.state = ISOTP_STATE_IDLE;
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
            break;
        case ISOTP_STATE_TX_CF:
        {
            // Check if we can send next CF
            // Enforce blockSize and stMin if needed
            // For demonstration: send frames continuously, ignoring STmin
            // Real code: you'd use a sub-timer to track STmin, block usage, etc.
            if (g_isoTpTx.txDataOffset < g_isoTpTx.txDataSize)
            {
                // Build the CF
                uint8 cfPayload[ISOTP_CAN_DL] = {0};
                cfPayload[0] = (uint8_t)(ISOTP_PCI_CF | (g_isoTpTx.txSequenceNumber & 0x0F));
                uint8 bytesToSend = (uint8_t)(g_isoTpTx.txDataSize - g_isoTpTx.txDataOffset);
                if (bytesToSend > 7)
                {
                    bytesToSend = 7;
                }
                else
                {
                    /* Do nothing. */
                }

                memcpy(&cfPayload[1], &g_isoTpTx.txDataPtr[g_isoTpTx.txDataOffset], bytesToSend);

                if (!Can_IsoTp_SendFrame((uint16)g_isoTpTx.canId, cfPayload, 1 + bytesToSend))
                {
                    // Could not send
                    g_isoTpTx.state = ISOTP_STATE_IDLE;
                    break;
                }
                else
                {
                    /* Do nothing. */
                }
                // Update for next CF
                g_isoTpTx.txDataOffset += bytesToSend;
                g_isoTpTx.txSequenceNumber = (g_isoTpTx.txSequenceNumber + 1) & 0x0F;
                g_isoTpTx.blockCounter++;
                // If block counter == blockSize, we should wait for next FlowControl
                if ((g_isoTpTx.blockSize > 0) &&
                        (g_isoTpTx.blockCounter >= g_isoTpTx.blockSize) &&
                        (g_isoTpTx.txDataOffset < g_isoTpTx.txDataSize))
                {
                    // We need to wait for next FC
                    g_isoTpTx.blockCounter = 0;
                    g_isoTpTx.state = ISOTP_STATE_TX_FF; // or a dedicated "WAIT_FC" state
                    // Start or reset a timer
                    g_isoTpTx.timer = 1;
                }
                else
                {
                    // Remain in TX_CF if not finished
                    if (g_isoTpTx.txDataOffset >= g_isoTpTx.txDataSize)
                    {
                        // Done
                        g_isoTpTx.state = ISOTP_STATE_IDLE;
                    }
                    else
                    {
                        /* Do nothing. */
                    }
                }
            }
            break;
        }
        default:
            // Idle or single-frame done
            break;
    }
}
