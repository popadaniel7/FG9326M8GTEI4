#include "EncCal.h"
#include "Nvm.h"
#include "string.h"

EncCal_VOData_t EncCal_VODataComplete_Default =
{
        /* model */
        {
                0x38, 0x20, 0x73, 0x65, 0x72, 0x69, 0x65, 0x73
        },
        /* VIN*/
        {
                0x57, 0x42, 0x41, 0x45, 0x47, 0x32, 0x33, 0x31,
                0x31, 0x4D, 0x43, 0x32, 0x39, 0x31, 0x31, 0x34,
                0x33
        },
        /* chassisNumber*/
        {
                0x46, 0x39, 0x33
        },

        /* up-holstery */
        {
                0x62, 0x6C, 0x61, 0x63, 0x6B, 0x20, 0x2F, 0x20,
                0x4D, 0x38, 0x47, 0x54, 0x45,
                0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        },

        /* swVersion*/
        {
                { 0x31, 0x2E, 0x31, 0x2E, 0x31 },
                { 0x31, 0x2E, 0x31, 0x2E, 0x31 },
                { 0x31, 0x2E, 0x31, 0x2E, 0x31 },
                { 0x31, 0x2E, 0x31, 0x2E, 0x31 }
        },

        /* hwVersion*/
        {
                { 0x31, 0x2E, 0x31, 0x2E, 0x31 },
                { 0x31, 0x2E, 0x31, 0x2E, 0x31 },
                { 0x31, 0x2E, 0x31, 0x2E, 0x31 },
                { 0x31, 0x2E, 0x31, 0x2E, 0x31 }
        },

        /* calibrationVersion */
        {
                { 0x31, 0x2E, 0x31, 0x2E, 0x31 },
                { 0x31, 0x2E, 0x31, 0x2E, 0x31 },
                { 0x31, 0x2E, 0x31, 0x2E, 0x31 },
                { 0x31, 0x2E, 0x31, 0x2E, 0x31 }
        },

        /* codingVersion */
        {
                { 0x31, 0x2E, 0x31, 0x2E, 0x31 },
                { 0x31, 0x2E, 0x31, 0x2E, 0x31 },
                { 0x31, 0x2E, 0x31, 0x2E, 0x31 },
                { 0x31, 0x2E, 0x31, 0x2E, 0x31 }
        },

        /* optionList:
         */
        {

                {
                        0x53,0x74,0x65,0x65,0x72,0x2D,0x62,0x79,0x2D,0x77,0x69,0x72,0x65,0x42,0x72,0x61,
                        0x6B,0x65,0x2D,0x62,0x79,0x2D,0x77,0x69,0x72,0x65,0x42,0x6C,0x75,0x65,0x74,0x6F,
                        0x6F,0x74,0x68,0x20,0x63,0x6F,0x6E,0x74,0x72,0x6F,0x6C,0x52,0x65,0x6D,0x6F,0x74,
                        0x65,0x20,0x76,0x65,0x6E,0x74,0x69,0x6C,0x61,0x74,0x69,0x6F,0x6E,0x41,0x75,0x74
                },
                {
                        0x6F,0x6D,0x61,0x74,0x69,0x63,0x20,0x63,0x6C,0x69,0x6D,0x61,0x74,0x65,0x20,0x63,
                        0x6F,0x6E,0x74,0x72,0x6F,0x6C,0x57,0x69,0x6E,0x64,0x73,0x68,0x69,0x65,0x6C,0x64,
                        0x20,0x77,0x69,0x70,0x65,0x72,0x20,0x6E,0x6F,0x72,0x6D,0x61,0x6C,0x20,0x73,0x70,
                        0x65,0x65,0x64,0x44,0x69,0x67,0x69,0x74,0x61,0x6C,0x20,0x43,0x6C,0x75,0x73,0x74
                },
                {
                        0x65,0x72,0x52,0x65,0x76,0x65,0x72,0x73,0x65,0x20,0x63,0x61,0x6D,0x65,0x72,0x61,
                        0x43,0x6F,0x6C,0x6C,0x69,0x73,0x69,0x6F,0x6E,0x20,0x61,0x73,0x73,0x69,0x73,0x74,
                        0x43,0x65,0x6E,0x74,0x72,0x61,0x6C,0x20,0x6C,0x6F,0x63,0x6B,0x69,0x6E,0x67,0x41,
                        0x75,0x74,0x6F,0x6D,0x61,0x74,0x69,0x63,0x20,0x6C,0x69,0x67,0x68,0x74,0x73
                },
                {
                        0x41,0x75,0x74,0x6F,0x6D,0x61,0x74,0x69,0x63,0x20,0x77,0x69,0x70,0x65,0x72,0x73,
                        0x41,0x75,0x74,0x6F,0x6D,0x61,0x74,0x69,0x63,0x20,0x74,0x72,0x61,0x6E,0x73,0x6D,
                        0x69,0x73,0x73,0x69,0x6F,0x6E,0x20,0x2F,0x20,0x65,0x6C,0x65,0x63,0x74,0x72,0x69,
                        0x63,0x20,0x64,0x72,0x69,0x76,0x65,0x74,0x72,0x61,0x69,0x6E,0x52,0x65,0x6D,0x6F
                },
                {
                        0x74,0x65,0x20,0x73,0x74,0x61,0x72,0x74,0x35,0x56,0x20,0x65,0x6C,0x65,0x63,0x74,
                        0x72,0x69,0x63,0x20,0x64,0x72,0x69,0x76,0x65,0x74,0x72,0x61,0x69,0x6E,0x00,0x00,
                        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
                },
                /* Rows 5..19: all zeros (no more text) */
                { {0} }, { {0} }, { {0} }, { {0} }, { {0} },
                { {0} }, { {0} }, { {0} }, { {0} }, { {0} },
                { {0} }, { {0} }, { {0} }, { {0} }, { {0} }
        },
        {
                0x30, 0x31, 0x2E, 0x30, 0x35, 0x2E, 0x32, 0x35
        },
        /* modulesList*/
        {

                {
                        0x43,0x65,0x6E,0x74,0x72,0x61,0x6C,0x20,0x42,0x6F,0x64,0x79,0x20,0x4D,0x6F,0x64,
                        0x75,0x6C,0x65,0x20,0x31,0x20,0x2D,0x20,0x43,0x42,0x4D,0x31,0x44,0x69,0x67,0x69,
                        0x74,0x61,0x6C,0x20,0x4D,0x61,0x69,0x6E,0x20,0x55,0x6E,0x69,0x74,0x20,0x31,0x20,
                        0x2D,0x20,0x44,0x4D,0x55,0x31,0x50,0x6F,0x77,0x65,0x72,0x20,0x44,0x69,0x73,0x74
                },
                {
                        0x72,0x69,0x62,0x75,0x74,0x69,0x6F,0x6E,0x20,0x4D,0x6F,0x64,0x75,0x6C,0x65,0x20,
                        0x31,0x20,0x2D,0x20,0x50,0x44,0x4D,0x31,0x44,0x72,0x69,0x76,0x65,0x20,0x45,0x6C,
                        0x65,0x63,0x74,0x72,0x6F,0x6E,0x69,0x63,0x20,0x43,0x6F,0x6E,0x74,0x72,0x6F,0x6C,
                        0x6C,0x65,0x72,0x20,0x31,0x20,0x2D,0x20,0x44,0x45,0x43,0x31,0x00,0x00,0x00,0x00
                },
                { {0} },
                { {0} }
        }
};

