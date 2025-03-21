/* START OF FILE */
/* INCLUDE START */
#include "EcuM.h"
#include "tim.h"
#include "adc.h"
#include "spi.h"
#include "Ain.h"
#include "TimH.h"
#include "CanSpi.h"
#include "UartH.h"
#include "Nvm.h"
#include "Dem.h"
#include "usart.h"
#include "gpio.h"
#include "cmsis_os2.h"
/* INCLUDE END */
/* VARIABLES START */
uint8 EcuM_WakeupReason = 0;
EcuMSystem_st EcuMSystem_OvwArr __attribute__((section(".ncr")));
EcuMError_t EcuM_ErrorHistory[8] __attribute__((section(".ncr")));
EcuMWakeup_t EcuM_WakeupHistory[8] __attribute__((section(".ncr")));
static uint8 EcuM_WakeupHistoryCounter __attribute__((section(".ncr")));
static uint8 EcuM_ErrorHistoryCounter __attribute__((section(".ncr")));
extern uint32 Dcm_AliveCounter __attribute__((section(".ncr")));
EcuMStates_t EcuM_State = STARTUP;
uint8 EcuM_Hc05StatePinCheck = 0;
uint8 EcuM_SleepModeState = 0;
uint32 EcuM_CyclicWakeupCounter = 0;
static uint32 EcuM_MainCounter = 0;
static uint32 EcuM_PostrunTimer = 0;
/* VARIABLES END */
/* FUNCTIONS START */
void EcuM_MainFunction(void);
void EcuM_PerformReset(EcuMReset_t param);
static void EcuM_ProcessEcuState(void);
static void EcuM_GoSleep(void);
static void EcuM_ProcessFaultState(void);
/* FUNCTIONS END */
/* FUNCTIONS START */
void EcuM_MainFunction(void)
{
	EcuM_ProcessEcuState();
	EcuM_ProcessFaultState();
	EcuM_MainCounter++;
}
void EcuM_PerformReset(EcuMReset_t param)
{
	__disable_irq();
	if(param)
	{
		Dcm_AliveCounter = 0;
		EcuMSystem_OvwArr.resetArr[param].reset = param;
		EcuMSystem_OvwArr.resetArr[param].counter++;
	}
	else
	{
		/* Do nothing. */
	}
	__NVIC_SystemReset();
}
static void EcuM_ProcessFaultState(void)
{
	uint8 localCounter = 0;
	if((RCC->CSR & RCC_CSR_IWDGRSTF) != 0)
	{
		/* Set the wake-up event. */
		if(EcuM_ErrorHistoryCounter < 8)
		{
			EcuM_ErrorHistory[EcuM_ErrorHistoryCounter] = IWDG_RESET_H;
			EcuM_ErrorHistoryCounter++;
		}
		else
		{
			EcuM_ErrorHistoryCounter = 0;
			EcuM_ErrorHistory[EcuM_ErrorHistoryCounter] = IWDG_RESET_H;
			EcuM_ErrorHistoryCounter++;
		}
		if(EcuM_WakeupHistoryCounter < 8)
		{
			EcuM_WakeupHistory[EcuM_WakeupHistoryCounter] = IWDG_WAKEUP;
			EcuM_WakeupHistoryCounter++;
		}
		else
		{
			EcuM_WakeupHistoryCounter = 0;
			EcuM_WakeupHistory[EcuM_WakeupHistoryCounter] = IWDG_WAKEUP;
			EcuM_WakeupHistoryCounter++;
		}
		/* Reset the flag. */
		RCC->CSR |= RCC_CSR_IWDGRSTF;
	}
	else if((RCC->CSR & RCC_CSR_SFTRSTF) != 0)
	{
		/* Set the wake-up event. */
		if(EcuM_WakeupHistory[EcuM_WakeupHistoryCounter - 1] != HC05_WAKEUP)
		{
			if(EcuM_WakeupHistoryCounter < 8)
			{
				EcuM_WakeupHistory[EcuM_WakeupHistoryCounter] = RESET_WAKEUP;
				EcuM_WakeupHistoryCounter++;
			}
			else
			{
				EcuM_WakeupHistoryCounter = 0;
				EcuM_WakeupHistory[EcuM_WakeupHistoryCounter] = RESET_WAKEUP;
				EcuM_WakeupHistoryCounter++;
			}
		}
		else
		{
			/* Do nothing. */
		}
		/* Reset the flag. */
		RCC->CSR |= RCC_CSR_SFTRSTF;
	}
	else
	{
		/* Do nothing.  */
	}
	RCC->CSR |= RCC_CSR_RMVF;
	for(uint8 i = 0; i < 9; i++)
	{
		if(CanSpi_ErrorArr[i] != 0)
		{
			EcuMSystem_OvwArr.faultArr[SPI_ERROR].fault = SPI_ERROR;
			EcuMSystem_OvwArr.faultArr[SPI_ERROR].counter++;
			if(EcuM_ErrorHistoryCounter < 8)
			{
				EcuM_ErrorHistory[EcuM_ErrorHistoryCounter] = SPI_ERROR_H;
				EcuM_ErrorHistoryCounter++;
			}
			else
			{
				EcuM_ErrorHistoryCounter = 0;
				EcuM_ErrorHistory[EcuM_ErrorHistoryCounter] = SPI_ERROR_H;
				EcuM_ErrorHistoryCounter++;
			}
			CanSpi_ErrorArr[i] = 0;
		}
		else
		{
			/* Do nothing. */
		}
	}
	for(uint8 i = 0; i < 3; i++)
	{
		if(CanSpi_Bus_ErrorArr[i] != 0)
		{
			EcuMSystem_OvwArr.faultArr[CANBUS_ERROR].fault = CANBUS_ERROR;
			EcuMSystem_OvwArr.faultArr[CANBUS_ERROR].counter++;
			if(EcuM_ErrorHistoryCounter < 8)
			{
				EcuM_ErrorHistory[EcuM_ErrorHistoryCounter] = CANBUS_ERROR_H;
				EcuM_ErrorHistoryCounter++;
			}
			else
			{
				EcuM_ErrorHistoryCounter = 0;
				EcuM_ErrorHistory[EcuM_ErrorHistoryCounter] = CANBUS_ERROR_H;
				EcuM_ErrorHistoryCounter++;
			}
			CanSpi_Bus_ErrorArr[i] = 0;
		}
		else
		{
			/* Do nothing. */
		}
		if(Adc_Error[i] != 0)
		{
			EcuMSystem_OvwArr.faultArr[ADC_ERROR].fault = ADC_ERROR;
			EcuMSystem_OvwArr.faultArr[ADC_ERROR].counter++;
			if(EcuM_ErrorHistoryCounter < 8)
			{
				EcuM_ErrorHistory[EcuM_ErrorHistoryCounter] = ADC_ERROR_H;
				EcuM_ErrorHistoryCounter++;
			}
			else
			{
				EcuM_ErrorHistoryCounter = 0;
				EcuM_ErrorHistory[EcuM_ErrorHistoryCounter] = ADC_ERROR_H;
				EcuM_ErrorHistoryCounter++;
			}
			Adc_Error[i] = 0;
		}
		else
		{
			/* Do nothing. */
		}
		if(i < 2)
		{
			if(Tim_ErrorStatus[i] != 0)
			{
				if(EcuM_ErrorHistoryCounter < 8)
				{
					EcuM_ErrorHistory[EcuM_ErrorHistoryCounter] = TIM_ERROR_H;
					EcuM_ErrorHistoryCounter++;
				}
				else
				{
					EcuM_ErrorHistoryCounter = 0;
					EcuM_ErrorHistory[EcuM_ErrorHistoryCounter] = TIM_ERROR_H;
					EcuM_ErrorHistoryCounter++;
				}
				Tim_ErrorStatus[i] = 0;
			}
			else
			{
				/* Do nothing. */
			}
		}
	}
	for(uint8 i = 0; i < 12; i++)
	{
		if(EcuMSystem_OvwArr.resetArr[i].counter != 0)
		{
			Dem_SaveDtc(10, 0xC);
			if(EcuM_ErrorHistoryCounter < 8)
			{
				EcuM_ErrorHistory[EcuM_ErrorHistoryCounter] = EcuMSystem_OvwArr.resetArr[i].reset - 11;
				EcuM_ErrorHistoryCounter++;
			}
			else
			{
				EcuM_ErrorHistoryCounter = 0;
				EcuM_ErrorHistory[EcuM_ErrorHistoryCounter] = EcuMSystem_OvwArr.resetArr[i].reset - 11;
				EcuM_ErrorHistoryCounter++;
			}
			EcuMSystem_OvwArr.resetArr[i].counter = 0;
		}
		else
		{
			localCounter++;
			if(localCounter == 12) Dem_SaveDtc(10, 0);
			else
			{
				/* Do nothing. */
			}
		}
	}
}
void EcuM_ProcessEcuState(void)
{
	if(EcuM_State == PRESLEEP) EcuM_GoSleep();
	else
	{
		/* Do nothing. */
	}
	if(EcuM_State == STARTUP)
	{
		EcuM_State = RUN;
		EcuM_PostrunTimer = 0;
	}
	else
	{
		/* Do nothing. */
	}
	if(((CanSpi_Communication_Status == FULL_COMMUNICATION) || (Ain_Mux[3] >= 400) || (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == 1)))
	{
		EcuM_State = RUN;
		EcuM_PostrunTimer = 0;
	}
	else
	{
		/* Do nothing. */
	}
	if((CanSpi_Communication_Status == NO_COMMUNICATION) && (Ain_Mux[3] < 400) && (EcuM_State == RUN) && (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == 0)) EcuM_State = POSTRUN;
	else
	{
		/* Do nothing. */
	}
	if(EcuM_State == POSTRUN)
	{
		if(EcuM_PostrunTimer == 0) EcuM_PostrunTimer = EcuM_MainCounter;
		else
		{
			/* Do nothing. */
		}
		if(((EcuM_MainCounter - EcuM_PostrunTimer) > 2000) && (EcuM_PostrunTimer != 0)) EcuM_State = PRESLEEP;
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

void EcuM_GoSleep(void)
{
	__disable_irq();
	CanSpi_Sleep();
	HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);
	HAL_PWR_EnterSTANDBYMode();
}

/* FUNCTIONS END */
/* STOP OF FILE */
