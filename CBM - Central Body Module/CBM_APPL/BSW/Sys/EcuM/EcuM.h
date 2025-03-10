/* START OF FILE */
/* INCLUDE START */
#include "Std_Types.h"
/* INCLUDE END */
/* DEFINE START */
#define ECUM_RESETDATAARR_ADDR 0x0807FC2C
#define ECUM_FAULTDATAARR_ADDR 0x0807FC76
#define ECUM_ERRORHISTORYARR_ADDR 0x0807FCB3
#define ECUM_WAKEUPHISTORYARR_ADDR 0x0807FCC6
#define ECUM_INDEX_ADDR 0x0807FCC0
#define DEM_DTC_ADDR 0x0807FCC5
/* DEFINE STOP */
/* TYPEDEF START */
typedef enum
{
	STARTUP = 0x00,
	INIT_SEQ = 0x01,
	RUN = 0x02,
	POSTRUN = 0x03,
	PRESLEEP = 0x04
}EcuMStates_t;
typedef enum
{
	NO_RESET = 0x00,
	MALLOC_RESET = 0x22,
	STACKOVERFLOW_RESET = 0x23,
	NMI_RESET = 0x24,
	HARDFAULT_RESET = 0x25,
	MEMMANAGE_RESET = 0x26,
	BUSFAULT_RESET = 0x27,
	USAGEFAULT_RESET = 0x28,
	FPU_RESET = 0x29,
	ERRORHANDLER_RESET = 0x2A,
	OSFAIL_RESET = 0x2B,
	IWDG_RESET = 0x2C
}EcuMReset_t;
typedef enum
{
	NO_ERROR = 0x00,
	SPI_ERROR = 0x2D,
	I2C_ERROR = 0x2E,
	UART_ERROR = 0x2F,
	ADC_ERROR = 0x30,
	TIM_ERROR = 0x31,
	CANBUS_ERROR = 0x32
}EcuMFault_t;
typedef enum
{
	NO_ERROR_H = 0x00,
	SPI_ERROR_H = 0x11,
	I2C_ERROR_H = 0x12,
	UART_ERROR_H = 0x13,
	ADC_ERROR_H = 0x14,
	TIM_ERROR_H = 0x15,
	CANBUS_ERROR_H = 0x16,
	MALLOC_RESET_H = 0x17,
	STACKOVERFLOW_RESET_H = 0x18,
	NMI_RESET_H = 0x19,
	HARDFAULT_RESET_H = 0x1A,
	MEMMANAGE_RESET_H = 0x1B,
	BUSFAULT_RESET_H = 0x1C,
	USAGEFAULT_RESET_H = 0x1D,
	FPU_RESET_H = 0x1E,
	ERRORHANDLER_RESET_H = 0x1F,
	OSFAIL_RESET_H = 0x20,
	IWDG_RESET_H = 0x21
}EcuMError_t;
typedef enum
{
	NO_WAKEUP = 0x00,
	HC05_WAKEUP = 0x33,
	RESET_WAKEUP = 0x34,
	IWDG_WAKEUP = 0x35
}EcuMWakeup_t;
typedef struct
{
	EcuMFault_t fault;
	uint32 counter;
}EcuMFault_st;
typedef struct
{
	EcuMReset_t reset;
	uint32 counter;
}EcuMReset_st;
typedef struct
{
	EcuMFault_st faultArr[7];
	EcuMReset_st resetArr[11];
}EcuMSystem_st;
/* TYPEDEF END */
/* VARIABLES START */
extern EcuMError_t EcuM_ErrorHistory[8] __attribute__((section(".ncr")));
extern EcuMWakeup_t EcuM_WakeupHistory[8] __attribute__((section(".ncr")));
extern EcuMSystem_st EcuMSystem_OvwArr __attribute__((section(".ncr")));
extern EcuMStates_t EcuM_State;
extern uint8 EcuM_SleepModeState;
/* VARIABLES END */
/* FUNCTIONS START */
extern void EcuM_MainFunction(void);
extern void EcuM_PerformReset(EcuMReset_t param);
extern void EcuM_CyclicSleepActivity(void);
/* FUNCTIONS END */
/* STOP OF FILE */
