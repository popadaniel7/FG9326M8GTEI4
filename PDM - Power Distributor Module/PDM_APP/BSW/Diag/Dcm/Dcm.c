#include "Dcm.h"
#include "CanH.h"
#include "can.h"
#include "Dem.h"
#include <string.h>
#define DCM_CAN_ID_TX 0x703
#define DCM_CAN_ID_RX 0x702
#define DCM_DATA_SIZE (DEM_NUMBER_OF_DTCS * sizeof(Dem_DTC_t))
#define DCM_MAX_CAN_DATA_LEN 8
typedef struct
{
	uint8 SoftwareVersion;
	uint8 FlashBootLoaderVersion;
	uint8 CodingVersion;
}SWV;
/* DIAGNOSTIC SESSION DATA TYPE */
typedef enum
{
	DEFAULT = 0U,
	EXTENDED = 1U,
	PROGRAMMING = 2U,
	CODING = 3U,
	CALIBRATION = 4U,
	HARDRESET = 5U,
	SOFTRESET = 6U,
	JUMPTOAPPL = 7U
}DiagState;
typedef void (*Dcm_FuncPtr)();
SWV Dcm_SWVersion =
{
		0x01, /* SW */
		0x01, /* FBL */
		0x01, /* CODING */
};

uint32 Dcm_MainCounter = 0x00;
CAN_TxHeaderTypeDef Dcm_TxHeader = {0, 0, 0, 0, 0, 0};
uint8 Dcm_TxData[8] = {0};
uint32 Dcm_TxMailbox = 0;
uint8 Dcm_CF_Step = 0;
uint8 Dcm_CFRDTCI_ActiveFlag = 0;
uint32 Dcm_DiagnosticCalibrationData[5];
extern Dem_DTC_t Dem_DTCArray[DEM_NUMBER_OF_DTCS];
extern CAN_HandleTypeDef hcan;
extern uint8 CanH_DiagArray[8];
extern CAN_RxHeaderTypeDef CanH_DiagRxHeader;
extern uint8 CanH_NoCommCounter;
extern CanH_ComStat_t CanH_CommunicationState;
extern uint8 CanH_RequestBusSleep;
extern uint8 Dem_DTCSettingDeactivated;
extern uint8 LoadCtrl_StatusList[8];
extern uint8 DiagLoadCtrl_StatusList[8];
__attribute__((section(".ncr"))) DiagState Dcm_DiagnosticSession;
__attribute__((section(".ncr"))) uint32 Dcm_AliveCounter;
__attribute__((section(".ncr"))) extern uint32 LoadCtrl_I2tCounter;
__attribute__((section(".ncr"))) extern uint8 LoadCtrl_SwitchOffFlag;
__attribute__((section(".ncr"))) extern uint8 LoadCtrl_PermanentSwitchOff_Rin2;
__attribute__((section(".ncr"))) extern uint8 LoadCtrl_PermanentSwitchOff_Rin3;
__attribute__((section(".ncr"))) extern uint8 LoadCtrl_PermanentSwitchOff_Rin4;
__attribute__((section(".ncr"))) extern uint8 LoadCtrl_Rin2Counter;
__attribute__((section(".ncr"))) extern uint8 LoadCtrl_Rin3Counter;
__attribute__((section(".ncr"))) extern uint8 LoadCtrl_Rin4Counter;
__attribute__((section(".ncr"))) extern uint8 LoadCtrl_LockRin2;
__attribute__((section(".ncr"))) extern uint8 LoadCtrl_LockRin3;
__attribute__((section(".ncr"))) extern uint8 LoadCtrl_LockRin4;

void Dcm_MainFunction(void);
void Dcm_TxIsoTp(uint8 *data, uint16 size);
void DiagService_DSC_DefaultSession(void);
void DiagService_DSC_ExtendedSession(void);
void DiagService_DSC_ProgrammingSession(void);
void DiagService_DSC_CalibrationSession(void);
void DiagService_ER_HardReset(void);
void DiagService_TP_TesterPresent(void);
void DiagService_CDTCI_ClearDiagnosticInformation(void);
void DiagService_RDTCI_ReadDTCInformationSupportedDtc(void);
void DiagService_RDBI_ReadSWData(void);
void DiagService_RDBI_ReadActiveDiagnosticSession(void);
void DiagService_RDBI_ReadAliveTime(void);
void DiagService_CC_CommunicationControl(void);
void DiagService_CDTCS_ControlDTCSetting(void);
void DiagService_WDBI_CalibrationData(void);
void DiagService_RC_LoadControl_Status(void);
void DiagService_RC_LoadControl_RestoreLoadsStatus(void);
extern void EcuM_PerformReset(uint8 param);
extern void Dem_ClearDtc(void);
extern void NvM_WriteAll(void);

