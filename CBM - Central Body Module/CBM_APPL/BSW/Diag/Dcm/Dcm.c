/* START OF FILE */
/* INCLUDE START */
#include "Dcm.h"
#include "EcuM.h"
#include "Dem.h"
#include "CanSpi.h"
#include "InputCtrl.h"
#include "TimH.h"
#include "Ain.h"
#include "UartH.h"
#include "tim.h"
#include "Nvm.h"
#include "ActCtrl.h"
#include "string.h"
/* INCLUDE END */
/* VARIABLES START */
/* Diagnostic session state. */
DiagState Dcm_DiagnosticSession __attribute__((section(".ncr")));
/* Time without reset. */
uint32 Dcm_AliveCounter __attribute__((section(".ncr")));
/* Received calibration data. */
uint32 Dcm_DiagnosticCalibrationData[16] = {0};
extern uint8 Dem_ControlDtcSettingStatus;
/* Software versions. */
static SWV Dcm_SWVersion =
{
		0x01, /* SW */
		0x01, /* FBL */
		0x01, /* CODING */
		0x01  /* CALIBRATION */
};
/* FUNCTIONS START */
void Dcm_MainFunction(void);
static void DiagService_DSC_DefaultSession(void);
static void DiagService_DSC_ExtendedSession(void);
static void DiagService_DSC_ProgrammingSession(void);
static void DiagService_DSC_CodingSession(void);
static void DiagService_DSC_CalibrationSession(void);
static void DiagService_ER_HardReset(void);
static void DiagService_ER_SoftReset(void);
static void DiagService_TP_TesterPresent(void);
static void DiagService_CDTCI_ClearDiagnosticInformation(void);
static void DiagService_RDTCI_ReadDTCInformationSupportedDtc(void);
static void DiagService_RDBI_ReadAwakeReasons(void);
static void DiagService_RDBI_ReadCodingData(void);
static void DiagService_RDBI_ReadCalibrationData(void);
static void DiagService_RDBI_ReadSWVersion(void);
static void DiagService_RDBI_ReadActiveDiagnosticSession(void);
static void DiagService_RDBI_ReadAliveTime(void);
static void DiagService_RDBPI_ReadPeriodicIgnitionCompleteStatus(void);
static void DiagService_RDBPI_ReadPeriodicGearboxSelectorCompleteStatus(void);
static void DiagService_RDBPI_ReadPeriodicPowerSteeringCompleteStatus(void);
static void DiagService_RDBPI_ReadPeriodicAccelerationCompleteStatus(void);
static void DiagService_RDBPI_ReadPeriodicBrakeCompleteStatus(void);
static void DiagService_RDBPI_ReadPeriodicDoorLockActuatorStatus(void);
static void DiagService_RDBPI_ReadPeriodicWindshieldWiperActuatorStatus(void);
static void DiagService_RDBPI_ReadPeriodicClimaFanStatus(void);
static void DiagService_RDBPI_ReadPeriodicAQS(void);
static void DiagService_RDBPI_ReadPeriodicGS(void);
static void DiagService_RDBPI_ReadPeriodicLS(void);
static void DiagService_RDBPI_ReadPeriodicRS(void);
static void DiagService_WDBI_WriteCalibrationData(void);
static void DiagService_CC_CommunicationControl(void);
static void DiagService_CDTCS_ControlDTCSetting(void);
void Dcm_TxIsoTp(uint8 *data, uint16 size);
extern void NvM_WriteAll(void);
/* FUNCTIONS STOP */
/* VARIABLES START */
/* Look-up table for RDBPI diagnostic routines. */
Dcm_RDBPI_Table_t Dcm_RDBPI_Table[] =
{
		{DiagService_RDBPI_ReadPeriodicIgnitionCompleteStatus},
		{DiagService_RDBPI_ReadPeriodicGearboxSelectorCompleteStatus},
		{DiagService_RDBPI_ReadPeriodicPowerSteeringCompleteStatus},
		{DiagService_RDBPI_ReadPeriodicAccelerationCompleteStatus},
		{DiagService_RDBPI_ReadPeriodicBrakeCompleteStatus},
		{DiagService_RDBPI_ReadPeriodicDoorLockActuatorStatus},
		{DiagService_RDBPI_ReadPeriodicWindshieldWiperActuatorStatus},
		{DiagService_RDBPI_ReadPeriodicClimaFanStatus},
		{DiagService_RDBPI_ReadPeriodicAQS},
		{DiagService_RDBPI_ReadPeriodicGS},
		{DiagService_RDBPI_ReadPeriodicLS},
		{DiagService_RDBPI_ReadPeriodicRS},
};
/* VARIABLES STOP */
/* FUNCTIONS START */
void Dcm_TxIsoTp(uint8 *data, uint16 size)
{
	CANSPI_uCAN_MSG isoTpFrame = {0};
	uint16 remainingData = size;
	uint16 offset = 0;
	uint8 seqNum = 1;
	// Configure the basic frame details
	isoTpFrame.frame.id = 0x701;      // Replace with your CAN TX ID
	isoTpFrame.frame.idType = 1;             // Standard ID
	isoTpFrame.frame.dlc = 8; // Set default DLC to max CAN data length
	// Send First Frame (FF)
	if (size > 7) // Data larger than 7 bytes requires First Frame
	{
		isoTpFrame.frame.data0 = 0x10 | ((size >> 8) & 0x0F); // FF, first nibble, total size (12 bits)
		isoTpFrame.frame.data1 = size & 0xFF;                // Remaining total size bytes
		memcpy(&isoTpFrame.frame.data2, &data[0], 6);        // First 6 bytes of data
		offset = 6;
		remainingData -= 6;
		// Transmit First Frame
		CanSpi_Transmit(&isoTpFrame);
	}
	else
	{
		// Single Frame (SF) if data <= 7 bytes
		isoTpFrame.frame.data0 = size & 0x0F;                // SF, first nibble is length
		memcpy(&isoTpFrame.frame.data1, &data[0], size);     // Remaining data
		isoTpFrame.frame.dlc = size + 1;                    // DLC is the actual data length
		// Transmit Single Frame
		CanSpi_Transmit(&isoTpFrame);
		return;  // No need for consecutive frames
	}

	// Send Consecutive Frames (CF)
	while (remainingData > 0)
	{
		isoTpFrame.frame.data0 = 0x20 | (seqNum & 0x0F); // CF with sequence number
		if (remainingData >= 7)
		{
			memcpy(&isoTpFrame.frame.data1, &data[offset], 7);
			offset += 7;
			remainingData -= 7;
		}
		else
		{
			// Last frame with remaining bytes
			memcpy(&isoTpFrame.frame.data1, &data[offset], remainingData);
			isoTpFrame.frame.dlc = remainingData + 1; // DLC adjusted for remaining data
			offset += remainingData;
			remainingData = 0;
		}
		// Transmit Consecutive Frame
		CanSpi_Transmit(&isoTpFrame);
		seqNum++;
		// Add delay between consecutive frames
		HAL_Delay(1); // Use your delay function equivalent
	}
}
static void DiagService_CDTCS_ControlDTCSetting(void)
{
	if(Dcm_DiagServiceRequest_Frame.frame.data2 == 0x01) Dem_ControlDtcSettingStatus = 1;
	else if(Dcm_DiagServiceRequest_Frame.frame.data2 == 0x02) Dem_ControlDtcSettingStatus = 0;
	else
	{
		/* Do nothing. */
	}
	Dcm_DiagServiceResponse_Frame.frame.dlc = 3;
	Dcm_DiagServiceResponse_Frame.frame.id = 0x701;
	Dcm_DiagServiceResponse_Frame.frame.idType = 1;
	Dcm_DiagServiceResponse_Frame.frame.data0 = 0x02;
	Dcm_DiagServiceResponse_Frame.frame.data1 = 0xC5;
	Dcm_DiagServiceResponse_Frame.frame.data2 = Dcm_DiagServiceRequest_Frame.frame.data2;
	CanSpi_Transmit(&Dcm_DiagServiceResponse_Frame);
	Dcm_DiagServiceResponse_Frame.frame.idType = 0;
	Dcm_DiagServiceResponse_Frame.frame.id = 0;
	Dcm_DiagServiceResponse_Frame.frame.dlc = 0;
	Dcm_DiagServiceResponse_Frame.frame.data0 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data1 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data2 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data3 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data4 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data5 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data6 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data7 = 0;
	Dcm_DiagServiceRequest_Frame.frame.idType = 0;
	Dcm_DiagServiceRequest_Frame.frame.id = 0;
	Dcm_DiagServiceRequest_Frame.frame.dlc = 0;
	Dcm_DiagServiceRequest_Frame.frame.data0 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data1 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data2 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data3 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data4 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data5 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data6 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data7 = 0;
}

