#include "Gtm_Pwm.h"
#include "Irq.h"

IfxGtm_Atom_Pwm_Config g_atomConfig;                            /* Timer configuration structure                    */
IfxGtm_Atom_Pwm_Driver g_atomDriver;                            /* Timer Driver structure                           */
IfxGtm_Atom_Pwm_Config g_atomConfig2;                           /* Timer configuration structure                    */
IfxGtm_Atom_Pwm_Driver g_atomDriver2;                           /* Timer Driver structure                           */
IfxGtm_Atom_Pwm_Config g_atomConfig3;                           /* Timer configuration structure                    */
IfxGtm_Atom_Pwm_Driver g_atomDriver3;                           /* Timer Driver structure                           */
IfxGtm_Atom_Pwm_Config g_atomConfig4;                           /* Timer configuration structure                    */
IfxGtm_Atom_Pwm_Driver g_atomDriver4;                           /* Timer Driver structure                           */
IfxGtm_Atom_Pwm_Config g_atomConfig5;                           /* Timer configuration structure                    */
IfxGtm_Atom_Pwm_Driver g_atomDriver5;                           /* Timer Driver structure                           */

void Gtm_Pwm_Init(void);
void Gtm_Pwm_SetDutyCycle(uint32 dutyCycle, IfxGtm_Atom_Pwm_Config* gATOMCfg, IfxGtm_Atom_Pwm_Driver* gATOMDriver);
/* This function initializes the ATOM */
void Gtm_Pwm_Init(void)
{
    /* Set the CMU clock 0 frequency */
    // PWM MOSFET EPS MOTOR FAN
    IfxGtm_Cmu_setClkFrequency(&MODULE_GTM, IfxGtm_Cmu_Clk_0, CLK_FREQ);
    /* Enable the CMU clock 0 */
    IfxGtm_Cmu_enableClocks(&MODULE_GTM, IFXGTM_CMU_CLKEN_CLK0);
    /* Initialize and start PWM for ATOM1_0 */
    IfxGtm_Atom_Pwm_initConfig(&g_atomConfig, &MODULE_GTM);
    g_atomConfig.atom = IfxGtm_ATOM3_5N_TOUT18_P00_9_OUT.atom;
    g_atomConfig.atomChannel = IfxGtm_ATOM3_5N_TOUT18_P00_9_OUT.channel;
    g_atomConfig.period = PWM_PERIOD;
    g_atomConfig.pin.outputPin = &IfxGtm_ATOM3_5N_TOUT18_P00_9_OUT;
    g_atomConfig.synchronousUpdateEnabled = TRUE;
    IfxGtm_Atom_Pwm_init(&g_atomDriver, &g_atomConfig);
    IfxGtm_Atom_Pwm_start(&g_atomDriver, TRUE);
    /* Initialize and start PWM for ATOM3_1N */
    // EPS LEFT
    IfxGtm_Atom_Pwm_initConfig(&g_atomConfig2, &MODULE_GTM);
    g_atomConfig2.atom = IfxGtm_ATOM3_1N_TOUT14_P00_5_OUT.atom;
    g_atomConfig2.atomChannel = IfxGtm_ATOM3_1N_TOUT14_P00_5_OUT.channel;
    g_atomConfig2.period = PWM_PERIOD2;
    g_atomConfig2.pin.outputPin = &IfxGtm_ATOM3_1N_TOUT14_P00_5_OUT;
    g_atomConfig2.synchronousUpdateEnabled = TRUE;
    IfxGtm_Atom_Pwm_init(&g_atomDriver2, &g_atomConfig2);
    IfxGtm_Atom_Pwm_start(&g_atomDriver2, TRUE);
    /* Initialize and start PWM for ATOM3_1N */
    // EPS RIGHT
    IfxGtm_Atom_Pwm_initConfig(&g_atomConfig3, &MODULE_GTM);
    g_atomConfig3.atom = IfxGtm_ATOM3_2N_TOUT15_P00_6_OUT.atom;
    g_atomConfig3.atomChannel = IfxGtm_ATOM3_2N_TOUT15_P00_6_OUT.channel;
    g_atomConfig3.period = PWM_PERIOD3;
    g_atomConfig3.pin.outputPin = &IfxGtm_ATOM3_2N_TOUT15_P00_6_OUT;
    g_atomConfig3.synchronousUpdateEnabled = TRUE;
    IfxGtm_Atom_Pwm_init(&g_atomDriver3, &g_atomConfig3);
    IfxGtm_Atom_Pwm_start(&g_atomDriver3, TRUE);
    /* Initialize and start PWM for ATOM3_2N */
    //EMOT
    IfxGtm_Atom_Pwm_initConfig(&g_atomConfig4, &MODULE_GTM);
    g_atomConfig4.atom = IfxGtm_ATOM3_3N_TOUT16_P00_7_OUT.atom;
    g_atomConfig4.atomChannel = IfxGtm_ATOM3_3N_TOUT16_P00_7_OUT.channel;
    g_atomConfig4.period = PWM_PERIOD4;
    g_atomConfig4.pin.outputPin = &IfxGtm_ATOM3_3N_TOUT16_P00_7_OUT;
    g_atomConfig4.synchronousUpdateEnabled = TRUE;
    IfxGtm_Atom_Pwm_init(&g_atomDriver4, &g_atomConfig4);
    IfxGtm_Atom_Pwm_start(&g_atomDriver4, TRUE);
    /* Initialize and start PWM for ATOM1_2 (SG90) */
    // SG90
    IfxGtm_Atom_Pwm_initConfig(&g_atomConfig5, &MODULE_GTM);
    g_atomConfig5.atom = IfxGtm_ATOM0_0N_TOUT17_P00_8_OUT.atom;
    g_atomConfig5.atomChannel = IfxGtm_ATOM0_0N_TOUT17_P00_8_OUT.channel;
    g_atomConfig5.period = PWM_PERIOD5;
    g_atomConfig5.pin.outputPin = &IfxGtm_ATOM0_0N_TOUT17_P00_8_OUT;
    g_atomConfig5.synchronousUpdateEnabled = TRUE;
    IfxGtm_Atom_Pwm_init(&g_atomDriver5, &g_atomConfig5);
    IfxGtm_Atom_Pwm_start(&g_atomDriver5, TRUE);
}
/* This function sets the duty cycle of the PWM */
void Gtm_Pwm_SetDutyCycle(uint32 dutyCycle, IfxGtm_Atom_Pwm_Config* gATOMCfg, IfxGtm_Atom_Pwm_Driver* gATOMDriver)
{
    gATOMCfg->dutyCycle = dutyCycle;                 /* Set duty cycle                                           */
    IfxGtm_Atom_Pwm_init(gATOMDriver, gATOMCfg);      /* Re-initialize the PWM                                    */
}
