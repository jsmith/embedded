#include "MK64F12.h"

void gpio_init(){
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK;

	PORTC_PCR6 |= PORT_PCR_MUX(1); // SWITCH
	PORTB_PCR21 |= PORT_PCR_MUX(1);  // LED
	PORTB_PCR22 |= PORT_PCR_MUX(1);  // LED

	GPIOC_PDDR &= ~(1 << 6);
	GPIOB_PDDR |= 1 << 21;
	GPIOB_PDDR |= 1 << 22;
}

void gpio_go() {
	while (1) {
		int input = (GPIOC_PDIR >> 6) & 1;
		GPIOB_PSOR = input << 21;
		GPIOB_PCOR = (input ^ 1) << 21;
		GPIOB_PSOR = input << 22;
		GPIOB_PCOR = (input ^ 1) << 22;
	}
}
