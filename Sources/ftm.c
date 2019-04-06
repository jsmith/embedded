/**
 * The FTM Module. Initializes the FTM and provides a method delay for a certain amount of time.
 *
 * Modules: FTM
 * Inputs: None
 * Outputs: None
 *
 * Authors: Mike Walz & Jacob Smith
 */
#include "MK64F12.h"

#define FREQ 20971520
#define MODO 0xFFFF

/**
 * Initializes the FTM module.
 */
void ftm_init() {
	// Init the clock
	SIM_SCGC6 |= SIM_SCGC6_FTM0_MASK;

	// Disable write protection
	FTM0_MODE |= FTM_MODE_WPDIS_MASK;

	// Turn on the system clock
	FTM0_SC |= FTM_SC_CLKS(1);

	FTM0_MOD = MODO;
}

/**
 * The delay function. Takes in a time in seconds and delays for that amount of time.
 * 
 * @param seconds The amount of seconds to delay.
 */
void ftm_delay(double seconds) {
	// The amount of overflows to delay by
	int n = seconds * FREQ;
	int count = 0;

	// Reset overflow
	FTM0_SC &= ~FTM_SC_TOF_MASK;

	// Writing any value resets the timer
	// Reset count to 0
	FTM0_CNT = 0;

	while(count + FTM0_CNT < n) {
		if (FTM0_SC & FTM_SC_TOF_MASK) {
			count += MODO;
			// Writing 0 to TOF to clear bit
			FTM0_SC &= ~FTM_SC_TOF_MASK;
		}
	}
}
