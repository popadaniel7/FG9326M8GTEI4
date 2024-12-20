#ifndef DEM_H_
#define DEM_H_

#include "Std_Types.h"

typedef struct
{
	uint32 counter;
	uint32 isActiveNow;
	uint8 VoltageFF[3];
	uint8 SpeedFF[3];
	uint8 RpmFF[3];
	uint8 TimeFF[3];
	uint8 IgnFF[3];
	uint8 GearFF[3];
	uint32 ReasonFF[3];
}Dem_DTC_t;

#define DEM_NUMBER_OF_DTCS 9
#define DEM_CONTROLLER_INTERNALFAULT_ID 0
#define DEM_COMMUNICATION_ERROR_ID 1
#define DEM_REVERSECAMERA_CONTROLINTERFACE_MALFUNCTION_ID 2
#define DEM_REVERSECAMERA_DCMIINTERFACE_MALFUNCTION_ID 3
#define DEM_DIGITALCLUSTER_CONTROLINTERFACE_MALFUNCTION_ID 4
#define DEM_VEHSTATE_MESSAGEMISSING_ID 5
#define DEM_BODYSTATE_MESSAGEMISSING_ID 6
#define DEM_DATARECORDER_MESSAGEMISSING_ID 7
#define DEM_BODYSTATE2_MESSAGEMISSING_ID 8

extern void Dem_SetDtc(uint8 IDPrimary, uint32 Reason, uint8 Status);
extern void Dem_MainFunction(void);

#endif /* DEM_H_ */