EncCal_VOData_t EncCal_VODataComplete;
uint32 EncCal_Calibration_Buffer[ENCCAL_CALIBRATION_SIZE] = {0xFFu};
uint32 EncCal_Calibration_DefaultBuffer[ENCCAL_CALIBRATION_SIZE] =
{
        25u,10u,5u,100u,100u,10u,90u,500u,15u,15u,15u,15u,1u,1u,1u,1000u,100u,100u,100u,1u,1u,1u,10u,10u,1u,50u,3000u,2u,1u,1u,10u,80u,80u,2u,5000u,35u
};
uint32 EncCal_Calibration_EPSCtrlPID_Kp                                         = 0u;
uint32 EncCal_Calibration_EPSCtrlPID_Ki                                         = 0u;
uint32 EncCal_Calibration_EPSCtrlPID_Kd                                         = 0u;
uint32 EncCal_Calibration_EPSCtrlPID_IntegralWindupMin                          = 0u;
uint32 EncCal_Calibration_EPSCtrlPID_IntegralWindupMax                          = 0u;
uint32 EncCal_Calibration_EPSCtrl_SteeringToMotorRatio                          = 0u;
uint32 EncCal_Calibration_EPSCtrl_InitialSetpoint                               = 0u;
uint32 EncCal_Calibration_EPSCtrl_Bts7960PwmFrequencyMax                        = 0u;
uint32 EncCal_Calibration_EPSCtrl_DcMotorStartupDutyCycle                       = 0u;
uint32 EncCal_Calibration_EPSCtrl_PIDtoPwmDutyCycleMin                          = 0u;
uint32 EncCal_Calibration_EPSCtrl_PIDtoPwmDutyCycleMax                          = 0u;
uint32 EncCal_Calibration_EPSCtrl_OverloadCurrentDcMotor                        = 0u;
uint32 EncCal_Calibration_EPSCtrl_DriveModeSportModeFactorForAssistance         = 0u;
uint32 EncCal_Calibration_EPSCtrl_DriveModeComfortModeFactorForAssistance       = 0u;
uint32 EncCal_Calibration_EPSCtrl_DriveModeEcoModeFactorForAssistance           = 0u;
uint32 EncCal_Calibration_EMotorCtrl_PwmFrequenceyMax                           = 0u;
uint32 EncCal_Calibration_EMotorCtrl_DutyCycleToRpmFactor                       = 0u;
uint32 EncCal_Calibration_EMotorCtrl_PwmDutyCycleMin                            = 0u;
uint32 EncCal_Calibration_EMotorCtrl_PwmDutyCycleMax                            = 0u;
uint32 EncCal_Calibration_EMotorCtrl_DriveModeSportModeFactorForAcceleration    = 0u;
uint32 EncCal_Calibration_EMotorCtrl_DriveModeComfortModeFactorForAcceleration  = 0u;
uint32 EncCal_Calibration_EMotorCtrl_DriveModeEcoModeFactorForAcceleration      = 0u;
uint32 EncCal_Calibration_EMotorCtrl_FactorForAcceleration                      = 0u;
uint32 EncCal_Calibration_EMotorCtrl_FactorForDecceleration                     = 0u;
uint32 EncCal_Calibration_EMotorCtrl_DutyCycleStep                              = 0u;
uint32 EncCal_Calibration_EMotorCtrl_WheelCircumference                         = 0u;
uint32 EncCal_Calibration_EMotorCtrl_OutputSensorActiveValue                    = 0u;
uint32 EncCal_Calibration_ColDet_StableDistanceCm                               = 0u;
uint32 EncCal_Calibration_ColDet_TtcWarn                                        = 0u;
uint32 EncCal_Calibration_ColDet_TtcBrake                                       = 0u;
uint32 EncCal_Calibration_ColDet_MinDist                                        = 0u;
uint32 EncCal_Calibration_ColDet_MaxDist                                        = 0u;
uint32 EncCal_Calibration_ColDet_InvalidDist                                    = 0u;
uint32 EncCal_Calibration_EnergyMgmt_CurrentConsumptionTimer                    = 0u;
uint32 EncCal_Calibration_EnergyMgmt_MaxOutputCurrent                           = 0u;
uint32 EncCal_Calibration_EnergyMgmt_UnderVoltageTh                             = 0u;
uint8 EncCal_Coding_DefaultBuffer[ENCCAL_CODING_SIZE] =
{
        1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u
};
uint8 EncCal_Coding_Buffer[ENCCAL_CODING_SIZE] = {0xFFu};
uint8 EncCal_Coding_PsCtrl = 0u;
uint8 EncCal_Coding_IgnCtrl = 0u;
uint8 EncCal_Coding_AccCtrl = 0u;
uint8 EncCal_Coding_BrCtrl = 0u;
uint8 EncCal_Coding_CenLoc = 0u;
uint8 EncCal_Coding_Hc05 = 0u;
uint8 EncCal_Coding_Hc05Vent = 0u;
uint8 EncCal_Coding_Lsen = 0u;
uint8 EncCal_Coding_CbmL = 0u;
uint8 EncCal_Coding_DmuL = 0u;
uint8 EncCal_Coding_L4 = 0u;
uint8 EncCal_Coding_L5 = 0u;
uint8 EncCal_Coding_L6 = 0u;
uint8 EncCal_Coding_L7 = 0u;
uint8 EncCal_Coding_L8 = 0u;
uint8 EncCal_Coding_EpsCtrl = 0u;
uint8 EncCal_Coding_EMotorCtrl = 0u;
uint8 EncCal_Coding_EpsMotorFan = 0u;
uint8 EncCal_Coding_SpeedSen = 0u;
uint8 EncCal_Coding_TSen = 0u;
uint8 EncCal_Coding_ConsumerCutoffTime = 0u;
uint8 EncCal_CodingValidResult = 0u;
uint8 EncCal_CalibrationValidResult = 0u;
uint8 EncCal_CheckResult_Coding = 0u;
uint8 EncCal_CheckResult_Calibration = 0u;
uint8 EncCal_CheckResult_VoData = 0u;
uint8 EncCal_IsVoDataValid = 0u;

