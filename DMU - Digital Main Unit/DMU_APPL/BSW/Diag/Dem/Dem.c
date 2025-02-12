#include "Dem.h"
#include "can.h"

__attribute__((section(".ccmram"))) uint32 Dem_MainCounter = 0;
__attribute__((section(".ccmram"))) Dem_DTC_t Dem_DTCArray[DEM_NUMBER_OF_DTCS];
__attribute__((section(".ccmram"))) uint8 Dem_DTCSettingDeactivated = 0;
__attribute__((section(".ccmram"))) CAN_TxHeaderTypeDef Dem_TxHeader = {0, 0, 0, 0, 0, 0};
__attribute__((section(".ccmram"))) uint8 Dem_TxData[8] = {0};
__attribute__((section(".ccmram"))) uint32 Dem_TxMailbox = 0;
extern __attribute__((section(".ccmram"))) uint32 CanH_ErrArr[21];
extern __attribute__((section(".ccmram"))) uint32 I2cH_ErrArr[9];
extern __attribute__((section(".ccmram"))) uint32 Ain_DmaErrArr[7];
extern __attribute__((section(".ccmram"))) uint32 Ain_ErrArr[3];
extern __attribute__((section(".ccmram"))) uint32 DcmiH_ErrArr[4];
extern __attribute__((section(".ccmram"))) uint32 DcmiH_DmaErrArr[7];
extern __attribute__((section(".ccmram"))) uint32 FsmcH_DmaErrArr[7];
extern __attribute__((section(".ccmram"))) uint8 CanH_RxSig_Ignition;
extern __attribute__((section(".ccmram"))) uint8 CanH_RxSig_Speed;
extern __attribute__((section(".ccmram"))) uint8 CanH_RxSig_Rpm;
extern __attribute__((section(".ccmram"))) uint8 CanH_RxSig_VBat;
extern __attribute__((section(".ccmram"))) uint8 CanH_RxSig_Gear;
extern __attribute__((section(".ncr"))) uint32 Dcm_AliveCounter;

void Dem_MainFunction(void);
void Dem_SetDtc(uint8 IDPrimary, uint32 Reason, uint8 Status);
void Dem_ClearDtc(void);