static void DiagService_CC_CommunicationControl(void)
{
	if(Dcm_DiagServiceRequest_Frame.frame.data2 == 0x01) CanSpi_Communication_Status = CC_ACTIVE;
	else if(Dcm_DiagServiceRequest_Frame.frame.data2 == 0x00) CanSpi_Communication_Status = FULL_COMMUNICATION;
	else
	{
		/* Do nothing. */
	}
	Dcm_DiagServiceResponse_Frame.frame.dlc = 3;
	Dcm_DiagServiceResponse_Frame.frame.id = 0x701;
	Dcm_DiagServiceResponse_Frame.frame.idType = 1;
	Dcm_DiagServiceResponse_Frame.frame.data0 = 0x02;
	Dcm_DiagServiceResponse_Frame.frame.data1 = 0x68;
	Dcm_DiagServiceResponse_Frame.frame.data2 = Dcm_DiagServiceRequest_Frame.frame.data2;
	CanSpi_Transmit(&Dcm_DiagServiceResponse_Frame);
	Dcm_DiagServiceResponse_Frame.frame.idType = 0;
	Dcm_DiagServiceResponse_Frame.frame.id = 0;
	Dcm_DiagServiceResponse_Frame.frame.dlc = 0;
	Dcm_DiagServiceResponse_Frame.frame.data0 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data1 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data2 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data3 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data4 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data5 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data6 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data7 = 0;
	Dcm_DiagServiceRequest_Frame.frame.idType = 0;
	Dcm_DiagServiceRequest_Frame.frame.id = 0;
	Dcm_DiagServiceRequest_Frame.frame.dlc = 0;
	Dcm_DiagServiceRequest_Frame.frame.data0 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data1 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data2 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data3 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data4 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data5 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data6 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data7 = 0;
}
static void DiagService_TP_TesterPresent(void)
{
	Dcm_DiagServiceResponse_Frame.frame.dlc = 3;
	Dcm_DiagServiceResponse_Frame.frame.id = 0x701;
	Dcm_DiagServiceResponse_Frame.frame.idType = 1;
	Dcm_DiagServiceResponse_Frame.frame.data0 = 0x02;
	Dcm_DiagServiceResponse_Frame.frame.data1 = 0x7E;
	Dcm_DiagServiceResponse_Frame.frame.data2 = 0x00;
	CanSpi_Transmit(&Dcm_DiagServiceResponse_Frame);
	Dcm_DiagServiceResponse_Frame.frame.idType = 0;
	Dcm_DiagServiceResponse_Frame.frame.id = 0;
	Dcm_DiagServiceResponse_Frame.frame.dlc = 0;
	Dcm_DiagServiceResponse_Frame.frame.data0 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data1 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data2 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data3 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data4 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data5 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data6 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data7 = 0;
	Dcm_DiagServiceRequest_Frame.frame.idType = 0;
	Dcm_DiagServiceRequest_Frame.frame.id = 0;
	Dcm_DiagServiceRequest_Frame.frame.dlc = 0;
	Dcm_DiagServiceRequest_Frame.frame.data0 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data1 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data2 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data3 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data4 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data5 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data6 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data7 = 0;
}
static void DiagService_WDBI_WriteCalibrationData(void)
{
	Dcm_DiagServiceResponse_Frame.frame.dlc = 4;
	Dcm_DiagServiceResponse_Frame.frame.id = 0x701;
	Dcm_DiagServiceResponse_Frame.frame.idType = 1;
	Dcm_DiagServiceResponse_Frame.frame.data0 = 0x03;
	Dcm_DiagServiceResponse_Frame.frame.data1 = 0x6E;
	Dcm_DiagServiceResponse_Frame.frame.data2 = 0x00;
	Dcm_DiagServiceResponse_Frame.frame.data3 = 0x00;
	Dcm_DiagnosticCalibrationData[Dcm_DiagServiceRequest_Frame.frame.data4] = Dcm_DiagServiceRequest_Frame.frame.data5;
	CanSpi_Transmit(&Dcm_DiagServiceResponse_Frame);
	Dcm_DiagServiceResponse_Frame.frame.idType = 0;
	Dcm_DiagServiceResponse_Frame.frame.id = 0;
	Dcm_DiagServiceResponse_Frame.frame.dlc = 0;
	Dcm_DiagServiceResponse_Frame.frame.data0 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data1 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data2 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data3 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data4 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data5 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data6 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data7 = 0;
	Dcm_DiagServiceRequest_Frame.frame.idType = 0;
	Dcm_DiagServiceRequest_Frame.frame.id = 0;
	Dcm_DiagServiceRequest_Frame.frame.dlc = 0;
	Dcm_DiagServiceRequest_Frame.frame.data0 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data1 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data2 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data3 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data4 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data5 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data6 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data7 = 0;
}
static void DiagService_DSC_CodingSession(void)
{
	Dcm_DiagServiceResponse_Frame.frame.dlc = 3;
	Dcm_DiagServiceResponse_Frame.frame.id = 0x701;
	Dcm_DiagServiceResponse_Frame.frame.idType = 1;
	Dcm_DiagServiceResponse_Frame.frame.data0 = 0x02;
	Dcm_DiagServiceResponse_Frame.frame.data1 = 0x50;
	Dcm_DiagServiceResponse_Frame.frame.data2 = 0x04;
	Dcm_DiagnosticSession = CODING;
	CanSpi_Transmit(&Dcm_DiagServiceResponse_Frame);
	NvM_WriteAll();
	EcuM_PerformReset(0);
}
static void DiagService_DSC_CalibrationSession(void)
{
	Dcm_DiagServiceResponse_Frame.frame.dlc = 3;
	Dcm_DiagServiceResponse_Frame.frame.id = 0x701;
	Dcm_DiagServiceResponse_Frame.frame.idType = 1;
	Dcm_DiagServiceResponse_Frame.frame.data0 = 0x02;
	Dcm_DiagServiceResponse_Frame.frame.data1 = 0x50;
	Dcm_DiagServiceResponse_Frame.frame.data2 = 0x05;
	Dcm_DiagnosticSession = CALIBRATION;
	CanSpi_Transmit(&Dcm_DiagServiceResponse_Frame);
	NvM_WriteAll();
	EcuM_PerformReset(0);
}
static void DiagService_RDBI_ReadAwakeReasons(void)
{
	Dcm_DiagServiceResponse_Frame.frame.dlc = 7;
	Dcm_DiagServiceResponse_Frame.frame.id = 0x701;
	Dcm_DiagServiceResponse_Frame.frame.idType = 1;
	Dcm_DiagServiceResponse_Frame.frame.data0 = 0x06;
	Dcm_DiagServiceResponse_Frame.frame.data1 = 0x62;
	Dcm_DiagServiceResponse_Frame.frame.data2 = 0x00;
	Dcm_DiagServiceResponse_Frame.frame.data3 = 0x01;
	Dcm_DiagServiceResponse_Frame.frame.data4 = CanSpi_Networkmanagement3_Signal;
	Dcm_DiagServiceResponse_Frame.frame.data5 = StatusList_ComOutValue[0];
	Dcm_DiagServiceResponse_Frame.frame.data6 = StatusList_ComOutValue[9];
	CanSpi_Transmit(&Dcm_DiagServiceResponse_Frame);
	Dcm_DiagServiceResponse_Frame.frame.idType = 0;
	Dcm_DiagServiceResponse_Frame.frame.id = 0;
	Dcm_DiagServiceResponse_Frame.frame.dlc = 0;
	Dcm_DiagServiceResponse_Frame.frame.data0 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data1 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data2 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data3 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data4 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data5 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data6 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data7 = 0;
	Dcm_DiagServiceRequest_Frame.frame.idType = 0;
	Dcm_DiagServiceRequest_Frame.frame.id = 0;
	Dcm_DiagServiceRequest_Frame.frame.dlc = 0;
	Dcm_DiagServiceRequest_Frame.frame.data0 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data1 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data2 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data3 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data4 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data5 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data6 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data7 = 0;
}
static void DiagService_RDBI_ReadCodingData(void)
{
	Dcm_DiagServiceResponse_Frame.frame.dlc = 8;
	Dcm_DiagServiceResponse_Frame.frame.id = 0x701;
	Dcm_DiagServiceResponse_Frame.frame.idType = 1;
	Dcm_DiagServiceResponse_Frame.frame.data0 = 0x08;
	Dcm_DiagServiceResponse_Frame.frame.data1 = 0x62;
	Dcm_DiagServiceResponse_Frame.frame.data2 = 0x00;
	Dcm_DiagServiceResponse_Frame.frame.data3 = 0x02;
	Dcm_DiagServiceResponse_Frame.frame.data4 = NvMBlock_Coding[0];
	Dcm_DiagServiceResponse_Frame.frame.data5 = NvMBlock_Coding[1];
	Dcm_DiagServiceResponse_Frame.frame.data6 = NvMBlock_Coding[2];
	Dcm_DiagServiceResponse_Frame.frame.data7 = NvMBlock_Coding[3];
	CanSpi_Transmit(&Dcm_DiagServiceResponse_Frame);
	Dcm_TxIsoTp((uint8*)NvMBlock_Coding, 10);
	Dcm_DiagServiceResponse_Frame.frame.idType = 0;
	Dcm_DiagServiceResponse_Frame.frame.id = 0;
	Dcm_DiagServiceResponse_Frame.frame.dlc = 0;
	Dcm_DiagServiceResponse_Frame.frame.data0 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data1 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data2 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data3 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data4 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data5 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data6 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data7 = 0;
	Dcm_DiagServiceRequest_Frame.frame.idType = 0;
	Dcm_DiagServiceRequest_Frame.frame.id = 0;
	Dcm_DiagServiceRequest_Frame.frame.dlc = 0;
	Dcm_DiagServiceRequest_Frame.frame.data0 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data1 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data2 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data3 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data4 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data5 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data6 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data7 = 0;
}
static void DiagService_RDBI_ReadCalibrationData(void)
{
	Dcm_DiagServiceResponse_Frame.frame.dlc = 8;
	Dcm_DiagServiceResponse_Frame.frame.id = 0x701;
	Dcm_DiagServiceResponse_Frame.frame.idType = 1;
	Dcm_DiagServiceResponse_Frame.frame.data0 = 0x13;
	Dcm_DiagServiceResponse_Frame.frame.data1 = 0x62;
	Dcm_DiagServiceResponse_Frame.frame.data2 = 0x00;
	Dcm_DiagServiceResponse_Frame.frame.data3 = 0x03;
	Dcm_DiagServiceResponse_Frame.frame.data4 = NvMBlock_Calibration[0];
	Dcm_DiagServiceResponse_Frame.frame.data5 = NvMBlock_Calibration[1];
	Dcm_DiagServiceResponse_Frame.frame.data6 = NvMBlock_Calibration[2];
	Dcm_DiagServiceResponse_Frame.frame.data7 = NvMBlock_Calibration[3];
	CanSpi_Transmit(&Dcm_DiagServiceResponse_Frame);
	Dcm_TxIsoTp((uint8*)NvMBlock_Calibration, 17);
	Dcm_DiagServiceResponse_Frame.frame.idType = 0;
	Dcm_DiagServiceResponse_Frame.frame.id = 0;
	Dcm_DiagServiceResponse_Frame.frame.dlc = 0;
	Dcm_DiagServiceResponse_Frame.frame.data0 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data1 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data2 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data3 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data4 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data5 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data6 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data7 = 0;
	Dcm_DiagServiceRequest_Frame.frame.idType = 0;
	Dcm_DiagServiceRequest_Frame.frame.id = 0;
	Dcm_DiagServiceRequest_Frame.frame.dlc = 0;
	Dcm_DiagServiceRequest_Frame.frame.data0 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data1 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data2 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data3 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data4 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data5 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data6 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data7 = 0;
}
static void DiagService_RDBI_ReadSWVersion(void)
{
	Dcm_DiagServiceResponse_Frame.frame.dlc = 8;
	Dcm_DiagServiceResponse_Frame.frame.id = 0x701;
	Dcm_DiagServiceResponse_Frame.frame.idType = 1;
	Dcm_DiagServiceResponse_Frame.frame.data0 = 0x07;
	Dcm_DiagServiceResponse_Frame.frame.data1 = 0x62;
	Dcm_DiagServiceResponse_Frame.frame.data2 = 0xF1;
	Dcm_DiagServiceResponse_Frame.frame.data3 = 0x80;
	Dcm_DiagServiceResponse_Frame.frame.data4 = Dcm_SWVersion.FlashBootLoaderVersion;
	Dcm_DiagServiceResponse_Frame.frame.data5 = Dcm_SWVersion.SoftwareVersion;
	Dcm_DiagServiceResponse_Frame.frame.data6 = Dcm_SWVersion.CodingVersion;
	Dcm_DiagServiceResponse_Frame.frame.data7 = Dcm_SWVersion.CalibrationVersion;
	CanSpi_Transmit(&Dcm_DiagServiceResponse_Frame);
	Dcm_DiagServiceResponse_Frame.frame.idType = 0;
	Dcm_DiagServiceResponse_Frame.frame.id = 0;
	Dcm_DiagServiceResponse_Frame.frame.dlc = 0;
	Dcm_DiagServiceResponse_Frame.frame.data0 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data1 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data2 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data3 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data4 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data5 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data6 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data7 = 0;
	Dcm_DiagServiceRequest_Frame.frame.idType = 0;
	Dcm_DiagServiceRequest_Frame.frame.id = 0;
	Dcm_DiagServiceRequest_Frame.frame.dlc = 0;
	Dcm_DiagServiceRequest_Frame.frame.data0 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data1 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data2 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data3 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data4 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data5 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data6 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data7 = 0;
}

