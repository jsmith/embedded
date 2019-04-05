/**
 * The interrupt module
 *
 * Inputs: None
 * Outputs: None
 * Authors: Mike Walz & Jacob Smith
 */
#include "MK64F12.h"

#define CLOCK_FREQUENCY 20960000

void FTM2_init(double delay) {
	/*
	 * The two important functions here are:
	 * NVIC_DisableIRQ( IRQn ) and NVIC_EnableIRQ( IRQn )
	 *
	 * Disable will disable any interrupts for that function if they are
	 * enabled.
	 * Enabled will enable any interrupts for that function if the apropriate
	 * flags have been set.
	 */
	NVIC_DisableIRQ( FTM2_IRQn ); // Disable interrupts for FTM2 if they are currently enabled

	// NO CLOCK IS SELECTED FOR FTM2_SC_CLKS SO THE WAVE WILL NOT START UNTIL
	// THE buzz() FUNCTION IS CALLED UPON

	SIM_SCGC6 |= SIM_SCGC6_FTM2_MASK; // Enable the FTM2 Clock

	FTM2_MODE |= FTM_MODE_WPDIS_MASK; // Turn Write Protection Off

	FTM2_SC |= (0b000 << 0); // selects pre-scale factor of 1 (default)

	/*
	 * This is where we tell the FTM2 module that we want the TOF flag to
	 * generate an interrupt
	 */
	FTM2_SC |= (0b1 << 6 ); // Enable timer overflow interrupt

	FTM2_CNTIN = 0x00; // Initial value of counter = 0

	/*
	 * The TOF flag will be set whenever the FTM2_CNT register reaches the MOD value
	 * that means that using the system clock (20.96 Mhz) with a scaling factor of
	 * 1 this will be triggered 3.13ms, for greater granularity decrease the MOD value
	 *
	 * Called every 1 / 20.96mhz * 0xFFFF = 3.12666985 ms
	 */
	FTM2_MOD = 0xFFFF & ((int)(delay * CLOCK_FREQUENCY));

	/*
	 * Set MSB=1 and ELSnB=1
	 * This is output compare and will toggle the channel value each time the
	 * counter reaches the value specified in FTM0_C0V (doesn't really matter
	 * in this case sense we just want to know when there is a overflow)
	 */
	FTM2_C0SC |= 0x28;

	/*
	 * This enables the interrupts, now when the TOF flag is set execution will
	 * be switched to FTM2_IRQHandler()
	 */
	NVIC_EnableIRQ( FTM2_IRQn );

	FTM2_SC |= (0b1000); // selects the system clock, the counter will now start
}
