#include "MK64F12.h"

#define FREQ 20971520
#define MODO 0xFFFF

void ftm_init() {
	// Init the clock
	SIM_SCGC6 |= SIM_SCGC6_FTM0_MASK;

	// Disable write protection
	FTM0_MODE |= FTM_MODE_WPDIS_MASK;

	// Turn on the system clock
	FTM0_SC |= FTM_SC_CLKS(1);

	FTM0_MOD = MODO;
}

void ftm_delay(double t) {
	int n = t * FREQ;
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


