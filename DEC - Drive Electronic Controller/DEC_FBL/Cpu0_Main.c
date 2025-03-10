#include <string.h>
#include "Ifx_Types.h"
#include "IfxFlash.h"
#include "IfxCpu.h"
#include "IfxCan_Can.h"
#include "IfxScuRcu.h"
#include "aurix_pin_mappings.h"

#define MAXIMUM_CAN_DATA_PAYLOAD    8u
#define IRQ_CANRX_CHANNEL           4U
#define IRQ_CANTX_CHANNEL           5U
#define SESSIONSTATUS_ADDR          0xb000173c
#define APPL_START_ADDRESS          0x80060000
#define PFLASH_PAGE_LENGTH          IFXFLASH_PFLASH_PAGE_LENGTH /* 0x20 = 32 Bytes (smallest unit that can be programmed in the Program Flash memory (PFLASH)) */
#define FLASH_MODULE                0                           /* Macro to select the flash (PMU) module           */
#define PROGRAM_FLASH_0             IfxFlash_FlashType_P0       /* Define the Program Flash Bank to be used         */
#define PFLASH_NUM_PAGE_TO_FLASH    2                           /* Number of pages to flash in the PFLASH           */
#define PFLASH_NUM_SECTORS          20                           /* Number of PFLASH sectors to be erased            */
/* Reserved space for erase and program routines in bytes */
#define ERASESECTOR_LEN             (110)
#define WAITUNBUSY_LEN              (110)
#define ENTERPAGEMODE_LEN           (110)
#define LOADPAGE2X32_LEN            (110)
#define WRITEPAGE_LEN               (110)
#define ERASEPFLASH_LEN             (0x186)
#define WRITEPFLASH_LEN             (0x228)
/* Relocation address for the erase and program routines: Program Scratch-Pad SRAM (PSPR) of CPU0 */
#define RELOCATION_START_ADDR       (0x70100000U)
/* Definition of the addresses where to relocate the erase and program routines, given their reserved space */
#define ERASESECTOR_ADDR            (RELOCATION_START_ADDR)
#define WAITUNBUSY_ADDR             (ERASESECTOR_ADDR + ERASESECTOR_LEN)
#define ENTERPAGEMODE_ADDR          (WAITUNBUSY_ADDR + WAITUNBUSY_LEN)
#define LOAD2X32_ADDR               (ENTERPAGEMODE_ADDR + ENTERPAGEMODE_LEN)
#define WRITEPAGE_ADDR              (LOAD2X32_ADDR + LOADPAGE2X32_LEN)
#define ERASEPFLASH_ADDR            (WRITEPAGE_ADDR + WRITEPAGE_LEN)
#define WRITEPFLASH_ADDR            (ERASEPFLASH_ADDR + ERASEPFLASH_LEN)
#define MEM(address)                *((uint32 *)(address))      /* Macro to simplify the access to a memory address */

typedef enum
{
    DEFAULT = 0U,
    EXTENDED = 1U,
    PROGRAMMING = 2U,
    HARDRESET = 5U,
    SOFTRESET = 6U,
    JUMPTOAPPL = 7U
}FBL_DSC_t;

typedef struct
{
        IfxCan_Can_Config canConfig;                            /* CAN module configuration structure                   */
        IfxCan_Can canModule;                                   /* CAN module handle                                    */
        IfxCan_Can_Node canSrcNode;                             /* CAN source node handle data structure                */
        IfxCan_Can_Node canDstNode;                             /* CAN destination node handle data structure           */
        IfxCan_Can_NodeConfig canNodeConfig;                    /* CAN node configuration structure                     */
        IfxCan_Filter canFilter;                                /* CAN filter configuration structure                   */
        IfxCan_Message txMsg;                                   /* Transmitted CAN message structure                    */
        IfxCan_Message rxMsg;                                   /* Received CAN message structure                       */
        uint32 txData[MAXIMUM_CAN_DATA_PAYLOAD];                 /* Transmitted CAN data array                           */
        uint32 rxData[MAXIMUM_CAN_DATA_PAYLOAD];                 /* Received CAN data array                              */
} McmcanType;

McmcanType g_mcmcan;
uint8 FBL_RxFrame[8] = {0};
uint8 FBL_TxFrame[8] = {0};
uint32* FBL_DSC_Pointer = (uint32*)(SESSIONSTATUS_ADDR);
FBL_DSC_t FBL_DSC_State = JUMPTOAPPL;
uint32 FBL_ProgrammingData = 0;
uint32 FBL_ProgrammingIndex = 0;
uint32 FBL_ProgrammingAddress = 0;
uint32 ROM_APPL_START_ADDR_storedValue = 0;
uint32 ROM_APPL_START_ADDR = APPL_START_ADDRESS;
uint32 FBL_DSC_Status = 0;

