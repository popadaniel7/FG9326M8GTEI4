#include "Ifx_Types.h"

#define ENCCAL_CODING_SIZE      8U
#define ENCCAL_CALIBRATION_SIZE 8U

typedef struct
{
        uint8 model[8];
        uint8 vin[17];
        uint8 chassisNumber[3];
        uint8 upholstery[24];
        uint8 swVersion[4][5];
        uint8 hwVersion[4][5];
        uint8 calibrationVersion[4][5];
        uint8 codingVersion[4][5];
        uint8 optionList[20][64];
        uint8 buildDate[8];
        uint8 modulesList[4][64];
        uint32 padding1;
}EncCal_VOData_t;

extern EncCal_VOData_t EncCal_VODataComplete;
extern uint32 EncCal_Calibration_Buffer[ENCCAL_CALIBRATION_SIZE];
extern uint32 EncCal_Calibration_DefaultBuffer[ENCCAL_CALIBRATION_SIZE];
extern uint32 EncCal_Calibration_ColDet_StableDistanceCm                              ;
extern uint32 EncCal_Calibration_ColDet_TtcWarn                                       ;
extern uint32 EncCal_Calibration_ColDet_TtcBrake                                      ;
extern uint32 EncCal_Calibration_ColDet_MinDist                                       ;
extern uint32 EncCal_Calibration_ColDet_MaxDist                                       ;
extern uint32 EncCal_Calibration_ColDet_InvalidDist                                   ;
extern uint8 EncCal_Coding_DefaultBuffer[ENCCAL_CODING_SIZE];
extern uint8 EncCal_Coding_Buffer[ENCCAL_CODING_SIZE];
extern uint8 EncCal_Coding_IgnCtrl;
extern uint8 EncCal_Coding_CenLoc;
extern uint8 EncCal_Coding_Hc05;
extern uint8 EncCal_Coding_Hc05Vent;
extern uint8 EncCal_Coding_Lsen;
extern uint8 EncCal_CodingValidResult;
extern uint8 EncCal_CalibrationValidResult;
extern uint8 EncCal_CheckResult_Coding;
extern uint8 EncCal_CheckResult_Calibration;
extern uint8 EncCal_CheckResult_VoData;
extern uint8 EncCal_IsVoDataValid;

extern void EncCal_InitVoData(void);
extern void EncCal_WriteVoData(uint32* data);
extern void EncCal_ReadVoData(uint32* data);
extern uint8 EncCal_CheckVoData(uint32* data);
extern void EncCal_InitCalibration(void);
extern void EncCal_InitCoding(void);
extern void EncCal_ReadCalibration(uint32* data);
extern void EncCal_ReadCoding(uint32* data);
extern void EncCal_WriteCalibration(uint32* data);
extern void EncCal_WriteCoding(uint32* data);
extern uint8 EncCal_CheckCalibration();
extern uint8 EncCal_CheckCoding(uint32* data);
extern void EncCal_MainFunction(void);
