#include "Eru.h"
#include "Irq.h"

ERUconfig g_ERUconfig;   /* ERU configuration for Encoder A */
ERUconfig g_ERUconfig2;   /* ERU configuration for Encoder B */
ERUconfig g_ERUconfig3;   /* ERU configuration */
ERUconfig g_ERUconfig4;   /* ERU configuration */
/* Global Variables for Encoder */
volatile sint32 encoderCount = 0u;
volatile sint8 direction = 0u; // 1 for forward, -1 for backward
volatile uint8 lastEncodedA = 0u;
volatile uint8 lastEncodedB = 0u;
uint32 Eru_PpsSpeedSenStat = 0u;

void Eru_Init(void);
void Eru_EncoderA(void);
void Eru_EncoderB(void);
void Eru_SpeedSen(void);

void Eru_SpeedSen(void)
{
    IfxScuEru_clearEventFlag(g_ERUconfig3.inputChannel);
    IfxSrc_clearRequest(g_ERUconfig3.src);
    Eru_PpsSpeedSenStat++;
}
/* This functions initializes the output pin for the LED and the pin which toggles the state for generating
 * the falling and rising edges which are used to trigger the interrupt.
 * Additionally this function is configuring the ERU module including the service request configuration */
void Eru_Init(void)
{
    /* Initialize pins which are used to trigger and visualize the interrupt and set the default states */
    IfxPort_setPinMode(TRIGGER_PIN, IfxPort_Mode_inputPullUp);              /* Initialize TRIGGER_PIN port pin  */
    IfxPort_setPinMode(TRIGGER_PIN2, IfxPort_Mode_inputPullUp);              /* Initialize TRIGGER_PIN port pin  */
    IfxPort_setPinMode(TRIGGER_PIN3, IfxPort_Mode_inputPullUp);              /* Initialize TRIGGER_PIN port pin  */
    /* Trigger pin */
    g_ERUconfig.reqPin = REQ_IN; /* Select external request pin */
    g_ERUconfig2.reqPin = REQ_IN2;
    g_ERUconfig3.reqPin = REQ_IN3; /* Select external request pin */
    /* Initialize this pin with pull-down enabled
     * This function will also configure the input multiplexers of the ERU (Register EXISx)*/
    IfxScuEru_initReqPin(g_ERUconfig.reqPin, IfxPort_InputMode_pullDown);
    IfxScuEru_initReqPin(g_ERUconfig2.reqPin, IfxPort_InputMode_pullDown);
    IfxScuEru_initReqPin(g_ERUconfig3.reqPin, IfxPort_InputMode_pullDown);
    /* Determine input channel depending on input pin */
    g_ERUconfig.inputChannel = (IfxScuEru_InputChannel) g_ERUconfig.reqPin->channelId;
    g_ERUconfig2.inputChannel = (IfxScuEru_InputChannel) g_ERUconfig2.reqPin->channelId;
    g_ERUconfig3.inputChannel = (IfxScuEru_InputChannel) g_ERUconfig3.reqPin->channelId;
    /* Input channel configuration */
    IfxScuEru_enableRisingEdgeDetection(g_ERUconfig.inputChannel);          /* Interrupt triggers on rising edge (Register RENx) and  */
    IfxScuEru_enableFallingEdgeDetection(g_ERUconfig.inputChannel);         /* on falling edge (Register FENx)  */
    IfxScuEru_enableRisingEdgeDetection(g_ERUconfig2.inputChannel);          /* Interrupt triggers on rising edge (Register RENx) and  */
    IfxScuEru_enableFallingEdgeDetection(g_ERUconfig2.inputChannel);         /* on falling edge (Register FENx)  */
    IfxScuEru_enableRisingEdgeDetection(g_ERUconfig3.inputChannel);          /* Interrupt triggers on rising edge (Register RENx) and  */
    IfxScuEru_enableFallingEdgeDetection(g_ERUconfig3.inputChannel);         /* on falling edge (Register FENx)  */
    /* Signal destination */
    g_ERUconfig.outputChannel = IfxScuEru_OutputChannel_0;                  /* OGU channel 0                    */
    g_ERUconfig2.outputChannel = IfxScuEru_OutputChannel_1;                  /* OGU channel 1                    */
    g_ERUconfig3.outputChannel = IfxScuEru_OutputChannel_2;                  /* OGU channel 0                    */
    /* Event from input ETL0 triggers output OGU0 (signal TRx0) */
    g_ERUconfig.triggerSelect = IfxScuEru_InputNodePointer_0;
    g_ERUconfig2.triggerSelect = IfxScuEru_InputNodePointer_1;
    g_ERUconfig3.triggerSelect = IfxScuEru_InputNodePointer_2;
    /* Connecting Matrix, Event Trigger Logic ETL block */
    /* Enable generation of trigger event (Register EIENx) */
    IfxScuEru_enableTriggerPulse(g_ERUconfig.inputChannel);
    IfxScuEru_enableTriggerPulse(g_ERUconfig2.inputChannel);
    IfxScuEru_enableTriggerPulse(g_ERUconfig3.inputChannel);
    /* Determination of output channel for trigger event (Register INPx) */
    IfxScuEru_connectTrigger(g_ERUconfig.inputChannel, g_ERUconfig.triggerSelect);
    IfxScuEru_connectTrigger(g_ERUconfig2.inputChannel, g_ERUconfig2.triggerSelect);
    IfxScuEru_connectTrigger(g_ERUconfig3.inputChannel, g_ERUconfig3.triggerSelect);
    /* Configure Output channels, OutputGating Unit OGU (Register IGPy) */
    IfxScuEru_setInterruptGatingPattern(g_ERUconfig.outputChannel, IfxScuEru_InterruptGatingPattern_alwaysActive);
    IfxScuEru_setInterruptGatingPattern(g_ERUconfig2.outputChannel, IfxScuEru_InterruptGatingPattern_alwaysActive);
    IfxScuEru_setInterruptGatingPattern(g_ERUconfig3.outputChannel, IfxScuEru_InterruptGatingPattern_alwaysActive);
    /* Service request configuration */
    /* Get source pointer depending on outputChannel (SRC_SCUERU0 for outputChannel0) */
    g_ERUconfig.src = &MODULE_SRC.SCU.SCUERU[(int) g_ERUconfig.outputChannel % 8]; // EncoderA
    g_ERUconfig2.src = &MODULE_SRC.SCU.SCUERU[(int) g_ERUconfig2.outputChannel % 8]; // EncoderB
    g_ERUconfig3.src = &MODULE_SRC.SCU.SCUERU[(int) g_ERUconfig3.outputChannel % 8]; // SpeedSen
    IfxSrc_init(g_ERUconfig.src, IfxSrc_Tos_cpu1, ISR_PRIORITY_SCUERU_INT0);
    IfxSrc_enable(g_ERUconfig.src);
    IfxSrc_init(g_ERUconfig2.src, IfxSrc_Tos_cpu1, ISR_PRIORITY_SCUERU_INT1);
    IfxSrc_enable(g_ERUconfig2.src);
    IfxSrc_init(g_ERUconfig3.src, IfxSrc_Tos_cpu2, ISR_PRIORITY_SCUERU_INT2);
    IfxSrc_enable(g_ERUconfig3.src);
//    IfxCpu_Irq_installInterruptHandler(SCUERU_Int0_Handler, ISR_PRIORITY_SCUERU_INT0);
//    IfxCpu_Irq_installInterruptHandler(SCUERU_Int1_Handler, ISR_PRIORITY_SCUERU_INT1);
//    IfxCpu_Irq_installInterruptHandler(SCUERU_Int2_Handler, ISR_PRIORITY_SCUERU_INT2);
}

