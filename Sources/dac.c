#include "MK64F12.h"
#include "ftm.h"
#include "gpio.h"

// USE DAC0

void dac_init() {
	SIM_SCGC2 |= SIM_SCGC2_DAC0_MASK;
	DAC0_C0 |= 0xC0;
}

void dac_set_output() {
	GPIOE_PSOR = 1 << 25;
}

void dac_clear_output() {
	GPIOE_PCOR = 1 << 25;
}

void dac_out(short voltage) {
	DAC0_DAT0L = voltage & 0x0FF;
	DAC0_DAT0H = (voltage & 0x0F00) >> 8;
}
