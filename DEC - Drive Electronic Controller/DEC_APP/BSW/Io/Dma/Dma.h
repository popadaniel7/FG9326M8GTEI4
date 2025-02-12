#include "Ifx_Types.h"
#include "IfxDma_Dma.h"
#include "Ain.h"
/* DMA Macros */
#define DMA_CHANNEL             0    /* DMA Channel used in this example (0 lowest priority - 127 highest priority) */
#define ISR_PRIORITY_DMA        20          /* Priority for interrupt after DMA transfer done                       */
/* Start address of the CPU0 Data Scratch-Pad SRAM: destination address for the DMA channel */
#define DEST_ADDR               0x70000000
/* Start address of the EVADC result register Group 0-Channel 0 (EVADC_G0RES0).
 * The Result Register addresses can be found in IfxEvadc_reg.h */
#define SRC_ADDR                0xF0020700
/* EVADC Macros */
#define GROUPID_0               IfxEvadc_GroupId_0  /* EVADC group                                                  */
#define CHANNEL_ID              0           /* EVADC Channel ID                                                     */
#define ISR_PRIORITY_ADC        DMA_CHANNEL /* Priority for interrupt after ADC conversion. Should be equal to
                                             * DMA_CHANNEL when DMA is the Interrupt Service Provider               */
#define NO_OPTIONS              0           /* Macro for adding a channel in the EVADC queue with no refill option  */

extern uint32 Dma_DestinationAdcBuffer[CHANNELS_NUM];
extern IfxDma_Dma_Channel g_DMAchn;
extern IfxDma_Dma_Config dmaConfig;
extern IfxDma_Dma_ChannelConfig dmaChnsCfg;
extern IfxDma_Dma dma;
void Dma_Init(void);
