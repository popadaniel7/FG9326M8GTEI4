/* USER CODE BEGIN Header */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "can.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdbool.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef enum
{
	DEFAULT = 0U,
	EXTENDED = 1U,
	PROGRAMMING = 2U,
	CALIBRATION = 4U,
	HARDRESET = 5U,
	JUMPTOAPPL = 7U
}FBL_DSC_t;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SESSIONSTATUS_ADDR 					0x20004c20
#define APPL_START_ADDRESS 					0x8003C00
#define NCR_START_ADDRESS 					0x20004c00
#define NCR_END_ADDRESS   					0x20004fff
#define NCR_SIZE          					(NCR_END_ADDRESS - NCR_START_ADDRESS + 1)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8 FBL_RxFrame[8] = {0};
uint8 FBL_TxFrame[8] = {0};
CAN_RxHeaderTypeDef FBL_RxHeader = {0, 0, 0, 0, 0, 0, 0};
CAN_TxHeaderTypeDef FBL_TxHeader = {0, 0, 0, 0, 0, 0};
uint32 FBL_TxMailbox = 0;
uint32* FBL_DSC_Pointer = (uint32*)(SESSIONSTATUS_ADDR);
FBL_DSC_t FBL_DSC_State = JUMPTOAPPL;
uint32 FBL_ProgrammingData = 0;
uint32 FBL_ProgrammingIndex = 0;
uint32 FBL_ProgrammingAddress = 0;
uint32 ROM_APPL_START_ADDR_storedValue = 0;
uint32 ROM_APPL_START_ADDR = 0x8003C00;
uint32 FBL_DSC_Status = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_NVIC_Init(void);
/* USER CODE BEGIN PFP */
void FBL_JumpToAppl(void);
void FBL_DiagService_ER_SoftReset(void);
void FBL_DiagService_ER_HardReset(void);
void FBL_DiagService_DSC_Programming(void);
void FBL_DiagService_DSC_CodingSession(void);
void FBL_DiagService_DSC_Calibration(void);
void FBL_DiagService_DSC_RequestDownload(void);
void FBL_DiagService_DSC_TransferData(void);
void FBL_DiagService_DSC_RequestTransferExit(void);
void FBL_DiagService_RAR_MassEraseAPPL(void);
void FBL_DiagService_RAR_MassEraseCALIBRATION(void);
void FBL_DiagService_RDBI_ReadActiveDiagnosticSession(void);
void FBL_NvM_FlashReadData(uint32 StartSectorAddress, uint32 *RxBuf, uint16 numberofwords);
uint32 FBL_NvM_FlashWriteData(uint32 StartSectorAddress, uint32 *Data, uint16 numberofwords);
uint32 FBL_NvM_EraseFlash_APPL(void);
uint32 FBL_NvM_EraseFlash_CALIBRATION(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_CAN_RxFifo0FullCallback(CAN_HandleTypeDef *hcan)
{
	/* Pending and full callbacks used to make sure no message is lost. */
	HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &FBL_RxHeader, FBL_RxFrame);
	switch(FBL_DSC_State)
	{
	case PROGRAMMING:
		FBL_DiagService_DSC_Programming();
		break;
	case CALIBRATION:
		FBL_DiagService_DSC_Calibration();
		break;
	case HARDRESET:
		FBL_DiagService_ER_HardReset();
		break;
	default:/* Default case to allow programming in case of faulty application software. */
		if(FBL_RxFrame[1] == 0x10 &&
				FBL_RxFrame[2] == 0x02)
		{
			FBL_TxHeader.DLC = FBL_RxHeader.DLC;
			FBL_TxHeader.StdId = FBL_RxHeader.StdId + 1;
			FBL_TxFrame[0] = FBL_RxFrame[0];
			FBL_TxFrame[1] = FBL_RxFrame[1] + 0x40;
			FBL_TxFrame[2] = FBL_RxFrame[2];
			FBL_DSC_State = PROGRAMMING;
			HAL_CAN_AddTxMessage(hcan, &FBL_TxHeader, FBL_TxFrame, &FBL_TxMailbox);
			FBL_DiagService_DSC_Programming();
		}
		else
		{
			/* Do nothing. */
		}
		break;
	}
}
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	/* Pending and full callbacks used to make sure no message is lost. */
	HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &FBL_RxHeader, FBL_RxFrame);
	switch(FBL_DSC_State)
	{
	case PROGRAMMING:
		FBL_DiagService_DSC_Programming();
		break;
	case CALIBRATION:
		FBL_DiagService_DSC_Calibration();
		break;
	case HARDRESET:
		FBL_DiagService_ER_HardReset();
		break;
	default:
		if(FBL_RxFrame[1] == 0x10 &&
				FBL_RxFrame[2] == 0x02)
		{
			FBL_TxHeader.DLC = FBL_RxHeader.DLC;
			FBL_TxHeader.StdId = FBL_RxHeader.StdId + 1;
			FBL_TxFrame[0] = FBL_RxFrame[0];
			FBL_TxFrame[1] = FBL_RxFrame[1] + 0x40;
			FBL_TxFrame[2] = FBL_RxFrame[2];
			FBL_DSC_State = PROGRAMMING;
			HAL_CAN_AddTxMessage(hcan, &FBL_TxHeader, FBL_TxFrame, &FBL_TxMailbox);
			FBL_DiagService_DSC_Programming();
		}
		else
		{
			/* Do nothing. */
		}
		break;
	}
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_CAN_Init();

  /* Initialize interrupts */
  MX_NVIC_Init();
  /* USER CODE BEGIN 2 */
	FBL_ProgrammingAddress = 0;
	FBL_ProgrammingIndex = 0;
	FBL_NvM_FlashReadData(ROM_APPL_START_ADDR, &ROM_APPL_START_ADDR_storedValue, 1);
	if(ROM_APPL_START_ADDR_storedValue != 0xFFFFFFFF)
	{
		FBL_DSC_Pointer = (uint32*)(SESSIONSTATUS_ADDR);
		FBL_DSC_Status = *FBL_DSC_Pointer;
	}
	else
	{
		/* Do nothing. */
	}
	if((FBL_DSC_Status == PROGRAMMING)
			|| (FBL_DSC_Status == CALIBRATION))
	{
		FBL_DSC_State = FBL_DSC_Status;
	}
	else
	{
		if(ROM_APPL_START_ADDR_storedValue != 0xFFFFFFFF)
		{
			if((RCC->CSR & RCC_CSR_PORRSTF) != 0)
			{
				for(uint32* addr = ((uint32_t*)NCR_START_ADDRESS); addr <= ((uint32_t*)NCR_END_ADDRESS); addr++) *addr = 0;
				/* Reset the flag. */
				RCC->CSR |= RCC_CSR_PORRSTF;
				RCC->CSR |= RCC_CSR_RMVF;
			}
			else
			{
				/* Do nothing. */
			}
			FBL_JumpToAppl();
		}
		else
		{
			/* Do nothing. */
		}
	}
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1)
	{
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief NVIC Configuration.
  * @retval None
  */
static void MX_NVIC_Init(void)
{
  /* CAN1_SCE_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(CAN1_SCE_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(CAN1_SCE_IRQn);
  /* CAN1_RX1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(CAN1_RX1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(CAN1_RX1_IRQn);
  /* RCC_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(RCC_IRQn, 4, 0);
  HAL_NVIC_EnableIRQ(RCC_IRQn);
  /* FLASH_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(FLASH_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(FLASH_IRQn);
  /* PVD_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(PVD_IRQn, 6, 0);
  HAL_NVIC_EnableIRQ(PVD_IRQn);
  /* USB_HP_CAN1_TX_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USB_HP_CAN1_TX_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USB_HP_CAN1_TX_IRQn);
  /* USB_LP_CAN1_RX0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USB_LP_CAN1_RX0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
}

/* USER CODE BEGIN 4 */
uint32 Nvm_GetPage(uint32 Address);
uint32 Nvm_FlashWriteData(uint32 StartPageAddress, uint32 *Data, uint16 numberofwords);
uint32 FBL_NvM_EraseFlash_APPL(void)
{
	static FLASH_EraseInitTypeDef EraseInitStruct;
	uint32 PAGEError;
	uint32 StartPage = 0x8004000;
	uint32 EndPageAdress = 0x800f7ff;
	uint32 EndPage = Nvm_GetPage(EndPageAdress);
	HAL_FLASH_Unlock();
	EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.PageAddress = StartPage;
	EraseInitStruct.NbPages     = ((EndPage - StartPage)/FLASH_PAGE_SIZE) +1;
	if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK)
	{
		return HAL_FLASH_GetError ();
	}
	else
	{
		/* Do nothing. */
	}
	HAL_FLASH_Lock();
	return 0;
}
uint32 FBL_NvM_EraseFlash_CALIBRATION(void)
{
	static FLASH_EraseInitTypeDef EraseInitStruct;
	uint32 PAGEError;
	uint32 StartPage = 0x0800F800;
	uint32 EndPageAdress = 0x0800FBFF;
	uint32 EndPage = Nvm_GetPage(EndPageAdress);
	HAL_FLASH_Unlock();
	EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.PageAddress = StartPage;
	EraseInitStruct.NbPages     = ((EndPage - StartPage)/FLASH_PAGE_SIZE) +1;
	if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK)
	{
		return HAL_FLASH_GetError ();
	}
	else
	{
		/* Do nothing. */
	}
	HAL_FLASH_Lock();
	return 0;
}
uint32 Nvm_GetPage(uint32 Address)
{
	for (uint8 indx=0; indx < 128; indx++) if((Address < (0x08000000 + (FLASH_PAGE_SIZE *(indx + 1))) ) && (Address >= (0x08000000 + FLASH_PAGE_SIZE * indx))) return (0x08000000 + FLASH_PAGE_SIZE * indx);
	return 0;
}
uint32 FBL_NvM_FlashWriteData(uint32 StartPageAddress, uint32 *Data, uint16 numberofwords)
{
	__disable_irq();
	uint32 sofar=0;
	HAL_FLASH_Unlock();
	while (sofar<numberofwords)
	{
		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, StartPageAddress, Data[sofar]) == HAL_OK)
		{
			StartPageAddress += 4;
			sofar++;
		}
		else
		{
			return HAL_FLASH_GetError ();
		}
	}
	HAL_FLASH_Lock();
	__enable_irq();
	return 0;
}
void FBL_NvM_FlashReadData(uint32 StartPageAddress, uint32 *RxBuf, uint16 numberofwords)
{
	__disable_irq();
	for (uint16 i = 0; i < numberofwords; i++) RxBuf[i] = *(__IO uint32 *)(StartPageAddress + (i * 4));
	__enable_irq();
}

