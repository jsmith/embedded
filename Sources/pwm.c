#include "MK64F12.h"

void pwm_init() {
	SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;

	PORTE_PCR25 |= PORT_PCR_MUX(1); // SWITCH

	// Set PORTA 1 as output.
	GPIOE_PDDR |= 1 << 25;
}

void pwm_write() {
	GPIOE_PCOR = 1 << 25;
//	GPIOA_PCOR = (input ^ 1) << 21;
}
