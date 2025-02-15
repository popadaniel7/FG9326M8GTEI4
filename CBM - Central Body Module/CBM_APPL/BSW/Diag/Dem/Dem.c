/* START OF FILE */
/* INCLUDE START */
#include "Dem.h"
#include "CanSpi.h"
/* INCLUDE END */
/* VARIABLES START */
CANSPI_uCAN_MSG Dem_DtcMessage;
uint8 Dem_ControlDtcSettingStatus = 0;
Dem_DTC_t Dem_DTCStoreArray[13];
extern uint32 Dcm_AliveCounter __attribute__((section(".ncr")));
extern uint8 VehicleState_Voltage;
extern uint8 StatusList_ComOutValue[10];
/* VARIABLES STOP */
/* FUNCTIONS START */
void Dem_SaveDtc(uint8 index, uint8 status);
/* FUNCTIONS STOP */
/* FUNCTIONS START */
void Dem_SaveDtc(uint8 index, uint8 status)
{
	__disable_irq();
	/* Store DTC. */
	if(0 == Dem_ControlDtcSettingStatus)
	{
		if(3 > Dem_DTCStoreArray[index].counter)
		{
			if(1 == status && 0 == Dem_DTCStoreArray[index].isActiveNow)
			{
				Dem_DtcMessage.frame.idType = 1;
				Dem_DtcMessage.frame.id = 0x10C;
				Dem_DtcMessage.frame.dlc = 1;
				Dem_DtcMessage.frame.data0 = index;
				CanSpi_Transmit(&Dem_DtcMessage);

				Dem_DTCStoreArray[index].isActiveNow = status;
				Dem_DTCStoreArray[index].IgnFF[Dem_DTCStoreArray[index].counter] = StatusList_ComOutValue[0];
				Dem_DTCStoreArray[index].TimeFF[Dem_DTCStoreArray[index].counter] = Dcm_AliveCounter;
				Dem_DTCStoreArray[index].VoltageFF[Dem_DTCStoreArray[index].counter] = VehicleState_Voltage;
				Dem_DTCStoreArray[index].GearFF[Dem_DTCStoreArray[index].counter] = StatusList_ComOutValue[4];
				Dem_DTCStoreArray[index].counter++;
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

		if(0 == status)
		{
			Dem_DTCStoreArray[index].isActiveNow = 0;
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
	__enable_irq();
}
/* FUNCTIONS STOP */
/* STOP OF FILE */