void EncCal_InitVoData(void);
void EncCal_WriteVoData(uint32* data);
void EncCal_ReadVoData(uint32* data);
uint8 EncCal_CheckVoData(uint32* data);
void EncCal_InitCalibration(void);
void EncCal_InitCoding(void);
uint8 EncCal_CodingValidity(void);
uint8 EncCal_CalibrationValidity(void);
void EncCal_ReadCalibration(uint32* data);
void EncCal_ReadCoding(uint32* data);
void EncCal_WriteCalibration(uint32* data);
void EncCal_WriteCoding(uint32* data);
uint8 EncCal_CheckCalibration(uint32* data);
uint8 EncCal_CheckCoding(uint32* data);
void EncCal_MainFunction(void);

void EncCal_InitVoData(void)
{
    uint8 zeroBuffer[sizeof(EncCal_VODataComplete)] = {0};
    EncCal_IsVoDataValid = (memcmp(&EncCal_VODataComplete, zeroBuffer, sizeof(EncCal_VODataComplete)) == 0);
}

void EncCal_WriteVoData(uint32* data)
{
    Nvm_WriteBlock(3u, data, sizeof(EncCal_VOData_t));
}

void EncCal_ReadVoData(uint32* data)
{
    Nvm_ReadBlock(3u, data, sizeof(EncCal_VOData_t));
}

uint8 EncCal_CheckVoData(uint32* data)
{
    static uint32 crcData = 0u;
    static uint32 crcNvm = 0u;
    static EncCal_VOData_t localData;
    static uint8 localStat = 0u;
    static uint8 retVal = 0u;

    crcData = Nvm_CalculateCrc(data, sizeof(data));
    EncCal_ReadVoData((uint32*)&localData);
    localStat = Nvm_GetBlockStatus(3u);
    crcNvm = Nvm_GetBlockCrc(3u);

    if(crcData == crcNvm && localStat != 0xFE)
    {
        retVal = 0u;
    }
    else
    {
        retVal = 1u;
    }

    return retVal;
}