void Eru_EncoderA(void)
{
    /* Clear the interrupt flag for Encoder A */
    IfxScuEru_clearEventFlag(g_ERUconfig.inputChannel);
    IfxSrc_clearRequest(g_ERUconfig.src);
    /* Read current state of Encoder A and Encoder B */
    uint8 A = IfxPort_getPinState(TRIGGER_PIN); /* Read Encoder A state */
    uint8 B = IfxPort_getPinState(TRIGGER_PIN2); /* Read Encoder B state */
    /* Encode current state */
    uint8 encoded = (A << 1) | B;
    /* Compute state transition */
    uint8 sum = (lastEncodedA << 2) | encoded;
    /* Quadrature decoding logic */
    switch(sum)
    {
        case 0x01:
        case 0x02:
        case 0x04:
        case 0x0B:
            encoderCount++;
            direction = 1; // Forward
            break;
        case 0x0E:
        case 0x07:
        case 0x03:
        case 0x08:
            encoderCount--;
            direction = -1; // Backward
            break;
        default:
            // Invalid transition, can be ignored or handled as needed
            break;
    }
    /* Update lastEncoded */
    lastEncodedA = encoded;
}

void Eru_EncoderB(void)
{
    /* Clear the interrupt flag for Encoder B */
    IfxScuEru_clearEventFlag(g_ERUconfig2.inputChannel);
    IfxSrc_clearRequest(g_ERUconfig2.src);
    /* Read current state of Encoder A and Encoder B */
    uint8 A = IfxPort_getPinState(TRIGGER_PIN); /* Read Encoder A state */
    uint8 B = IfxPort_getPinState(TRIGGER_PIN2); /* Read Encoder B state */
    /* Encode current state */
    uint8 encoded = (A << 1) | B;
    /* Compute state transition */
    uint8 sum = (lastEncodedB << 2) | encoded;
    /* Quadrature decoding logic */
    switch(sum)
    {
        case 0x01:
        case 0x02:
        case 0x04:
        case 0x0B:
            encoderCount++;
            direction = 1; // Forward
            break;
        case 0x0E:
        case 0x07:
        case 0x03:
        case 0x08:
            encoderCount--;
            direction = -1; // Backward
            break;
        default:
            // Invalid transition, can be ignored or handled as needed
            break;
    }
    /* Update lastEncoded */
    lastEncodedB = encoded;
}