IFX_INTERRUPT(ISR_CanTx, 0, 6u);
IFX_INTERRUPT(ISR_CanRx, 0, 5u);

void Can_ReInitAfterError(void);
void Can_Init(void);
uint32 Can_Tx(McmcanType message);
void Can_Rx(void);
void FBL_JumpToAppl(void);
void FBL_DiagService_ER_SoftReset(void);
void FBL_DiagService_ER_HardReset(void);
void FBL_DiagService_DSC_Programming(void);
void FBL_DiagService_DSC_RequestDownload(void);
void FBL_DiagService_DSC_TransferData(void);
void FBL_DiagService_DSC_RequestTransferExit(void);
void FBL_DiagService_RAR_MassEraseAPPL(void);
void FBL_DiagService_RDBI_ReadActiveDiagnosticSession(void);
void FBL_FlashReadData(uint32 address, uint32* rxBuffer);
uint32 FBL_FlashWriteData(uint32 address, uint32 data);
void FBL_EraseFlash_APPL(void);
void FBL_CopyFunctionsToPSPR(void);

typedef struct
{
        void (*eraseSectors)(uint32 sectorAddr, uint32 numSector);
        uint8 (*waitUnbusy)(uint32 flash, IfxFlash_FlashType flashType);
        uint8 (*enterPageMode)(uint32 pageAddr);
        void (*load2X32bits)(uint32 pageAddr, uint32 wordL, uint32 wordU);
        void (*writePage)(uint32 pageAddr);
        void (*eraseFlash)(uint32 sectorAddr);
        void (*writeFlash)(uint32 startingAddr);
} Function;

Function g_commandFromPSPR;

void core0_main(void)
{
    IfxCpu_enableInterrupts();
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
    IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());
    can0_node0_init_pins();
    Can_Init();
    FBL_ProgrammingAddress = 0;
    FBL_ProgrammingIndex = 0;
    FBL_FlashReadData(ROM_APPL_START_ADDR, &ROM_APPL_START_ADDR_storedValue);
    if(ROM_APPL_START_ADDR_storedValue != 0xFFFFFFFF)
    {
        FBL_DSC_Pointer = (uint32*)(SESSIONSTATUS_ADDR);
        FBL_DSC_Status = *FBL_DSC_Pointer;
    }
    else
    {
        /* Do nothing. */
    }
    if((FBL_DSC_Status == PROGRAMMING) || (FBL_DSC_Status == SOFTRESET) || (FBL_DSC_Status == HARDRESET))
    {
        FBL_DSC_State = FBL_DSC_Status;

        if(FBL_DSC_Status == SOFTRESET)
        {
            FBL_DiagService_ER_SoftReset();
        }
        else if(FBL_DSC_Status == HARDRESET)
        {
            FBL_DiagService_ER_HardReset();
        }
        else
        {
            /* Do nothing. */
        }
    }
    else
    {
        if(ROM_APPL_START_ADDR_storedValue != 0xFFFFFFFF)
        {
            FBL_JumpToAppl();
        }
        else
        {
            /* Do nothing. */
        }
    }
    
    while(1)
    {
        static uint8 localBusOff = 0u;
        static uint8 localWarningStatus = 0u;

        localBusOff = IfxCan_Node_getBusOffStatus(g_mcmcan.canDstNode.node);
        localWarningStatus = IfxCan_Node_getWarningStatus(g_mcmcan.canDstNode.node);

        if(0u != localBusOff || 0u != localWarningStatus)
        {
            Can_ReInitAfterError();
        }
        else
        {
            /* Do nothing. */
        }
    }
}