void FBL_DiagService_DSC_RequestDownload(void)
{
	FBL_TxHeader.DLC = FBL_RxHeader.DLC;
	FBL_TxHeader.StdId = FBL_RxHeader.StdId + 1;
	FBL_TxFrame[0] = FBL_RxFrame[0];
	FBL_TxFrame[1] = FBL_RxFrame[1] + 0x40;
	FBL_TxFrame[2] = FBL_RxFrame[2];
	FBL_TxFrame[3] = FBL_RxFrame[3];
	FBL_TxFrame[4] = FBL_RxFrame[4];
	HAL_CAN_AddTxMessage(&hcan, &FBL_TxHeader, FBL_TxFrame, &FBL_TxMailbox);
	for(uint8 i = 0; i < 8 ; i++) FBL_TxFrame[i] = 0;
	FBL_TxHeader.DLC = 0;
	FBL_TxHeader.ExtId = 0;
	FBL_TxHeader.IDE = 0;
	FBL_TxHeader.RTR = 0;
	FBL_TxHeader.StdId = 0;
	FBL_TxHeader.TransmitGlobalTime = 0;
	FBL_TxMailbox = 0;
	FBL_RxHeader.StdId = 0;
	FBL_RxHeader.DLC = 0;
	FBL_RxHeader.ExtId = 0;
	FBL_RxHeader.FilterMatchIndex = 0;
	FBL_RxHeader.IDE = 0;
	FBL_RxHeader.RTR = 0;
	FBL_RxHeader.Timestamp = 0;
	for(uint8 i = 0; i < 8; i++) FBL_RxFrame[i] = 0;
}
void FBL_DiagService_DSC_TransferData(void)
{
	if(PROGRAMMING == FBL_DSC_State)
	{
		FBL_ProgrammingData = (FBL_RxFrame[6] << 24) | (FBL_RxFrame[5] << 16) | (FBL_RxFrame[4] << 8) | FBL_RxFrame[3];
		FBL_NvM_FlashWriteData(FBL_ProgrammingAddress, &FBL_ProgrammingData, 1);
		FBL_ProgrammingAddress += 4;
		FBL_ProgrammingIndex += 1;
		FBL_TxHeader.DLC = FBL_RxHeader.DLC;
		FBL_TxHeader.StdId = FBL_RxHeader.StdId + 1;
		FBL_TxFrame[0] = FBL_RxFrame[0];
		FBL_TxFrame[1] = FBL_RxFrame[1] + 0x40;
		FBL_TxFrame[2] = FBL_RxFrame[2];
		HAL_CAN_AddTxMessage(&hcan, &FBL_TxHeader, FBL_TxFrame, &FBL_TxMailbox);
	}
	else if(CALIBRATION == FBL_DSC_State)
	{
		FBL_ProgrammingData = (FBL_RxFrame[6] << 0) | (FBL_RxFrame[5] << 8) | (FBL_RxFrame[4] << 16) | (FBL_RxFrame[3] << 24);
		FBL_NvM_FlashWriteData(FBL_ProgrammingAddress, &FBL_ProgrammingData, 1);
		FBL_ProgrammingAddress += 4;
		FBL_ProgrammingIndex += 1;
		FBL_TxHeader.DLC = FBL_RxHeader.DLC;
		FBL_TxHeader.StdId = FBL_RxHeader.StdId + 1;
		FBL_TxFrame[0] = FBL_RxFrame[0];
		FBL_TxFrame[1] = FBL_RxFrame[1] + 0x40;
		FBL_TxFrame[2] = FBL_RxFrame[2];
		HAL_CAN_AddTxMessage(&hcan, &FBL_TxHeader, FBL_TxFrame, &FBL_TxMailbox);
	}
	else
	{
		/* Do nothing. */
	}
	for(uint8 i = 0; i < 8 ; i++) FBL_TxFrame[i] = 0;
	FBL_TxHeader.DLC = 0;
	FBL_TxHeader.ExtId = 0;
	FBL_TxHeader.IDE = 0;
	FBL_TxHeader.RTR = 0;
	FBL_TxHeader.StdId = 0;
	FBL_TxHeader.TransmitGlobalTime = 0;
	FBL_TxMailbox = 0;
	FBL_RxHeader.StdId = 0;
	FBL_RxHeader.DLC = 0;
	FBL_RxHeader.ExtId = 0;
	FBL_RxHeader.FilterMatchIndex = 0;
	FBL_RxHeader.IDE = 0;
	FBL_RxHeader.RTR = 0;
	FBL_RxHeader.Timestamp = 0;
	for(uint8 i = 0; i < 8; i++) FBL_RxFrame[i] = 0;
}
void FBL_DiagService_DSC_RequestTransferExit(void)
{
	FBL_TxHeader.DLC = FBL_RxHeader.DLC;
	FBL_TxHeader.StdId = FBL_RxHeader.StdId + 1;
	FBL_TxFrame[0] = FBL_RxFrame[0];
	FBL_TxFrame[1] = FBL_RxFrame[1] + 0x40;
	FBL_TxFrame[2] = FBL_RxFrame[2];
	HAL_CAN_AddTxMessage(&hcan, &FBL_TxHeader, FBL_TxFrame, &FBL_TxMailbox);
	for(uint8 i = 0; i < 8 ; i++) FBL_TxFrame[i] = 0;
	FBL_TxHeader.DLC = 0;
	FBL_TxHeader.ExtId = 0;
	FBL_TxHeader.IDE = 0;
	FBL_TxHeader.RTR = 0;
	FBL_TxHeader.StdId = 0;
	FBL_TxHeader.TransmitGlobalTime = 0;
	FBL_TxMailbox = 0;
	FBL_RxHeader.StdId = 0;
	FBL_RxHeader.DLC = 0;
	FBL_RxHeader.ExtId = 0;
	FBL_RxHeader.FilterMatchIndex = 0;
	FBL_RxHeader.IDE = 0;
	FBL_RxHeader.RTR = 0;
	FBL_RxHeader.Timestamp = 0;
	for(uint8 i = 0; i < 8; i++) FBL_RxFrame[i] = 0;
}
void FBL_DiagService_RAR_MassEraseCALIBRATION(void)
{
	FBL_TxHeader.DLC = 0x05;
	FBL_TxHeader.StdId = FBL_RxHeader.StdId + 1;
	FBL_TxFrame[0] = 0x04;
	FBL_TxFrame[1] = 0x71;
	FBL_TxFrame[2] = 0x00;
	FBL_TxFrame[3] = 0x00;
	FBL_TxFrame[4] = 0x02;
	HAL_CAN_AddTxMessage(&hcan, &FBL_TxHeader, FBL_TxFrame, &FBL_TxMailbox);
	FBL_NvM_EraseFlash_CALIBRATION();
	for(uint8 i = 0; i < 8 ; i++) FBL_TxFrame[i] = 0;
	FBL_TxHeader.DLC = 0;
	FBL_TxHeader.ExtId = 0;
	FBL_TxHeader.IDE = 0;
	FBL_TxHeader.RTR = 0;
	FBL_TxHeader.StdId = 0;
	FBL_TxHeader.TransmitGlobalTime = 0;
	FBL_TxMailbox = 0;
	FBL_RxHeader.StdId = 0;
	FBL_RxHeader.DLC = 0;
	FBL_RxHeader.ExtId = 0;
	FBL_RxHeader.FilterMatchIndex = 0;
	FBL_RxHeader.IDE = 0;
	FBL_RxHeader.RTR = 0;
	FBL_RxHeader.Timestamp = 0;
	for(uint8 i = 0; i < 8; i++) FBL_RxFrame[i] = 0;
}
void FBL_DiagService_RAR_MassEraseAPPL(void)
{
	FBL_TxHeader.DLC = FBL_RxHeader.DLC;
	FBL_TxHeader.StdId = FBL_RxHeader.StdId + 1;
	FBL_TxFrame[0] = FBL_RxFrame[0];
	FBL_TxFrame[1] = FBL_RxFrame[1] + 0x40;
	FBL_TxFrame[2] = FBL_RxFrame[2];
	HAL_CAN_AddTxMessage(&hcan, &FBL_TxHeader, FBL_TxFrame, &FBL_TxMailbox);
	FBL_NvM_EraseFlash_APPL();
	for(uint8 i = 0; i < 8 ; i++) FBL_TxFrame[i] = 0;
	FBL_TxHeader.DLC = 0;
	FBL_TxHeader.ExtId = 0;
	FBL_TxHeader.IDE = 0;
	FBL_TxHeader.RTR = 0;
	FBL_TxHeader.StdId = 0;
	FBL_TxHeader.TransmitGlobalTime = 0;
	FBL_TxMailbox = 0;
	FBL_RxHeader.StdId = 0;
	FBL_RxHeader.DLC = 0;
	FBL_RxHeader.ExtId = 0;
	FBL_RxHeader.FilterMatchIndex = 0;
	FBL_RxHeader.IDE = 0;
	FBL_RxHeader.RTR = 0;
	FBL_RxHeader.Timestamp = 0;
	for(uint8 i = 0; i < 8; i++) FBL_RxFrame[i] = 0;
}
void FBL_JumpToAppl(void)
{
	typedef void (*pFunction)(void);
	static uint32 app_address = APPL_START_ADDRESS;
	static pFunction app_entry;
	uint32 msp_value = *((uint32*)app_address);
	__disable_irq(); // disable interrupts because the controller can't alone and will have interrupts triggered in the application for some reason...
	HAL_NVIC_ClearPendingIRQ(CAN1_SCE_IRQn); // manually clear the interrupts because why not...
	HAL_NVIC_ClearPendingIRQ(CAN1_RX1_IRQn);
	HAL_NVIC_ClearPendingIRQ(USB_LP_CAN1_RX0_IRQn);
	HAL_NVIC_ClearPendingIRQ(USB_HP_CAN1_TX_IRQn);
	HAL_NVIC_ClearPendingIRQ(RCC_IRQn);
	HAL_NVIC_ClearPendingIRQ(FLASH_IRQn);
	HAL_NVIC_ClearPendingIRQ(PVD_IRQn);
	SysTick->CTRL = 0; // manually disable sys tick because it will not by itself...
	SysTick->LOAD = 0;
	SysTick->VAL = 0;
	SCB->VTOR = app_address;
	__set_MSP(msp_value);
	app_entry = (pFunction)(*((uint32*)(app_address + 4)));
	app_entry(); // goto app
}
void FBL_DiagService_ER_HardReset(void)
{
	__disable_irq();
	*FBL_DSC_Pointer = 0;
	__NVIC_SystemReset();
}
void FBL_DiagService_RDBI_ReadActiveDiagnosticSession(void)
{
	FBL_TxHeader.DLC = 5;
	FBL_TxHeader.StdId = FBL_RxHeader.StdId + 1;
	FBL_TxFrame[0] = 0x04;
	FBL_TxFrame[1] = FBL_RxFrame[1] + 0x40;
	FBL_TxFrame[2] = FBL_RxFrame[2];
	FBL_TxFrame[3] = FBL_RxFrame[3];
	FBL_TxFrame[4] = FBL_DSC_State;
	HAL_CAN_AddTxMessage(&hcan, &FBL_TxHeader, FBL_TxFrame, &FBL_TxMailbox);
	for(uint8 i = 0; i < 8 ; i++) FBL_TxFrame[i] = 0;
	FBL_TxHeader.DLC = 0;
	FBL_TxHeader.ExtId = 0;
	FBL_TxHeader.IDE = 0;
	FBL_TxHeader.RTR = 0;
	FBL_TxHeader.StdId = 0;
	FBL_TxHeader.TransmitGlobalTime = 0;
	FBL_TxMailbox = 0;
	FBL_RxHeader.StdId = 0;
	FBL_RxHeader.DLC = 0;
	FBL_RxHeader.ExtId = 0;
	FBL_RxHeader.FilterMatchIndex = 0;
	FBL_RxHeader.IDE = 0;
	FBL_RxHeader.RTR = 0;
	FBL_RxHeader.Timestamp = 0;
	for(uint8 i = 0; i < 8; i++) FBL_RxFrame[i] = 0;
}
void FBL_DiagService_DSC_Programming(void)
{
	if(FBL_RxFrame[1] == 0x22 &&
			FBL_RxFrame[2] == 0xF1 &&
			FBL_RxFrame[3] == 0x86)
	{
		FBL_DiagService_RDBI_ReadActiveDiagnosticSession();
	}
	else
	{
		/* Do nothing. */
	}
	if(FBL_RxFrame[0] == 0x04 &&
			FBL_RxFrame[1] == 0x31 &&
			FBL_RxFrame[4] == 0x00)
	{
		FBL_DiagService_RAR_MassEraseAPPL();
	}
	else
	{
		/* Do nothing. */
	}
	if(FBL_RxFrame[1] == 0x34)
	{
		FBL_ProgrammingAddress = (0x08 << 24)
		| (FBL_RxFrame[4] << 16)
		| (FBL_RxFrame[5] << 8)
		| (FBL_RxFrame[6]);
		FBL_DiagService_DSC_RequestDownload();
	}
	else
	{
		/* Do nothing. */
	}
	if(FBL_RxFrame[1] == 0x36)
	{
		FBL_DiagService_DSC_TransferData();
	}
	else
	{
		/* Do nothing. */
	}
	if(FBL_RxFrame[1] == 0x37
			&& FBL_RxFrame[0] == 0x01)
	{
		FBL_DiagService_DSC_RequestTransferExit();
	}
	else
	{
		/* Do nothing. */
	}
	if(FBL_RxFrame[1] == 0x10 &&
			FBL_RxFrame[2] == 0x05)
	{
		FBL_DSC_State = CALIBRATION;
		FBL_TxHeader.DLC = FBL_RxHeader.DLC;
		FBL_TxHeader.StdId = FBL_RxHeader.StdId + 1;
		FBL_TxFrame[0] = FBL_RxFrame[0];
		FBL_TxFrame[1] = FBL_RxFrame[1] + 0x40;
		FBL_TxFrame[2] = FBL_RxFrame[2];
		FBL_TxFrame[3] = FBL_RxFrame[3];
		HAL_CAN_AddTxMessage(&hcan, &FBL_TxHeader, FBL_TxFrame, &FBL_TxMailbox);
		for(uint8 i = 0; i < 8 ; i++)
		{
			FBL_TxFrame[i] = 0;
		}
		FBL_TxHeader.DLC = 0;
		FBL_TxHeader.ExtId = 0;
		FBL_TxHeader.IDE = 0;
		FBL_TxHeader.RTR = 0;
		FBL_TxHeader.StdId = 0;
		FBL_TxHeader.TransmitGlobalTime = 0;
		FBL_TxMailbox = 0;
	}
	if(FBL_RxFrame[1] == 0x11 &&
			FBL_RxFrame[2] == 0x01)
	{
		FBL_TxHeader.DLC = FBL_RxHeader.DLC;
		FBL_TxHeader.StdId = FBL_RxHeader.StdId + 1;
		FBL_TxFrame[0] = FBL_RxFrame[0];
		FBL_TxFrame[1] = FBL_RxFrame[1] + 0x40;
		FBL_TxFrame[2] = FBL_RxFrame[2];
		FBL_TxFrame[3] = FBL_RxFrame[3];
		HAL_CAN_AddTxMessage(&hcan, &FBL_TxHeader, FBL_TxFrame, &FBL_TxMailbox);
		for(uint32 i = 0; i < 9000; i++);
		FBL_DiagService_ER_HardReset();
	}
	else
	{
		/* Do nothing. */
	}
	for(uint8 i = 0; i < 8 ; i++) FBL_TxFrame[i] = 0;
	FBL_TxHeader.DLC = 0;
	FBL_TxHeader.ExtId = 0;
	FBL_TxHeader.IDE = 0;
	FBL_TxHeader.RTR = 0;
	FBL_TxHeader.StdId = 0;
	FBL_TxHeader.TransmitGlobalTime = 0;
	FBL_TxMailbox = 0;
	FBL_RxHeader.StdId = 0;
	FBL_RxHeader.DLC = 0;
	FBL_RxHeader.ExtId = 0;
	FBL_RxHeader.FilterMatchIndex = 0;
	FBL_RxHeader.IDE = 0;
	FBL_RxHeader.RTR = 0;
	FBL_RxHeader.Timestamp = 0;
	for(uint8 i = 0; i < 8; i++) FBL_RxFrame[i] = 0;
}
void FBL_DiagService_DSC_Calibration(void)
{
	if(FBL_RxFrame[1] == 0x22 &&
			FBL_RxFrame[2] == 0xF1 &&
			FBL_RxFrame[3] == 0x86)
	{
		FBL_DiagService_RDBI_ReadActiveDiagnosticSession();
	}
	else
	{
		/* Do nothing. */
	}
	if(FBL_RxFrame[0] == 0x04 &&
			FBL_RxFrame[1] == 0x31 &&
			FBL_RxFrame[4] == 0x02)
	{
		FBL_DiagService_RAR_MassEraseCALIBRATION();
	}
	else
	{
		/* Do nothing. */
	}
	if(FBL_RxFrame[1] == 0x34)
	{
		if(FBL_ProgrammingAddress == 0)
		{
			FBL_ProgrammingAddress = 0x0800F800;
		}
		else
		{
			/* Do nothing. */
		}

		FBL_DiagService_DSC_RequestDownload();
	}
	else
	{
		/* Do nothing. */
	}
	if(FBL_RxFrame[1] == 0x36)
	{
		FBL_DiagService_DSC_TransferData();
	}
	else
	{
		/* Do nothing. */
	}
	if(FBL_RxFrame[1] == 0x37
			&& FBL_RxFrame[0] == 0x01)
	{
		FBL_DiagService_DSC_RequestTransferExit();
	}
	else
	{
		/* Do nothing. */
	}
	if(FBL_RxFrame[1] == 0x10 &&
			FBL_RxFrame[2] == 0x02)
	{
		FBL_DSC_State = PROGRAMMING;
		FBL_TxHeader.DLC = FBL_RxHeader.DLC;
		FBL_TxHeader.StdId = FBL_RxHeader.StdId + 1;
		FBL_TxFrame[0] = FBL_RxFrame[0];
		FBL_TxFrame[1] = FBL_RxFrame[1] + 0x40;
		FBL_TxFrame[2] = FBL_RxFrame[2];
		FBL_TxFrame[3] = FBL_RxFrame[3];
		HAL_CAN_AddTxMessage(&hcan, &FBL_TxHeader, FBL_TxFrame, &FBL_TxMailbox);
		for(uint8 i = 0; i < 8 ; i++)
		{
			FBL_TxFrame[i] = 0;
		}
		FBL_TxHeader.DLC = 0;
		FBL_TxHeader.ExtId = 0;
		FBL_TxHeader.IDE = 0;
		FBL_TxHeader.RTR = 0;
		FBL_TxHeader.StdId = 0;
		FBL_TxHeader.TransmitGlobalTime = 0;
		FBL_TxMailbox = 0;
	}
	if(FBL_RxFrame[1] == 0x11 &&
			FBL_RxFrame[2] == 0x01)
	{
		FBL_TxHeader.DLC = FBL_RxHeader.DLC;
		FBL_TxHeader.StdId = FBL_RxHeader.StdId + 1;
		FBL_TxFrame[0] = FBL_RxFrame[0];
		FBL_TxFrame[1] = FBL_RxFrame[1] + 0x40;
		FBL_TxFrame[2] = FBL_RxFrame[2];
		FBL_TxFrame[3] = FBL_RxFrame[3];
		HAL_CAN_AddTxMessage(&hcan, &FBL_TxHeader, FBL_TxFrame, &FBL_TxMailbox);
		for(uint32 i = 0; i < 9000; i++);
		FBL_DiagService_ER_HardReset();
	}
	else
	{
		/* Do nothing. */
	}
	FBL_RxHeader.StdId = 0;
	FBL_RxHeader.DLC = 0;
	FBL_RxHeader.ExtId = 0;
	FBL_RxHeader.FilterMatchIndex = 0;
	FBL_RxHeader.IDE = 0;
	FBL_RxHeader.RTR = 0;
	FBL_RxHeader.Timestamp = 0;
	for(uint8 i = 0; i < 8; i++)
	{
		FBL_RxFrame[i] = 0;
	}
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	__disable_irq();
	__NVIC_SystemReset();
	while (1)
	{
	}
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */