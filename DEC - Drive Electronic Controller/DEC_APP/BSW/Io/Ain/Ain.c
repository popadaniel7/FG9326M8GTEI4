#include "Ain.h"
#include "Dma.h"
#include <math.h>


static uint32 Ain_MainCounter = AIN_ZERO;
uint32 Ain_AuxBufferDma[CHANNELS_NUM] = {AIN_ZERO};
float IRSensorValue = AIN_ZERO;
float CurrentSenseEpsValue_Right = AIN_ZERO;
float CurrentSenseEpsValue_Left = AIN_ZERO;
float VoltageSenseEpsFanValue = AIN_ZERO;
float VoltageSenseEpsLine = AIN_ZERO;
uint8 Ain_CanRx_MeasuredVoltage = AIN_ZERO;

void Ain_MainFunction(void);
float Ain_AdcToVoltage(float rawAdc, float refVoltage);
float Ain_ConvertCurrentSense(float rawAdc);
float Ain_ConvertVoltageSense(float rawAdc, float voltageReference);
float Ain_ConvertVoltageToDistanceGP2Y0A21YK0F(float voltage);
void Ain_ProcessIRSensorValue(void);

float Ain_ConvertVoltageSense(float rawAdc, float voltageReference)
{
    return ((rawAdc * AIN_EVADC_VFB) / ADC_MAX_VALUE) * voltageReference;
}

float Ain_ConvertCurrentSense(float rawAdc)
{
    return ((float)rawAdc / ADC_MAX_VALUE) * AIN_BTS7960_MAXCURRENT;
}

void Ain_ProcessIRSensorValue(void)
{
    static float actualVoltageReference = AIN_ZERO;
    static float localVoltageCalculated = AIN_ZERO;
    static uint32 localCounter = AIN_ZERO;
    static float localAverage = AIN_ZERO;
    static uint16 localRefValue = AIN_ZERO;

    localRefValue  = ADC_VREF_VALUE;
    localCounter++;
    actualVoltageReference = localRefValue | Ain_CanRx_MeasuredVoltage;
    localVoltageCalculated = Ain_AdcToVoltage((float)Ain_AuxBufferDma[0u], actualVoltageReference);
    localAverage += Ain_ConvertVoltageToDistanceGP2Y0A21YK0F(localVoltageCalculated);

    if(AIN_IRCYCLECOUNTER == localCounter)
    {
        IRSensorValue = (float)localAverage / (float)localCounter;
    }
    else
    {
        /* Do nothing. */
    }
}

/* Approximate GP2Y0A21YK0F distance calculation from sensor voltage */
float Ain_ConvertVoltageToDistanceGP2Y0A21YK0F(float voltage)
{
    float distanceCm = AIN_ZERO;
    if (voltage <= AIN_GP2Y0A21YK0F_BLINDVOLTAGE)
    {
        return AIN_INVALIDVALUE;
    }
    else
    {
        /* Do nothing. */
    }

    distanceCm = 27.728f * powf(voltage, -1.2045f);

    if (distanceCm < AIN_GP2Y0A21YK0F_MINDIST)
    {
        distanceCm = AIN_GP2Y0A21YK0F_MINDIST;
    }
    else if (distanceCm > AIN_GP2Y0A21YK0F_MAXDIST)
    {
        distanceCm = AIN_GP2Y0A21YK0F_MAXDIST;
    }
    else
    {
        /* Do nothing. */
    }

    return distanceCm;
}

float Ain_AdcToVoltage(float rawAdc, float refVoltage)
{
    return ((float)rawAdc / ADC_MAX_VALUE) * (float)(refVoltage / AIN_DIVISIONFACTOR_TEN);
}

void Ain_MainFunction(void)
{
    static uint8 localCnt = AIN_ZERO;

    if(AIN_MAXAVG_CNT == localCnt)
    {
        for(uint8 i = AIN_ZERO; i < CHANNELS_NUM; i++)  Ain_AuxBufferDma[i] /= localCnt;

        Ain_ProcessIRSensorValue();
        CurrentSenseEpsValue_Right = Ain_ConvertCurrentSense((float)Ain_AuxBufferDma[1u]);
        CurrentSenseEpsValue_Left = Ain_ConvertCurrentSense((float)Ain_AuxBufferDma[2u]);
        VoltageSenseEpsFanValue = Ain_ConvertVoltageSense((float)Ain_AuxBufferDma[3u], AIN_VFB_5V);
        VoltageSenseEpsLine = Ain_ConvertVoltageSense((float)Ain_AuxBufferDma[4u], AIN_VFB_12V);

        localCnt = AIN_ZERO;
    }
    else
    {
        for(uint8 i = AIN_ZERO; i < CHANNELS_NUM; i++) Ain_AuxBufferDma[i] += Dma_DestinationAdcBuffer[i];
        localCnt++;
    }


    Ain_MainCounter++;
}