void Can_ReInitAfterError(void)
{
    IfxScuWdt_clearCpuEndinit(IfxScuWdt_getCpuWatchdogPassword());
    IfxScuWdt_clearSafetyEndinit(IfxScuWdt_getSafetyWatchdogPassword());
    IfxCan_Can_initModuleConfig(&g_mcmcan.canConfig, &MODULE_CAN0);
    IfxCan_Can_initModule(&g_mcmcan.canModule, &g_mcmcan.canConfig);
    IfxCan_Can_initNodeConfig(&g_mcmcan.canNodeConfig, &g_mcmcan.canModule);
    g_mcmcan.canNodeConfig.baudRate.baudrate = 500000u;
    g_mcmcan.canNodeConfig.baudRate.prescaler = 3u;
    g_mcmcan.canNodeConfig.baudRate.samplePoint = 8095;
    g_mcmcan.canNodeConfig.baudRate.syncJumpWidth = 0u;
    g_mcmcan.canNodeConfig.baudRate.timeSegment1 = 14u;
    g_mcmcan.canNodeConfig.baudRate.timeSegment2 = 3u;
    g_mcmcan.canNodeConfig.calculateBitTimingValues = FALSE;
    g_mcmcan.canNodeConfig.busLoopbackEnabled = FALSE;
    g_mcmcan.canNodeConfig.txConfig.txMode = IfxCan_TxMode_dedicatedBuffers;
    g_mcmcan.canNodeConfig.txConfig.dedicatedTxBuffersNumber = 63u;
    g_mcmcan.canNodeConfig.txConfig.txBufferDataFieldSize = IfxCan_DataFieldSize_8;
    g_mcmcan.canNodeConfig.frame.mode = IfxCan_FrameMode_standard;
    g_mcmcan.canNodeConfig.frame.type = IfxCan_FrameType_transmitAndReceive;
    g_mcmcan.canNodeConfig.nodeId = IfxCan_NodeId_0;
    g_mcmcan.canNodeConfig.rxConfig.rxMode = IfxCan_RxMode_dedicatedBuffers;
    g_mcmcan.canNodeConfig.rxConfig.rxBufferDataFieldSize = IfxCan_DataFieldSize_8;
    g_mcmcan.canNodeConfig.interruptConfig.messageRAMAccessFailureEnabled = TRUE;
    g_mcmcan.canNodeConfig.interruptConfig.timeoutOccurredEnabled = TRUE;
    g_mcmcan.canNodeConfig.interruptConfig.errorLoggingOverflowEnabled = TRUE;
    g_mcmcan.canNodeConfig.interruptConfig.errorPassiveEnabled = TRUE;
    g_mcmcan.canNodeConfig.interruptConfig.errorLoggingOverflowEnabled = TRUE;
    g_mcmcan.canNodeConfig.interruptConfig.warningStatusEnabled = TRUE;
    g_mcmcan.canNodeConfig.interruptConfig.busOffStatusEnabled = TRUE;
    g_mcmcan.canNodeConfig.interruptConfig.protocolErrorArbitrationEnabled = TRUE;
    g_mcmcan.canNodeConfig.interruptConfig.protocolErrorDataEnabled = TRUE;
    g_mcmcan.canNodeConfig.interruptConfig.messageStoredToDedicatedRxBufferEnabled = TRUE;
    g_mcmcan.canNodeConfig.interruptConfig.reint.priority = IRQ_CANRX_CHANNEL;
    g_mcmcan.canNodeConfig.interruptConfig.reint.interruptLine = IfxCan_InterruptLine_0;
    g_mcmcan.canNodeConfig.interruptConfig.reint.typeOfService = IfxSrc_Tos_cpu0;
    /* Message RAM configuration */
    g_mcmcan.canNodeConfig.messageRAM.baseAddress = (uint32)(g_mcmcan.canNodeConfig.can);
    g_mcmcan.canNodeConfig.messageRAM.standardFilterListStartAddress = 0x0u;
    g_mcmcan.canNodeConfig.messageRAM.extendedFilterListStartAddress = 0x0u;
    g_mcmcan.canNodeConfig.messageRAM.rxFifo0StartAddress = 0x0u;
    g_mcmcan.canNodeConfig.messageRAM.rxFifo1StartAddress = 0x0u;
    g_mcmcan.canNodeConfig.messageRAM.rxBuffersStartAddress = 0x120;
    g_mcmcan.canNodeConfig.messageRAM.txEventFifoStartAddress = 0x0u;
    g_mcmcan.canNodeConfig.messageRAM.txBuffersStartAddress = 0x520u;
    /* Setup CAN node handles */
    g_mcmcan.canDstNode.can = &MODULE_CAN0;
    g_mcmcan.canDstNode.frameMode = IfxCan_FrameMode_standard;
    g_mcmcan.canSrcNode.can = &MODULE_CAN0;
    g_mcmcan.canSrcNode.frameMode = IfxCan_FrameMode_standard;
    g_mcmcan.canNodeConfig.filterConfig.messageIdLength = IfxCan_MessageIdLength_standard;
    g_mcmcan.canNodeConfig.filterConfig.standardListSize = 11u;
    g_mcmcan.canNodeConfig.filterConfig.extendedListSize = 0u;
    g_mcmcan.canNodeConfig.filterConfig.standardFilterForNonMatchingFrames = IfxCan_NonMatchingFrame_reject;
    g_mcmcan.canNodeConfig.filterConfig.extendedFilterForNonMatchingFrames = IfxCan_NonMatchingFrame_reject;
    g_mcmcan.canNodeConfig.filterConfig.rejectRemoteFramesWithStandardId = TRUE;
    g_mcmcan.canNodeConfig.filterConfig.rejectRemoteFramesWithExtendedId = TRUE;
    /* Initialize the node */
    IfxCan_Can_initNode(&g_mcmcan.canDstNode, &g_mcmcan.canNodeConfig);
    IfxCan_Can_setStandardFilter(&g_mcmcan.canDstNode, &g_mcmcan.canFilter);
    g_mcmcan.canFilter.elementConfiguration = IfxCan_FilterElementConfiguration_storeInRxBuffer;
    g_mcmcan.canFilter.number = 0u;
    g_mcmcan.canFilter.id1 = 0x6FEu;
    g_mcmcan.canFilter.rxBufferOffset = IfxCan_RxBufferId_0;
    /* Apply the standard filter */
    IfxCan_Can_setStandardFilter(&g_mcmcan.canDstNode, &g_mcmcan.canFilter);
    IfxScuWdt_setCpuEndinit(IfxScuWdt_getCpuWatchdogPassword());
    IfxScuWdt_setSafetyEndinit(IfxScuWdt_getSafetyWatchdogPassword());
}