uint8 EncCal_CalibrationValidity(void)
{
    static uint8 retVal = 0u;

    if(5u > EncCal_Calibration_EPSCtrlPID_Kp || 50u < EncCal_Calibration_EPSCtrlPID_Kp)
    {
        EncCal_Calibration_EPSCtrlPID_Kp = 25u;
        retVal = 1u;
    }
    else
    {
        /* Do nothing. */
    }

    if(1u > EncCal_Calibration_EPSCtrlPID_Ki || 20u < EncCal_Calibration_EPSCtrlPID_Ki)
    {
        EncCal_Calibration_EPSCtrlPID_Ki = 10u;
        retVal = 1u;
    }
    else
    {
        /* Do nothing. */
    }

    if(1u > EncCal_Calibration_EPSCtrlPID_Ki || 10u < EncCal_Calibration_EPSCtrlPID_Ki)
    {
        EncCal_Calibration_EPSCtrlPID_Kd = 5u;
        retVal = 1u;
    }
    else
    {
        /* Do nothing. */
    }

    if(50u > EncCal_Calibration_EPSCtrlPID_IntegralWindupMin || 200u < EncCal_Calibration_EPSCtrlPID_IntegralWindupMin)
    {
        EncCal_Calibration_EPSCtrlPID_IntegralWindupMin = 100u;
        retVal = 1u;
    }
    else
    {
        /* Do nothing. */
    }

    if(50u > EncCal_Calibration_EPSCtrlPID_IntegralWindupMax || 200u < EncCal_Calibration_EPSCtrlPID_IntegralWindupMax)
    {
        EncCal_Calibration_EPSCtrlPID_IntegralWindupMax = 100u;
        retVal = 1u;
    }
    else
    {
        /* Do nothing. */
    }

    if(8u > EncCal_Calibration_EPSCtrl_SteeringToMotorRatio || 12u < EncCal_Calibration_EPSCtrl_SteeringToMotorRatio)
    {
        EncCal_Calibration_EPSCtrl_SteeringToMotorRatio = 10u;
        retVal = 1u;
    }
    else
    {
        /* Do nothing. */
    }

    if(0u == EncCal_Calibration_EPSCtrl_SteeringToMotorRatio || 180u < EncCal_Calibration_EPSCtrl_SteeringToMotorRatio)
    {
        EncCal_Calibration_EPSCtrl_SteeringToMotorRatio = 90u;
        retVal = 1u;
    }
    else
    {
        /* Do nothing. */
    }

    if(500u != EncCal_Calibration_EPSCtrl_Bts7960PwmFrequencyMax)
    {
        EncCal_Calibration_EPSCtrl_Bts7960PwmFrequencyMax = 500u;
        retVal = 1u;
    }
    else
    {
        /* Do nothing. */
    }

    if(15u > EncCal_Calibration_EPSCtrl_DcMotorStartupDutyCycle || 100u < EncCal_Calibration_EPSCtrl_DcMotorStartupDutyCycle)
    {
        EncCal_Calibration_EPSCtrl_DcMotorStartupDutyCycle = 15u;
        retVal = 1u;
    }
    else
    {
        /* Do nothing. */
    }

    if(1u > EncCal_Calibration_EPSCtrl_PIDtoPwmDutyCycleMin || 3u < EncCal_Calibration_EPSCtrl_PIDtoPwmDutyCycleMin)
    {
        EncCal_Calibration_EPSCtrl_PIDtoPwmDutyCycleMin = 15u;
        retVal = 1u;
    }
    else
    {
        /* Do nothing. */
    }

    if(1u > EncCal_Calibration_EPSCtrl_PIDtoPwmDutyCycleMax || 3u < EncCal_Calibration_EPSCtrl_PIDtoPwmDutyCycleMax)
    {
        EncCal_Calibration_EPSCtrl_PIDtoPwmDutyCycleMax = 15u;
        retVal = 1u;
    }
    else
    {
        /* Do nothing. */
    }

    if(6u > EncCal_Calibration_EPSCtrl_OverloadCurrentDcMotor || 8u < EncCal_Calibration_EPSCtrl_OverloadCurrentDcMotor)
    {
        EncCal_Calibration_EPSCtrl_OverloadCurrentDcMotor = 15u;
        retVal = 1u;
    }
    else
    {
        /* Do nothing. */
    }

    if(1u > EncCal_Calibration_EPSCtrl_DriveModeSportModeFactorForAssistance || 10u < EncCal_Calibration_EPSCtrl_DriveModeSportModeFactorForAssistance)
    {
        EncCal_Calibration_EPSCtrl_DriveModeSportModeFactorForAssistance = 1u;
        retVal = 1u;
    }
    else
    {
        /* Do nothing. */
    }

    if(1u > EncCal_Calibration_EPSCtrl_DriveModeComfortModeFactorForAssistance || 10u < EncCal_Calibration_EPSCtrl_DriveModeComfortModeFactorForAssistance)
    {
        EncCal_Calibration_EPSCtrl_DriveModeComfortModeFactorForAssistance = 1u;
        retVal = 1u;
    }
    else
    {
        /* Do nothing. */
    }

    if(1u > EncCal_Calibration_EPSCtrl_DriveModeEcoModeFactorForAssistance || 10u < EncCal_Calibration_EPSCtrl_DriveModeEcoModeFactorForAssistance)
    {
        EncCal_Calibration_EPSCtrl_DriveModeEcoModeFactorForAssistance = 1u;
        retVal = 1u;
    }
    else
    {
        /* Do nothing. */
    }

    if(1u > EncCal_Calibration_EMotorCtrl_PwmFrequenceyMax || 1000u < EncCal_Calibration_EMotorCtrl_PwmFrequenceyMax)
    {
        EncCal_Calibration_EMotorCtrl_PwmFrequenceyMax = 1000u;
        retVal = 1u;
    }
    else
    {
        /* Do nothing. */
    }

    if(100u > EncCal_Calibration_EMotorCtrl_DutyCycleToRpmFactor || 1000u < EncCal_Calibration_EMotorCtrl_DutyCycleToRpmFactor)
    {
        EncCal_Calibration_EMotorCtrl_DutyCycleToRpmFactor = 100u;
        retVal = 1u;
    }
    else
    {
        /* Do nothing. */
    }

    if(1u > EncCal_Calibration_EMotorCtrl_PwmDutyCycleMin || 100u < EncCal_Calibration_EMotorCtrl_PwmDutyCycleMin)
    {
        EncCal_Calibration_EMotorCtrl_PwmDutyCycleMin = 100u;
        retVal = 1u;
    }
    else
    {
        /* Do nothing. */
    }

    if(1u > EncCal_Calibration_EMotorCtrl_PwmDutyCycleMax || 100u < EncCal_Calibration_EMotorCtrl_PwmDutyCycleMax)
    {
        EncCal_Calibration_EMotorCtrl_PwmDutyCycleMax = 100u;
        retVal = 1u;
    }
    else
    {
        /* Do nothing. */
    }

    if(1u > EncCal_Calibration_EMotorCtrl_DriveModeSportModeFactorForAcceleration || 10u < EncCal_Calibration_EMotorCtrl_DriveModeSportModeFactorForAcceleration)
    {
        EncCal_Calibration_EMotorCtrl_DriveModeSportModeFactorForAcceleration = 1u;
        retVal = 1u;
    }
    else
    {
        /* Do nothing. */
    }

    if(1u > EncCal_Calibration_EMotorCtrl_DriveModeComfortModeFactorForAcceleration || 10u < EncCal_Calibration_EMotorCtrl_DriveModeComfortModeFactorForAcceleration)
    {
        EncCal_Calibration_EMotorCtrl_DriveModeComfortModeFactorForAcceleration = 1u;
        retVal = 1u;
    }
    else
    {
        /* Do nothing. */
    }

    if(1u > EncCal_Calibration_EMotorCtrl_DriveModeEcoModeFactorForAcceleration || 10u < EncCal_Calibration_EMotorCtrl_DriveModeEcoModeFactorForAcceleration)
    {
        EncCal_Calibration_EMotorCtrl_DriveModeEcoModeFactorForAcceleration = 1u;
        retVal = 1u;
    }
    else
    {
        /* Do nothing. */
    }

    if(1u > EncCal_Calibration_EMotorCtrl_FactorForAcceleration || 10u < EncCal_Calibration_EMotorCtrl_FactorForAcceleration)
    {
        EncCal_Calibration_EMotorCtrl_FactorForAcceleration = 10u;
        retVal = 1u;
    }
    else
    {
        /* Do nothing. */
    }

    if(1u > EncCal_Calibration_EMotorCtrl_FactorForDecceleration || 10u < EncCal_Calibration_EMotorCtrl_FactorForDecceleration)
    {
        EncCal_Calibration_EMotorCtrl_FactorForDecceleration = 10u;
        retVal = 1u;
    }
    else
    {
        /* Do nothing. */
    }

    if(1u > EncCal_Calibration_EMotorCtrl_DutyCycleStep || 100u < EncCal_Calibration_EMotorCtrl_DutyCycleStep)
    {
        EncCal_Calibration_EMotorCtrl_DutyCycleStep = 1u;
        retVal = 1u;
    }
    else
    {
        /* Do nothing. */
    }

    if(1u > EncCal_Calibration_EMotorCtrl_WheelCircumference || 100u < EncCal_Calibration_EMotorCtrl_WheelCircumference)
    {
        EncCal_Calibration_EMotorCtrl_WheelCircumference = 50u;
        retVal = 1u;
    }
    else
    {
        /* Do nothing. */
    }

    if(2000u > EncCal_Calibration_EMotorCtrl_OutputSensorActiveValue || 4095 < EncCal_Calibration_EMotorCtrl_OutputSensorActiveValue)
    {
        EncCal_Calibration_EMotorCtrl_OutputSensorActiveValue = 3000u;
        retVal = 1u;
    }
    else
    {
        /* Do nothing. */
    }

    if(2u > EncCal_Calibration_ColDet_StableDistanceCm || 3u < EncCal_Calibration_ColDet_StableDistanceCm)
    {
        EncCal_Calibration_ColDet_StableDistanceCm = 2u;
        retVal = 1u;
    }
    else
    {
        /* Do nothing. */
    }

    if(1u > EncCal_Calibration_ColDet_TtcWarn || 3u < EncCal_Calibration_ColDet_TtcWarn)
    {
        EncCal_Calibration_ColDet_TtcWarn = 1u;
        retVal = 1u;
    }
    else
    {
        /* Do nothing. */
    }

    if(1u > EncCal_Calibration_ColDet_TtcBrake || 2u < EncCal_Calibration_ColDet_TtcBrake)
    {
        EncCal_Calibration_ColDet_TtcBrake = 1u;
        retVal = 1u;
    }
    else
    {
        /* Do nothing. */
    }

    if(1u > EncCal_Calibration_ColDet_MinDist || 15u < EncCal_Calibration_ColDet_MinDist)
    {
        EncCal_Calibration_ColDet_MinDist = 10u;
        retVal = 1u;
    }
    else
    {
        /* Do nothing. */
    }

    if(75u > EncCal_Calibration_ColDet_MaxDist || 90u < EncCal_Calibration_ColDet_MaxDist)
    {
        EncCal_Calibration_ColDet_MaxDist = 80u;
        retVal = 1u;
    }
    else
    {
        /* Do nothing. */
    }

    if(80u > EncCal_Calibration_ColDet_InvalidDist)
    {
        EncCal_Calibration_ColDet_InvalidDist = 80u;
        retVal = 1u;
    }
    else
    {
        /* Do nothing. */
    }

    if(0u == EncCal_Calibration_EnergyMgmt_CurrentConsumptionTimer || 10u < EncCal_Calibration_EnergyMgmt_CurrentConsumptionTimer)
    {
        EncCal_Calibration_EnergyMgmt_CurrentConsumptionTimer = 2u;
        retVal = 1u;
    }
    else
    {
        /* Do nothing. */
    }

    if(2500u > EncCal_Calibration_EnergyMgmt_MaxOutputCurrent || 5000u < EncCal_Calibration_EnergyMgmt_MaxOutputCurrent)
    {
        EncCal_Calibration_EnergyMgmt_MaxOutputCurrent = 5000u;
        retVal = 1u;
    }
    else
    {
        /* Do nothing. */
    }

    if(30u > EncCal_Calibration_EnergyMgmt_UnderVoltageTh || 40u < EncCal_Calibration_EnergyMgmt_UnderVoltageTh)
    {
        EncCal_Calibration_EnergyMgmt_UnderVoltageTh = 35u;
        retVal = 1u;
    }
    else
    {
        /* Do nothing. */
    }

    return retVal;
}

