#include "MK64F12.h"
#include "console.h"
#include "adc.h"
#include "ftm.h"
#include "pwm.h"
#include "gpio.h"
#include "dac.h"
#include "converter.h"

#define MAX 50

// Threshold of open / close action.
double threshold_1_low = 1.1; // 0.7
double threshold_1_high = 1.1; // 1.3
double threshold_2_low = 2.2; // 2.0
double threshold_2_high = 2.2; // 2.6

// Control state of hand open/close.
int p_hand_close = 0;
int p_hand_open = 0;

// ADC input value.
double value = 0.0;

// 0.0025 max rotation (Open), 0.0005 min rotation (Close)
double duty_min = 0.0005;
double duty_max = 0.0025;

// Variable duty
double duty = 0.0005;

#define ADC_MAX 3.3
short DAC_VALUE = 0;

double DAC_FACTOR = (4095.0 / 3.3);
#define DAC_MAX 0.04
#define DAC_MIN 0

#define DELAY 0.005
#define CLOCK_FREQUENCY 20960000
int HIGH = 0;

void FTM2_IRQHandler( void ) {
	if (HIGH) {
		dac_out(0);
	} else {
		dac_out(DAC_VALUE);
	}
	HIGH ^= 1;

	// set the timer overflow flag to 0 to clear interrupt.
	FTM2_SC &= ~FTM_SC_TOF_MASK;
}

void FTM2_init() {
	NVIC_DisableIRQ( FTM2_IRQn ); // Disable interrupts for FTM2 if they are currently enabled
	SIM_SCGC6 |= SIM_SCGC6_FTM2_MASK; // Enable the FTM2 Clock
	FTM2_MODE |= FTM_MODE_WPDIS_MASK; // Turn Write Protection Off
	FTM2_SC |= (0b000 << 0); // selects pre-scale factor of 1 (default)
	FTM2_SC |= (0b1 << 6 ); // Enable timer overflow interrupt
	FTM2_CNTIN = 0x00; // Initial value of counter = 0
	FTM2_MOD = 0xFFFF & ((int)(DELAY * CLOCK_FREQUENCY));
	FTM2_C0SC |= 0x28;
	NVIC_EnableIRQ( FTM2_IRQn );
	FTM2_SC |= (0b1000); // selects the system clock, the counter will now start
}

double in_adc_to_dac(double value) {
	double conversion = value / ADC_MAX;
	return DAC_MAX * conversion * DAC_FACTOR;
}

// NOTE:  UART disabled as causes weird jitter glitch with servo.
void check_hand_state() {
	if (value <= threshold_1_low && (p_hand_close != 0 || p_hand_open != 1)) {
		p_hand_close = 0;
		p_hand_open = 1;
	}
	else if(value > threshold_1_high && value <= threshold_2_low && (p_hand_close != 0 || p_hand_open != 0)) {
		p_hand_close = 0;
		p_hand_open = 0;
	}
	else if(value > threshold_2_high && (p_hand_close != 1 || p_hand_open != 0)) {
		p_hand_close = 1;
		p_hand_open = 0;
	}
}

int main() {
	// Setup pins and registers
	uart_init();
	adc_init();
	pwm_init();
	ftm_init();
	gpio_init();
	dac_init();
	FTM2_init();

	while(1) {
		value = adc_read();
		DAC_VALUE = in_adc_to_dac(value);

		// dtoa(value, formatted, 2);
		// dac_out(value * 1241);
		check_hand_state();

		if(p_hand_close == 1 &&  p_hand_open == 0) {
			duty -= 0.00001;
			if (duty < duty_min) {
				duty = duty_min;
			}
		}
		else if(p_hand_close == 0 &&  p_hand_open == 1){
			duty += 0.00001;
			if (duty > duty_max) {
				duty = duty_max;
			}
		}

		pwm_clear_output();
		ftm_delay(0.02 - duty);

		pwm_set_output();
		ftm_delay(duty);
	}

	while (1);


	return 0;
}
