/* USER CODE BEGIN Header */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "event_groups.h"
#include "DigitalCluster.h"
#include "DataRecorder.h"
#include "RevCam.h"
#include "CanH.h"
#include "I2cH.h"
#include "Dcm.h"
#include "Dem.h"
#include "Ain.h"
#include "Nvm.h"
#include "EcuM.h"
#include "FsmcH.h"
#include "DcmiH_OV7670.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
long long OS_Counter = 0;
long long IDLE_Counter = 0;
TaskStatus_t xTaskStatusArray[12] = {0};
UBaseType_t uxArraySize = {0};
uint32 ulTotalRunTime = {0};
uint8 OS_IdleIndex = 0;
float OS_IdleRunTime = 0;
float OS_DeltaIdleRunTime = 0;
float OS_CpuLoad = 0;
uint32 OS_AverageCpuLoad = 0;
uint8 OS_XCP_CpuLoad = 0;
uint32 localTaskCounter = 0;
__attribute__((section(".ccmram"))) uint8 Alarm5ms_QM_NVM = 0;
__attribute__((section(".ccmram"))) uint8 Alarm5ms_QM_APPL = 0;
__attribute__((section(".ccmram"))) uint8 Alarm5ms_QM_BSW = 0;
__attribute__((section(".ccmram"))) uint8 Alarm5ms_QM_DCM = 0;
__attribute__((section(".ccmram"))) uint8 Alarm10ms_QM_APPL = 0;
__attribute__((section(".ccmram"))) uint8 Alarm10ms_QM_BSW = 0;
volatile unsigned long ulHighFrequencyTimerTicks;
extern __attribute__((section(".ccmram"))) uint8 EcuM_State;
extern __attribute__((section(".ccmram"))) uint8 Nvm_ReadAllPending;
extern TIM_HandleTypeDef htim9;
/* USER CODE END Variables */
/* Definitions for QM_APPL */
osThreadId_t QM_APPLHandle;
const osThreadAttr_t QM_APPL_attributes = {
  .name = "QM_APPL",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityLow2,
};
/* Definitions for QM_BSW */
osThreadId_t QM_BSWHandle;
const osThreadAttr_t QM_BSW_attributes = {
  .name = "QM_BSW",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityRealtime7,
};
/* Definitions for OS_CPULOAD_MONITORING */
osThreadId_t OS_CPULOAD_MONITORINGHandle;
const osThreadAttr_t OS_CPULOAD_MONITORING_attributes = {
  .name = "OS_CPULOAD_MONITORING",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for NVM_HANDLING */
osThreadId_t NVM_HANDLINGHandle;
const osThreadAttr_t NVM_HANDLING_attributes = {
  .name = "NVM_HANDLING",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow1,
};
/* Definitions for QM_DCM */
osThreadId_t QM_DCMHandle;
const osThreadAttr_t QM_DCM_attributes = {
  .name = "QM_DCM",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityRealtime6,
};
/* Definitions for Alarm5ms_QM_APPL */
osTimerId_t Alarm5ms_QM_APPLHandle;
const osTimerAttr_t Alarm5ms_QM_APPL_attributes = {
  .name = "Alarm5ms_QM_APPL"
};
/* Definitions for Alarm5ms_QM_BSW */
osTimerId_t Alarm5ms_QM_BSWHandle;
const osTimerAttr_t Alarm5ms_QM_BSW_attributes = {
  .name = "Alarm5ms_QM_BSW"
};
/* Definitions for Alarm10ms_QM_APPL */
osTimerId_t Alarm10ms_QM_APPLHandle;
const osTimerAttr_t Alarm10ms_QM_APPL_attributes = {
  .name = "Alarm10ms_QM_APPL"
};
/* Definitions for Alarm10ms_QM_BSW */
osTimerId_t Alarm10ms_QM_BSWHandle;
const osTimerAttr_t Alarm10ms_QM_BSW_attributes = {
  .name = "Alarm10ms_QM_BSW"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
extern void EcuM_PerformReset(uint8 param);
/* USER CODE END FunctionPrototypes */

void QM_APPL_OS_TASK(void *argument);
void QM_BSW_OS_TASK(void *argument);
void OS_CPULOAD_MONITORING_TASK(void *argument);
void NVM_HANDLING_OS_TASK(void *argument);
void OS_TASK_QM_DCM(void *argument);
void Alarm5ms_QM_APPL_Callback(void *argument);
void Alarm5ms_QM_BSW_Callback(void *argument);
void Alarm10ms_QM_APPL_Callback(void *argument);
void Alarm10ms_QM_BSW_Callback(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void configureTimerForRunTimeStats(void);
unsigned long getRunTimeCounterValue(void);
void vApplicationIdleHook(void);
void vApplicationTickHook(void);
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName);
void vApplicationMallocFailedHook(void);

/* USER CODE BEGIN 1 */
void configureTimerForRunTimeStats(void)
{
	ulHighFrequencyTimerTicks = 0;
	HAL_TIM_Base_Start_IT(&htim9);
}

unsigned long getRunTimeCounterValue(void)
{
	return ulHighFrequencyTimerTicks;
}
/* USER CODE END 1 */

/* USER CODE BEGIN 2 */
void vApplicationIdleHook( void )
{
	IDLE_Counter++;
	uxArraySize = uxTaskGetSystemState(xTaskStatusArray, 12, &ulTotalRunTime);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, 0);

}
/* USER CODE END 2 */

/* USER CODE BEGIN 3 */
void vApplicationTickHook( void )
{
	OS_Counter++;
}
/* USER CODE END 3 */

/* USER CODE BEGIN 4 */
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName)
{
	EcuM_PerformReset(12);
}
/* USER CODE END 4 */

