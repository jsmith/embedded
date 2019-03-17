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

int gpio_check_switch() {
	return (GPIOC_PDIR >> 6) & 1;
}