uint8 EncCal_CodingValidity(void)
{
    static uint8 retVal = 0u;

    if(2u > EncCal_Coding_ConsumerCutoffTime && 16u < EncCal_Coding_ConsumerCutoffTime)
    {
        EncCal_Coding_ConsumerCutoffTime = 1u;
        retVal = 1u;
    }
    else
    {
        /* Do nothing. */
    }

    if(2u < EncCal_Coding_PsCtrl)
    {
        EncCal_Coding_PsCtrl = 1u;
        retVal = 1u;
    }
    else
    {
        /* Do nothing. */
    }

    if(2u < EncCal_Coding_IgnCtrl)
    {
        EncCal_Coding_IgnCtrl = 1u;
        retVal = 1u;
    }
    else
    {
        /* Do nothing. */
    }

    if(2u < EncCal_Coding_AccCtrl)
    {
        EncCal_Coding_AccCtrl = 1u;
        retVal = 1u;
    }
    else
    {
        /* Do nothing. */
    }

    if(2u < EncCal_Coding_BrCtrl)
    {
        EncCal_Coding_BrCtrl = 1u;
        retVal = 1u;
    }
    else
    {
        /* Do nothing. */
    }

    if(2u < EncCal_Coding_CenLoc)
    {
        EncCal_Coding_CenLoc = 1u;
        retVal = 1u;
    }
    else
    {
        /* Do nothing. */
    }

    if(2u < EncCal_Coding_Hc05)
    {
        EncCal_Coding_Hc05 = 1u;
        retVal = 1u;
    }
    else
    {
        /* Do nothing. */
    }

    if(2u < EncCal_Coding_Hc05Vent)
    {
        EncCal_Coding_Hc05Vent = 1u;
        retVal = 1u;
    }
    else
    {
        /* Do nothing. */
    }

    if(2u < EncCal_Coding_Lsen)
    {
        EncCal_Coding_Lsen = 1u;
        retVal = 1u;
    }
    else
    {
        /* Do nothing. */
    }

    if(2u < EncCal_Coding_CbmL)
    {
        EncCal_Coding_CbmL = 1u;
        retVal = 1u;
    }
    else
    {
        /* Do nothing. */
    }

    if(2u < EncCal_Coding_DmuL)
    {
        EncCal_Coding_DmuL = 1u;
        retVal = 1u;
    }
    else
    {
        /* Do nothing. */
    }

    if(2u < EncCal_Coding_L4)
    {
        EncCal_Coding_L4 = 1u;
        retVal = 1u;
    }
    else
    {
        /* Do nothing. */
    }

    if(2u < EncCal_Coding_L5)
    {
        EncCal_Coding_L5 = 1u;
        retVal = 1u;
    }
    else
    {
        /* Do nothing. */
    }

    if(2u < EncCal_Coding_L6)
    {
        EncCal_Coding_L6 = 1u;
        retVal = 1u;
    }
    else
    {
        /* Do nothing. */
    }

    if(2u < EncCal_Coding_L7)
    {
        EncCal_Coding_L7 = 1u;
        retVal = 1u;
    }
    else
    {
        /* Do nothing. */
    }

    if(2u < EncCal_Coding_L8)
    {
        EncCal_Coding_L8 = 1u;
        retVal = 1u;
    }
    else
    {
        /* Do nothing. */
    }

    if(2u < EncCal_Coding_EpsCtrl)
    {
        EncCal_Coding_EpsCtrl = 1u;
        retVal = 1u;
    }
    else
    {
        /* Do nothing. */
    }

    if(2u < EncCal_Coding_EMotorCtrl)
    {
        EncCal_Coding_EMotorCtrl = 1u;
        retVal = 1u;
    }
    else
    {
        /* Do nothing. */
    }

    if(2u < EncCal_Coding_EpsMotorFan)
    {
        EncCal_Coding_EpsMotorFan = 1u;
        retVal = 1u;
    }
    else
    {
        /* Do nothing. */
    }

    if(2u < EncCal_Coding_SpeedSen)
    {
        EncCal_Coding_SpeedSen = 1u;
        retVal = 1u;
    }
    else
    {
        /* Do nothing. */
    }

    if(2u < EncCal_Coding_TSen)
    {
        EncCal_Coding_TSen = 1u;
        retVal = 1u;
    }
    else
    {
        /* Do nothing. */
    }

    return 0u;
}

