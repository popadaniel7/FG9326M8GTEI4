#ifndef DEM_H_
#define DEM_H_

#include "Std_Types.h"

typedef struct
{
	uint32 isActiveNow;
}Dem_DTC_t;

#define DEM_NUMBER_OF_DTCS 8
#define DEM_CONTROLLER_INTERNALFAULT_ID 0
#define DEM_COMMUNICATION_ERROR_ID 1
#define DEM_REVERSECAMERA_DCMIINTERFACE_MALFUNCTION_ID 2
#define DEM_DIGITALCLUSTER_CONTROLINTERFACE_MALFUNCTION_ID 3
#define DEM_VEHSTATE_MESSAGEMISSING_ID 4
#define DEM_BODYSTATE_MESSAGEMISSING_ID 5
#define DEM_DATARECORDER_MESSAGEMISSING_ID 6
#define DEM_BODYSTATE2_MESSAGEMISSING_ID 7

extern void Dem_SetDtc(uint8 IDPrimary, uint32 Reason, uint8 Status);
extern void Dem_MainFunction(void);

#endif /* DEM_H_ */
