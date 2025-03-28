#include "ComMaster.h"
#include "EncCal.h"
#include "Crc.h"
#include "SysMgr.h"
#include "Dem.h"
#include "Dcm.h"

ComMaster_TransmitType_t ComMaster_TransmitTable[COMMASTER_NO_TX_MSG] =
{
        /* Vehicle State */
        {
                .transmitMessage =
                {
                        .txMsg =
                        {
                                .bufferNumber = 0u,
                                .messageId = 0x97u,
                                .remoteTransmitRequest = 0u,
                                .messageIdLength = IfxCan_MessageIdLength_standard,
                                .errorStateIndicator = 0u,
                                .dataLengthCode = IfxCan_DataLengthCode_8,
                                .frameMode = IfxCan_FrameMode_standard,
                                .txEventFifoControl = FALSE,
                                .storeInTxFifoQueue = FALSE,
                                .readFromRxFifo0 = FALSE,
                                .readFromRxFifo1 = FALSE
                        },
                },
                .transmitFlag = 1u,
                .cycleTime = 2u,
        },
        /* Status Body Control */
        {
                .transmitMessage =
                {
                        .txMsg =
                        {
                                .bufferNumber = 1u,
                                .messageId = 0x98u,
                                .remoteTransmitRequest = 0u,
                                .messageIdLength = IfxCan_MessageIdLength_standard,
                                .errorStateIndicator = 0u,
                                .dataLengthCode = IfxCan_DataLengthCode_6,
                                .frameMode = IfxCan_FrameMode_standard,
                                .txEventFifoControl = FALSE,
                                .storeInTxFifoQueue = FALSE,
                                .readFromRxFifo0 = FALSE,
                                .readFromRxFifo1 = FALSE
                        },
                },
                .transmitFlag = 1u,
                .cycleTime = 10u,
        },
        /* Status Body Control 2 */
        {
                .transmitMessage =
                {
                        .txMsg =
                        {
                                .bufferNumber = 2u,
                                .messageId = 0x99u,
                                .remoteTransmitRequest = 0u,
                                .messageIdLength = IfxCan_MessageIdLength_standard,
                                .errorStateIndicator = 0u,
                                .dataLengthCode = IfxCan_DataLengthCode_5,
                                .frameMode = IfxCan_FrameMode_standard,
                                .txEventFifoControl = FALSE,
                                .storeInTxFifoQueue = FALSE,
                                .readFromRxFifo0 = FALSE,
                                .readFromRxFifo1 = FALSE
                        },
                },
                .transmitFlag = 1u,
                .cycleTime = 10u,
        },
        /* Info Cluster Message */
        {
                .transmitMessage =
                {
                        .txMsg =
                        {
                                .bufferNumber = 3u,
                                .messageId = 0x10Bu,
                                .remoteTransmitRequest = 0u,
                                .messageIdLength = IfxCan_MessageIdLength_standard,
                                .errorStateIndicator = 0u,
                                .dataLengthCode = IfxCan_DataLengthCode_1,
                                .frameMode = IfxCan_FrameMode_standard,
                                .txEventFifoControl = FALSE,
                                .storeInTxFifoQueue = FALSE,
                                .readFromRxFifo0 = FALSE,
                                .readFromRxFifo1 = FALSE
                        },
                },
                .transmitFlag = 1u,
                .cycleTime = 20u,
        },
        /* DataRecorder */
        {
                .transmitMessage =
                {
                        .txMsg =
                        {
                                .bufferNumber = 4u,
                                .messageId = 0x10Fu,
                                .remoteTransmitRequest = 0u,
                                .messageIdLength = IfxCan_MessageIdLength_standard,
                                .errorStateIndicator = 0u,
                                .dataLengthCode = IfxCan_DataLengthCode_1,
                                .frameMode = IfxCan_FrameMode_standard,
                                .txEventFifoControl = FALSE,
                                .storeInTxFifoQueue = FALSE,
                                .readFromRxFifo0 = FALSE,
                                .readFromRxFifo1 = FALSE
                        },
                },
                .transmitFlag = 1u,
                .cycleTime = 20u,
        },
        /* Networkmanagement3 */
        {
                .transmitMessage =
                {
                        .txMsg =
                        {
                                .bufferNumber = 5u,
                                .messageId = 0x510u,
                                .remoteTransmitRequest = 0u,
                                .messageIdLength = IfxCan_MessageIdLength_standard,
                                .errorStateIndicator = 0u,
                                .dataLengthCode = IfxCan_DataLengthCode_5,
                                .frameMode = IfxCan_FrameMode_standard,
                                .txEventFifoControl = FALSE,
                                .storeInTxFifoQueue = FALSE,
                                .readFromRxFifo0 = FALSE,
                                .readFromRxFifo1 = FALSE
                        },
                },
                .transmitFlag = 1u,
                .cycleTime = 20u,
        },
        /* SafeIgnitionStatus */
        {
                .transmitMessage =
                {
                        .txMsg =
                        {
                                .bufferNumber = 6u,
                                .messageId = 0x005u,
                                .remoteTransmitRequest = 0u,
                                .messageIdLength = IfxCan_MessageIdLength_standard,
                                .errorStateIndicator = 0u,
                                .dataLengthCode = IfxCan_DataLengthCode_3,
                                .frameMode = IfxCan_FrameMode_standard,
                                .txEventFifoControl = FALSE,
                                .storeInTxFifoQueue = FALSE,
                                .readFromRxFifo0 = FALSE,
                                .readFromRxFifo1 = FALSE
                        },
                },
                .transmitFlag = 1u,
                .cycleTime = 2u,
        },
        /* SafeVehicleError */
        {
                .transmitMessage =
                {
                        .txMsg =
                        {
                                .bufferNumber = 7u,
                                .messageId = 0x004u,
                                .remoteTransmitRequest = 0u,
                                .messageIdLength = IfxCan_MessageIdLength_standard,
                                .errorStateIndicator = 0u,
                                .dataLengthCode = IfxCan_DataLengthCode_3,
                                .frameMode = IfxCan_FrameMode_standard,
                                .txEventFifoControl = FALSE,
                                .storeInTxFifoQueue = FALSE,
                                .readFromRxFifo0 = FALSE,
                                .readFromRxFifo1 = FALSE
                        },
                },
                .transmitFlag = 1u,
                .cycleTime = 2u,
        },
        /* SafeBrakeAssist */
        {
                .transmitMessage =
                {
                        .txMsg =
                        {
                                .bufferNumber = 8u,
                                .messageId = 0x002u,
                                .remoteTransmitRequest = 0u,
                                .messageIdLength = IfxCan_MessageIdLength_standard,
                                .errorStateIndicator = 0u,
                                .dataLengthCode = IfxCan_DataLengthCode_4,
                                .frameMode = IfxCan_FrameMode_standard,
                                .txEventFifoControl = FALSE,
                                .storeInTxFifoQueue = FALSE,
                                .readFromRxFifo0 = FALSE,
                                .readFromRxFifo1 = FALSE
                        },
                },
                .transmitFlag = 1u,
                .cycleTime = 2u,
        }
};
ComMaster_ReceiveType_t ComMaster_ReceiveTable[COMMASTER_NO_RX_MSG] =
{
        /* Status Drive Control */
        {
                .receiveMessage =
                {
                        .rxMsg =
                        {
                                .bufferNumber = 0u,
                                .messageId = 0x100u,
                                .remoteTransmitRequest = FALSE,
                                .messageIdLength = IfxCan_MessageIdLength_standard,
                                .errorStateIndicator = FALSE,
                                .dataLengthCode = 2u,
                                .frameMode = IfxCan_FrameMode_standard,
                                .txEventFifoControl = FALSE,
                                .storeInTxFifoQueue = FALSE,
                                .readFromRxFifo0 = FALSE,
                                .readFromRxFifo1 = FALSE
                        },
                        .rxData = {0u}
                },
                .receivedValidFlag = 0u
        },
        /* Status Actuator */
        {
                .receiveMessage =
                {
                        .rxMsg =
                        {
                                .bufferNumber = 1u,
                                .messageId = 0x101u,
                                .remoteTransmitRequest = FALSE,
                                .messageIdLength = IfxCan_MessageIdLength_standard,
                                .errorStateIndicator = FALSE,
                                .dataLengthCode = 3u,
                                .frameMode = IfxCan_FrameMode_standard,
                                .txEventFifoControl = FALSE,
                                .storeInTxFifoQueue = FALSE,
                                .readFromRxFifo0 = FALSE,
                                .readFromRxFifo1 = FALSE
                        },
                        .rxData = {0u}
                },
                .receivedValidFlag = 0u
        },
        /* Light Status */
        {
                .receiveMessage =
                {
                        .rxMsg =
                        {
                                .bufferNumber = 5u,
                                .messageId = 0x108u,
                                .remoteTransmitRequest = FALSE,
                                .messageIdLength = IfxCan_MessageIdLength_standard,
                                .errorStateIndicator = FALSE,
                                .dataLengthCode = 3u,
                                .frameMode = IfxCan_FrameMode_standard,
                                .txEventFifoControl = FALSE,
                                .storeInTxFifoQueue = FALSE,
                                .readFromRxFifo0 = FALSE,
                                .readFromRxFifo1 = FALSE
                        },
                        .rxData = {0u}
                },
                .receivedValidFlag = 0u
        },
        /* ClimateWiperControl */
        {
                .receiveMessage =
                {
                        .rxMsg =
                        {
                                .bufferNumber = 6u,
                                .messageId = 0x10Au,
                                .remoteTransmitRequest = FALSE,
                                .messageIdLength = IfxCan_MessageIdLength_standard,
                                .errorStateIndicator = FALSE,
                                .dataLengthCode = 8u,
                                .frameMode = IfxCan_FrameMode_standard,
                                .txEventFifoControl = FALSE,
                                .storeInTxFifoQueue = FALSE,
                                .readFromRxFifo0 = FALSE,
                                .readFromRxFifo1 = FALSE
                        },
                        .rxData = {0u}
                },
                .receivedValidFlag = 0u
        },
        /* StatusErrorSetting_701 */
        {
                .receiveMessage =
                {
                        .rxMsg =
                        {
                                .bufferNumber = 7u,
                                .messageId = 0x10Cu,
                                .remoteTransmitRequest = FALSE,
                                .messageIdLength = IfxCan_MessageIdLength_standard,
                                .errorStateIndicator = FALSE,
                                .dataLengthCode = 1u,
                                .frameMode = IfxCan_FrameMode_standard,
                                .txEventFifoControl = FALSE,
                                .storeInTxFifoQueue = FALSE,
                                .readFromRxFifo0 = FALSE,
                                .readFromRxFifo1 = FALSE
                        },
                        .rxData = {0u}
                },
                .receivedValidFlag = 0u
        },
        /* SafeDriveTrainStatus */
        {
                .receiveMessage =
                {
                        .rxMsg =
                        {
                                .bufferNumber = 16u,
                                .messageId = 0x001u,
                                .remoteTransmitRequest = FALSE,
                                .messageIdLength = IfxCan_MessageIdLength_standard,
                                .errorStateIndicator = FALSE,
                                .dataLengthCode = 5u,
                                .frameMode = IfxCan_FrameMode_standard,
                                .txEventFifoControl = FALSE,
                                .storeInTxFifoQueue = FALSE,
                                .readFromRxFifo0 = FALSE,
                                .readFromRxFifo1 = FALSE
                        },
                        .rxData = {0u}
                },
                .receivedValidFlag = 0u
        }
};
uint32 ComMaster_SdcMsgStat_Cnt = 0u;
uint32 ComMaster_LlMsgStat_Cnt = 0u;
uint32 ComMaster_SaMsgStat_Cnt = 0u;
uint32 ComMaster_CwcMsgStat_Cnt = 0u;
uint32 ComMaster_LsMsgStat_Cnt = 0u;
uint32 ComMaster_SdtsMsgStat_Cnt = 0u;
uint32 ComMaster_MainCounter = 0u;
uint8 ComMaster_RxSignal_AutoClimate = 0u;
uint8 ComMaster_RxSignal_ClimateTemperature = 0u;
uint8 ComMaster_RxSignal_DisplayMode = 0u;
uint8 ComMaster_RxSignal_Err701_ID = 0u;
uint8 ComMaster_RxSignal_FanValue = 0u;
uint8 ComMaster_RxSignal_Recirculation = 0u;
uint8 ComMaster_RxSignal_RotaryLightSwitch = 0u;
uint8 ComMaster_RxSignal_StatusIgn = 0u;
uint8 ComMaster_RxSignal_StatusGb = 0u;
uint8 ComMaster_RxSignal_StatusDoorLeft = 0u;
uint8 ComMaster_RxSignal_StatusDoorRight = 0u;
uint8 ComMaster_RxSignal_StatusHighBeam = 0u;
uint8 ComMaster_RxSignal_StatusFogLights = 0u;
uint8 ComMaster_RxSignal_WiperStock = 0u;
uint8 ComMaster_RxSignal_OutsideTemperature = 0u;
uint8 ComMaster_RxSignal_TurnSignals = 0u;
uint8 ComMaster_RxSignal_StatusHc05 = 0u;
uint8 ComMaster_TxSignal_AutoClimateRequest = 0u;
uint8 ComMaster_TxSignal_ClimaTempRequest = 0u;
uint8 ComMaster_TxSignal_DcyStatus = 0u;
uint8 ComMaster_TxSignal_DisplayModeRequest = 0u;
uint8 ComMaster_TxSignal_FanValueRequest = 0u;
uint8 ComMaster_TxSignal_FogLights = 0u;
uint8 ComMaster_TxSignal_Gear = 0u;
uint8 ComMaster_TxSignal_HighBeam = 0u;
uint8 ComMaster_TxSignal_ICM_ID = 0u;
uint8 ComMaster_TxSignal_Ignition = 0u;
uint8 ComMaster_TxSignal_IrSenStat = 0u;
uint8 ComMaster_TxSignal_NM3 = 0u;
uint8 ComMaster_TxSignal_NM3_PN1 = 0u;
uint8 ComMaster_TxSignal_StatusOutTemp = 0u;
uint8 ComMaster_TxSignal_RecirculationRequest = 0u;
uint8 ComMaster_TxSignal_RlsRequest = 0u;
uint8 ComMaster_RxSignal_Rpm = 0u;
uint8 ComMaster_RxSignal_Speed = 0u;
uint8 ComMaster_TxSignal_TurnSignals = 0u;
uint8 ComMaster_TxSignal_WiperStockRequest = 0u;
uint8 ComMaster_TxSignal_VehicleStatus = 0u;
uint8 ComMaster_ActivityOnTheBus = 0u;
uint8 ComMaster_SwitchTxOff = 1u;
uint8 ComMaster_SafeDriveTrainStatusMessageState = 0u;
uint8 ComMaster_StatusDriveControlMessageState = 0u;
uint8 ComMaster_StatusActuatorMessageState = 0u;
uint8 ComMaster_E2eError_Sdts = 0u;
uint8 ComMaster_E2eSeqCnt_Sdts = 0u;
uint8 ComMaster_RxSignal_SdtsCrc = 0u;
uint8 ComMaster_RxSignal_SdtsSeqCnt = 0u;
uint8 ComMaster_RxSignal_SdtsVehSpeed = 0u;
uint8 ComMaster_RxSignal_SdtsMotorRpm = 0u;
uint8 ComMaster_RxSignal_SdtsDriveTrainStatus = 0u;
uint8 ComMaster_TxSignal_SbaCrc = 0u;
uint8 ComMaster_TxSignal_SbaSeqCnt = 0u;
uint8 ComMaster_TxSignal_SbaAssistRequestStatus = 0u;
uint8 ComMaster_TxSignal_SbaBrakeLevel = 0u;
uint8 ComMaster_TxSignal_SveCrc = 0u;
uint8 ComMaster_TxSignal_SveSeqCnt = 0u;
uint8 ComMaster_TxSignal_SveIvenSafe = 0u;
uint8 ComMaster_TxSignal_SisCrc = 0u;
uint8 ComMaster_TxSignal_SisSeqCnt = 0u;
uint8 ComMaster_TxSignal_SisIgnitionStatus = 0u;
uint8 ComMaster_CanTx_InVehicleSafetyErrorFlag = 0u;
uint8 ComMaster_HasStatusDriveControlBeenReceived = 0u;

