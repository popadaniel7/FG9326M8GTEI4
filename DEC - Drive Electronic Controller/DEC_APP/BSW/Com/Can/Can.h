#ifndef MCMCAN_H_
#define MCMCAN_H_ 1

#include <stdio.h>
#include <string.h>
#include "Ifx_Types.h"
#include "IfxCan_Can.h"
#include "IfxCpu_Irq.h"
#include "IfxPort.h"
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#define MAXIMUM_CAN_DATA_PAYLOAD 8u
#define CAN_NO_RX_MSG 6u
#define CAN_NO_TX_MSG 9u
#define CAN_RX_BUFFER_COUNT        9      // Number of hardware Rx buffers
#define ISO_TP_MAX_PAYLOAD         2048   // Maximum reassembly buffer size
#define ISO_TP_CAN_DL              8       // CAN data length (8 bytes)

typedef struct
{
        uint16 id;        // CAN identifier
        uint8  dlc;       // Data Length Code (number of valid data bytes)
        uint8  data[ISO_TP_CAN_DL];   // Up to 8 data bytes per CAN frame
} CanMsg;

typedef enum
{
    ISO_TP_TX_IDLE = 0,
    ISO_TP_TX_WAIT_FC,  // Waiting for Flow Control (FC) frame from tester
    ISO_TP_TX_CF       // Transmitting Consecutive Frames
} IsoTpTxState_t;

typedef struct
{
    uint16 txId;           // CAN Identifier for transmission (ECU sends on 0x6FF)
    const uint8 *txData;   // Pointer to the full response payload to be sent
    uint16 txDataSize;     // Total length of the response payload
    uint16 txDataOffset;   // Offset into the payload for the next frame
    uint8  txSequenceNumber; // Next consecutive frame sequence number (0-15)
    uint8  blockSize;      // Block size as received in FC (0 means “send all”)
    uint8  stMin;          // Minimum separation time (in ms) from FC
    uint8  blockCounter;   // Counter for frames sent in current block
    IsoTpTxState_t state;    // Current state of the transmit state machine
    // Optionally, add a timer for STmin handling if needed.
} IsoTpTx_t;


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
        uint8 txData[MAXIMUM_CAN_DATA_PAYLOAD];                 /* Transmitted CAN data array                           */
        uint8 rxData[MAXIMUM_CAN_DATA_PAYLOAD];                 /* Received CAN data array                              */
} McmcanType;

typedef struct
{
        IfxCan_Message rxMsg;                                   /* Received CAN message structure                       */
        uint8 rxData[MAXIMUM_CAN_DATA_PAYLOAD];                 /* Transmitted CAN data array                           */
}Can_RxMsg_t;

typedef struct
{
        IfxCan_Message txMsg;                                   /* Transmitted CAN message structure                    */
        uint8 txData[MAXIMUM_CAN_DATA_PAYLOAD];                 /* Transmitted CAN data array                           */
}Can_TxMsg_t;

typedef struct
{
        Can_TxMsg_t transmitMessage;
        uint8 transmitFlag;
        uint8 cycleTime;
        uint8 transmissionRule;
}Can_TransmitType_t;

typedef struct
{
        Can_RxMsg_t receiveMessage;
        uint8 receivedValidFlag;
}Can_ReceiveType_t;

extern Can_TransmitType_t Can_TransmitTable[CAN_NO_TX_MSG];
extern Can_ReceiveType_t Can_ReceiveTable[CAN_NO_RX_MSG];
extern McmcanType g_mcmcan;
extern uint32 Can_Rx_DiagBufCnt;
extern Can_RxMsg_t Can_RxMessageBuffer[50u];
extern Can_RxMsg_t Can_Rx_DiagnosticBuffer[50u];
extern uint32 Can_BufferIndex_ReceivedMessages;
extern uint8 Can_BusOff_Flag;
extern uint8 Can_Alrt_Flag;
extern uint8 Can_Moer_Flag;
extern uint8 Can_Loi_Flag;
extern Can_Status_t Can_State;
extern uint8 Can_ActivityOnTheBus;
extern uint8 Can_DedBuff;

extern void Can_Sleep(void);
extern void Can_ProcessReceivedMessages(void);
extern void Can_TransmitScheduleTable(void);
extern void Can_TransmitAllMessages(void);
extern void Can_TransmitDiagnosis(void);
extern void Can_Init(void);
extern bool Can_Tx(McmcanType message);
extern void Can_Rx(void);
extern void Can_MainFunction(void);
extern bool Can_IsoTp_SendFrame(uint16 canId, const uint8 *data, uint8 size);
extern void Can_IsoTp_MainFunction(void);
// Low-level CAN transmit function (assumed to be implemented to drive your hardware)
extern bool Can_SendFrame(uint16 id, const uint8_t *data, uint8 dlc);
// ISO-TP transmit: sends a full ISO-TP message (single or multi-frame)
extern bool Can_IsoTpTransmit(uint16 txId, const uint8 *data, uint16 length);
// ISO-TP processing for one incoming CAN frame
extern void Can_ProcessIsoTpMessage(const CanMsg *msg);
// Transmit a Flow Control frame in response to a First Frame
extern void Can_SendFlowControlFrame(void);
// Called periodically (or in main loop) to handle ISO-TP CF transmissions
extern void Can_IsoTpTx_MainFunction(void);
#endif /* MCMCAN_H_ */
