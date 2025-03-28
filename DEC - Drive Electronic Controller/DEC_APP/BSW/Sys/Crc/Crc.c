#include "Crc.h"
#include "IfxCpu.h"

fceCrc g_fceCrc2; /* Structure to store information */

void Crc_Init(void);
uint32 Crc_Calculate(uint32 *crcData, uint16 crcDataLength, uint32 crcStartValue);
/* Initialization of FCE
 * This function is called from main during initialization phase
 */
void Crc_Init(void)
{
    /* Create FCE module configuration */
    IfxFce_Crc_Config fceConfig;
    IfxFce_Crc_initModuleConfig(&fceConfig, &MODULE_FCE);
    /* Initialize module */
    IfxFce_Crc_initModule(&g_fceCrc2.fce, &fceConfig);
    /* Initialize CRC kernel with default configuration */
    IfxFce_Crc_CrcConfig crcConfig;
    IfxFce_Crc_initCrcConfig(&crcConfig, &g_fceCrc2.fce);
    /* Initialize FCE CRC */
    crcConfig.crcKernel = IfxFce_CrcKernel_0;
    crcConfig.fceChannelId = IfxDma_ChannelId_1;
    IfxFce_Crc_initCrc(&g_fceCrc2.fceCrc, &crcConfig);
}

uint32 Crc_Calculate(uint32 *crcData, uint16 crcDataLength, uint32 crcStartValue)
{
    uint32 crcRet = 0u;

    IfxScuWdt_clearCpuEndinit(IfxScuWdt_getCpuWatchdogPassword());
    IfxScuWdt_clearSafetyEndinit(IfxScuWdt_getSafetyWatchdogPassword());
    crcRet =  IfxFce_Crc_calculateCrc(&g_fceCrc2.fceCrc, crcData, crcDataLength, crcStartValue);
    IfxScuWdt_setCpuEndinit(IfxScuWdt_getCpuWatchdogPassword());
    IfxScuWdt_setSafetyEndinit(IfxScuWdt_getSafetyWatchdogPassword());

    return crcRet;
}
