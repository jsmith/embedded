#include "fsl_device_registers.h"

#define DUTY_MIN 0.001
#define DUTY_MIN 0.001
#define PWM_PERIOD 0.02
#define DUTY_MAX 0.002
double DUTY = DUTY_MIN;
double PWM_DELAY = 0.002; // the default will be overwritten
int CLOCK_FREQUENCY = 20960000 / 8;

void FTM0CenterAlign() {
	SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK; // enable clock for PTA
	SIM_SCGC6 |= SIM_SCGC6_FTM0_MASK; // enable clock for FTM0

	//Edit registers when no clock is fed to timer so the MOD value, gets pushed
	// in immediately
	FTM0_SC = 0; // Make sure its Off!

	// CH3
	// PTA0 ALT3
	// FTM0_CH5
	PORTA_PCR0 |= PORT_PCR_MUX(3); // FTM0 CH5
	GPIOA_PDDR |= (1 << 0); // set as output

	FTM0_MODE |= FTM_MODE_WPDIS_MASK; // disable write protection
	FTM0_MODE |= FTM_MODE_FTMEN_MASK; // FTM0 enable

	FTM0_CNT = 0; // counter initialization
	FTM0_MOD = PWM_PERIOD * CLOCK_FREQUENCY;
	FTM0_CNTIN = 0;

	// edge aligned PWM MSB = 1
	FTM0_C5SC = 0x28;

	FTM0_SYNCONF|=0x80; //enable enhanced PWM synchronization

	// when the CNT reaches this value -> FTM does what CnSC tells (clear)
	FTM0_C5V = DUTY_MAX * CLOCK_FREQUENCY;

	FTM0_CONF |= FTM_CONF_BDMMODE(5); // run in debug mode

	FTM0_SC |= 0b1011; // select system clock and divide by 8
	// FTM0_SC |= FTM_SC_CLKS(0x01); // Select the system clock => turn on the FTM

}

int mmm() {
//	FTM0CenterAlign();

	SIM_SCGC6|=0x03000000; //enable FTM0 and FTM0 module clock
	FTM0_CONF=0xC0; //set up BDM in 11
	FTM0_FMS=0x00; //clear the WPEN so that WPDIS is set in FTM0_MODE register
	FTM0_MODE|=0x05; //enable write the FTM CnV register
	FTM0_MOD=1000;
	FTM0_C0SC=0x28; //edge-alignment, PWM initial state is High, becomes low //after

	FTM0_C1SC=0x28;
	FTM0_COMBINE=0x02; //complementary mode for CH0&CH1 of FTM0
	FTM0_COMBINE|=0x10; //dead timer insertion enabled in complementary mode for //CH0&CH1 of

	FTM0_C1V=500;
	FTM0_C0V=500;
	FTM0_C2SC=0x28;
	FTM0_C3SC=0x28;
	FTM0_COMBINE|=0x0200;
	FTM0_COMBINE|=0x1000;
	FTM0_DEADTIME=0x00;
	FTM0_C3V=250;
	FTM0_C2V=250;
	FTM0_CNTIN=0x00;
	FTM0_SC=0x08; //PWM edge_alignment, system clock driving, dividing by 1

	return 0;
}
