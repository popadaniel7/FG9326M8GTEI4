#include "Ain_Filtering.h"
#include "Ain.h"

void Ain_Filtering_InitEvAdcModule(void);   /* Function to initialize the EVADC module with default parameters      */
void Ain_Filtering_InitEvAdcGroup(void);    /* Function to initialize the EVADC group                               */
void Ain_Filtering_InitEvAdcChannels(void); /* Function to initialize the EVADC used channels                       */
void Ain_Filtering_ApplyFiltering(void);    /* Function to apply the filters to the EVADC channels                  */

IfxEvadc_Adc g_evadc;                                     /* Global variable for configuring the EVADC module       */
IfxEvadc_Adc_Group g_evadcGroup;                          /* Global variable for configuring the EVADC group        */
IfxEvadc_Adc_Channel g_evadcChannel[CHANNELS_NUM];        /* Global array for configuring the EVADC channels        */
channel g_chn[] =
{
        {&IfxEvadc_G0CH0_AN0_IN,  IfxEvadc_ChannelResult_0},
};

void Ain_FilteringInit(void)
{
    Ain_Filtering_InitEvAdcModule();                                                  /* Initialize the EVADC module              */
    Ain_Filtering_InitEvAdcGroup();                                                   /* Initialize the EVADC group               */
    Ain_Filtering_InitEvAdcChannels();                                                /* Initialize the used channels             */
    Ain_Filtering_ApplyFiltering();                                                   /* Configure the Data Modification registers*/
    /* Start the scan */
    IfxEvadc_Adc_startQueue(&g_evadcGroup, IfxEvadc_RequestSource_queue0);
}
/* Function to initialize the EVADC module with default parameters */
void Ain_Filtering_InitEvAdcModule(void)
{
    IfxEvadc_Adc_Config adcConf;                                        /* Define a configuration structure         */
    IfxEvadc_Adc_initModuleConfig(&adcConf, &MODULE_EVADC);             /* Fill it with default values              */
    IfxEvadc_Adc_initModule(&g_evadc, &adcConf);                        /* Apply the default configuration          */
}
/* Function to initialize the EVADC group */
void Ain_Filtering_InitEvAdcGroup(void)
{
    /* Initialize the group */
    IfxEvadc_Adc_GroupConfig adcGroupConf;                              /* Define a configuration structure         */
    IfxEvadc_Adc_initGroupConfig(&adcGroupConf, &g_evadc);              /* Fill it with default values              */
    /* Enable queue 0 source */
    adcGroupConf.arbiter.requestSlotQueue0Enabled = TRUE;
    /* Enable the gate in "always" mode (no edge detection) */
    adcGroupConf.queueRequest[0].triggerConfig.gatingMode = IfxEvadc_GatingMode_always;
    /* Set the group 0 as master group */
    adcGroupConf.master = IfxEvadc_GroupId_0;
    /* Initialize the used group */
    adcGroupConf.groupId = IfxEvadc_GroupId_0;                          /* Select the group                         */
    /* Apply the configuration to the group */
    IfxEvadc_Adc_initGroup(&g_evadcGroup, &adcGroupConf);
}
/* Function to initialize the used EVADC channels */
void Ain_Filtering_InitEvAdcChannels(void)
{
    IfxEvadc_Adc_ChannelConfig adcChannelConf;                  /* Configuration structure                          */
    uint16 chnNum;

    for(chnNum = 0u; chnNum < CHANNELS_NUM; chnNum++)            /* The channels included in g_chn are initialized   */
    {
        /* Fill the configuration with default values */
        IfxEvadc_Adc_initChannelConfig(&adcChannelConf, &g_evadcGroup);
        /* Set the channel ID and the corresponding result register */
        adcChannelConf.channelId = g_chn[chnNum].analogInput->channelId;
        adcChannelConf.resultRegister = g_chn[chnNum].resultRegister;
        /* Apply the channel configuration */
        IfxEvadc_Adc_initChannel(&g_evadcChannel[chnNum], &adcChannelConf);
        /* Add channel to queue with refill option enabled */
        IfxEvadc_Adc_addToQueue(&g_evadcChannel[chnNum], IfxEvadc_RequestSource_queue0, IFXEVADC_QUEUE_REFILL);
    }
}
/* Function to apply the filters to the EVADC channels */
void Ain_Filtering_ApplyFiltering(void)
{
    /* Apply a 1st-order Infinite Impulse Response Filter (IIR) to analog inputs */
    EVADC_G0_RCR0.B.DMM = IfxEvadc_DataModificationMode_resultFilteringMode;  /* Set Data Modification Mode to Result Filtering Mode */
    EVADC_G0_RCR0.B.DRCTR = IfxEvadc_DataReductionControlMode_15;             /* Configure RCR0 for IIR filter (a=3, b=4) for AN0 */
}
