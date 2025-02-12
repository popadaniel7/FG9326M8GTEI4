#include "Dma.h"
#include "Irq.h"
/* DMA channel handle */
IfxDma_Dma_Channel g_DMAchn;
IfxDma_Dma_Config dmaConfig;
IfxDma_Dma_ChannelConfig dmaChnsCfg;
IfxDma_Dma dma;
/* DMA buffer */
uint32 Dma_DestinationAdcBuffer[CHANNELS_NUM];
/* Function to configure and initialize the DMA transaction (transfers, moves, addresses...) */
void Dma_Init(void)
{
    /* Create module configuration */
    IfxDma_Dma_initModuleConfig(&dmaConfig, &MODULE_DMA);
    /* Initialize module */
    IfxDma_Dma_initModule(&dma, &dmaConfig);
    /* Initial configuration for the channel */
    IfxDma_Dma_initChannelConfig(&dmaChnsCfg, &dma);
    dmaChnsCfg.transferCount = CHANNELS_NUM;                                                      /* Number of transfers per transaction                          */
    dmaChnsCfg.requestMode = IfxDma_ChannelRequestMode_completeTransactionPerRequest;           /* Complete a full transaction on request                       */
    dmaChnsCfg.moveSize = IfxDma_ChannelMoveSize_32bit;                                         /* Size of the data to be moved                                 */
    dmaChnsCfg.operationMode = IfxDma_ChannelOperationMode_continuous;                          /* DMA channel stays enabled after one request                  */
    dmaChnsCfg.hardwareRequestEnabled = TRUE;                                                   /* DMA as Interrupt Service Provider                            */
    /* Source and destination addresses */
    dmaChnsCfg.destinationAddressCircularRange = IfxDma_ChannelIncrementCircular_32;            /* Source address remains the same after transfer               */
    dmaChnsCfg.sourceCircularBufferEnabled = TRUE;                                              /* Enable the source circular buffering                         */
    dmaChnsCfg.sourceAddressCircularRange = IfxDma_ChannelIncrementCircular_32;                 /* Destination address remains the same after transfer          */
    dmaChnsCfg.destinationCircularBufferEnabled = TRUE;                                         /* Enable the destination circular buffering                    */
    /* Interrupt generated after transactions */
    dmaChnsCfg.channelInterruptEnabled = TRUE;                                                  /* Enable the interrupt generation                              */
    dmaChnsCfg.channelInterruptControl = IfxDma_ChannelInterruptControl_thresholdLimitMatch;    /* DMA triggers an interrupt once the full transaction is done  */
    dmaChnsCfg.channelInterruptTypeOfService = IfxSrc_Tos_cpu0;                                 /* CPU0 is the Interrupt Service Provider of this interrupt     */
    dmaChnsCfg.channelInterruptPriority = ISR_PRIORITY_DMA;                                     /* Assign a priority to the interrupt                           */
    /* Channel specific configurations */
    dmaChnsCfg.channelId = (IfxDma_ChannelId) DMA_CHANNEL;                                      /* Select the Channel 1                                         */
    dmaChnsCfg.sourceAddress = SRC_ADDR;                                                        /* Address of EVADC Group 0 Result Register 0                   */
    dmaChnsCfg.sourceAddressIncrementStep = sizeof(uint32);
    dmaChnsCfg.destinationAddress = (uint32)&Dma_DestinationAdcBuffer[0];                               /* Start address of DSPR0                                       */
    dmaChnsCfg.destinationAddressIncrementStep = sizeof(uint32);
    /* Initialize the DMA channel */
    IfxDma_Dma_initChannel(&g_DMAchn, &dmaChnsCfg);
    //IfxCpu_Irq_installInterruptHandler(ISR_DMA1_Transfer, IRQ_ISR_PRIORITY_DMA);
}
