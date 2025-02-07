#ifndef MCMCAN_H_
#define MCMCAN_H_ 1

#include <stdio.h>
#include <string.h>
#include "Ifx_Types.h"
#include "IfxCan_Can.h"
#include "IfxCpu_Irq.h"
#include "IfxPort.h"

#define MAXIMUM_CAN_DATA_PAYLOAD 8u

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

typedef struct
{
        IfxCan_Message rxMsg;                                   /* Received CAN message structure                       */
        uint32 rxData[MAXIMUM_CAN_DATA_PAYLOAD];                 /* Transmitted CAN data array                           */
}Can_RxMsg_t;

typedef struct
{
        IfxCan_Message txMsg;                                   /* Transmitted CAN message structure                    */
        uint32 txData[MAXIMUM_CAN_DATA_PAYLOAD];                 /* Transmitted CAN data array                           */
}Can_TxMsg_t;

extern McmcanType g_mcmcan;
extern Can_RxMsg_t Can_RxMessageBuffer[50u];
extern Can_TxMsg_t Can_TxMessageBuffer[50u];
extern uint32 Can_BufferIndex_ReceivedMessages;
extern uint32 Can_BufferIndex_TransmittedMessages;
extern uint8 Can_BusOff_Flag;
extern uint8 Can_Error_Flag;

extern void Can_Sleep(void);
extern void Can_Wakeup(void);
extern void Can_ProcessReceivedMessages(void);
extern void Can_TransmitScheduleTable(void);
extern void Can_TransmitAllMessages(void);
extern void Can_Init(void);
extern void Can_Tx(McmcanType message);
extern void Can_Rx(void);
extern void Can_MainFunction(void);
#endif /* MCMCAN_H_ */
