/**
 * The DAC Module. Initializes the DAC and provides method to write a voltage.
 *
 * Modules: DAC
 * Inputs: None
 * Outputs: PTE25
 *
 * Authors: Mike Walz & Jacob Smith
 */
#include "MK64F12.h"
#include <math.h>

#define REFERENCE 3.3
#define BITS 12

double DAC_FACTOR = 1;

/**
 * Initializes the DAC.
 */
void dac_init() {
	// Init the DAC FACTOR to the correct value
	DAC_FACTOR = ((double)pow(2, BITS)) / 3.3;
	SIM_SCGC2 |= SIM_SCGC2_DAC0_MASK;
	DAC0_C0 |= 0xC0;
}

/**
 * Writes a voltage to DAC0 (PTE25).
 * 
 * @param voltage The voltage to write. MUst be < 0 and > 3.3.
 */
void dac_out(double voltage) {
	short value = voltage * DAC_FACTOR;
	DAC0_DAT0L = value & 0x0FF;
	DAC0_DAT0H = (value & 0x0F00) >> 8;
}