void EncCal_InitCalibration(void)
{
    static uint8 retValStat = 0u;
    static uint8 i = 0u;
    static uint8 checkBufferNotEmptyFlag = 0u;
    retValStat = Nvm_GetBlockStatus(1u);

    if(0xFEU != retValStat)
    {
        for(i = 0u; i < ENCCAL_CALIBRATION_SIZE; i++)
        {
            if(0xFFu != EncCal_Calibration_Buffer[i])
            {
                checkBufferNotEmptyFlag = 1u;
            }
            else
            {
                checkBufferNotEmptyFlag = 0u;
            }
        }

        if(1u == checkBufferNotEmptyFlag)
        {
            /* Do nothing. */
        }
        else
        {
            memcpy(EncCal_Calibration_Buffer, EncCal_Calibration_DefaultBuffer, sizeof(EncCal_Calibration_Buffer));
        }
    }
    else
    {
        /* Do nothing. */
    }

    EncCal_Calibration_EPSCtrlPID_Kp                                         = EncCal_Calibration_Buffer[0u];
    EncCal_Calibration_EPSCtrlPID_Ki                                         = EncCal_Calibration_Buffer[1u];
    EncCal_Calibration_EPSCtrlPID_Kd                                         = EncCal_Calibration_Buffer[2u];
    EncCal_Calibration_EPSCtrlPID_IntegralWindupMin                          = EncCal_Calibration_Buffer[3u];
    EncCal_Calibration_EPSCtrlPID_IntegralWindupMax                          = EncCal_Calibration_Buffer[4u];
    EncCal_Calibration_EPSCtrl_SteeringToMotorRatio                          = EncCal_Calibration_Buffer[5u];
    EncCal_Calibration_EPSCtrl_InitialSetpoint                               = EncCal_Calibration_Buffer[6u];
    EncCal_Calibration_EPSCtrl_Bts7960PwmFrequencyMax                        = EncCal_Calibration_Buffer[7u];
    EncCal_Calibration_EPSCtrl_DcMotorStartupDutyCycle                       = EncCal_Calibration_Buffer[8u];
    EncCal_Calibration_EPSCtrl_PIDtoPwmDutyCycleMin                          = EncCal_Calibration_Buffer[9u];
    EncCal_Calibration_EPSCtrl_PIDtoPwmDutyCycleMax                          = EncCal_Calibration_Buffer[10u];
    EncCal_Calibration_EPSCtrl_OverloadCurrentDcMotor                        = EncCal_Calibration_Buffer[11u];
    EncCal_Calibration_EPSCtrl_DriveModeSportModeFactorForAssistance         = EncCal_Calibration_Buffer[12u];
    EncCal_Calibration_EPSCtrl_DriveModeComfortModeFactorForAssistance       = EncCal_Calibration_Buffer[13u];
    EncCal_Calibration_EPSCtrl_DriveModeEcoModeFactorForAssistance           = EncCal_Calibration_Buffer[14u];
    EncCal_Calibration_EMotorCtrl_PwmFrequenceyMax                           = EncCal_Calibration_Buffer[15u];
    EncCal_Calibration_EMotorCtrl_DutyCycleToRpmFactor                       = EncCal_Calibration_Buffer[16u];
    EncCal_Calibration_EMotorCtrl_PwmDutyCycleMin                            = EncCal_Calibration_Buffer[17u];
    EncCal_Calibration_EMotorCtrl_PwmDutyCycleMax                            = EncCal_Calibration_Buffer[18u];
    EncCal_Calibration_EMotorCtrl_DriveModeSportModeFactorForAcceleration    = EncCal_Calibration_Buffer[19u];
    EncCal_Calibration_EMotorCtrl_DriveModeComfortModeFactorForAcceleration  = EncCal_Calibration_Buffer[20u];
    EncCal_Calibration_EMotorCtrl_DriveModeEcoModeFactorForAcceleration      = EncCal_Calibration_Buffer[21u];
    EncCal_Calibration_EMotorCtrl_FactorForAcceleration                      = EncCal_Calibration_Buffer[22u];
    EncCal_Calibration_EMotorCtrl_FactorForDecceleration                     = EncCal_Calibration_Buffer[23u];
    EncCal_Calibration_EMotorCtrl_DutyCycleStep                              = EncCal_Calibration_Buffer[24u];
    EncCal_Calibration_EMotorCtrl_WheelCircumference                         = EncCal_Calibration_Buffer[25u];
    EncCal_Calibration_EMotorCtrl_OutputSensorActiveValue                    = EncCal_Calibration_Buffer[26u];
    EncCal_Calibration_ColDet_StableDistanceCm                               = EncCal_Calibration_Buffer[27u];
    EncCal_Calibration_ColDet_TtcWarn                                        = EncCal_Calibration_Buffer[28u];
    EncCal_Calibration_ColDet_TtcBrake                                       = EncCal_Calibration_Buffer[29u];
    EncCal_Calibration_ColDet_MinDist                                        = EncCal_Calibration_Buffer[30u];
    EncCal_Calibration_ColDet_MaxDist                                        = EncCal_Calibration_Buffer[31u];
    EncCal_Calibration_ColDet_InvalidDist                                    = EncCal_Calibration_Buffer[32u];
    EncCal_Calibration_EnergyMgmt_CurrentConsumptionTimer                    = EncCal_Calibration_Buffer[33u];
    EncCal_Calibration_EnergyMgmt_MaxOutputCurrent                           = EncCal_Calibration_Buffer[34u];
    EncCal_Calibration_EnergyMgmt_UnderVoltageTh                             = EncCal_Calibration_Buffer[35u];

    EncCal_CalibrationValidResult = EncCal_CalibrationValidity();
}

