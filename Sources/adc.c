#include "MK64F12.h"
#include <math.h>

double adc_read() {
	ADC0_SC1A &= 17;
	while (!(ADC0_SC1A & ADC_SC1_COCO_MASK)) {}
	return ((double) ADC0_RA) * 3.3 / pow(2, 10);
}

void adc_init(void) {
	SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;
	SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK;
	ADC0_CFG1 |= 0b0101000;

	while (1) {
		double result = read();
	}
}