void Can_Init(void)
{
    IfxScuWdt_clearCpuEndinit(IfxScuWdt_getCpuWatchdogPassword());
    IfxScuWdt_clearSafetyEndinit(IfxScuWdt_getSafetyWatchdogPassword());
    IfxPort_setPinModeOutput(&MODULE_P20, 6, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
    IfxPort_setPinLow(&MODULE_P20, 6);
    IfxCan_Can_initModuleConfig(&g_mcmcan.canConfig, &MODULE_CAN0);
    IfxCan_Can_initModule(&g_mcmcan.canModule, &g_mcmcan.canConfig);
    IfxCan_Can_initNodeConfig(&g_mcmcan.canNodeConfig, &g_mcmcan.canModule);
    IfxScuCcu_setMcanFrequency(40000000.0f);
    g_mcmcan.canNodeConfig.baudRate.baudrate = 500000u;
    g_mcmcan.canNodeConfig.baudRate.prescaler = 3u;
    g_mcmcan.canNodeConfig.baudRate.samplePoint = 8095;
    g_mcmcan.canNodeConfig.baudRate.syncJumpWidth = 0u;
    g_mcmcan.canNodeConfig.baudRate.timeSegment1 = 14u;
    g_mcmcan.canNodeConfig.baudRate.timeSegment2 = 3u;
    g_mcmcan.canNodeConfig.calculateBitTimingValues = FALSE;
    g_mcmcan.canNodeConfig.busLoopbackEnabled = FALSE;
    g_mcmcan.canNodeConfig.txConfig.txMode = IfxCan_TxMode_dedicatedBuffers;
    g_mcmcan.canNodeConfig.txConfig.dedicatedTxBuffersNumber = 63u;
    g_mcmcan.canNodeConfig.txConfig.txBufferDataFieldSize = IfxCan_DataFieldSize_8;
    g_mcmcan.canNodeConfig.frame.mode = IfxCan_FrameMode_standard;
    g_mcmcan.canNodeConfig.frame.type = IfxCan_FrameType_transmitAndReceive;
    g_mcmcan.canNodeConfig.nodeId = IfxCan_NodeId_0;
    g_mcmcan.canNodeConfig.rxConfig.rxMode = IfxCan_RxMode_dedicatedBuffers;
    g_mcmcan.canNodeConfig.rxConfig.rxBufferDataFieldSize = IfxCan_DataFieldSize_8;
    g_mcmcan.canNodeConfig.interruptConfig.messageRAMAccessFailureEnabled = TRUE;
    g_mcmcan.canNodeConfig.interruptConfig.timeoutOccurredEnabled = TRUE;
    g_mcmcan.canNodeConfig.interruptConfig.errorLoggingOverflowEnabled = TRUE;
    g_mcmcan.canNodeConfig.interruptConfig.errorPassiveEnabled = TRUE;
    g_mcmcan.canNodeConfig.interruptConfig.errorLoggingOverflowEnabled = TRUE;
    g_mcmcan.canNodeConfig.interruptConfig.warningStatusEnabled = TRUE;
    g_mcmcan.canNodeConfig.interruptConfig.busOffStatusEnabled = TRUE;
    g_mcmcan.canNodeConfig.interruptConfig.protocolErrorArbitrationEnabled = TRUE;
    g_mcmcan.canNodeConfig.interruptConfig.protocolErrorDataEnabled = TRUE;
    g_mcmcan.canNodeConfig.interruptConfig.messageStoredToDedicatedRxBufferEnabled = TRUE;
    g_mcmcan.canNodeConfig.interruptConfig.reint.priority = IRQ_CANRX_CHANNEL;
    g_mcmcan.canNodeConfig.interruptConfig.reint.interruptLine = IfxCan_InterruptLine_0;
    g_mcmcan.canNodeConfig.interruptConfig.reint.typeOfService = IfxSrc_Tos_cpu0;
    /* Message RAM configuration */
    g_mcmcan.canNodeConfig.messageRAM.baseAddress = (uint32)(g_mcmcan.canNodeConfig.can);
    g_mcmcan.canNodeConfig.messageRAM.standardFilterListStartAddress = 0x0u;
    g_mcmcan.canNodeConfig.messageRAM.extendedFilterListStartAddress = 0x0u;
    g_mcmcan.canNodeConfig.messageRAM.rxFifo0StartAddress = 0x0u;
    g_mcmcan.canNodeConfig.messageRAM.rxFifo1StartAddress = 0x0u;
    g_mcmcan.canNodeConfig.messageRAM.rxBuffersStartAddress = 0x120;
    g_mcmcan.canNodeConfig.messageRAM.txEventFifoStartAddress = 0x0u;
    g_mcmcan.canNodeConfig.messageRAM.txBuffersStartAddress = 0x520u;
    /* Setup CAN node handles */
    g_mcmcan.canDstNode.can = &MODULE_CAN0;
    g_mcmcan.canDstNode.frameMode = IfxCan_FrameMode_standard;
    g_mcmcan.canSrcNode.can = &MODULE_CAN0;
    g_mcmcan.canSrcNode.frameMode = IfxCan_FrameMode_standard;
    g_mcmcan.canNodeConfig.filterConfig.messageIdLength = IfxCan_MessageIdLength_standard;
    g_mcmcan.canNodeConfig.filterConfig.standardListSize = 1u;
    g_mcmcan.canNodeConfig.filterConfig.extendedListSize = 0u;
    g_mcmcan.canNodeConfig.filterConfig.standardFilterForNonMatchingFrames = IfxCan_NonMatchingFrame_reject;
    g_mcmcan.canNodeConfig.filterConfig.extendedFilterForNonMatchingFrames = IfxCan_NonMatchingFrame_reject;
    g_mcmcan.canNodeConfig.filterConfig.rejectRemoteFramesWithStandardId = TRUE;
    g_mcmcan.canNodeConfig.filterConfig.rejectRemoteFramesWithExtendedId = TRUE;
    /* Initialize the node */
    IfxCan_Can_initNode(&g_mcmcan.canDstNode, &g_mcmcan.canNodeConfig);
    g_mcmcan.canFilter.elementConfiguration = IfxCan_FilterElementConfiguration_storeInRxBuffer;
    g_mcmcan.canFilter.number = 0u;
    g_mcmcan.canFilter.id1 = 0x6FEu;
    g_mcmcan.canFilter.rxBufferOffset = IfxCan_RxBufferId_0;
    /* Apply the standard filter */
    IfxCan_Can_setStandardFilter(&g_mcmcan.canDstNode, &g_mcmcan.canFilter);
    IfxCan_Node_initRxPin(g_mcmcan.canDstNode.node, &IfxCan_RXD00B_P20_7_IN, IfxPort_Mode_inputPullUp, IfxPort_PadDriver_cmosAutomotiveSpeed1);
    IfxCan_Node_initTxPin(&IfxCan_TXD00_P20_8_OUT, IfxPort_OutputMode_pushPull, IfxPort_PadDriver_cmosAutomotiveSpeed4);
    IfxScuWdt_setCpuEndinit(IfxScuWdt_getCpuWatchdogPassword());
    IfxScuWdt_setSafetyEndinit(IfxScuWdt_getSafetyWatchdogPassword());
}

uint32 Can_Tx(McmcanType message)
{
    return IfxCan_Can_sendMessage(&message.canSrcNode, &message.txMsg, message.txData);
}

void Can_Rx(void)
{
    g_mcmcan.rxMsg.bufferNumber = 0u;
    /* Clear the "Message stored to Dedicated RX Buffer" interrupt flag */
    IfxCan_Node_clearInterruptFlag(g_mcmcan.canDstNode.node, IfxCan_Interrupt_messageStoredToDedicatedRxBuffer);
    /* Read the received CAN message */
    IfxCan_Can_readMessage(&g_mcmcan.canDstNode, &g_mcmcan.rxMsg, g_mcmcan.rxData);

    switch(FBL_DSC_State)
    {
        case PROGRAMMING:
            FBL_DiagService_DSC_Programming();
            break;
        case HARDRESET:
            FBL_DiagService_ER_HardReset();
            break;
        case SOFTRESET:
            FBL_DiagService_ER_SoftReset();
            break;
        default:
            if(g_mcmcan.rxData[1u] == 0x50u && g_mcmcan.rxData[2u] == 0x02u)
            {
                g_mcmcan.txMsg.dataLengthCode = g_mcmcan.rxMsg.dataLengthCode;
                g_mcmcan.txMsg.messageId = g_mcmcan.rxMsg.messageId + 1u;
                g_mcmcan.txData[0u] = g_mcmcan.rxData[0u];
                g_mcmcan.txData[1u] = g_mcmcan.rxData[1u] + 0x40u;
                g_mcmcan.txData[2u] = g_mcmcan.rxData[2u];
                FBL_DSC_State = PROGRAMMING;
                Can_Tx(g_mcmcan);
                FBL_DiagService_DSC_Programming();
            }
            else
            {
                /* Do nothing. */
            }
            break;
    }
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

void FBL_CopyFunctionsToPSPR(void)
{
    /* Copy the IfxFlash_eraseMultipleSectors() routine and assign it to a function pointer */
    memcpy((void *)ERASESECTOR_ADDR, (const void *)IfxFlash_eraseMultipleSectors, ERASESECTOR_LEN);
    g_commandFromPSPR.eraseSectors = (void *)ERASESECTOR_ADDR;
    /* Copy the IfxFlash_waitUnbusy() routine and assign it to a function pointer */
    memcpy((void *)WAITUNBUSY_ADDR, (const void *)IfxFlash_waitUnbusy, WAITUNBUSY_LEN);
    g_commandFromPSPR.waitUnbusy = (void *)WAITUNBUSY_ADDR;
    /* Copy the IfxFlash_enterPageMode() routine and assign it to a function pointer */
    memcpy((void *)ENTERPAGEMODE_ADDR, (const void *)IfxFlash_enterPageMode, ENTERPAGEMODE_LEN);
    g_commandFromPSPR.enterPageMode = (void *)ENTERPAGEMODE_ADDR;
    /* Copy the IfxFlash_loadPage2X32() routine and assign it to a function pointer */
    memcpy((void *)LOAD2X32_ADDR, (const void *)IfxFlash_loadPage2X32, LOADPAGE2X32_LEN);
    g_commandFromPSPR.load2X32bits = (void *)LOAD2X32_ADDR;
    /* Copy the IfxFlash_writePage() routine and assign it to a function pointer */
    memcpy((void *)WRITEPAGE_ADDR, (const void *)IfxFlash_writePage, WRITEPAGE_LEN);
    g_commandFromPSPR.writePage = (void *)WRITEPAGE_ADDR;
    /* Copy the erasePFLASH() routine and assign it to a function pointer */
    memcpy((void *)ERASEPFLASH_ADDR, (const void *)FBL_EraseFlash_APPL, ERASEPFLASH_LEN);
    g_commandFromPSPR.eraseFlash = (void *)ERASEPFLASH_ADDR;
    /* Copy the erasePFLASH() routine and assign it to a function pointer */
    memcpy((void *)WRITEPFLASH_ADDR, (const void *)FBL_FlashWriteData, WRITEPFLASH_LEN);
    g_commandFromPSPR.writeFlash = (void *)WRITEPFLASH_ADDR;
}

void FBL_FlashReadData(uint32 address, uint32* rxBuffer)
{
    address += 32;
    *rxBuffer = *((volatile uint32 *)address);
}

uint32 FBL_FlashWriteData(uint32 address, uint32 data)
{
    uint32 targetOffset = address % PFLASH_PAGE_LENGTH;       /* Offset within the page */
    uint32 pageAddr = address - targetOffset;                  /* Start address of the flash page */
    uint32 numWords = PFLASH_PAGE_LENGTH / 4;                  /* Number of 32-bit words in the page */
    uint32 pageBuffer[numWords];
    uint32 i;
    /* Read the current contents of the flash page into a RAM buffer (Assuming flash is memory-mapped, you can read it directly.) */
    for(i = 0; i < numWords; i++)
    {
        pageBuffer[i] = *((volatile uint32*)(pageAddr + (i * 4)));
    }
    /* Update the target word in the buffer (Assumes that 'address' is aligned to 4 bytes.) */
    pageBuffer[targetOffset / 4] = data;
    /* Get the current Safety Watch-dog password */
    uint16 endInitSafetyPassword = IfxScuWdt_getSafetyWatchdogPasswordInline();
    /* Program the page by loading the entire page buffer. Programming is done in chunks of 8 bytes (two 32-bit words). */
    for(uint32 offset = 0; offset < PFLASH_PAGE_LENGTH; offset += 0x8)
    {
        uint32 wordIndex = offset / 4;
        uint32 wordL = pageBuffer[wordIndex];
        uint32 wordU = pageBuffer[wordIndex + 1];
        /* Load the two words into the flash programming buffer. Note: Use (pageAddr + offset) as the target address */
        g_commandFromPSPR.load2X32bits(pageAddr + offset, wordL, wordU);
    }
    /* Write the page. Disable EndInit protection, write, then re-enable protection. */
    IfxScuWdt_clearSafetyEndinitInline(endInitSafetyPassword);
    g_commandFromPSPR.writePage(pageAddr);
    IfxScuWdt_setSafetyEndinitInline(endInitSafetyPassword);
    /* Wait until the page is written in flash */
    g_commandFromPSPR.waitUnbusy(FLASH_MODULE, PROGRAM_FLASH_0);
    return 0u;
}

void FBL_EraseFlash_APPL(void)
{
    FBL_CopyFunctionsToPSPR();
    /* Get the current password of the Safety WatchDog module */
    uint16 endInitSafetyPassword = IfxScuWdt_getSafetyWatchdogPasswordInline();
    /* Erase the sector */
    IfxScuWdt_clearSafetyEndinitInline(endInitSafetyPassword);      /* Disable EndInit protection                   */
    g_commandFromPSPR.eraseSectors(APPL_START_ADDRESS, PFLASH_NUM_SECTORS); /* Erase the given sector               */
    IfxScuWdt_setSafetyEndinitInline(endInitSafetyPassword);        /* Enable EndInit protection                    */
    /* Wait until the sector is erased */
    g_commandFromPSPR.waitUnbusy(FLASH_MODULE, PROGRAM_FLASH_0);
}

void FBL_DiagService_DSC_RequestDownload(void)
{
    g_mcmcan.txMsg.dataLengthCode = g_mcmcan.rxMsg.dataLengthCode;
    g_mcmcan.txMsg.messageId = g_mcmcan.rxMsg.messageId + 1u;
    g_mcmcan.txData[0] = g_mcmcan.rxData[0];
    g_mcmcan.txData[1] = g_mcmcan.rxData[1] + 0x40u;
    g_mcmcan.txData[2] = g_mcmcan.rxData[2];
    Can_Tx(g_mcmcan);
    for(uint8 i = 0; i < 8 ; i++) g_mcmcan.txData[i] = 0u;
    g_mcmcan.txMsg.dataLengthCode = 0u;
}
void FBL_DiagService_DSC_TransferData(void)
{
    FBL_ProgrammingData = (FBL_RxFrame[6] << 24) | (FBL_RxFrame[5] << 16) | (FBL_RxFrame[4] << 8) | FBL_RxFrame[3];
    IfxCpu_disableInterrupts();
    FBL_FlashWriteData(FBL_ProgrammingAddress, FBL_ProgrammingData);
    IfxCpu_enableInterrupts();
    FBL_ProgrammingAddress += 4;
    FBL_ProgrammingIndex += 1;
    g_mcmcan.txMsg.dataLengthCode = g_mcmcan.rxMsg.dataLengthCode;
    g_mcmcan.txMsg.messageId = g_mcmcan.rxMsg.messageId + 1u;
    g_mcmcan.txData[0] = g_mcmcan.rxData[0];
    g_mcmcan.txData[1] = g_mcmcan.rxData[1] + 0x40u;
    g_mcmcan.txData[2] = g_mcmcan.rxData[2];
    Can_Tx(g_mcmcan);
    for(uint8 i = 0; i < 8 ; i++) g_mcmcan.txData[i] = 0u;
    g_mcmcan.txMsg.dataLengthCode = 0u;
}
void FBL_DiagService_DSC_RequestTransferExit(void)
{
    g_mcmcan.txMsg.dataLengthCode = g_mcmcan.rxMsg.dataLengthCode;
    g_mcmcan.txMsg.messageId = g_mcmcan.rxMsg.messageId + 1u;
    g_mcmcan.txData[0] = g_mcmcan.rxData[0];
    g_mcmcan.txData[1] = g_mcmcan.rxData[1] + 0x40u;
    g_mcmcan.txData[2] = g_mcmcan.rxData[2];
    Can_Tx(g_mcmcan);
    for(uint8 i = 0; i < 8 ; i++) g_mcmcan.txData[i] = 0u;
    g_mcmcan.txMsg.dataLengthCode = 0u;
}
void FBL_DiagService_RAR_MassEraseAPPL(void)
{
    IfxCpu_disableInterrupts();
    g_mcmcan.txMsg.dataLengthCode = g_mcmcan.rxMsg.dataLengthCode;
    g_mcmcan.txMsg.messageId = g_mcmcan.rxMsg.messageId + 1u;
    g_mcmcan.txData[0] = g_mcmcan.rxData[0];
    g_mcmcan.txData[1] = g_mcmcan.rxData[1] + 0x40u;
    g_mcmcan.txData[2] = g_mcmcan.rxData[2];
    Can_Tx(g_mcmcan);
    FBL_EraseFlash_APPL();
    for(uint8 i = 0; i < 8 ; i++) g_mcmcan.txData[i] = 0u;
    g_mcmcan.txMsg.dataLengthCode = 0u;
    IfxCpu_enableInterrupts();
}
volatile uint32 appEntryGlobal;
void FBL_JumpToAppl(void)
{
    appEntryGlobal = APPL_START_ADDRESS + 0x04u;

    __disable();
    __asm("movh.a  a15,#@his(appEntryGlobal)");
    __asm("lea     a15,[a15]@los(appEntryGlobal)");
    __asm("ld.a    a15,[a15]");
    __asm("isync");
    __asm("ji      a15");
}
void FBL_DiagService_ER_SoftReset(void)
{
    IfxCpu_disableInterrupts();
    *FBL_DSC_Pointer = 0;
    IfxScuRcu_performReset(2, 0);
}
void FBL_DiagService_ER_HardReset(void)
{
    IfxCpu_disableInterrupts();
    *FBL_DSC_Pointer = 0;
    IfxScuRcu_performReset(2, 0);
}

void FBL_DiagService_RDBI_ReadActiveDiagnosticSession(void)
{
    g_mcmcan.txMsg.dataLengthCode = g_mcmcan.rxMsg.dataLengthCode;
    g_mcmcan.txMsg.messageId = g_mcmcan.rxMsg.messageId + 1u;
    g_mcmcan.txData[0] = g_mcmcan.rxData[0];
    g_mcmcan.txData[1] = g_mcmcan.rxData[1] + 0x40u;
    g_mcmcan.txData[2] = g_mcmcan.rxData[2];
    g_mcmcan.txData[3] = g_mcmcan.rxData[3];
    g_mcmcan.txData[4] =  FBL_DSC_State;
    Can_Tx(g_mcmcan);
    for(uint8 i = 0; i < 8 ; i++) g_mcmcan.txData[i] = 0u;
    g_mcmcan.txMsg.dataLengthCode = 0u;
}
void FBL_DiagService_DSC_Programming(void)
{
    if(FBL_RxFrame[1] == 0x22 && FBL_RxFrame[2] == 0xF1 && FBL_RxFrame[3] == 0x86)
    {
        FBL_DiagService_RDBI_ReadActiveDiagnosticSession();
    }
    else
    {
        /* Do nothing. */
    }
    if(FBL_RxFrame[0] == 0x04 && FBL_RxFrame[1] == 0x31 && FBL_RxFrame[4] == 0x00)
    {
        FBL_DiagService_RAR_MassEraseAPPL();
    }
    else
    {
        /* Do nothing. */
    }

    if(FBL_RxFrame[1] == 0x34)
    {
        FBL_ProgrammingAddress = (0x08 << 24)|(FBL_RxFrame[4] << 16)|(FBL_RxFrame[5] << 8)|(FBL_RxFrame[6]);
        FBL_DiagService_DSC_RequestDownload();
    }
    else
    {
        /* Do nothing. */
    }

    if(FBL_RxFrame[1] == 0x36)
    {
        FBL_DiagService_DSC_TransferData();
    }
    else
    {
        /* Do nothing. */
    }

    if(FBL_RxFrame[1] == 0x37 && FBL_RxFrame[0] == 0x01)
    {
        FBL_DiagService_DSC_RequestTransferExit();
    }
    else
    {
        /* Do nothing. */
    }

    if(FBL_RxFrame[1] == 0x11 && FBL_RxFrame[2] == 0x01)
    {
        g_mcmcan.txMsg.dataLengthCode = g_mcmcan.rxMsg.dataLengthCode;
        g_mcmcan.txMsg.messageId = g_mcmcan.rxMsg.messageId + 1u;
        g_mcmcan.txData[0] = g_mcmcan.rxData[0];
        g_mcmcan.txData[1] = g_mcmcan.rxData[1] + 0x40u;
        g_mcmcan.txData[2] = g_mcmcan.rxData[2];
        g_mcmcan.txData[3] = g_mcmcan.rxData[3];
        Can_Tx(g_mcmcan);
        for(uint32 i = 0; i < 200000; i ++);
        FBL_DiagService_ER_HardReset();
    }
    else
    {
        /* Do nothing. */
    }
}
