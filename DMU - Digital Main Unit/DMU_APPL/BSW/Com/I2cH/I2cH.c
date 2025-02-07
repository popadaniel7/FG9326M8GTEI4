#include "I2cH.h"
#include "i2c.h"

__attribute__((section(".ccmram"))) uint32 I2cH_MainCounter = 0;
__attribute__((section(".ccmram"))) uint32 I2cH_Status = 0;
__attribute__((section(".ccmram"))) uint32 I2cH_ErrArr[9] = {0};
extern __attribute__((section(".ccmram"))) uint32 RevCam_I2cInitRetry;
extern __attribute__((section(".ccmram"))) uint32 RevCam_I2cStatus;
extern I2C_HandleTypeDef hi2c2;

void I2cH_MainFunction(void);
void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c);
extern void MX_I2C2_Init(void);

void I2cH_MainFunction(void)
{
	I2cH_Status = HAL_I2C_GetState(&hi2c2);
	RevCam_I2cStatus = HAL_I2C_GetError(&hi2c2);
	if(4 == RevCam_I2cStatus)
	{
		/* Retry initialization. */
		MX_I2C2_Init();
		RevCam_I2cStatus = 0;
		RevCam_I2cInitRetry++;
	}/* Error reset. */
	else for(uint8 i = 0; i < 8; i++) I2cH_ErrArr[i] = 0;
	I2cH_MainCounter++;
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
	/* Error detection. */
	switch(hi2c->ErrorCode)
	{
	case HAL_I2C_ERROR_BERR:
		I2cH_ErrArr[0]++;
		break;
	case HAL_I2C_ERROR_ARLO:
		I2cH_ErrArr[1]++;
		break;
	case HAL_I2C_ERROR_AF:
		I2cH_ErrArr[2]++;
		break;
	case HAL_I2C_ERROR_OVR:
		I2cH_ErrArr[3]++;
		break;
	case HAL_I2C_ERROR_DMA:
		I2cH_ErrArr[4]++;
		break;
	case HAL_I2C_ERROR_TIMEOUT:
		I2cH_ErrArr[5]++;
		break;
	case HAL_I2C_ERROR_SIZE:
		I2cH_ErrArr[6]++;
		break;
	case HAL_I2C_ERROR_DMA_PARAM:
		I2cH_ErrArr[7]++;
		break;
	case HAL_I2C_WRONG_START:
		I2cH_ErrArr[8]++;
		break;
	default:
		break;
	}
}