/* USER CODE BEGIN 5 */
void vApplicationMallocFailedHook(void)
{
	EcuM_PerformReset(11);
}
/* USER CODE END 5 */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* creation of Alarm5ms_QM_APPL */
  Alarm5ms_QM_APPLHandle = osTimerNew(Alarm5ms_QM_APPL_Callback, osTimerPeriodic, NULL, &Alarm5ms_QM_APPL_attributes);

  /* creation of Alarm5ms_QM_BSW */
  Alarm5ms_QM_BSWHandle = osTimerNew(Alarm5ms_QM_BSW_Callback, osTimerPeriodic, NULL, &Alarm5ms_QM_BSW_attributes);

  /* creation of Alarm10ms_QM_APPL */
  Alarm10ms_QM_APPLHandle = osTimerNew(Alarm10ms_QM_APPL_Callback, osTimerPeriodic, NULL, &Alarm10ms_QM_APPL_attributes);

  /* creation of Alarm10ms_QM_BSW */
  Alarm10ms_QM_BSWHandle = osTimerNew(Alarm10ms_QM_BSW_Callback, osTimerPeriodic, NULL, &Alarm10ms_QM_BSW_attributes);

  /* USER CODE BEGIN RTOS_TIMERS */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of QM_APPL */
  QM_APPLHandle = osThreadNew(QM_APPL_OS_TASK, NULL, &QM_APPL_attributes);

  /* creation of QM_BSW */
  QM_BSWHandle = osThreadNew(QM_BSW_OS_TASK, NULL, &QM_BSW_attributes);

  /* creation of OS_CPULOAD_MONITORING */
  OS_CPULOAD_MONITORINGHandle = osThreadNew(OS_CPULOAD_MONITORING_TASK, NULL, &OS_CPULOAD_MONITORING_attributes);

  /* creation of NVM_HANDLING */
  NVM_HANDLINGHandle = osThreadNew(NVM_HANDLING_OS_TASK, NULL, &NVM_HANDLING_attributes);

  /* creation of QM_DCM */
  QM_DCMHandle = osThreadNew(OS_TASK_QM_DCM, NULL, &QM_DCM_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
	osTimerStart(Alarm10ms_QM_BSWHandle, 10);
	osTimerStart(Alarm5ms_QM_BSWHandle, 5);
	osTimerStart(Alarm10ms_QM_APPLHandle, 10);
	osTimerStart(Alarm5ms_QM_APPLHandle, 5);
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_QM_APPL_OS_TASK */
/* USER CODE END Header_QM_APPL_OS_TASK */
void QM_APPL_OS_TASK(void *argument)
{
  /* USER CODE BEGIN QM_APPL_OS_TASK */
	for(;;)
	{
		if(1 == Alarm5ms_QM_APPL)
		{
			if(1 == EcuM_State && 2 == Nvm_ReadAllPending) DigitalCluster_MainFunction();
			else
			{
				/* Do nothing. */
			}
			Alarm5ms_QM_APPL = 0;
		}
		else
		{
			/* Do nothing.  */
		}
		if(1 == Alarm10ms_QM_APPL && 2 == Nvm_ReadAllPending)
		{
			DataRecorder_MainFunction();
			Alarm10ms_QM_APPL = 0;
		}
		else
		{
			/* Do nothing.  */
		}
		vTaskSuspend(NULL);
	}
  /* USER CODE END QM_APPL_OS_TASK */
}

/* USER CODE BEGIN Header_QM_BSW_OS_TASK */
/* USER CODE END Header_QM_BSW_OS_TASK */
void QM_BSW_OS_TASK(void *argument)
{
  /* USER CODE BEGIN QM_BSW_OS_TASK */
	for(;;)
	{
		if(1 == Alarm5ms_QM_BSW)
		{
			EcuM_MainFunction();
			CanH_MainFunction();
			Alarm5ms_QM_BSW = 0;
		}
		else
		{
			/* Do nothing.  */
		}
		if(1 == Alarm10ms_QM_BSW)
		{
			Ain_MainFunction();
			I2cH_MainFunction();
			DcmiH_MainFunction();
			FsmcH_MainFunction();
			if(1 == EcuM_State) Dem_MainFunction();
			else
			{
				/* Do nothing. */
			}
			Alarm10ms_QM_BSW = 0;
		}
		else
		{
			/* Do nothing.  */
		}
		vTaskSuspend(NULL);
	}
  /* USER CODE END QM_BSW_OS_TASK */
}

/* USER CODE BEGIN Header_OS_CPULOAD_MONITORING_TASK */
/* USER CODE END Header_OS_CPULOAD_MONITORING_TASK */
void OS_CPULOAD_MONITORING_TASK(void *argument)
{
  /* USER CODE BEGIN OS_CPULOAD_MONITORING_TASK */
	for(;;)
	{
		localTaskCounter++;
		OS_DeltaIdleRunTime = xTaskStatusArray[OS_IdleIndex].ulRunTimeCounter - OS_IdleRunTime;
		if(0 != ulHighFrequencyTimerTicks) OS_CpuLoad = 100 - (OS_DeltaIdleRunTime / ulHighFrequencyTimerTicks) * 100;
		else
		{
			/* Do nothing. */
		}
		if(0 < OS_CpuLoad) OS_AverageCpuLoad += OS_CpuLoad;
		else
		{
			/* Do nothing. */
		}
		ulHighFrequencyTimerTicks = 0;
		OS_IdleRunTime = xTaskStatusArray[OS_IdleIndex].ulRunTimeCounter;
		if(localTaskCounter % 40 == 0)
		{
			OS_XCP_CpuLoad = OS_AverageCpuLoad / 40;
			OS_AverageCpuLoad = 0;
		}
		else
		{
			/* Do nothing. */
		}
		vTaskSuspend(NULL);
	}
  /* USER CODE END OS_CPULOAD_MONITORING_TASK */
}

/* USER CODE BEGIN Header_NVM_HANDLING_OS_TASK */
/* USER CODE END Header_NVM_HANDLING_OS_TASK */
void NVM_HANDLING_OS_TASK(void *argument)
{
  /* USER CODE BEGIN NVM_HANDLING_OS_TASK */
	for(;;)
	{
		if(1 == Alarm5ms_QM_NVM)
		{
			Nvm_MainFunction();
			Alarm5ms_QM_NVM = 0;
		}
		else
		{
			/* Do nothing.  */
		}
		vTaskSuspend(NULL);
	}
  /* USER CODE END NVM_HANDLING_OS_TASK */
}

/* USER CODE BEGIN Header_OS_TASK_QM_DCM */
/* USER CODE END Header_OS_TASK_QM_DCM */
void OS_TASK_QM_DCM(void *argument)
{
  /* USER CODE BEGIN OS_TASK_QM_DCM */
	for(;;)
	{
		if(1 == Alarm5ms_QM_DCM)
		{
			if(1 == EcuM_State) Dcm_MainFunction();
			else
			{
				/* Do nothing. */
			}
			Alarm5ms_QM_DCM = 0;
		}
		else
		{
			/* Do nothing. */
		}
		vTaskSuspend(NULL);
	}
  /* USER CODE END OS_TASK_QM_DCM */
}

/* Alarm5ms_QM_APPL_Callback function */
void Alarm5ms_QM_APPL_Callback(void *argument)
{
  /* USER CODE BEGIN Alarm5ms_QM_APPL_Callback */
	vTaskResume(QM_APPLHandle);
	Alarm5ms_QM_APPL = 1;
  /* USER CODE END Alarm5ms_QM_APPL_Callback */
}

/* Alarm5ms_QM_BSW_Callback function */
void Alarm5ms_QM_BSW_Callback(void *argument)
{
  /* USER CODE BEGIN Alarm5ms_QM_BSW_Callback */
	Alarm5ms_QM_BSW = 1;
	Alarm5ms_QM_DCM = 1;
	Alarm5ms_QM_NVM = 1;
	vTaskResume(QM_BSWHandle);
	vTaskResume(QM_DCMHandle);
	vTaskResume(NVM_HANDLINGHandle);
  /* USER CODE END Alarm5ms_QM_BSW_Callback */
}

/* Alarm10ms_QM_APPL_Callback function */
void Alarm10ms_QM_APPL_Callback(void *argument)
{
  /* USER CODE BEGIN Alarm10ms_QM_APPL_Callback */
	Alarm10ms_QM_APPL = 1;
	vTaskResume(QM_APPLHandle);
  /* USER CODE END Alarm10ms_QM_APPL_Callback */
}

/* Alarm10ms_QM_BSW_Callback function */
void Alarm10ms_QM_BSW_Callback(void *argument)
{
  /* USER CODE BEGIN Alarm10ms_QM_BSW_Callback */
	static uint32 localCounter = 0;
	Alarm10ms_QM_BSW = 1;
	localCounter++;
	if(localCounter != 0 && localCounter % 4 == 0) vTaskResume(OS_CPULOAD_MONITORINGHandle);
	else
	{
		/* Do nothing. */
	}
	vTaskResume(QM_BSWHandle);
  /* USER CODE END Alarm10ms_QM_BSW_Callback */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
/* USER CODE END Application */
