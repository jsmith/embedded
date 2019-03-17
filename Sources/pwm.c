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

void pwm_write() {
	//0.0025 max rotation, 0.0005 min rotation
	double duty = 0.0025;

	while(1) {
		if(gpio_check_switch() == 1) {
			duty = 0.0025;
		}
		else {
			duty = 0.0005;
		}

		pwm_clear_output();
		ftm_delay(0.02 - duty);

		pwm_set_output();
		ftm_delay(duty);
	}
}

