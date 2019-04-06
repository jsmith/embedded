/**
 * The ADC Module. Initializes the ADC and reads in a value from the ADC.
 *
 * Modules: ADC
 * Inputs: PTE24
 * Outputs: None
 *
 * Authors: Jacob Smith & Mike Walz
 */

#include "MK64F12.h"
#include <math.h>

#define BITS 10
#define REFERENCE 3.3

/**
 * Reads in and converts a value from ADC (PTE24).e
 */
double adc_read() {
	// PTE24 maps to ADC0_SE17
	// Select ADC0_SE17
	ADC0_SC1A &= 17;
	while (!(ADC0_SC1A & ADC_SC1_COCO_MASK)) {}
	return ((double) ADC0_RA) * REFERENCE / pow(2, BITS);
}

/**
 * Initializes the ADC to 10 bit mode.
 */
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