void EncCal_InitCoding(void)
{
    static uint8 retValStat = 0u;
    static uint8 i = 0u;
    static uint8 checkBufferNotEmptyFlag = 0u;
    retValStat = Nvm_GetBlockStatus(2u);

    if(0xFEU != retValStat)
    {
        for(i = 0u; i < ENCCAL_CODING_SIZE; i++)
        {
            if(0xFFu != EncCal_Coding_Buffer[i])
            {
                checkBufferNotEmptyFlag = 1u;
            }
            else
            {
                checkBufferNotEmptyFlag = 0u;
            }
        }

        if(1u == checkBufferNotEmptyFlag)
        {
            /* Do nothing. */
        }
        else
        {
            memcpy(EncCal_Coding_Buffer, EncCal_Coding_DefaultBuffer, sizeof(EncCal_Coding_Buffer));
        }
    }
    else
    {
        /* Do nothing. */
    }

    EncCal_Coding_PsCtrl            = (uint8)EncCal_Coding_Buffer[0u];
    EncCal_Coding_IgnCtrl           = (uint8)EncCal_Coding_Buffer[1u];
    EncCal_Coding_AccCtrl           = (uint8)EncCal_Coding_Buffer[2u];
    EncCal_Coding_BrCtrl            = (uint8)EncCal_Coding_Buffer[3u];
    EncCal_Coding_CenLoc            = (uint8)EncCal_Coding_Buffer[4u];
    EncCal_Coding_Hc05              = (uint8)EncCal_Coding_Buffer[5u];
    EncCal_Coding_Hc05Vent          = (uint8)EncCal_Coding_Buffer[6u];
    EncCal_Coding_Lsen              = (uint8)EncCal_Coding_Buffer[7u];
    EncCal_Coding_CbmL              = (uint8)EncCal_Coding_Buffer[8u];
    EncCal_Coding_DmuL              = (uint8)EncCal_Coding_Buffer[9u];
    EncCal_Coding_L4                = (uint8)EncCal_Coding_Buffer[10u];
    EncCal_Coding_L5                = (uint8)EncCal_Coding_Buffer[11u];
    EncCal_Coding_L6                = (uint8)EncCal_Coding_Buffer[12u];
    EncCal_Coding_L7                = (uint8)EncCal_Coding_Buffer[13u];
    EncCal_Coding_L8                = (uint8)EncCal_Coding_Buffer[14u];
    EncCal_Coding_EpsCtrl           = (uint8)EncCal_Coding_Buffer[15u];
    EncCal_Coding_EMotorCtrl        = (uint8)EncCal_Coding_Buffer[16u];
    EncCal_Coding_EpsMotorFan       = (uint8)EncCal_Coding_Buffer[17u];
    EncCal_Coding_SpeedSen          = (uint8)EncCal_Coding_Buffer[18u];
    EncCal_Coding_TSen              = (uint8)EncCal_Coding_Buffer[19u];
    EncCal_Coding_ConsumerCutoffTime = (uint8)EncCal_Coding_Buffer[20u];

    EncCal_CodingValidResult = EncCal_CodingValidity();
}