void ComMaster_MainFunction(void);
void ComMaster_E2e_UpdateTx(Can_TxMsg_t *message, uint8 sequenceCounter);
uint8 ComMaster_E2e_CheckRx(Can_RxMsg_t *message, uint8 sequenceCounter);

void ComMaster_MainFunction(void)
{
    static uint32 timestampSwitchOffNm3s = 0u;
    static uint32 timestampActivityOnTheBus = 0u;
    static uint8 pComMaster_RxSignal_StatusIgn = 0u;
    static uint8 pComMaster_RxSignal_StatusGb = 0u;
    static uint8 debFlagExceeded = 0u;

    if(200u <= ComMaster_MainCounter)
    {
        debFlagExceeded = 1u;
    }
    else
    {
        /* Do nothing. */
    }

    if(0x00u == ComMaster_TxSignal_NM3)
    {
        ComMaster_SdcMsgStat_Cnt = 0u;
        ComMaster_SaMsgStat_Cnt = 0u;
        ComMaster_CwcMsgStat_Cnt = 0u;
        ComMaster_LsMsgStat_Cnt = 0u;
        ComMaster_SdtsMsgStat_Cnt = 0u;
    }
    else
    {
        ComMaster_SdcMsgStat_Cnt++;
        ComMaster_SaMsgStat_Cnt++;
        ComMaster_CwcMsgStat_Cnt++;
        ComMaster_LsMsgStat_Cnt++;
        ComMaster_SdtsMsgStat_Cnt++;
    }

    if(CAN_PROCESS_RXTX == Can_State)
    {
        for(uint8 i = 0; i < COMMASTER_NO_RX_MSG; i ++)
        {
            if(0u == ComMaster_ReceiveTable[i].receiveMessage.rxMsg.errorStateIndicator)
            {
                switch(ComMaster_ReceiveTable[i].receiveMessage.rxMsg.messageId)
                {
                    case 0x001u:
                        ComMaster_E2eError_Sdts = ComMaster_E2e_CheckRx(&ComMaster_ReceiveTable[i].receiveMessage, ComMaster_E2eSeqCnt_Sdts);
                        ComMaster_RxSignal_SdtsCrc = (uint8)ComMaster_ReceiveTable[i].receiveMessage.rxData[0u];
                        ComMaster_RxSignal_SdtsSeqCnt = (uint8)ComMaster_ReceiveTable[i].receiveMessage.rxData[1u];
                        ComMaster_RxSignal_SdtsVehSpeed = (uint8)ComMaster_ReceiveTable[i].receiveMessage.rxData[2u];
                        ComMaster_RxSignal_SdtsMotorRpm = (uint8)ComMaster_ReceiveTable[i].receiveMessage.rxData[3u];
                        ComMaster_RxSignal_SdtsDriveTrainStatus = (uint8)ComMaster_ReceiveTable[i].receiveMessage.rxData[4u];
                        ComMaster_E2eSeqCnt_Sdts = ComMaster_RxSignal_SdtsSeqCnt;
                        ComMaster_SdtsMsgStat_Cnt = 0u;
                        break;
                    case 0x100u:
                        ComMaster_RxSignal_StatusGb = (uint8)ComMaster_ReceiveTable[i].receiveMessage.rxData[1u];
                        ComMaster_RxSignal_StatusIgn = (uint8)ComMaster_ReceiveTable[i].receiveMessage.rxData[0u];
                        ComMaster_SdcMsgStat_Cnt = 0u;
                        ComMaster_HasStatusDriveControlBeenReceived = 1u;
                        break;
                    case 0x101u:
                        ComMaster_RxSignal_StatusDoorLeft = (uint8)ComMaster_ReceiveTable[i].receiveMessage.rxData[0u];
                        ComMaster_RxSignal_StatusDoorRight = (uint8)ComMaster_ReceiveTable[i].receiveMessage.rxData[1u];
                        ComMaster_RxSignal_StatusHc05 = (uint8)ComMaster_ReceiveTable[i].receiveMessage.rxData[2u];
                        ComMaster_SaMsgStat_Cnt = 0u;
                        break;
                    case 0x108u:
                        ComMaster_RxSignal_StatusFogLights = (uint8)ComMaster_ReceiveTable[i].receiveMessage.rxData[0u];
                        ComMaster_RxSignal_StatusHighBeam = (uint8)ComMaster_ReceiveTable[i].receiveMessage.rxData[1u];
                        ComMaster_RxSignal_TurnSignals = (uint8)ComMaster_ReceiveTable[i].receiveMessage.rxData[2u];
                        ComMaster_LsMsgStat_Cnt = 0u;
                        break;
                    case 0x10Au:
                        ComMaster_RxSignal_AutoClimate = (uint8)ComMaster_ReceiveTable[i].receiveMessage.rxData[0u];
                        ComMaster_RxSignal_ClimateTemperature = (uint8)ComMaster_ReceiveTable[i].receiveMessage.rxData[1u];
                        ComMaster_RxSignal_DisplayMode = (uint8)ComMaster_ReceiveTable[i].receiveMessage.rxData[2u];
                        ComMaster_RxSignal_FanValue = (uint8)ComMaster_ReceiveTable[i].receiveMessage.rxData[3u];
                        ComMaster_RxSignal_Recirculation = (uint8)ComMaster_ReceiveTable[i].receiveMessage.rxData[4u];
                        ComMaster_RxSignal_RotaryLightSwitch = (uint8)ComMaster_ReceiveTable[i].receiveMessage.rxData[5u];
                        ComMaster_RxSignal_WiperStock = (uint8)ComMaster_ReceiveTable[i].receiveMessage.rxData[6u];
                        ComMaster_RxSignal_OutsideTemperature = (uint8)ComMaster_ReceiveTable[i].receiveMessage.rxData[7u];
                        ComMaster_CwcMsgStat_Cnt = 0u;
                        break;
                    case 0x10Cu:
                        ComMaster_RxSignal_Err701_ID = (uint8)ComMaster_ReceiveTable[i].receiveMessage.rxData[0u];
                        break;
                    default:
                        ComMaster_ReceiveTable[i].receivedValidFlag = 0xFFU;
                        break;
                }
            }
            else
            {
                ComMaster_ReceiveTable[i].receivedValidFlag = 0xFFU;
            }
            ComMaster_ReceiveTable[i].receiveMessage.rxMsg.messageId = 0u;
        }
    }
    else
    {
        /* Do nothing. */
    }

    if(ComMaster_RxSignal_SdtsDriveTrainStatus != 253u)
    {
        ComMaster_RxSignal_Speed = ComMaster_RxSignal_SdtsVehSpeed;
        ComMaster_RxSignal_Rpm = ComMaster_RxSignal_SdtsMotorRpm;
    }
    else
    {
        ComMaster_RxSignal_Speed = 0u;
        ComMaster_RxSignal_Rpm = 0u;
    }

    if(200u < ComMaster_SdtsMsgStat_Cnt && 1u == ComMaster_HasStatusDriveControlBeenReceived)
    {
        ComMaster_SafeDriveTrainStatusMessageState = 253u;
        Dem_SetDtc(COMMASTER_DTC_ID_MESSAGE_0X001_MISSING, 1u, 32u);
        ComMaster_RxSignal_Speed = 0u;
        ComMaster_RxSignal_Rpm = 0u;
    }
    else
    {
        ComMaster_SafeDriveTrainStatusMessageState = 0u;
        Dem_SetDtc(COMMASTER_DTC_ID_MESSAGE_0X100_MISSING, 0u, 32u);
    }

    if(200u < ComMaster_SdcMsgStat_Cnt)
    {
        ComMaster_StatusDriveControlMessageState = 253u;
        Dem_SetDtc(COMMASTER_DTC_ID_MESSAGE_0X100_MISSING, 1u, 7u);
        ComMaster_RxSignal_StatusGb = 0u;
        ComMaster_RxSignal_StatusIgn = 0u;
    }
    else
    {
        ComMaster_StatusDriveControlMessageState = 0u;
        Dem_SetDtc(COMMASTER_DTC_ID_MESSAGE_0X100_MISSING, 0u, 7u);
    }

    if(200u < ComMaster_SaMsgStat_Cnt && 1u == ComMaster_HasStatusDriveControlBeenReceived)
    {
        ComMaster_StatusActuatorMessageState = 253u;
        Dem_SetDtc(COMMASTER_DTC_ID_MESSAGE_0X101_MISSING, 1u, 6u);
        ComMaster_RxSignal_StatusDoorLeft = 0u;
        ComMaster_RxSignal_StatusDoorRight = 0u;
        ComMaster_RxSignal_StatusHc05 = 0u;
    }
    else
    {
        ComMaster_StatusActuatorMessageState = 0u;
        Dem_SetDtc(COMMASTER_DTC_ID_MESSAGE_0X101_MISSING, 0u, 6u);
    }

    if(200u < ComMaster_CwcMsgStat_Cnt && 1u == ComMaster_HasStatusDriveControlBeenReceived)
    {
        Dem_SetDtc(COMMASTER_DTC_ID_MESSAGE_0X10A_MISSING, 1u, 1u);
        ComMaster_RxSignal_AutoClimate = 0u;
        ComMaster_RxSignal_ClimateTemperature = 0u;
        ComMaster_RxSignal_DisplayMode = 0u;
        ComMaster_RxSignal_FanValue = 0u;
        ComMaster_RxSignal_Recirculation = 0u;
        ComMaster_RxSignal_RotaryLightSwitch = 0u;
        ComMaster_RxSignal_WiperStock = 0u;
        ComMaster_RxSignal_OutsideTemperature = 0u;
    }
    else
    {
        Dem_SetDtc(COMMASTER_DTC_ID_MESSAGE_0X10A_MISSING, 0u, 1u);
    }

    if(200u < ComMaster_LsMsgStat_Cnt && 1u == ComMaster_HasStatusDriveControlBeenReceived)
    {
        Dem_SetDtc(COMMASTER_DTC_ID_MESSAGE_0X108_MISSING, 1u, 2u);
        ComMaster_RxSignal_StatusFogLights = 0u;
        ComMaster_RxSignal_StatusHighBeam = 0u;
        ComMaster_RxSignal_TurnSignals = 0u;
    }
    else
    {
        Dem_SetDtc(COMMASTER_DTC_ID_MESSAGE_0X108_MISSING, 0u, 2u);
    }

    if(1u < ComMaster_RxSignal_AutoClimate && 1u == debFlagExceeded)
    {
        ComMaster_RxSignal_AutoClimate = 253u; // invalid value
        ComMaster_TxSignal_AutoClimateRequest = 0u; // default value
        Dem_SetDtc(COMMASTER_DTC_ID_SIGNAL_AUTOCLIMATE_INVALID, 1u, 9u);
    }
    else
    {
        ComMaster_TxSignal_AutoClimateRequest = ComMaster_RxSignal_AutoClimate;
        Dem_SetDtc(COMMASTER_DTC_ID_SIGNAL_AUTOCLIMATE_INVALID, 0u, 9u);
    }

    if(16u < ComMaster_RxSignal_ClimateTemperature && 1u == debFlagExceeded)
    {
        ComMaster_RxSignal_ClimateTemperature = 253u; // invalid value
        ComMaster_TxSignal_ClimaTempRequest = 6u; // default value
        Dem_SetDtc(COMMASTER_DTC_ID_SIGNAL_CLIMATETEMPERATURE_INVALID, 1u, 10u);
    }
    else
    {
        ComMaster_TxSignal_ClimaTempRequest = ComMaster_RxSignal_ClimateTemperature;
        Dem_SetDtc(COMMASTER_DTC_ID_SIGNAL_CLIMATETEMPERATURE_INVALID, 0u, 10u);
    }

    if(3u < ComMaster_RxSignal_DisplayMode && 1u == debFlagExceeded)
    {
        ComMaster_RxSignal_DisplayMode = 253u; // invalid value
        ComMaster_TxSignal_DisplayModeRequest = 0u; // default value
        Dem_SetDtc(COMMASTER_DTC_ID_SIGNAL_DISPLAYMODEREQUEST_INVALID, 1u, 11u);
    }
    else
    {
        ComMaster_TxSignal_DisplayModeRequest = ComMaster_RxSignal_DisplayMode;
        Dem_SetDtc(COMMASTER_DTC_ID_SIGNAL_DISPLAYMODEREQUEST_INVALID, 0u, 11u);
    }

    if(8u < ComMaster_RxSignal_FanValue && 1u == debFlagExceeded)
    {
        ComMaster_RxSignal_FanValue = 253u; // invalid value
        ComMaster_TxSignal_FanValueRequest = 0u; // default value
        Dem_SetDtc(COMMASTER_DTC_ID_SIGNAL_FANVALUE_INVALID, 1u, 12u);
    }
    else
    {
        ComMaster_TxSignal_FanValueRequest = ComMaster_RxSignal_FanValue;
        Dem_SetDtc(COMMASTER_DTC_ID_SIGNAL_FANVALUE_INVALID, 0u, 12u);
    }

    if(3u < ComMaster_RxSignal_Recirculation && 1u == debFlagExceeded)
    {
        ComMaster_RxSignal_Recirculation = 253u; // invalid value
        ComMaster_TxSignal_RecirculationRequest = 0u; // default value
        Dem_SetDtc(COMMASTER_DTC_ID_SIGNAL_RECIRCULATION_INVALID, 1u, 16u);
    }
    else
    {
        ComMaster_TxSignal_RecirculationRequest = ComMaster_RxSignal_Recirculation;
        Dem_SetDtc(COMMASTER_DTC_ID_SIGNAL_RECIRCULATION_INVALID, 0u, 16u);
    }

    if(3u < ComMaster_RxSignal_RotaryLightSwitch && 1u == debFlagExceeded)
    {
        ComMaster_RxSignal_RotaryLightSwitch = 253u; // invalid value
        ComMaster_TxSignal_RlsRequest = 0u; // default value
        Dem_SetDtc(COMMASTER_DTC_ID_SIGNAL_ROTARYLIGHTSWITCH_INVALID, 1u, 17u);
    }
    else
    {
        ComMaster_TxSignal_RlsRequest = ComMaster_RxSignal_RotaryLightSwitch;
        Dem_SetDtc(COMMASTER_DTC_ID_SIGNAL_ROTARYLIGHTSWITCH_INVALID, 0u, 17u);
    }

    if(3u < ComMaster_RxSignal_StatusGb && 1u == debFlagExceeded)
    {
        ComMaster_RxSignal_StatusGb = 253u;
        ComMaster_TxSignal_Gear = 253u;
        Dem_SetDtc(COMMASTER_DTC_ID_SIGNAL_STATUSGEARBOX_INVALID, 1u, 22u);
    }
    else
    {
        if((pComMaster_RxSignal_StatusGb != ComMaster_RxSignal_StatusGb)  &&
                (ComMaster_RxSignal_StatusGb != 253u) && 1u == debFlagExceeded)
        {
            pComMaster_RxSignal_StatusGb = ComMaster_RxSignal_StatusGb;
        }
        else
        {
            /* Do nothing. */
        }

        if(ComMaster_TxSignal_Ignition != 2u)
        {
            ComMaster_TxSignal_Gear = 0u;
        }
        else
        {
            ComMaster_TxSignal_Gear = ComMaster_RxSignal_StatusGb;
        }
    }

    if(4u < ComMaster_RxSignal_StatusDoorLeft && 1u == debFlagExceeded)
    {
        ComMaster_RxSignal_StatusDoorLeft = 0u;
        Dem_SetDtc(COMMASTER_DTC_ID_SIGNAL_STATUSDOORLEFT_INVALID, 1u, 23u);
    }
    else
    {
        Dem_SetDtc(COMMASTER_DTC_ID_SIGNAL_STATUSDOORLEFT_INVALID, 0u, 23u);
    }

    if(4u < ComMaster_RxSignal_StatusDoorRight && 1u == debFlagExceeded)
    {
        ComMaster_RxSignal_StatusDoorRight = 0u;
        Dem_SetDtc(COMMASTER_DTC_ID_SIGNAL_STATUSDOORRIGHT_INVALID, 1u, 24u);
    }
    else
    {
        Dem_SetDtc(COMMASTER_DTC_ID_SIGNAL_STATUSDOORRIGHT_INVALID, 0u, 24u);
    }

    if(4u < ComMaster_RxSignal_StatusHc05 && 1u == debFlagExceeded)
    {
        ComMaster_RxSignal_StatusHc05 = 0u;
    }
    else
    {
        /* Do nothing. */
    }

    if(1u < ComMaster_RxSignal_StatusHighBeam && 1u == debFlagExceeded)
    {
        ComMaster_RxSignal_StatusHighBeam = 253u;
        ComMaster_TxSignal_HighBeam = 0u;
        Dem_SetDtc(COMMASTER_DTC_ID_SIGNAL_STATUSHIGHBEAM_INVALID, 1u, 25u);
    }
    else
    {
        ComMaster_TxSignal_HighBeam = ComMaster_RxSignal_StatusHighBeam;
        Dem_SetDtc(COMMASTER_DTC_ID_SIGNAL_STATUSHIGHBEAM_INVALID, 0u, 25u);
    }

    if(3u < ComMaster_RxSignal_StatusFogLights && 1u == debFlagExceeded)
    {
        ComMaster_RxSignal_StatusFogLights = 253u;
        ComMaster_TxSignal_FogLights = 0u;
        Dem_SetDtc(COMMASTER_DTC_ID_SIGNAL_STATUSFOGLIGHTS_INVALID, 1u, 26u);
    }
    else
    {
        ComMaster_TxSignal_FogLights = ComMaster_RxSignal_StatusFogLights;
        Dem_SetDtc(COMMASTER_DTC_ID_SIGNAL_STATUSFOGLIGHTS_INVALID, 0u, 26u);
    }

    if(4u < ComMaster_RxSignal_WiperStock && 1u == debFlagExceeded)
    {
        ComMaster_RxSignal_WiperStock = 253u;
        ComMaster_TxSignal_WiperStockRequest = 0u;
        Dem_SetDtc(COMMASTER_DTC_ID_SIGNAL_STATUSWIPERSTOCK_INVALID, 1u, 27u);
    }
    else
    {
        ComMaster_TxSignal_WiperStockRequest = ComMaster_RxSignal_WiperStock;
        Dem_SetDtc(COMMASTER_DTC_ID_SIGNAL_STATUSWIPERSTOCK_INVALID, 0u, 27u);
    }

    if(99u < ComMaster_RxSignal_OutsideTemperature && 1u == debFlagExceeded)
    {
        ComMaster_RxSignal_OutsideTemperature = 253u;
        ComMaster_TxSignal_StatusOutTemp = 0u;
        Dem_SetDtc(COMMASTER_DTC_ID_SIGNAL_STATUSOUTSIDETEMPERATURE_INVALID, 1u, 28u);
    }
    else
    {
        ComMaster_TxSignal_StatusOutTemp = ComMaster_RxSignal_OutsideTemperature;
        Dem_SetDtc(COMMASTER_DTC_ID_SIGNAL_STATUSOUTSIDETEMPERATURE_INVALID, 0u, 28u);
    }

    if(3u < ComMaster_RxSignal_TurnSignals && 1u == debFlagExceeded)
    {
        ComMaster_RxSignal_TurnSignals = 253u;
        ComMaster_TxSignal_TurnSignals = 0u;
        Dem_SetDtc(COMMASTER_DTC_ID_SIGNAL_STATUSTURNSIGNALS_INVALID, 1u, 29u);
    }
    else
    {
        ComMaster_TxSignal_TurnSignals = ComMaster_RxSignal_TurnSignals;
        Dem_SetDtc(COMMASTER_DTC_ID_SIGNAL_STATUSTURNSIGNALS_INVALID, 0u, 29u);
    }

    if(2u < ComMaster_RxSignal_StatusIgn && 1u == debFlagExceeded)
    {
        ComMaster_RxSignal_StatusIgn = 253u;
        Dem_SetDtc(COMMASTER_DTC_ID_SIGNAL_STATUSIGNITION_INVALID, 1u, 21u);
        ComMaster_TxSignal_Ignition = 253u;
    }
    else
    {
        ComMaster_TxSignal_Ignition = ComMaster_RxSignal_StatusIgn;

        if(pComMaster_RxSignal_StatusIgn != ComMaster_RxSignal_StatusIgn &&
                253u != ComMaster_RxSignal_StatusIgn && 1u == debFlagExceeded)
        {
            pComMaster_RxSignal_StatusIgn = ComMaster_RxSignal_StatusIgn;
        }
        else
        {
            /* Do nothing. */
        }
    }

    if(8u == ComMaster_TxSignal_VehicleStatus)
    {
        if(253u == ComMaster_TxSignal_Ignition)
        {
            ComMaster_TxSignal_Ignition = 254u;
        }
        else if(1u > ComMaster_TxSignal_Ignition)
        {
            ComMaster_TxSignal_Ignition = 1u;
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

    switch(ComMaster_TxSignal_Ignition)
    {
        case 2u:
            ComMaster_TxSignal_NM3 = 0x10u;
            ComMaster_TxSignal_NM3_PN1 = 0x11u;
            timestampSwitchOffNm3s = 0u;
            break;
        case 1u:
        case 254u:
            ComMaster_TxSignal_NM3 = 0x10u;
            ComMaster_TxSignal_NM3_PN1 = 0x11u;
            timestampSwitchOffNm3s = 0u;
            break;
        case 0u:
        case 253u:
            ComMaster_TxSignal_NM3 = 0x10u;
            ComMaster_TxSignal_NM3_PN1 = 0x11u;

            if((0u == ComMaster_RxSignal_StatusDoorLeft || 0u == ComMaster_RxSignal_StatusDoorRight) && 0u == ComMaster_RxSignal_StatusHc05)
            {
                ComMaster_TxSignal_NM3_PN1 = 0x00u;

                if(0u == timestampSwitchOffNm3s)
                {
                    timestampSwitchOffNm3s = ComMaster_MainCounter;
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

            if(20u < ComMaster_MainCounter - timestampSwitchOffNm3s && timestampSwitchOffNm3s != 0u)
            {
                ComMaster_TxSignal_NM3 = 0x00u;
            }
            else
            {
                /* Do nothing. */
            }

            break;
        default:
            break;
    }

    if(1u == Can_ActivityOnTheBus)
    {
        timestampActivityOnTheBus = 0u;
        Can_ActivityOnTheBus = 0u;
        ComMaster_ActivityOnTheBus = 1u;
        SysMgr_NoBusActivity = ComMaster_ActivityOnTheBus;

        if(1u == Dcm_SwitchTxOff)
        {
            ComMaster_SwitchTxOff = 0u;
        }
        else
        {
            ComMaster_SwitchTxOff = 1u;
        }
    }
    else
    {
        if(0u == timestampActivityOnTheBus)
        {
            timestampActivityOnTheBus = ComMaster_MainCounter;
        }
        else
        {
            /* Do nothing. */
        }

        if(40u < ComMaster_MainCounter - timestampActivityOnTheBus && 0u != timestampActivityOnTheBus)
        {
            if(0x00u == ComMaster_TxSignal_NM3 && 0x00u == ComMaster_TxSignal_NM3_PN1)
            {
                ComMaster_SwitchTxOff = 0u;
                ComMaster_ActivityOnTheBus = 0u;
                SysMgr_NoBusActivity = ComMaster_ActivityOnTheBus;
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

    ComMaster_TxSignal_SbaAssistRequestStatus = ComMaster_TxSignal_IrSenStat;
    ComMaster_TxSignal_SveIvenSafe = ComMaster_CanTx_InVehicleSafetyErrorFlag;
    ComMaster_TxSignal_SisIgnitionStatus = ComMaster_TxSignal_Ignition;

    ComMaster_TransmitTable[0u].transmitMessage.txData[0u] = ComMaster_TxSignal_Ignition;
    ComMaster_TransmitTable[0u].transmitMessage.txData[1u] = ComMaster_TxSignal_Gear;
    ComMaster_TransmitTable[0u].transmitMessage.txData[2u] = ComMaster_TxSignal_IrSenStat;
    ComMaster_TransmitTable[0u].transmitMessage.txData[3u] = ComMaster_RxSignal_Rpm;
    ComMaster_TransmitTable[0u].transmitMessage.txData[4u] = ComMaster_RxSignal_Speed;
    ComMaster_TransmitTable[0u].transmitMessage.txData[5u] = ComMaster_TxSignal_VehicleStatus;
    ComMaster_TransmitTable[0u].transmitMessage.txMsg.dataLengthCode = IfxCan_DataLengthCode_6;

    ComMaster_TransmitTable[1u].transmitMessage.txData[0u] = ComMaster_TxSignal_RecirculationRequest;
    ComMaster_TransmitTable[1u].transmitMessage.txData[1u] = ComMaster_TxSignal_FogLights;
    ComMaster_TransmitTable[1u].transmitMessage.txData[2u] = ComMaster_TxSignal_HighBeam;
    ComMaster_TransmitTable[1u].transmitMessage.txData[3u] = ComMaster_TxSignal_StatusOutTemp;
    ComMaster_TransmitTable[1u].transmitMessage.txData[4u] = ComMaster_TxSignal_TurnSignals;
    ComMaster_TransmitTable[1u].transmitMessage.txData[5u] = ComMaster_TxSignal_AutoClimateRequest;
    ComMaster_TransmitTable[1u].transmitMessage.txMsg.dataLengthCode = IfxCan_DataLengthCode_6;

    ComMaster_TransmitTable[2u].transmitMessage.txData[0u] = ComMaster_TxSignal_FanValueRequest;
    ComMaster_TransmitTable[2u].transmitMessage.txData[1u] = ComMaster_TxSignal_DisplayModeRequest;
    ComMaster_TransmitTable[2u].transmitMessage.txData[2u] = ComMaster_TxSignal_ClimaTempRequest;
    ComMaster_TransmitTable[2u].transmitMessage.txData[3u] = ComMaster_TxSignal_RlsRequest;
    ComMaster_TransmitTable[2u].transmitMessage.txData[4u] = ComMaster_TxSignal_WiperStockRequest;
    ComMaster_TransmitTable[2u].transmitMessage.txMsg.dataLengthCode = IfxCan_DataLengthCode_5;

    if(0u != ComMaster_TxSignal_ICM_ID)
    {
        ComMaster_TransmitTable[3u].transmitMessage.txData[0u] = ComMaster_TxSignal_ICM_ID;
        ComMaster_TransmitTable[3u].transmitMessage.txMsg.dataLengthCode = IfxCan_DataLengthCode_1;
    }
    else
    {
        /* Do nothing. */
    }

    ComMaster_TransmitTable[5u].transmitMessage.txData[0u] = ComMaster_TxSignal_NM3;
    ComMaster_TransmitTable[5u].transmitMessage.txData[1u] = ComMaster_TxSignal_NM3_PN1;
    ComMaster_TransmitTable[5u].transmitMessage.txMsg.dataLengthCode = IfxCan_DataLengthCode_2;

    ComMaster_TxSignal_SisSeqCnt++;
    ComMaster_TxSignal_SbaSeqCnt++;
    ComMaster_TxSignal_SveSeqCnt++;

    ComMaster_TransmitTable[6u].transmitMessage.txData[0u] = ComMaster_TxSignal_SisCrc;
    ComMaster_TransmitTable[6u].transmitMessage.txData[1u] = ComMaster_TxSignal_SisSeqCnt;
    ComMaster_TransmitTable[6u].transmitMessage.txData[2u] = ComMaster_TxSignal_SisIgnitionStatus;
    ComMaster_TransmitTable[6u].transmitMessage.txMsg.dataLengthCode = IfxCan_DataLengthCode_3;

    ComMaster_E2e_UpdateTx(&ComMaster_TransmitTable[6u].transmitMessage, ComMaster_TxSignal_SisSeqCnt);

    ComMaster_TransmitTable[7u].transmitMessage.txData[0u] = ComMaster_TxSignal_SveCrc;
    ComMaster_TransmitTable[7u].transmitMessage.txData[1u] = ComMaster_TxSignal_SveSeqCnt;
    ComMaster_TransmitTable[7u].transmitMessage.txData[2u] = ComMaster_TxSignal_SveIvenSafe;
    ComMaster_TransmitTable[7u].transmitMessage.txMsg.dataLengthCode = IfxCan_DataLengthCode_3;

    ComMaster_E2e_UpdateTx(&ComMaster_TransmitTable[7u].transmitMessage, ComMaster_TxSignal_SveSeqCnt);

    ComMaster_TransmitTable[8u].transmitMessage.txData[0u] = ComMaster_TxSignal_SbaCrc;
    ComMaster_TransmitTable[8u].transmitMessage.txData[1u] = ComMaster_TxSignal_SbaSeqCnt;
    ComMaster_TransmitTable[8u].transmitMessage.txData[2u] = ComMaster_TxSignal_SbaAssistRequestStatus;
    ComMaster_TransmitTable[8u].transmitMessage.txData[2u] = ComMaster_TxSignal_SbaBrakeLevel;
    ComMaster_TransmitTable[8u].transmitMessage.txMsg.dataLengthCode = IfxCan_DataLengthCode_4;

    ComMaster_E2e_UpdateTx(&ComMaster_TransmitTable[8u].transmitMessage, ComMaster_TxSignal_SbaSeqCnt);

    if((ComMaster_MainCounter) != 0u && (ComMaster_SwitchTxOff == 1u))
    {
        if(ComMaster_MainCounter % ComMaster_TransmitTable[0u].cycleTime == 0u)
        {
            ComMaster_TransmitTable[0u].transmitFlag = 1u;
        }
        else
        {
            /* Do nothing. */
        }

        if(ComMaster_MainCounter % ComMaster_TransmitTable[1u].cycleTime == 0u)
        {
            ComMaster_TransmitTable[1u].transmitFlag = 1u;
        }
        else
        {
            /* Do nothing. */
        }

        if(ComMaster_MainCounter % ComMaster_TransmitTable[2u].cycleTime == 0u)
        {
            ComMaster_TransmitTable[2u].transmitFlag = 1u;
        }
        else
        {
            /* Do nothing. */
        }

        if(ComMaster_MainCounter % ComMaster_TransmitTable[5u].cycleTime == 0u)
        {
            ComMaster_TransmitTable[5u].transmitFlag = 1u;
        }
        else
        {
            /* Do nothing. */
        }

        if(ComMaster_MainCounter % ComMaster_TransmitTable[6u].cycleTime == 0u)
        {
            ComMaster_TransmitTable[6u].transmitFlag = 1u;
        }
        else
        {
            /* Do nothing. */
        }

        if(ComMaster_MainCounter % ComMaster_TransmitTable[7u].cycleTime == 0u)
        {
            ComMaster_TransmitTable[7u].transmitFlag = 1u;
        }
        else
        {
            /* Do nothing. */
        }

        if(ComMaster_MainCounter % ComMaster_TransmitTable[8u].cycleTime == 0u)
        {
            ComMaster_TransmitTable[8u].transmitFlag = 1u;
        }
        else
        {
            /* Do nothing. */
        }

        if(ComMaster_MainCounter % ComMaster_TransmitTable[3u].cycleTime == 0u)
        {
            ComMaster_TransmitTable[3u].transmitFlag = 1u;
        }
        else
        {
            /* Do nothing. */
        }

        if(ComMaster_MainCounter % ComMaster_TransmitTable[4u].cycleTime == 0u)
        {
            ComMaster_TransmitTable[4u].transmitFlag = 1u;
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

    //IfxCpu_disableInterrupts();
    memcpy(Can_TransmitTable, ComMaster_TransmitTable, sizeof(ComMaster_TransmitTable));
    for(uint8 i = 0; i < 7u; i++)
    {
        ComMaster_TransmitTable[i].transmitFlag = 0u;
        for(uint8 j = 0; j < 8u; j++)
        {
            ComMaster_TransmitTable[i].transmitMessage.txData[j] = 0u;
        }
    }
    //IfxCpu_enableInterrupts();
    ComMaster_MainCounter++;
}

void ComMaster_E2e_UpdateTx(Can_TxMsg_t *message, uint8 sequenceCounter)
{
    uint8 calculatedCrc = 0u;

    message->txData[1u] = sequenceCounter;
    calculatedCrc = (uint8)Crc_Calculate((uint32*) message->txData, 2u, 0u);
    message->txData[0u] = calculatedCrc;
}

uint8 ComMaster_E2e_CheckRx(Can_RxMsg_t *message, uint8 sequenceCounter)
{
    uint8 receivedCrc = 0u;
    uint8 seqCnt = 0u;
    uint8 calculatedCrc = 0u;
    uint8 retVal = 0u;

    receivedCrc = (uint8)message->rxData[0u];
    seqCnt = (uint8)message->rxData[1u];
    calculatedCrc = (uint8)Crc_Calculate((uint32*)message->rxData, 2u, 0u);

    if (receivedCrc == calculatedCrc && seqCnt == sequenceCounter) retVal = 0u;
    else retVal = 1u;

    return retVal;
}
