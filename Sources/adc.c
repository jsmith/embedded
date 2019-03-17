#include "MK64F12.h"
#include <math.h>

double adc_read() {
	// PTE24 maps to ADC0_SE17
	// Select ADC0_SE17
	ADC0_SC1A &= 17;
	while (!(ADC0_SC1A & ADC_SC1_COCO_MASK)) {}
	return ((double) ADC0_RA) * 3.3 / pow(2, 10);
}

void adc_init(void) {
	// Enable port E
	SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;

	// Enable clock to ADC0
	SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK;

	// 01 input_clock / 2
	// 0 short time sample
	// 10 since DIFF == 0
	// 00 clock_source = bus clock
	ADC0_CFG1 |= 0b0101000;
}