void EncCal_ReadCalibration(uint32* data)
{
    Nvm_ReadBlock(1u, data, ENCCAL_CALIBRATION_SIZE);
}

void EncCal_ReadCoding(uint32* data)
{
    Nvm_ReadBlock(2u, data, ENCCAL_CODING_SIZE);
}

void EncCal_WriteCalibration(uint32* data)
{
    Nvm_WriteBlock(1u, data, ENCCAL_CALIBRATION_SIZE);
}

void EncCal_WriteCoding(uint32* data)
{
    Nvm_WriteBlock(2u, data, ENCCAL_CODING_SIZE);
}

uint8 EncCal_CheckCalibration(uint32* data)
{
    static uint32 crcData = 0u;
    static uint32 crcNvm = 0u;
    static uint32 localData[ENCCAL_CALIBRATION_SIZE] = {0u};
    static uint8 localStat = 0u;
    static uint8 retVal = 0u;

    crcData = Nvm_CalculateCrc(data, sizeof(data));
    EncCal_ReadCalibration(localData);
    localStat = Nvm_GetBlockStatus(1u);
    crcNvm = Nvm_GetBlockCrc(1u);

    if(crcData == crcNvm && localStat != 0xFE)
    {
        retVal = 0u;
    }
    else
    {
        retVal = 1u;
    }

    return retVal;
}

uint8 EncCal_CheckCoding(uint32* data)
{
    static uint32 crcData = 0u;
    static uint32 crcNvm = 0u;
    static uint32 localData[ENCCAL_CODING_SIZE] = {0u};
    static uint8 localStat = 0u;
    static uint8 retVal = 0u;

    crcData = Nvm_CalculateCrc(data, sizeof(data));
    Nvm_ReadBlock(2u, localData, ENCCAL_CODING_SIZE);
    localStat = Nvm_GetBlockStatus(2u);
    crcNvm = Nvm_GetBlockCrc(2u);

    if(crcData == crcNvm && localStat != 0xFE)
    {
        retVal = 0u;
    }
    else
    {
        retVal = 1u;
    }

    return retVal;
}

void EncCal_MainFunction(void)
{
    EncCal_InitCalibration();
    EncCal_InitCoding();
    EncCal_InitVoData();

    EncCal_CheckResult_Calibration = EncCal_CheckCalibration(EncCal_Calibration_Buffer);
    EncCal_CheckResult_Coding = EncCal_CheckCoding((uint32*)EncCal_Coding_Buffer);
    EncCal_CheckResult_VoData = EncCal_CheckVoData((uint32*)&EncCal_VODataComplete);

    if(1u == EncCal_CheckResult_Calibration)
    {
        EncCal_WriteCalibration(EncCal_Calibration_DefaultBuffer);
    }
    else
    {
        /* Do nothing. */
    }

    if(1u == EncCal_CheckResult_Coding)
    {
        EncCal_WriteCoding((uint32*)EncCal_Coding_DefaultBuffer);
    }
    else
    {
        /* Do nothing. */
    }

    if(1u == EncCal_CheckResult_Calibration)
    {
        EncCal_WriteVoData((uint32 *)&EncCal_VODataComplete_Default);
    }
    else
    {
        /* Do nothing. */
    }
}