static void DiagService_RDBI_ReadActiveDiagnosticSession(void)
{
	Dcm_DiagServiceResponse_Frame.frame.dlc = 5;
	Dcm_DiagServiceResponse_Frame.frame.id = 0x701;
	Dcm_DiagServiceResponse_Frame.frame.idType = 1;
	Dcm_DiagServiceResponse_Frame.frame.data0 = 0x04;
	Dcm_DiagServiceResponse_Frame.frame.data1 = 0x62;
	Dcm_DiagServiceResponse_Frame.frame.data2 = 0xF1;
	Dcm_DiagServiceResponse_Frame.frame.data3 = 0x86;
	Dcm_DiagServiceResponse_Frame.frame.data4 = Dcm_DiagnosticSession;
	CanSpi_Transmit(&Dcm_DiagServiceResponse_Frame);
	Dcm_DiagServiceResponse_Frame.frame.idType = 0;
	Dcm_DiagServiceResponse_Frame.frame.id = 0;
	Dcm_DiagServiceResponse_Frame.frame.dlc = 0;
	Dcm_DiagServiceResponse_Frame.frame.data0 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data1 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data2 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data3 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data4 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data5 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data6 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data7 = 0;
	Dcm_DiagServiceRequest_Frame.frame.idType = 0;
	Dcm_DiagServiceRequest_Frame.frame.id = 0;
	Dcm_DiagServiceRequest_Frame.frame.dlc = 0;
	Dcm_DiagServiceRequest_Frame.frame.data0 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data1 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data2 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data3 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data4 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data5 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data6 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data7 = 0;
}
static void DiagService_RDBI_ReadAliveTime(void)
{
	Dcm_DiagServiceResponse_Frame.frame.dlc = 5;
	Dcm_DiagServiceResponse_Frame.frame.id = 0x701;
	Dcm_DiagServiceResponse_Frame.frame.idType = 1;
	Dcm_DiagServiceResponse_Frame.frame.data0 = 0x04;
	Dcm_DiagServiceResponse_Frame.frame.data1 = 0x62;
	Dcm_DiagServiceResponse_Frame.frame.data2 = 0x00;
	Dcm_DiagServiceResponse_Frame.frame.data3 = 0x05;
	Dcm_DiagServiceResponse_Frame.frame.data4 = (uint8)((Dcm_AliveCounter * 5) / 60000 * 60);
	CanSpi_Transmit(&Dcm_DiagServiceResponse_Frame);
	Dcm_DiagServiceResponse_Frame.frame.idType = 0;
	Dcm_DiagServiceResponse_Frame.frame.id = 0;
	Dcm_DiagServiceResponse_Frame.frame.dlc = 0;
	Dcm_DiagServiceResponse_Frame.frame.data0 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data1 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data2 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data3 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data4 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data5 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data6 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data7 = 0;
	Dcm_DiagServiceRequest_Frame.frame.idType = 0;
	Dcm_DiagServiceRequest_Frame.frame.id = 0;
	Dcm_DiagServiceRequest_Frame.frame.dlc = 0;
	Dcm_DiagServiceRequest_Frame.frame.data0 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data1 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data2 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data3 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data4 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data5 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data6 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data7 = 0;
}
static void DiagService_RDBPI_ReadPeriodicIgnitionCompleteStatus(void)
{
	Dcm_DiagServiceResponse_Frame.frame.dlc = 8;
	Dcm_DiagServiceResponse_Frame.frame.id = 0x701;
	Dcm_DiagServiceResponse_Frame.frame.idType = 1;
	Dcm_DiagServiceResponse_Frame.frame.data0 = 0x07;
	Dcm_DiagServiceResponse_Frame.frame.data1 = 0x6A;
	if(Dcm_DiagServiceRequest_Frame.frame.data2 != 0x04) Dcm_DiagServiceResponse_Frame.frame.data2 = 0x01;
	else if(Dcm_DiagServiceRequest_Frame.frame.data2 == 0x04) Dcm_DiagServiceResponse_Frame.frame.data2 = 0x04;
	else
	{
		/* Do nothing. */
	}
	Dcm_DiagServiceResponse_Frame.frame.data3 = 0x00;
	Dcm_DiagServiceResponse_Frame.frame.data4 = StatusList_ComOutValue[0];
	Dcm_DiagServiceResponse_Frame.frame.data5 = StatusList_RawValue[0];
	Dcm_DiagServiceResponse_Frame.frame.data6 = StatusList_InputStatus[0].errorStatus;
	Dcm_DiagServiceResponse_Frame.frame.data7 = StatusList_InputStatus[0].DCYStatus;
	CanSpi_Transmit(&Dcm_DiagServiceResponse_Frame);
	Dcm_DiagServiceResponse_Frame.frame.idType = 0;
	Dcm_DiagServiceResponse_Frame.frame.id = 0;
	Dcm_DiagServiceResponse_Frame.frame.dlc = 0;
	Dcm_DiagServiceResponse_Frame.frame.data0 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data1 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data2 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data3 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data4 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data5 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data6 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data7 = 0;
	Dcm_DiagServiceRequest_Frame.frame.idType = 0;
	Dcm_DiagServiceRequest_Frame.frame.id = 0;
	Dcm_DiagServiceRequest_Frame.frame.dlc = 0;
	Dcm_DiagServiceRequest_Frame.frame.data0 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data1 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data2 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data3 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data4 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data5 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data6 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data7 = 0;
}
static void DiagService_RDBPI_ReadPeriodicGearboxSelectorCompleteStatus(void)
{
	Dcm_DiagServiceResponse_Frame.frame.dlc = 8;
	Dcm_DiagServiceResponse_Frame.frame.id = 0x701;
	Dcm_DiagServiceResponse_Frame.frame.idType = 1;
	Dcm_DiagServiceResponse_Frame.frame.data0 = 0x07;
	Dcm_DiagServiceResponse_Frame.frame.data1 = 0x6A;
	if(Dcm_DiagServiceRequest_Frame.frame.data2 != 0x04) Dcm_DiagServiceResponse_Frame.frame.data2 = 0x01;
	else if(Dcm_DiagServiceRequest_Frame.frame.data2 == 0x04) Dcm_DiagServiceResponse_Frame.frame.data2 = 0x04;
	else
	{
		/* Do nothing. */
	}
	Dcm_DiagServiceResponse_Frame.frame.data3 = 0x01;
	Dcm_DiagServiceResponse_Frame.frame.data4 = StatusList_ComOutValue[4];
	Dcm_DiagServiceResponse_Frame.frame.data5 = StatusList_RawValue[4];
	Dcm_DiagServiceResponse_Frame.frame.data6 = StatusList_InputStatus[4].errorStatus;
	Dcm_DiagServiceResponse_Frame.frame.data7 = StatusList_InputStatus[4].DCYStatus;
	CanSpi_Transmit(&Dcm_DiagServiceResponse_Frame);
	Dcm_DiagServiceResponse_Frame.frame.idType = 0;
	Dcm_DiagServiceResponse_Frame.frame.id = 0;
	Dcm_DiagServiceResponse_Frame.frame.dlc = 0;
	Dcm_DiagServiceResponse_Frame.frame.data0 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data1 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data2 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data3 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data4 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data5 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data6 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data7 = 0;
	Dcm_DiagServiceRequest_Frame.frame.idType = 0;
	Dcm_DiagServiceRequest_Frame.frame.id = 0;
	Dcm_DiagServiceRequest_Frame.frame.dlc = 0;
	Dcm_DiagServiceRequest_Frame.frame.data0 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data1 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data2 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data3 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data4 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data5 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data6 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data7 = 0;
}
static void DiagService_RDBPI_ReadPeriodicPowerSteeringCompleteStatus(void)
{
	Dcm_DiagServiceResponse_Frame.frame.dlc = 8;
	Dcm_DiagServiceResponse_Frame.frame.id = 0x701;
	Dcm_DiagServiceResponse_Frame.frame.idType = 1;
	Dcm_DiagServiceResponse_Frame.frame.data0 = 0x07;
	Dcm_DiagServiceResponse_Frame.frame.data1 = 0x6A;
	if(Dcm_DiagServiceRequest_Frame.frame.data2 != 0x04) Dcm_DiagServiceResponse_Frame.frame.data2 = 0x01;
	else if(Dcm_DiagServiceRequest_Frame.frame.data2 == 0x04) Dcm_DiagServiceResponse_Frame.frame.data2 = 0x04;
	else
	{
		/* Do nothing. */
	}
	Dcm_DiagServiceResponse_Frame.frame.data3 = 0x02;
	Dcm_DiagServiceResponse_Frame.frame.data4 = StatusList_ComOutValue[3];
	Dcm_DiagServiceResponse_Frame.frame.data5 = StatusList_RawValue[3];
	Dcm_DiagServiceResponse_Frame.frame.data6 = StatusList_InputStatus[3].errorStatus;
	Dcm_DiagServiceResponse_Frame.frame.data7 = StatusList_InputStatus[3].DCYStatus;
	CanSpi_Transmit(&Dcm_DiagServiceResponse_Frame);
	Dcm_DiagServiceResponse_Frame.frame.idType = 0;
	Dcm_DiagServiceResponse_Frame.frame.id = 0;
	Dcm_DiagServiceResponse_Frame.frame.dlc = 0;
	Dcm_DiagServiceResponse_Frame.frame.data0 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data1 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data2 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data3 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data4 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data5 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data6 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data7 = 0;
	Dcm_DiagServiceRequest_Frame.frame.idType = 0;
	Dcm_DiagServiceRequest_Frame.frame.id = 0;
	Dcm_DiagServiceRequest_Frame.frame.dlc = 0;
	Dcm_DiagServiceRequest_Frame.frame.data0 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data1 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data2 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data3 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data4 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data5 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data6 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data7 = 0;
}
static void DiagService_RDBPI_ReadPeriodicAccelerationCompleteStatus(void)
{
	Dcm_DiagServiceResponse_Frame.frame.dlc = 8;
	Dcm_DiagServiceResponse_Frame.frame.id = 0x701;
	Dcm_DiagServiceResponse_Frame.frame.idType = 1;
	Dcm_DiagServiceResponse_Frame.frame.data0 = 0x07;
	Dcm_DiagServiceResponse_Frame.frame.data1 = 0x6A;
	if(Dcm_DiagServiceRequest_Frame.frame.data2 != 0x04) Dcm_DiagServiceResponse_Frame.frame.data2 = 0x01;
	else if(Dcm_DiagServiceRequest_Frame.frame.data2 == 0x04) Dcm_DiagServiceResponse_Frame.frame.data2 = 0x04;
	else
	{
		/* Do nothing. */
	}
	Dcm_DiagServiceResponse_Frame.frame.data3 = 0x03;
	Dcm_DiagServiceResponse_Frame.frame.data4 = StatusList_ComOutValue[1];
	Dcm_DiagServiceResponse_Frame.frame.data5 = StatusList_RawValue[1];
	Dcm_DiagServiceResponse_Frame.frame.data6 = StatusList_InputStatus[1].errorStatus;
	Dcm_DiagServiceResponse_Frame.frame.data7 = StatusList_InputStatus[1].DCYStatus;
	CanSpi_Transmit(&Dcm_DiagServiceResponse_Frame);
	Dcm_DiagServiceResponse_Frame.frame.idType = 0;
	Dcm_DiagServiceResponse_Frame.frame.id = 0;
	Dcm_DiagServiceResponse_Frame.frame.dlc = 0;
	Dcm_DiagServiceResponse_Frame.frame.data0 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data1 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data2 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data3 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data4 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data5 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data6 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data7 = 0;
	Dcm_DiagServiceRequest_Frame.frame.idType = 0;
	Dcm_DiagServiceRequest_Frame.frame.id = 0;
	Dcm_DiagServiceRequest_Frame.frame.dlc = 0;
	Dcm_DiagServiceRequest_Frame.frame.data0 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data1 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data2 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data3 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data4 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data5 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data6 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data7 = 0;
}
static void DiagService_RDBPI_ReadPeriodicBrakeCompleteStatus(void)
{
	Dcm_DiagServiceResponse_Frame.frame.dlc = 8;
	Dcm_DiagServiceResponse_Frame.frame.id = 0x701;
	Dcm_DiagServiceResponse_Frame.frame.idType = 1;
	Dcm_DiagServiceResponse_Frame.frame.data0 = 0x07;
	Dcm_DiagServiceResponse_Frame.frame.data1 = 0x6A;
	if(Dcm_DiagServiceRequest_Frame.frame.data2 != 0x04) Dcm_DiagServiceResponse_Frame.frame.data2 = 0x01;
	else if(Dcm_DiagServiceRequest_Frame.frame.data2 == 0x04) Dcm_DiagServiceResponse_Frame.frame.data2 = 0x04;
	else
	{
		/* Do nothing. */
	}
	Dcm_DiagServiceResponse_Frame.frame.data3 = 0x04;
	Dcm_DiagServiceResponse_Frame.frame.data4 = StatusList_ComOutValue[2];
	Dcm_DiagServiceResponse_Frame.frame.data5 = StatusList_RawValue[2];
	Dcm_DiagServiceResponse_Frame.frame.data6 = StatusList_InputStatus[2].errorStatus;
	Dcm_DiagServiceResponse_Frame.frame.data7 = StatusList_InputStatus[2].DCYStatus;
	CanSpi_Transmit(&Dcm_DiagServiceResponse_Frame);
	Dcm_DiagServiceResponse_Frame.frame.idType = 0;
	Dcm_DiagServiceResponse_Frame.frame.id = 0;
	Dcm_DiagServiceResponse_Frame.frame.dlc = 0;
	Dcm_DiagServiceResponse_Frame.frame.data0 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data1 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data2 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data3 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data4 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data5 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data6 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data7 = 0;
	Dcm_DiagServiceRequest_Frame.frame.idType = 0;
	Dcm_DiagServiceRequest_Frame.frame.id = 0;
	Dcm_DiagServiceRequest_Frame.frame.dlc = 0;
	Dcm_DiagServiceRequest_Frame.frame.data0 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data1 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data2 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data3 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data4 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data5 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data6 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data7 = 0;
}
static void DiagService_RDBPI_ReadPeriodicDoorLockActuatorStatus(void)
{
	Dcm_DiagServiceResponse_Frame.frame.dlc = 8;
	Dcm_DiagServiceResponse_Frame.frame.id = 0x701;
	Dcm_DiagServiceResponse_Frame.frame.idType = 1;
	Dcm_DiagServiceResponse_Frame.frame.data0 = 0x07;
	Dcm_DiagServiceResponse_Frame.frame.data1 = 0x6A;
	if(Dcm_DiagServiceRequest_Frame.frame.data2 != 0x04) Dcm_DiagServiceResponse_Frame.frame.data2 = 0x01;
	else if(Dcm_DiagServiceRequest_Frame.frame.data2 == 0x04) Dcm_DiagServiceResponse_Frame.frame.data2 = 0x04;
	else
	{
		/* Do nothing. */
	}
	Dcm_DiagServiceResponse_Frame.frame.data3 = 0x05;
	Dcm_DiagServiceResponse_Frame.frame.data4 = CmdList_RawValue[2];
	Dcm_DiagServiceResponse_Frame.frame.data5 = CmdList_RawValue[3];
	Dcm_DiagServiceResponse_Frame.frame.data6 = CmdList_ActualValue[2];
	Dcm_DiagServiceResponse_Frame.frame.data7 = CmdList_ActualValue[3];
	CanSpi_Transmit(&Dcm_DiagServiceResponse_Frame);
	Dcm_DiagServiceResponse_Frame.frame.idType = 0;
	Dcm_DiagServiceResponse_Frame.frame.id = 0;
	Dcm_DiagServiceResponse_Frame.frame.dlc = 0;
	Dcm_DiagServiceResponse_Frame.frame.data0 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data1 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data2 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data3 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data4 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data5 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data6 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data7 = 0;
	Dcm_DiagServiceRequest_Frame.frame.idType = 0;
	Dcm_DiagServiceRequest_Frame.frame.id = 0;
	Dcm_DiagServiceRequest_Frame.frame.dlc = 0;
	Dcm_DiagServiceRequest_Frame.frame.data0 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data1 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data2 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data3 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data4 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data5 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data6 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data7 = 0;
}
static void DiagService_RDBPI_ReadPeriodicWindshieldWiperActuatorStatus(void)
{
	Dcm_DiagServiceResponse_Frame.frame.dlc = 8;
	Dcm_DiagServiceResponse_Frame.frame.id = 0x701;
	Dcm_DiagServiceResponse_Frame.frame.idType = 1;
	Dcm_DiagServiceResponse_Frame.frame.data0 = 0x07;
	Dcm_DiagServiceResponse_Frame.frame.data1 = 0x6A;
	if(Dcm_DiagServiceRequest_Frame.frame.data2 != 0x04) Dcm_DiagServiceResponse_Frame.frame.data2 = 0x01;
	else if(Dcm_DiagServiceRequest_Frame.frame.data2 == 0x04) Dcm_DiagServiceResponse_Frame.frame.data2 = 0x04;
	else
	{
		/* Do nothing. */
	}
	Dcm_DiagServiceResponse_Frame.frame.data3 = 0x06;
	Dcm_DiagServiceResponse_Frame.frame.data4 = CmdList_RawValue[0];
	Dcm_DiagServiceResponse_Frame.frame.data5 = CmdList_RawValue[1];
	Dcm_DiagServiceResponse_Frame.frame.data6 = CmdList_ActualValue[0];
	Dcm_DiagServiceResponse_Frame.frame.data7 = CmdList_ActualValue[1];
	CanSpi_Transmit(&Dcm_DiagServiceResponse_Frame);
	Dcm_DiagServiceResponse_Frame.frame.idType = 0;
	Dcm_DiagServiceResponse_Frame.frame.id = 0;
	Dcm_DiagServiceResponse_Frame.frame.dlc = 0;
	Dcm_DiagServiceResponse_Frame.frame.data0 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data1 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data2 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data3 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data4 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data5 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data6 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data7 = 0;
	Dcm_DiagServiceRequest_Frame.frame.idType = 0;
	Dcm_DiagServiceRequest_Frame.frame.id = 0;
	Dcm_DiagServiceRequest_Frame.frame.dlc = 0;
	Dcm_DiagServiceRequest_Frame.frame.data0 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data1 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data2 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data3 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data4 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data5 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data6 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data7 = 0;
}
static void DiagService_RDBPI_ReadPeriodicClimaFanStatus(void)
{
	Dcm_DiagServiceResponse_Frame.frame.dlc = 6;
	Dcm_DiagServiceResponse_Frame.frame.id = 0x701;
	Dcm_DiagServiceResponse_Frame.frame.idType = 1;
	Dcm_DiagServiceResponse_Frame.frame.data0 = 0x05;
	Dcm_DiagServiceResponse_Frame.frame.data1 = 0x6A;
	if(Dcm_DiagServiceRequest_Frame.frame.data2 != 0x04) Dcm_DiagServiceResponse_Frame.frame.data2 = 0x01;
	else if(Dcm_DiagServiceRequest_Frame.frame.data2 == 0x04) Dcm_DiagServiceResponse_Frame.frame.data2 = 0x04;
	else
	{
		/* Do nothing. */
	}
	Dcm_DiagServiceResponse_Frame.frame.data3 = 0x07;
	Dcm_DiagServiceResponse_Frame.frame.data4 = CmdList_RawValue[4];
	Dcm_DiagServiceResponse_Frame.frame.data5 = CmdList_ActualValue[4];
	CanSpi_Transmit(&Dcm_DiagServiceResponse_Frame);
	Dcm_DiagServiceResponse_Frame.frame.idType = 0;
	Dcm_DiagServiceResponse_Frame.frame.id = 0;
	Dcm_DiagServiceResponse_Frame.frame.dlc = 0;
	Dcm_DiagServiceResponse_Frame.frame.data0 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data1 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data2 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data3 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data4 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data5 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data6 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data7 = 0;
	Dcm_DiagServiceRequest_Frame.frame.idType = 0;
	Dcm_DiagServiceRequest_Frame.frame.id = 0;
	Dcm_DiagServiceRequest_Frame.frame.dlc = 0;
	Dcm_DiagServiceRequest_Frame.frame.data0 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data1 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data2 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data3 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data4 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data5 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data6 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data7 = 0;
}
static void DiagService_RDBPI_ReadPeriodicAQS(void)
{
	Dcm_DiagServiceResponse_Frame.frame.dlc = 8;
	Dcm_DiagServiceResponse_Frame.frame.id = 0x701;
	Dcm_DiagServiceResponse_Frame.frame.idType = 1;
	Dcm_DiagServiceResponse_Frame.frame.data0 = 0x07;
	Dcm_DiagServiceResponse_Frame.frame.data1 = 0x6A;
	if(Dcm_DiagServiceRequest_Frame.frame.data2 != 0x04) Dcm_DiagServiceResponse_Frame.frame.data2 = 0x01;
	else if(Dcm_DiagServiceRequest_Frame.frame.data2 == 0x04) Dcm_DiagServiceResponse_Frame.frame.data2 = 0x04;
	else
	{
		/* Do nothing. */
	}
	Dcm_DiagServiceResponse_Frame.frame.data3 = 0x08;
	Dcm_DiagServiceResponse_Frame.frame.data4 = StatusList_ComOutValue[7];
	Dcm_DiagServiceResponse_Frame.frame.data5 = StatusList_RawValue[7];
	Dcm_DiagServiceResponse_Frame.frame.data6 = StatusList_InputStatus[7].errorStatus;
	Dcm_DiagServiceResponse_Frame.frame.data7 = StatusList_InputStatus[7].DCYStatus;
	CanSpi_Transmit(&Dcm_DiagServiceResponse_Frame);
	Dcm_DiagServiceResponse_Frame.frame.idType = 0;
	Dcm_DiagServiceResponse_Frame.frame.id = 0;
	Dcm_DiagServiceResponse_Frame.frame.dlc = 0;
	Dcm_DiagServiceResponse_Frame.frame.data0 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data1 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data2 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data3 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data4 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data5 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data6 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data7 = 0;
	Dcm_DiagServiceRequest_Frame.frame.idType = 0;
	Dcm_DiagServiceRequest_Frame.frame.id = 0;
	Dcm_DiagServiceRequest_Frame.frame.dlc = 0;
	Dcm_DiagServiceRequest_Frame.frame.data0 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data1 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data2 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data3 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data4 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data5 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data6 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data7 = 0;
}
static void DiagService_RDBPI_ReadPeriodicGS(void)
{
	Dcm_DiagServiceResponse_Frame.frame.dlc = 8;
	Dcm_DiagServiceResponse_Frame.frame.id = 0x701;
	Dcm_DiagServiceResponse_Frame.frame.idType = 1;
	Dcm_DiagServiceResponse_Frame.frame.data0 = 0x07;
	Dcm_DiagServiceResponse_Frame.frame.data1 = 0x6A;
	if(Dcm_DiagServiceRequest_Frame.frame.data2 != 0x04) Dcm_DiagServiceResponse_Frame.frame.data2 = 0x01;
	else if(Dcm_DiagServiceRequest_Frame.frame.data2 == 0x04) Dcm_DiagServiceResponse_Frame.frame.data2 = 0x04;
	else
	{
		/* Do nothing. */
	}
	Dcm_DiagServiceResponse_Frame.frame.data3 = 0x09;
	Dcm_DiagServiceResponse_Frame.frame.data4 = StatusList_ComOutValue[8];
	Dcm_DiagServiceResponse_Frame.frame.data5 = StatusList_RawValue[8];
	Dcm_DiagServiceResponse_Frame.frame.data6 = StatusList_InputStatus[8].errorStatus;
	Dcm_DiagServiceResponse_Frame.frame.data7 = StatusList_InputStatus[8].DCYStatus;
	CanSpi_Transmit(&Dcm_DiagServiceResponse_Frame);
	Dcm_DiagServiceResponse_Frame.frame.idType = 0;
	Dcm_DiagServiceResponse_Frame.frame.id = 0;
	Dcm_DiagServiceResponse_Frame.frame.dlc = 0;
	Dcm_DiagServiceResponse_Frame.frame.data0 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data1 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data2 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data3 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data4 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data5 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data6 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data7 = 0;
	Dcm_DiagServiceRequest_Frame.frame.idType = 0;
	Dcm_DiagServiceRequest_Frame.frame.id = 0;
	Dcm_DiagServiceRequest_Frame.frame.dlc = 0;
	Dcm_DiagServiceRequest_Frame.frame.data0 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data1 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data2 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data3 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data4 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data5 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data6 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data7 = 0;
}
static void DiagService_RDBPI_ReadPeriodicLS(void)
{
	Dcm_DiagServiceResponse_Frame.frame.dlc = 8;
	Dcm_DiagServiceResponse_Frame.frame.id = 0x701;
	Dcm_DiagServiceResponse_Frame.frame.idType = 1;
	Dcm_DiagServiceResponse_Frame.frame.data0 = 0x07;
	Dcm_DiagServiceResponse_Frame.frame.data1 = 0x6A;
	if(Dcm_DiagServiceRequest_Frame.frame.data2 != 0x04) Dcm_DiagServiceResponse_Frame.frame.data2 = 0x01;
	else if(Dcm_DiagServiceRequest_Frame.frame.data2 == 0x04) Dcm_DiagServiceResponse_Frame.frame.data2 = 0x04;
	else
	{
		/* Do nothing. */
	}
	Dcm_DiagServiceResponse_Frame.frame.data3 = 0x0A;
	Dcm_DiagServiceResponse_Frame.frame.data4 = StatusList_ComOutValue[5];
	Dcm_DiagServiceResponse_Frame.frame.data5 = StatusList_RawValue[5];
	Dcm_DiagServiceResponse_Frame.frame.data6 = StatusList_InputStatus[5].errorStatus;
	Dcm_DiagServiceResponse_Frame.frame.data7 = StatusList_InputStatus[5].DCYStatus;
	CanSpi_Transmit(&Dcm_DiagServiceResponse_Frame);
	Dcm_DiagServiceResponse_Frame.frame.idType = 0;
	Dcm_DiagServiceResponse_Frame.frame.id = 0;
	Dcm_DiagServiceResponse_Frame.frame.dlc = 0;
	Dcm_DiagServiceResponse_Frame.frame.data0 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data1 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data2 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data3 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data4 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data5 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data6 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data7 = 0;
	Dcm_DiagServiceRequest_Frame.frame.idType = 0;
	Dcm_DiagServiceRequest_Frame.frame.id = 0;
	Dcm_DiagServiceRequest_Frame.frame.dlc = 0;
	Dcm_DiagServiceRequest_Frame.frame.data0 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data1 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data2 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data3 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data4 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data5 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data6 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data7 = 0;
}
static void DiagService_RDBPI_ReadPeriodicRS(void)
{
	Dcm_DiagServiceResponse_Frame.frame.dlc = 8;
	Dcm_DiagServiceResponse_Frame.frame.id = 0x701;
	Dcm_DiagServiceResponse_Frame.frame.idType = 1;
	Dcm_DiagServiceResponse_Frame.frame.data0 = 0x07;
	Dcm_DiagServiceResponse_Frame.frame.data1 = 0x6A;
	if(Dcm_DiagServiceRequest_Frame.frame.data2 != 0x04) Dcm_DiagServiceResponse_Frame.frame.data2 = 0x01;
	else if(Dcm_DiagServiceRequest_Frame.frame.data2 == 0x04) Dcm_DiagServiceResponse_Frame.frame.data2 = 0x04;
	else
	{
		/* Do nothing. */
	}
	Dcm_DiagServiceResponse_Frame.frame.data3 = 0x0B;
	Dcm_DiagServiceResponse_Frame.frame.data4 = StatusList_ComOutValue[6];
	Dcm_DiagServiceResponse_Frame.frame.data5 = StatusList_RawValue[6];
	Dcm_DiagServiceResponse_Frame.frame.data6 = StatusList_InputStatus[6].errorStatus;
	Dcm_DiagServiceResponse_Frame.frame.data7 = StatusList_InputStatus[6].DCYStatus;
	CanSpi_Transmit(&Dcm_DiagServiceResponse_Frame);
	Dcm_DiagServiceResponse_Frame.frame.idType = 0;
	Dcm_DiagServiceResponse_Frame.frame.id = 0;
	Dcm_DiagServiceResponse_Frame.frame.dlc = 0;
	Dcm_DiagServiceResponse_Frame.frame.data0 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data1 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data2 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data3 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data4 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data5 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data6 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data7 = 0;
	Dcm_DiagServiceRequest_Frame.frame.idType = 0;
	Dcm_DiagServiceRequest_Frame.frame.id = 0;
	Dcm_DiagServiceRequest_Frame.frame.dlc = 0;
	Dcm_DiagServiceRequest_Frame.frame.data0 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data1 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data2 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data3 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data4 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data5 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data6 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data7 = 0;
}
static void DiagService_DSC_DefaultSession(void)
{
	Dcm_DiagServiceResponse_Frame.frame.dlc = 3;
	Dcm_DiagServiceResponse_Frame.frame.id = 0x701;
	Dcm_DiagServiceResponse_Frame.frame.idType = 1;
	Dcm_DiagServiceResponse_Frame.frame.data0 = 0x02;
	Dcm_DiagServiceResponse_Frame.frame.data1 = 0x50;
	Dcm_DiagServiceResponse_Frame.frame.data2 = 0x01;
	Dcm_DiagnosticSession = DEFAULT;
	CanSpi_Transmit(&Dcm_DiagServiceResponse_Frame);
	Dcm_DiagServiceResponse_Frame.frame.idType = 0;
	Dcm_DiagServiceResponse_Frame.frame.id = 0;
	Dcm_DiagServiceResponse_Frame.frame.dlc = 0;
	Dcm_DiagServiceResponse_Frame.frame.data0 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data1 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data2 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data3 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data4 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data5 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data6 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data7 = 0;
	Dcm_DiagServiceRequest_Frame.frame.idType = 0;
	Dcm_DiagServiceRequest_Frame.frame.id = 0;
	Dcm_DiagServiceRequest_Frame.frame.dlc = 0;
	Dcm_DiagServiceRequest_Frame.frame.data0 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data1 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data2 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data3 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data4 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data5 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data6 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data7 = 0;
}
static void DiagService_DSC_ExtendedSession(void)
{
	Dcm_DiagServiceResponse_Frame.frame.dlc = 3;
	Dcm_DiagServiceResponse_Frame.frame.id = 0x701;
	Dcm_DiagServiceResponse_Frame.frame.idType = 1;
	Dcm_DiagServiceResponse_Frame.frame.data0 = 0x02;
	Dcm_DiagServiceResponse_Frame.frame.data1 = 0x50;
	Dcm_DiagServiceResponse_Frame.frame.data2 = 0x03;
	Dcm_DiagnosticSession = EXTENDED;
	CanSpi_Transmit(&Dcm_DiagServiceResponse_Frame);
	Dcm_DiagServiceResponse_Frame.frame.idType = 0;
	Dcm_DiagServiceResponse_Frame.frame.id = 0;
	Dcm_DiagServiceResponse_Frame.frame.dlc = 0;
	Dcm_DiagServiceResponse_Frame.frame.data0 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data1 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data2 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data3 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data4 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data5 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data6 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data7 = 0;
	Dcm_DiagServiceRequest_Frame.frame.idType = 0;
	Dcm_DiagServiceRequest_Frame.frame.id = 0;
	Dcm_DiagServiceRequest_Frame.frame.dlc = 0;
	Dcm_DiagServiceRequest_Frame.frame.data0 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data1 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data2 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data3 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data4 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data5 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data6 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data7 = 0;
}
static void DiagService_DSC_ProgrammingSession(void)
{
	Dcm_DiagServiceResponse_Frame.frame.dlc = 3;
	Dcm_DiagServiceResponse_Frame.frame.id = 0x701;
	Dcm_DiagServiceResponse_Frame.frame.idType = 1;
	Dcm_DiagServiceResponse_Frame.frame.data0 = 0x02;
	Dcm_DiagServiceResponse_Frame.frame.data1 = 0x50;
	Dcm_DiagServiceResponse_Frame.frame.data2 = 0x02;
	Dcm_DiagnosticSession = PROGRAMMING;
	CanSpi_Transmit(&Dcm_DiagServiceResponse_Frame);
	NvM_WriteAll();
	EcuM_PerformReset(0);
}
static void DiagService_ER_HardReset(void)
{
	Dcm_DiagServiceResponse_Frame.frame.dlc = 3;
	Dcm_DiagServiceResponse_Frame.frame.id = 0x701;
	Dcm_DiagServiceResponse_Frame.frame.idType = 1;
	Dcm_DiagServiceResponse_Frame.frame.data0 = 0x02;
	Dcm_DiagServiceResponse_Frame.frame.data1 = 0x51;
	Dcm_DiagServiceResponse_Frame.frame.data2 = 0x01;
	Dcm_DiagnosticSession = HARDRESET;
	CanSpi_Transmit(&Dcm_DiagServiceResponse_Frame);
	NvM_WriteAll();
	EcuM_PerformReset(0);
}
static void DiagService_ER_SoftReset(void)
{
	Dcm_DiagServiceResponse_Frame.frame.dlc = 3;
	Dcm_DiagServiceResponse_Frame.frame.id = 0x701;
	Dcm_DiagServiceResponse_Frame.frame.idType = 1;
	Dcm_DiagServiceResponse_Frame.frame.data0 = 0x02;
	Dcm_DiagServiceResponse_Frame.frame.data1 = 0x51;
	Dcm_DiagServiceResponse_Frame.frame.data2 = 0x03;
	Dcm_DiagnosticSession = SOFTRESET;
	CanSpi_Transmit(&Dcm_DiagServiceResponse_Frame);
	NvM_WriteAll();
	EcuM_PerformReset(0);
}
static void DiagService_CDTCI_ClearDiagnosticInformation(void)
{
	Dcm_DiagServiceResponse_Frame.frame.dlc = 2;
	Dcm_DiagServiceResponse_Frame.frame.id = 0x701;
	Dcm_DiagServiceResponse_Frame.frame.idType = 1;
	Dcm_DiagServiceResponse_Frame.frame.data0 = 0x01;
	Dcm_DiagServiceResponse_Frame.frame.data1 = 0x54;
	memset(UartH_ErrorArr, 0, sizeof(UartH_ErrorArr));
	memset(EcuMSystem_OvwArr.faultArr, 0, sizeof(EcuMSystem_OvwArr.faultArr));
	memset(EcuMSystem_OvwArr.resetArr, 0, sizeof(EcuMSystem_OvwArr.resetArr));
	memset(CanSpi_ErrorArr, 0, sizeof(CanSpi_ErrorArr));
	memset(CanSpi_Bus_ErrorArr, 0, sizeof(CanSpi_Bus_ErrorArr));
	memset(Adc_Error, 0, sizeof(Adc_Error));
	memset(Tim_ErrorStatus, 0, sizeof(Tim_ErrorStatus));
	memset(Dem_DTCStoreArray, 0, sizeof(Dem_DTCStoreArray));
	CanSpi_Transmit(&Dcm_DiagServiceResponse_Frame);
	Dcm_DiagServiceResponse_Frame.frame.idType = 0;
	Dcm_DiagServiceResponse_Frame.frame.id = 0;
	Dcm_DiagServiceResponse_Frame.frame.dlc = 0;
	Dcm_DiagServiceResponse_Frame.frame.data0 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data1 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data2 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data3 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data4 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data5 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data6 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data7 = 0;
	Dcm_DiagServiceRequest_Frame.frame.idType = 0;
	Dcm_DiagServiceRequest_Frame.frame.id = 0;
	Dcm_DiagServiceRequest_Frame.frame.dlc = 0;
	Dcm_DiagServiceRequest_Frame.frame.data0 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data1 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data2 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data3 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data4 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data5 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data6 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data7 = 0;
}
static void DiagService_RDTCI_ReadDTCInformationSupportedDtc(void)
{
	__disable_irq();
	Dcm_DiagServiceResponse_Frame.frame.dlc = 8;
	Dcm_DiagServiceResponse_Frame.frame.id = 0x701;
	Dcm_DiagServiceResponse_Frame.frame.idType = 1;
	Dcm_DiagServiceResponse_Frame.frame.data0 = 0x02;
	Dcm_DiagServiceResponse_Frame.frame.data1 = 0x59;
	Dcm_DiagServiceResponse_Frame.frame.data2 = 0x0A;
	CanSpi_Transmit(&Dcm_DiagServiceResponse_Frame);
	Dcm_TxIsoTp((uint8*)Dem_DTCStoreArray, 13);
	__enable_irq();
}
void Dcm_MainFunction(void)
{
	/* Main counter. */
	static uint32 Dcm_MainCounter = 0;
	/* Increment time without reset. */
	Dcm_AliveCounter++;
	/* Set diagnostic session to default. */
	if(Dcm_MainCounter == 0) Dcm_DiagnosticSession = DEFAULT;
	else
	{
		/* Do nothing. */
	}
	if(Dcm_DiagServiceRequest_Frame.frame.data1 == 0x3E) DiagService_TP_TesterPresent();
	else
	{
		/* Do nothing. */
	}
	if(Dcm_DiagServiceRequest_Frame.frame.data1 == 0x28) DiagService_CC_CommunicationControl();
	else
	{
		/* Do nothing. */
	}
	if(Dcm_DiagServiceRequest_Frame.frame.data1 == 0x85) DiagService_CDTCS_ControlDTCSetting();
	else
	{
		/* Do nothing. */
	}
	/* Process diagnostic session control. */
	if(Dcm_DiagServiceRequest_Frame.frame.data1 == 0x10)
	{
		/* DEFAULT */
		if(Dcm_DiagServiceRequest_Frame.frame.data2 == 0x01) DiagService_DSC_DefaultSession();
		else
		{
			/* Do nothing. */
		}
		/* EXTENDED */
		if(Dcm_DiagServiceRequest_Frame.frame.data2 == 0x03) DiagService_DSC_ExtendedSession();
		else
		{
			/* Do nothing. */
		}
		/* PROGRAMMING */
		if(Dcm_DiagServiceRequest_Frame.frame.data2 == 0x02 && Dcm_DiagnosticSession == EXTENDED) DiagService_DSC_ProgrammingSession();
		else
		{
			/* Do nothing. */
		}
		/* CODING */
		if(Dcm_DiagServiceRequest_Frame.frame.data2 == 0x04 && Dcm_DiagnosticSession == EXTENDED) DiagService_DSC_CodingSession();
		else
		{
			/* Do nothing. */
		}
		/* CALIBRATION */
		if(Dcm_DiagServiceRequest_Frame.frame.data2 == 0x05 && Dcm_DiagnosticSession == EXTENDED) DiagService_DSC_CalibrationSession();
		else
		{
			/* Do nothing. */
		}
	}
	else
	{
		/* Do nothing. */
	}

	if(Dcm_DiagServiceRequest_Frame.frame.data3 == 0x86 && Dcm_DiagServiceRequest_Frame.frame.data2 == 0xF1 && Dcm_DiagServiceRequest_Frame.frame.data1 == 0x22) DiagService_RDBI_ReadActiveDiagnosticSession();
	else
	{
		/* Do nothing. */
	}
	/* Execute hard reset. */
	if(Dcm_DiagServiceRequest_Frame.frame.data1 == 0x11 && Dcm_DiagServiceRequest_Frame.frame.data2 == 0x01 &&Dcm_DiagServiceRequest_Frame.frame.data0 == 0x02) DiagService_ER_HardReset();
	else
	{
		/* Do nothing. */
	}
	/* Execute soft reset. */
	if(Dcm_DiagServiceRequest_Frame.frame.data1 == 0x11 && Dcm_DiagServiceRequest_Frame.frame.data2 == 0x03 && Dcm_DiagServiceRequest_Frame.frame.data0 == 0x02) DiagService_ER_SoftReset();
	else
	{
		/* Do nothing. */
	}
	/* Execute clear DTC. */
	if(Dcm_DiagServiceRequest_Frame.frame.data1 == 0x14) DiagService_CDTCI_ClearDiagnosticInformation();
	else
	{
		/* Do nothing. */
	}
	/* Execute read DTC. */
	if(Dcm_DiagServiceRequest_Frame.frame.data1 == 0x19) DiagService_RDTCI_ReadDTCInformationSupportedDtc();
	else
	{
		/* Do nothing. */
	}
	/* Extended session routines. */
	if(Dcm_DiagnosticSession == EXTENDED)
	{
		/* Write calibration data. */
		if(Dcm_DiagServiceRequest_Frame.frame.data1 == 0x2E)
		{
			if(Dcm_DiagServiceRequest_Frame.frame.data2 == 0x00)
			{
				if(Dcm_DiagServiceRequest_Frame.frame.data3 == 0x00) DiagService_WDBI_WriteCalibrationData();
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
		/* Process RDPBI. */
		if(Dcm_DiagServiceRequest_Frame.frame.data1 == 0x2A)
		{
			if(Dcm_DiagServiceRequest_Frame.frame.data2 == 0x04)
			{
				Dcm_RDBPI_Table[Dcm_DiagServiceRequest_Frame.frame.data3].FuncPtr();
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
		/* Read data routines. */
		if(Dcm_DiagServiceRequest_Frame.frame.data1 == 0x22)
		{
			if(Dcm_DiagServiceRequest_Frame.frame.data2 == 0x00)
			{
				if(Dcm_DiagServiceRequest_Frame.frame.data3 == 0x01) DiagService_RDBI_ReadAwakeReasons();
				else
				{
					/* Do nothing. */
				}
				if(Dcm_DiagServiceRequest_Frame.frame.data3 == 0x02) DiagService_RDBI_ReadCodingData();
				else
				{
					/* Do nothing. */
				}
				if(Dcm_DiagServiceRequest_Frame.frame.data3 == 0x03) DiagService_RDBI_ReadCalibrationData();
				else
				{
					/* Do nothing. */
				}
				if(Dcm_DiagServiceRequest_Frame.frame.data3 == 0x05) DiagService_RDBI_ReadAliveTime();
				else
				{
					/* Do nothing. */
				}
			}
			else
			{
				/* Do nothing. */
			}
			if(Dcm_DiagServiceRequest_Frame.frame.data2 == 0xF1)
			{
				if(Dcm_DiagServiceRequest_Frame.frame.data3 == 0x80) DiagService_RDBI_ReadSWVersion();
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
	{/* Send NRC conditionsNotCorrect. */
		if(Dcm_DiagServiceRequest_Frame.frame.data1 == 0x22 || (Dcm_DiagServiceRequest_Frame.frame.data1 == 0x10 && (Dcm_DiagServiceRequest_Frame.frame.data2 != 0x01 || Dcm_DiagServiceRequest_Frame.frame.data2 != 0x03)))
		{
			Dcm_DiagServiceResponse_Frame.frame.dlc = 4;
			Dcm_DiagServiceResponse_Frame.frame.id = 0x701;
			Dcm_DiagServiceResponse_Frame.frame.idType = 1;
			Dcm_DiagServiceResponse_Frame.frame.data0 = 0x03;
			Dcm_DiagServiceResponse_Frame.frame.data1 = 0x7F;
			Dcm_DiagServiceResponse_Frame.frame.data2 = 0x19;
			Dcm_DiagServiceResponse_Frame.frame.data3 = 0x22;
			CanSpi_Transmit(&Dcm_DiagServiceResponse_Frame);
			Dcm_DiagServiceResponse_Frame.frame.idType = 0;
			Dcm_DiagServiceResponse_Frame.frame.id = 0;
			Dcm_DiagServiceResponse_Frame.frame.dlc = 0;
			Dcm_DiagServiceResponse_Frame.frame.data0 = 0;
			Dcm_DiagServiceResponse_Frame.frame.data1 = 0;
			Dcm_DiagServiceResponse_Frame.frame.data2 = 0;
			Dcm_DiagServiceResponse_Frame.frame.data3 = 0;
			Dcm_DiagServiceResponse_Frame.frame.data4 = 0;
			Dcm_DiagServiceResponse_Frame.frame.data5 = 0;
			Dcm_DiagServiceResponse_Frame.frame.data6 = 0;
			Dcm_DiagServiceResponse_Frame.frame.data7 = 0;
			Dcm_DiagServiceRequest_Frame.frame.idType = 0;
			Dcm_DiagServiceRequest_Frame.frame.id = 0;
			Dcm_DiagServiceRequest_Frame.frame.dlc = 0;
			Dcm_DiagServiceRequest_Frame.frame.data0 = 0;
			Dcm_DiagServiceRequest_Frame.frame.data1 = 0;
			Dcm_DiagServiceRequest_Frame.frame.data2 = 0;
			Dcm_DiagServiceRequest_Frame.frame.data3 = 0;
			Dcm_DiagServiceRequest_Frame.frame.data4 = 0;
			Dcm_DiagServiceRequest_Frame.frame.data5 = 0;
			Dcm_DiagServiceRequest_Frame.frame.data6 = 0;
			Dcm_DiagServiceRequest_Frame.frame.data7 = 0;
		}
		else
		{
			/* Do nothing. */
		}
	}
	/* Reset buffers. */
	Dcm_DiagServiceResponse_Frame.frame.idType = 0;
	Dcm_DiagServiceResponse_Frame.frame.id = 0;
	Dcm_DiagServiceResponse_Frame.frame.dlc = 0;
	Dcm_DiagServiceResponse_Frame.frame.data0 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data1 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data2 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data3 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data4 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data5 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data6 = 0;
	Dcm_DiagServiceResponse_Frame.frame.data7 = 0;
	Dcm_DiagServiceRequest_Frame.frame.idType = 0;
	Dcm_DiagServiceRequest_Frame.frame.id = 0;
	Dcm_DiagServiceRequest_Frame.frame.dlc = 0;
	Dcm_DiagServiceRequest_Frame.frame.data0 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data1 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data2 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data3 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data4 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data5 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data6 = 0;
	Dcm_DiagServiceRequest_Frame.frame.data7 = 0;
	Dcm_MainCounter++;
}
/* FUNCTIONS END */
/* STOP OF FILE */