void Dcm_MainFunction(void)
{
	Dcm_AliveCounter++;
	if(Dcm_MainCounter == 0) Dcm_DiagnosticSession = DEFAULT;
	else
	{
		/* Do nothing. */
	}
	if(0x3E == CanH_DiagArray[1]) DiagService_TP_TesterPresent();
	else
	{
		/* Do nothing. */
	}
	if(0x28 == CanH_DiagArray[1]) DiagService_CC_CommunicationControl();
	else
	{
		/* Do nothing. */
	}
	if(0x85 == CanH_DiagArray[1]) DiagService_CDTCS_ControlDTCSetting();
	else
	{
		/* Do nothing. */
	}
	if(CanH_DiagArray[1] == 0x10)
	{
		/* DEFAULT */
		if(CanH_DiagArray[2] == 0x01) DiagService_DSC_DefaultSession();
		else
		{
			/* Do nothing. */
		}
		/* EXTENDED */
		if(CanH_DiagArray[2] == 0x03) DiagService_DSC_ExtendedSession();
		else
		{
			/* Do nothing. */
		}
		/* PROGRAMMING */
		if(CanH_DiagArray[2] == 0x02 && Dcm_DiagnosticSession == EXTENDED) DiagService_DSC_ProgrammingSession();
		else
		{
			/* Do nothing. */
		}
		if(CanH_DiagArray[2] == 0x05 && Dcm_DiagnosticSession == EXTENDED) DiagService_DSC_CalibrationSession();
		else
		{
			/* Do nothing. */
		}
	}
	else
	{
		/* Do nothing. */
	}
	if(CanH_DiagArray[3] == 0x86 && CanH_DiagArray[2] == 0xF1 && CanH_DiagArray[1] == 0x22) DiagService_RDBI_ReadActiveDiagnosticSession();
	else
	{
		/* Do nothing. */
	}
	/* Execute hard reset. */
	if(CanH_DiagArray[1] == 0x11 && CanH_DiagArray[2] == 0x01) DiagService_ER_HardReset();
	else
	{
		/* Do nothing. */
	}
	/* Execute clear DTC. */
	if(CanH_DiagArray[1] == 0x14) DiagService_CDTCI_ClearDiagnosticInformation();
	else
	{
		/* Do nothing. */
	}
	/* Execute read DTC. */
	if(CanH_DiagArray[1] == 0x19) DiagService_RDTCI_ReadDTCInformationSupportedDtc();
	else
	{
		/* Do nothing. */
	}
	/* Extended session requests. */
	if(Dcm_DiagnosticSession == EXTENDED)
	{
		if(0x31 == CanH_DiagArray[1] && 0x01 == CanH_DiagArray[2])
		{
			if(0x4f == CanH_DiagArray[3] && 0x4f == CanH_DiagArray[4]) DiagService_RC_LoadControl_Status();
			else
			{
				/* Do nothing. */
			}
			if(0x4e == CanH_DiagArray[3] && 0x4e == CanH_DiagArray[4]) DiagService_RC_LoadControl_RestoreLoadsStatus();
			else
			{
				/* Do nothing. */
			}
		}
		else
		{
			/* Do nothing. */
		}
		if(0x2E == CanH_DiagArray[1]) DiagService_WDBI_CalibrationData();
		else
		{
			/* Do nothing. */
		}
		/* Read data requests. */
		if(CanH_DiagArray[1] == 0x22)
		{
			if(CanH_DiagArray[2] == 0x00)
			{
				if(CanH_DiagArray[3] == 0x05) DiagService_RDBI_ReadAliveTime();
				else
				{
					/* Do nothing. */
				}
			}
			else
			{
				/* Do nothing. */
			}

			if(CanH_DiagArray[2] == 0xF1)
			{
				if(CanH_DiagArray[3] == 0x80) DiagService_RDBI_ReadSWData();
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
	else
	{
		/* Do nothing. */
	}
	Dcm_TxHeader.DLC = 0;
	Dcm_TxHeader.ExtId = 0;
	Dcm_TxHeader.IDE = 0;
	Dcm_TxHeader.RTR = 0;
	Dcm_TxHeader.StdId = 0;
	CanH_DiagRxHeader.DLC = 0;
	CanH_DiagRxHeader.ExtId = 0;
	CanH_DiagRxHeader.FilterMatchIndex = 0;
	CanH_DiagRxHeader.IDE = 0;
	CanH_DiagRxHeader.RTR = 0;
	CanH_DiagRxHeader.StdId = 0;
	CanH_DiagRxHeader.Timestamp = 0;
	for(uint8 i = 0; i < 8; i++)
	{
		Dcm_TxData[i] = 0;
		CanH_DiagArray[i] = 0;
	}
	Dcm_MainCounter++;
}
void DiagService_WDBI_CalibrationData(void)
{
	Dcm_TxHeader.DLC = CanH_DiagRxHeader.DLC;
	Dcm_TxHeader.StdId = CanH_DiagRxHeader.StdId + 1;
	Dcm_TxData[0] = CanH_DiagArray[0];
	Dcm_TxData[1] = CanH_DiagArray[1] + 0x40;
	Dcm_TxData[2] = CanH_DiagArray[2];
	Dcm_TxData[3] = CanH_DiagArray[3];
	Dcm_DiagnosticCalibrationData[CanH_DiagArray[4]] = CanH_DiagArray[5];
	HAL_CAN_AddTxMessage(&hcan, &Dcm_TxHeader, Dcm_TxData, &Dcm_TxMailbox);
}
void DiagService_RC_LoadControl_Status(void)
{
	if(0 != CanH_DiagArray[6])
	{
		if(0xff != CanH_DiagArray[5])
		{
			LoadCtrl_StatusList[CanH_DiagArray[5]] = CanH_DiagArray[6] - 1;
			DiagLoadCtrl_StatusList[CanH_DiagArray[5]]  = CanH_DiagArray[6];
		}
		else
		{
			for(uint8 i = 0; i < 8; i++)
			{
				LoadCtrl_StatusList[i] = CanH_DiagArray[6] - 1;
				DiagLoadCtrl_StatusList[i]  = CanH_DiagArray[6];
			}
		}
	}
	else
	{
		if(0xff != CanH_DiagArray[5]) DiagLoadCtrl_StatusList[CanH_DiagArray[5]]  = CanH_DiagArray[6];
		else for(uint8 i = 0; i < 8; i++) DiagLoadCtrl_StatusList[i]  = CanH_DiagArray[6];
	}
	Dcm_TxHeader.DLC = CanH_DiagRxHeader.DLC;
	Dcm_TxHeader.StdId = CanH_DiagRxHeader.StdId + 1;
	Dcm_TxData[0] = CanH_DiagArray[0];
	Dcm_TxData[1] = CanH_DiagArray[1] + 0x40;
	Dcm_TxData[2] = CanH_DiagArray[2];
	Dcm_TxData[3] = CanH_DiagArray[3];
	Dcm_TxData[4] = CanH_DiagArray[4];
	HAL_CAN_AddTxMessage(&hcan, &Dcm_TxHeader, Dcm_TxData, &Dcm_TxMailbox);
}
void DiagService_RC_LoadControl_RestoreLoadsStatus(void)
{
	LoadCtrl_PermanentSwitchOff_Rin2 = 0;
	LoadCtrl_PermanentSwitchOff_Rin3 = 0;
	LoadCtrl_PermanentSwitchOff_Rin4 = 0;
	LoadCtrl_Rin2Counter = 0;
	LoadCtrl_Rin3Counter = 0;
	LoadCtrl_Rin4Counter = 0;
	LoadCtrl_LockRin2 = 0;
	LoadCtrl_LockRin3 = 0;
	LoadCtrl_LockRin4 = 0;
	Dcm_TxHeader.DLC = CanH_DiagRxHeader.DLC;
	Dcm_TxHeader.StdId = CanH_DiagRxHeader.StdId + 1;
	Dcm_TxData[0] = CanH_DiagArray[0];
	Dcm_TxData[1] = CanH_DiagArray[1] + 0x40;
	Dcm_TxData[2] = CanH_DiagArray[2];
	Dcm_TxData[3] = CanH_DiagArray[3];
	Dcm_TxData[4] = CanH_DiagArray[4];
	HAL_CAN_AddTxMessage(&hcan, &Dcm_TxHeader, Dcm_TxData, &Dcm_TxMailbox);
}
void DiagService_DSC_DefaultSession(void)
{
	Dcm_TxHeader.DLC = CanH_DiagRxHeader.DLC;
	Dcm_TxHeader.StdId = CanH_DiagRxHeader.StdId + 1;
	Dcm_TxData[0] = CanH_DiagArray[0];
	Dcm_TxData[1] = CanH_DiagArray[1] + 0x40;
	Dcm_TxData[2] = CanH_DiagArray[2];
	Dcm_DiagnosticSession = DEFAULT;
	HAL_CAN_AddTxMessage(&hcan, &Dcm_TxHeader, Dcm_TxData, &Dcm_TxMailbox);
}
void DiagService_DSC_ExtendedSession(void)
{
	Dcm_TxHeader.DLC = CanH_DiagRxHeader.DLC;
	Dcm_TxHeader.StdId = CanH_DiagRxHeader.StdId + 1;
	Dcm_TxData[0] = CanH_DiagArray[0];
	Dcm_TxData[1] = CanH_DiagArray[1] + 0x40;
	Dcm_TxData[2] = CanH_DiagArray[2];
	Dcm_DiagnosticSession = EXTENDED;
	HAL_CAN_AddTxMessage(&hcan, &Dcm_TxHeader, Dcm_TxData, &Dcm_TxMailbox);
}
void DiagService_DSC_CalibrationSession(void)
{
	Dcm_TxHeader.DLC = CanH_DiagRxHeader.DLC;
	Dcm_TxHeader.StdId = CanH_DiagRxHeader.StdId + 1;
	Dcm_TxData[0] = CanH_DiagArray[0];
	Dcm_TxData[1] = CanH_DiagArray[1] + 0x40;
	Dcm_TxData[2] = CanH_DiagArray[2];
	Dcm_DiagnosticSession = CALIBRATION;
	HAL_CAN_AddTxMessage(&hcan, &Dcm_TxHeader, Dcm_TxData, &Dcm_TxMailbox);
	HAL_Delay(1);
	NvM_WriteAll();
	EcuM_PerformReset(0);
}
void DiagService_DSC_ProgrammingSession(void)
{
	Dcm_TxHeader.DLC = CanH_DiagRxHeader.DLC;
	Dcm_TxHeader.StdId = CanH_DiagRxHeader.StdId + 1;
	Dcm_TxData[0] = CanH_DiagArray[0];
	Dcm_TxData[1] = CanH_DiagArray[1] + 0x40;
	Dcm_TxData[2] = CanH_DiagArray[2];
	Dcm_DiagnosticSession = PROGRAMMING;
	HAL_CAN_AddTxMessage(&hcan, &Dcm_TxHeader, Dcm_TxData, &Dcm_TxMailbox);
	HAL_Delay(1);
	NvM_WriteAll();
	EcuM_PerformReset(0);
}
void DiagService_ER_HardReset(void)
{
	Dcm_TxHeader.DLC = CanH_DiagRxHeader.DLC;
	Dcm_TxHeader.StdId = CanH_DiagRxHeader.StdId + 1;
	Dcm_TxData[0] = CanH_DiagArray[0];
	Dcm_TxData[1] = CanH_DiagArray[1] + 0x40;
	Dcm_TxData[2] = CanH_DiagArray[2];
	Dcm_DiagnosticSession = HARDRESET;
	HAL_CAN_AddTxMessage(&hcan, &Dcm_TxHeader, Dcm_TxData, &Dcm_TxMailbox);
	HAL_Delay(1);
	NvM_WriteAll();
	EcuM_PerformReset(0);
}
void DiagService_TP_TesterPresent(void)
{
	Dcm_TxHeader.DLC = CanH_DiagRxHeader.DLC;
	Dcm_TxHeader.StdId = CanH_DiagRxHeader.StdId + 1;
	Dcm_TxData[0] = CanH_DiagArray[0];
	Dcm_TxData[1] = CanH_DiagArray[1] + 0x40;
	Dcm_TxData[2] = CanH_DiagArray[2];
	CanH_RequestBusSleep = 0;
	if(CanH_CommunicationState != CC_ACTIVE) CanH_CommunicationState = FULL_COMMUNICATION;
	else
	{
		/* Do nothing. */
	}
	CanH_NoCommCounter = 0;
	HAL_CAN_AddTxMessage(&hcan, &Dcm_TxHeader, Dcm_TxData, &Dcm_TxMailbox);
}

void DiagService_CDTCI_ClearDiagnosticInformation(void)
{
	Dcm_TxHeader.DLC = CanH_DiagRxHeader.DLC;
	Dcm_TxHeader.StdId = CanH_DiagRxHeader.StdId + 1;
	Dcm_TxData[0] = CanH_DiagArray[0];
	Dcm_TxData[1] = CanH_DiagArray[1] + 0x40;
	Dem_ClearDtc();
	HAL_CAN_AddTxMessage(&hcan, &Dcm_TxHeader, Dcm_TxData, &Dcm_TxMailbox);
}
void Dcm_TxIsoTp(uint8 *data, uint16 size)
{
	CAN_TxHeaderTypeDef localTxHeader = {0, 0, 0, 0, 0, 0};
	uint32 localTxMailbox = 0;
	uint16 localRemainingData = size;
	uint16 localOffset = 0;
	uint8 localCANData[DCM_MAX_CAN_DATA_LEN];
	uint8 localSeqNum = 1;
	localTxHeader.StdId = DCM_CAN_ID_TX;
	localTxHeader.RTR = CAN_RTR_DATA;
	localTxHeader.IDE = CAN_ID_STD;
	localTxHeader.DLC = DCM_MAX_CAN_DATA_LEN;
	localTxHeader.TransmitGlobalTime = DISABLE;
	// Send First Frame (FF)
	if (7 < size) // Data larger than 7 bytes requires FF
	{
		localCANData[0] = 0x10 | ((size >> 8) & 0x0F);  // FF, first nibble, total size (12 bits)
		localCANData[1] = (size & 0xFF);                // Remaining total size bytes
		memcpy(&localCANData[2], &data[0], 6);          // First 6 bytes of data
		localOffset = 6;
		localRemainingData -= 6;
		// Transmit First Frame
		HAL_CAN_AddTxMessage(&hcan, &localTxHeader, localCANData, &localTxMailbox);
	}
	else
	{
		// Single Frame (SF) if data <= 7 bytes
		localCANData[0] = size & 0x0F;  // SF, first nibble is length
		memcpy(&localCANData[1], &data[0], size);
		// Transmit Single Frame
		HAL_CAN_AddTxMessage(&hcan, &localTxHeader, localCANData, &localTxMailbox);
	}
	// Handle Flow Control (Wait for FC frame from receiver)
	// Normally you would wait for the receiver to send a Flow Control (FC) frame,
	// but here we'll assume it's always "Clear to Send" (CTS).
	// Send Consecutive Frames (CF)
	while(localRemainingData > 0)
	{
		localCANData[0] = 0x20 | (localSeqNum & 0x0F);  // CF with sequence number
		if (localRemainingData >= 7)
		{
			memcpy(&localCANData[1], &data[localOffset], 7);
			localOffset += 7;
			localRemainingData -= 7;
		}
		else
		{
			// Last frame with remaining bytes
			memcpy(&localCANData[1], &data[localOffset], localRemainingData);
			localOffset += localRemainingData;
			localRemainingData = 0;
		}
		// Transmit Consecutive Frame
		HAL_CAN_AddTxMessage(&hcan, &localTxHeader, localCANData, &localTxMailbox);
		HAL_Delay(1);  // Wait between consecutive frames
		localSeqNum++;
	}
}
void DiagService_RDTCI_ReadDTCInformationSupportedDtc(void)
{
	uint8 *data = (uint8*)Dem_DTCArray;
	uint16 total_size = DEM_NUMBER_OF_DTCS * sizeof(Dem_DTC_t);
	Dcm_TxHeader.DLC = 4;
	Dcm_TxHeader.StdId = 0x703;
	Dcm_TxData[0] = CanH_DiagArray[0];
	Dcm_TxData[1] = CanH_DiagArray[1] + 0x40;
	Dcm_TxData[2] = CanH_DiagArray[2];
	HAL_CAN_AddTxMessage(&hcan, &Dcm_TxHeader, Dcm_TxData, &Dcm_TxMailbox);
	Dcm_TxIsoTp(data, total_size);
}
void DiagService_RDBI_ReadSWData(void)
{
	Dcm_TxHeader.DLC = 0x08;
	Dcm_TxHeader.StdId = CanH_DiagRxHeader.StdId + 1;
	Dcm_TxData[0] = 0x06;
	Dcm_TxData[1] = CanH_DiagArray[1] + 0x40;
	Dcm_TxData[2] = CanH_DiagArray[2];
	Dcm_TxData[3] = CanH_DiagArray[3];
	Dcm_TxData[4] = Dcm_SWVersion.FlashBootLoaderVersion;
	Dcm_TxData[5] = Dcm_SWVersion.SoftwareVersion;
	Dcm_TxData[6] = Dcm_SWVersion.CodingVersion;
	HAL_CAN_AddTxMessage(&hcan, &Dcm_TxHeader, Dcm_TxData, &Dcm_TxMailbox);
}
void DiagService_RDBI_ReadActiveDiagnosticSession(void)
{
	Dcm_TxHeader.DLC = CanH_DiagRxHeader.DLC;
	Dcm_TxHeader.StdId = CanH_DiagRxHeader.StdId + 1;
	Dcm_TxData[0] = CanH_DiagArray[0];
	Dcm_TxData[1] = CanH_DiagArray[1] + 0x40;
	Dcm_TxData[2] = CanH_DiagArray[2];
	Dcm_TxData[3] = CanH_DiagArray[3];
	Dcm_TxData[4] = CanH_DiagArray[4];
	Dcm_TxData[5] = Dcm_DiagnosticSession;
	HAL_CAN_AddTxMessage(&hcan, &Dcm_TxHeader, Dcm_TxData, &Dcm_TxMailbox);
}
void DiagService_RDBI_ReadAliveTime(void)
{
	Dcm_TxHeader.DLC = 0x05;
	Dcm_TxHeader.StdId = CanH_DiagRxHeader.StdId + 1;
	Dcm_TxData[0] = 0x04;
	Dcm_TxData[1] = CanH_DiagArray[1] + 0x40;
	Dcm_TxData[2] = CanH_DiagArray[2];
	Dcm_TxData[3] = CanH_DiagArray[3];
	Dcm_TxData[4] = (uint8)(Dcm_AliveCounter * 5 / 60000);
	HAL_CAN_AddTxMessage(&hcan, &Dcm_TxHeader, Dcm_TxData, &Dcm_TxMailbox);
}
void DiagService_CC_CommunicationControl(void)
{
	Dcm_TxHeader.DLC = CanH_DiagRxHeader.DLC;
	Dcm_TxHeader.StdId = CanH_DiagRxHeader.StdId + 1;
	Dcm_TxData[0] = 0x02;
	Dcm_TxData[1] = CanH_DiagArray[1] + 0x40;
	Dcm_TxData[2] = CanH_DiagArray[2];
	if(CanH_DiagArray[2] == 0) CanH_CommunicationState = FULL_COMMUNICATION;
	else if(CanH_DiagArray[2] == 1) CanH_CommunicationState = CC_ACTIVE;
	else
	{
		/* Do nothing. */
	}
	HAL_CAN_AddTxMessage(&hcan, &Dcm_TxHeader, Dcm_TxData, &Dcm_TxMailbox);
}
void DiagService_CDTCS_ControlDTCSetting(void)
{
	Dcm_TxHeader.DLC = CanH_DiagRxHeader.DLC;
	Dcm_TxHeader.StdId = CanH_DiagRxHeader.StdId + 1;
	Dcm_TxData[0] = CanH_DiagArray[0];
	Dcm_TxData[1] = CanH_DiagArray[1] + 0x40;
	Dcm_TxData[2] = CanH_DiagArray[2];
	if(CanH_DiagArray[2] == 1) Dem_DTCSettingDeactivated = 0;
	else if(CanH_DiagArray[2] == 2) Dem_DTCSettingDeactivated = 1;
	else
	{
		/* Do nothing. */
	}
	HAL_CAN_AddTxMessage(&hcan, &Dcm_TxHeader, Dcm_TxData, &Dcm_TxMailbox);
}
