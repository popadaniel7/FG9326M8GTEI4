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

#define ISOTP_PCI_SF        0x00
#define ISOTP_PCI_FF        0x10
#define ISOTP_PCI_CF        0x20
#define ISOTP_PCI_FC        0x30
#define ISOTP_FC_STATUS_CTS 0x00
#define ISOTP_FC_STATUS_WAIT 0x01
#define ISOTP_FC_STATUS_OVFL 0x02
#define ISOTP_CAN_DL        8
#define ISOTP_DEFAULT_N_AR_TIMEOUT  1
#define ISOTP_DEFAULT_N_BR_TIMEOUT  1
#define ISOTP_DEFAULT_N_CR_TIMEOUT  1
#define ISOTP_DEFAULT_BLOCK_SIZE  8
#define ISOTP_DEFAULT_ST_MIN_MS   5
#define MAXIMUM_CAN_DATA_PAYLOAD 8u
#define CAN_NO_RX_MSG 10u
#define CAN_NO_TX_MSG 11u

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

typedef enum
{
    ISOTP_STATE_IDLE = 0,
    ISOTP_STATE_RX_FF,  // waiting for consecutive frames
    ISOTP_STATE_RX_CF,
    ISOTP_STATE_TX_FF,  // sending first frame
    ISOTP_STATE_TX_CF,
} IsoTpChannelState;

typedef struct
{
    // -- Common --
    IsoTpChannelState state;
    uint16_t          timer;         // for timeouts
    uint32_t          canId;         // physical CAN ID used for the channel
    // -- Rx only --
    uint16_t          payloadLength; // total payload length in bytes
    uint16_t          receivedBytes;
    uint8_t           nextCfSequenceNumber;
    // We store the incoming data here
    uint8_t          *rxBufferPtr;   // external or static buffer
    uint16_t          rxBufferSize;  // max buffer length
    // -- Tx only --
    const uint8_t    *txDataPtr;     // pointer to user data
    uint16_t          txDataSize;    // total data size to send
    uint16_t          txDataOffset;  // how many bytes have been sent
    uint8_t           txSequenceNumber;
    // Flow Control parameters
    uint8_t           blockSize;
    uint8_t           blockCounter;
    uint8_t           stMin;         // separation time in ms
} IsoTpChannel;

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
extern void Can_IsoTp_Init(void);
extern bool Can_IsoTp_Transmit(uint16 canId, const uint8 *data, uint16 size);
extern void Can_IsoTp_OnCanReceive(uint16 canId, const uint8 *data, uint8 size);
extern void Can_IsoTp_MainFunction(void);
#endif /* MCMCAN_H_ */
