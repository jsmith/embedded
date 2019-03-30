/*
 * pwm.c
 *
 *  Created on: 11 Nov 2011
 *      Author: Charles
 */
#include "derivative.h" /* include peripheral declarations */
#include "drivers.h"
#include "pwm.h"
#include "Clock.h"

#define FTM0_CLK_PRESCALE 3
#define FTM0_OVERFLOW_FREQUENCY (32000)

void InitPWM(void)
{
 
    //Enable the Clock to the FTM0 Module
    SIM_SCGC6 |= SIM_SCGC6_FTM0_MASK;  
    
    //Pin control Register (MUX allowing user to route the desired signal to the pin.  
    PORTC_PCR1   = PORT_PCR_MUX(0x4);  
    PORTC_PCR2   = PORT_PCR_MUX(0x4);  
    PORTC_PCR3   = PORT_PCR_MUX(0x4);  
    PORTC_PCR4   = PORT_PCR_MUX(0x4);  
    PORTD_PCR4   = PORT_PCR_MUX(0x4);  
    PORTD_PCR5   = PORT_PCR_MUX(0x4);
    
    //FTM0_MODE[WPDIS] = 1; //Disable Write Protection - enables changes to QUADEN, DECAPEN, etc.  
    FTM0_MODE |= FTM_MODE_WPDIS_MASK;
    
    //FTMEN is bit 0, need to set to zero so DECAPEN can be set to 0
    FTM0_MODE &=  ~FTM_MODE_FTMEN_MASK;
    
    //Set Edge Aligned PWM
    FTM0_QDCTRL &=~FTM_QDCTRL_QUADEN_MASK;  
    //QUADEN is Bit 1, Set Quadrature Decoder Mode (QUADEN) Enable to 0,   (disabled)
    
    // Also need to setup the FTM0C0SC channel control register
    FTM0_CNT = 0x0; //FTM Counter Value - reset counter to zero
    FTM0_MOD = (PERIPHERAL_BUS_CLOCK/(1<<FTM0_CLK_PRESCALE))/FTM0_OVERFLOW_FREQUENCY ;  //Set the overflow rate
    FTM0_CNTIN = 0; //Set the Counter Initial Value to 0
    
    // FTMx_CnSC - contains the channel-interrupt status flag control bits
    FTM0_C0SC |= FTM_CnSC_ELSB_MASK; //Edge or level select
    FTM0_C0SC &= ~FTM_CnSC_ELSA_MASK; //Edge or level Select
    FTM0_C0SC |= FTM_CnSC_MSB_MASK; //Channel Mode select

    // FTMx_CnSC - contains the channel-interrupt status flag control bits
    FTM0_C1SC |= FTM_CnSC_ELSB_MASK; //Edge or level select
    FTM0_C1SC &= ~FTM_CnSC_ELSA_MASK; //Edge or level Select
    FTM0_C1SC |= FTM_CnSC_MSB_MASK; //Channel Mode select

    // FTMx_CnSC - contains the channel-interrupt status flag control bits
    FTM0_C2SC |= FTM_CnSC_ELSB_MASK; //Edge or level select
    FTM0_C2SC &= ~FTM_CnSC_ELSA_MASK; //Edge or level Select
    FTM0_C2SC |= FTM_CnSC_MSB_MASK; //Channel Mode select

    // FTMx_CnSC - contains the channel-interrupt status flag control bits
    FTM0_C3SC |= FTM_CnSC_ELSB_MASK; //Edge or level select
    FTM0_C3SC &= ~FTM_CnSC_ELSA_MASK; //Edge or level Select
    FTM0_C3SC |= FTM_CnSC_MSB_MASK; //Channel Mode select
    
    // FTMx_CnSC - contains the channel-interrupt status flag control bits
    FTM0_C4SC |= FTM_CnSC_ELSB_MASK; //Edge or level select
    FTM0_C4SC &= ~FTM_CnSC_ELSA_MASK; //Edge or level Select
    FTM0_C4SC |= FTM_CnSC_MSB_MASK; //Channel Mode select

    // FTMx_CnSC - contains the channel-interrupt status flag control bits
    FTM0_C5SC |= FTM_CnSC_ELSB_MASK; //Edge or level select
    FTM0_C5SC &= ~FTM_CnSC_ELSA_MASK; //Edge or level Select
    FTM0_C5SC |= FTM_CnSC_MSB_MASK; //Channel Mode select

    //Edit registers when no clock is fed to timer so the MOD value, gets pushed in immediately
    FTM0_SC = 0; //Make sure its Off!
    

    //FTMx_CnV contains the captured FTM counter value, this value determines the pulse width
    FTM0_C0V = FTM0_MOD/2;
    FTM0_C1V = FTM0_MOD/2;
    FTM0_C2V = FTM0_MOD/2;
    FTM0_C3V = FTM0_MOD/2;
    FTM0_C4V = FTM0_MOD/2;
    FTM0_C5V = FTM0_MOD/2;
    FTM0_COMBINE = FTM_COMBINE_COMBINE1_MASK;
    FTM0_COMBINE = FTM_COMBINE_COMBINE0_MASK|FTM_COMBINE_COMP0_MASK|/*FTM_COMBINE_DECAPEN0_MASK|FTM_COMBINE_DECAP0_MASK|*/
               FTM_COMBINE_DTEN0_MASK|FTM_COMBINE_SYNCEN0_MASK|
               FTM_COMBINE_COMBINE1_MASK|FTM_COMBINE_COMP1_MASK|/*FTM_COMBINE_DECAPEN0_MASK|FTM_COMBINE_DECAP0_MASK|*/
               FTM_COMBINE_DTEN1_MASK|FTM_COMBINE_SYNCEN1_MASK|
               FTM_COMBINE_COMBINE2_MASK|FTM_COMBINE_COMP2_MASK|/*FTM_COMBINE_DECAPEN0_MASK|FTM_COMBINE_DECAP0_MASK|*/
               FTM_COMBINE_DTEN2_MASK|FTM_COMBINE_SYNCEN2_MASK;
 
    FTM0_DEADTIME = FTM_DEADTIME_DTVAL(50); //About 5usec
    /* SWOCTRL Bit Fields */
    //FTM0_SWOCTRL|= FTM_SWOCTRL_CH0OC_MASK|FTM_SWOCTRL_CH1OC_MASK|FTM_SWOCTRL_CH2OC_MASK|FTM_SWOCTRL_CH3OC_MASK|FTM_SWOCTRL_CH4OC_MASK|FTM_SWOCTRL_CH5OC_MASK;


    //Status and Control bits
    FTM0_SC =  FTM_SC_CLKS(1); // Selects Clock source to be "system clock" or (01)
    //sets pre-scale value see details below
    FTM0_SC |= FTM_SC_PS(FTM0_CLK_PRESCALE);
}

void UpdatePWM(unsigned short pwm1,unsigned short pwm2,unsigned short pwm3)
{
    unsigned mod = FTM0_MOD/2;
    FTM0_C0V = mod-pwm1;
    FTM0_C1V = mod+pwm1;
    FTM0_C2V = mod-pwm2;
    FTM0_C3V = mod+pwm2;
    FTM0_C4V = mod-pwm3;
    FTM0_C5V = mod+pwm3;
    FTM0_PWMLOAD |= FTM_PWMLOAD_LDOK_MASK;
}