/**
 * The PWM module.
 *
 * Modules: FTM
 * Inputs: None
 * Outputs: PTA25
 * Authors: Jacob Smith & Mike Walz
 */
#include "MK64F12.h"
#include "ftm.h"
#include "gpio.h"

void pwm_init() {
	SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;

	PORTE_PCR25 |= PORT_PCR_MUX(1); // SWITCH

	// Set PORTA 1 as output.
	GPIOE_PDDR |= 1 << 25;
}

void pwm_set_output() {
	GPIOE_PSOR = 1 << 25;
}

void pwm_clear_output() {
	GPIOE_PCOR = 1 << 25;
}