void Dem_ClearDtc(void)
{
	for(uint8 i = 0; i < 21; i++)
	{
		CanH_ErrArr[i] = 0;
		if(i < 7)
		{
			Ain_DmaErrArr[i] = 0;
			DcmiH_DmaErrArr[i] = 0;
			FsmcH_DmaErrArr[i] = 0;
		}
		else
		{
			/* Do nothing. */
		}
		if (i < 4) DcmiH_ErrArr[i] = 0;
		else
		{
			/* Do nothing. */
		}
		if(i < 3) Ain_ErrArr[i] = 0;
		else
		{
			/* Do nothing. */
		}
		if(i < 9) I2cH_ErrArr[i] = 0;
		else
		{
			/* Do nothing. */
		}
		if(i < 5)
		{
			Dem_DTCArray[i].counter = 0;
			Dem_DTCArray[i].isActiveNow = 0;
			for(uint8 j = 0; j < 3; j++)
			{
				Dem_DTCArray[i].GearFF[i] = 0;
				Dem_DTCArray[i].IgnFF[i] = 0;
				Dem_DTCArray[i].RpmFF[i] = 0;
				Dem_DTCArray[i].ReasonFF[i] = 0;
				Dem_DTCArray[i].SpeedFF[i] = 0;
				Dem_DTCArray[i].TimeFF[i] = 0;
				Dem_DTCArray[i].VoltageFF[i] = 0;
			}
		}
		else
		{
			/* Do nothing. */
		}
	}
}
void Dem_SetDtc(uint8 IDPrimary, uint32 Reason, uint8 Status)
{
	uint8 i = 0;
	if(0 == Dem_DTCSettingDeactivated)
	{
		if(1 == Status && Dem_DTCArray[IDPrimary].counter < 4)
		{
			Dem_TxHeader.DLC = 1;
			Dem_TxHeader.StdId = 0x10D;
			Dem_TxData[0] = IDPrimary;
			HAL_CAN_AddTxMessage(&hcan1, &Dem_TxHeader, Dem_TxData, &Dem_TxMailbox);
			for(i = 0; i < 3; i++)
			{
				if(0 != Dem_DTCArray[IDPrimary].GearFF[i]) break;
				else
				{
					/* Do nothing. */
				}
			}
			Dem_DTCArray[IDPrimary].counter++;
			Dem_DTCArray[IDPrimary].isActiveNow = 1;
			Dem_DTCArray[IDPrimary].GearFF[i] = CanH_RxSig_Gear;
			Dem_DTCArray[IDPrimary].IgnFF[i] = CanH_RxSig_Ignition;
			Dem_DTCArray[IDPrimary].RpmFF[i] = CanH_RxSig_Rpm;
			Dem_DTCArray[IDPrimary].ReasonFF[i] = Reason;
			Dem_DTCArray[IDPrimary].SpeedFF[i] = CanH_RxSig_Speed;
			Dem_DTCArray[IDPrimary].TimeFF[i] = Dcm_AliveCounter;
			Dem_DTCArray[IDPrimary].VoltageFF[i] = CanH_RxSig_VBat;
		}
		else Dem_DTCArray[IDPrimary].isActiveNow = 0;
	}
	else
	{
		/* Do nothing. */
	}
}
void Dem_MainFunction(void)
{
	if(0 == Dem_MainCounter) for(uint8 i = 0; i < 5; i++) Dem_DTCArray[i].isActiveNow = 0;
	else
	{
		/* Do nothing. */
	}
	for(uint8 i = 0; i < 21; i++)
	{
		if(0 != CanH_ErrArr[i]) Dem_SetDtc(DEM_COMMUNICATION_ERROR_ID, CanH_ErrArr[i], 1);
		else Dem_SetDtc(DEM_COMMUNICATION_ERROR_ID, CanH_ErrArr[i], 0);
		if(i < 7)
		{
			if(0 != Ain_DmaErrArr[i]) Dem_SetDtc(DEM_CONTROLLER_INTERNALFAULT_ID, Ain_DmaErrArr[i], 1);
			else Dem_SetDtc(DEM_CONTROLLER_INTERNALFAULT_ID, Ain_DmaErrArr[i], 0);
			if(0 != DcmiH_DmaErrArr[i]) Dem_SetDtc(DEM_REVERSECAMERA_DCMIINTERFACE_MALFUNCTION_ID, DcmiH_DmaErrArr[i], 1);
			else Dem_SetDtc(DEM_REVERSECAMERA_DCMIINTERFACE_MALFUNCTION_ID, DcmiH_DmaErrArr[i], 0);
			if(0 != FsmcH_DmaErrArr[i]) Dem_SetDtc(DEM_DIGITALCLUSTER_CONTROLINTERFACE_MALFUNCTION_ID, FsmcH_DmaErrArr[i], 1);
			else Dem_SetDtc(DEM_DIGITALCLUSTER_CONTROLINTERFACE_MALFUNCTION_ID, FsmcH_DmaErrArr[i], 0);
		}
		if (i < 4)
		{
			if(0 != DcmiH_ErrArr[i]) Dem_SetDtc(DEM_REVERSECAMERA_DCMIINTERFACE_MALFUNCTION_ID, DcmiH_ErrArr[i], 1);
			else Dem_SetDtc(DEM_REVERSECAMERA_DCMIINTERFACE_MALFUNCTION_ID, DcmiH_ErrArr[i], 0);
		}
		else
		{
			/* Do nothing. */
		}
		if(i < 3)
		{
			if(0 != Ain_ErrArr[i])Dem_SetDtc(DEM_CONTROLLER_INTERNALFAULT_ID, Ain_ErrArr[i], 1);
			else Dem_SetDtc(DEM_CONTROLLER_INTERNALFAULT_ID, Ain_ErrArr[i], 0);
		}
		else
		{
			/* Do nothing. */
		}
		if(i < 9)
		{
			if(0 != I2cH_ErrArr[i]) Dem_SetDtc(DEM_REVERSECAMERA_CONTROLINTERFACE_MALFUNCTION_ID, I2cH_ErrArr[i], 1);
			else Dem_SetDtc(DEM_REVERSECAMERA_CONTROLINTERFACE_MALFUNCTION_ID, I2cH_ErrArr[i], 0);
		}
		else
		{
			/* Do nothing. */
		}
	}
	Dem_MainCounter++;
}
