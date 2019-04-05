/**
 * The GPIO module.
 *
 * Inputs: PTC11
 * Outputs: PTB21, PTB22, PTE26
 * Authors: Jacob Smith & Mike Walz
 */
#include "MK64F12.h"

// RED PTB22/SPI2_SOUT/FB_AD29/CMP2_OUT
// BLUE PTB21/SPI2_SCK/FB_AD30/CMP1_OUT
// GREEN PTE26/ENET_1588_CLKIN/UART4_CTS_b/RTC_CLKOUT/USB0_CLKIN

#define RED 22
#define BLUE 21
#define GREEN 26
#define BUTTON 16

void gpio_init(){
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTE_MASK;

	PORTC_PCR16 |= PORT_PCR_MUX(1); // Calibration Button
	PORTB_PCR21 |= PORT_PCR_MUX(1);  // LED BLUE
	PORTB_PCR22 |= PORT_PCR_MUX(1);  // LED RED
	PORTE_PCR26 |= PORT_PCR_MUX(1);  // LED GREN

	GPIOC_PDDR &= ~(1 << BUTTON); // INPUT
	PORTC_PCR16 |= PORT_PCR_PE_MASK; // Pull Down

	// The rest are outputs
	GPIOB_PDDR |= 1 << BLUE;
	GPIOB_PDDR |= 1 << RED;
	GPIOE_PDDR |= 1 << GREEN;
}

int read_button() {
	return (GPIOC_PDIR >> BUTTON) & 1;
}

void wait_button_click() {
	// while high
	while (read_button()) {}

	// while low
	while (!read_button()) {}
}


void red(int on) {
	if (on) {
		GPIOB_PCOR |= 1 << RED;
		blue(0);
		green(0);
	} else {
		GPIOB_PSOR |= 1 << RED;
	}
}

void blue(int on) {
	if (on) {
		red(0);
		green(0);
		GPIOB_PCOR |= 1 << BLUE;
	} else {
		GPIOB_PSOR |= 1 << BLUE;
	}
}

void green(int on) {
	if (on) {
		GPIOE_PCOR |= 1 << GREEN;
		red(0);
		blue(0);
	} else {
		GPIOE_PSOR |= 1 << GREEN;
	}
}

