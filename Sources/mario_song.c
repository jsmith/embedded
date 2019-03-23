#include "console.h"
#include "adc.h"
#include "ftm.h"
#include "pwm.h"
#include "gpio.h"
#include "dac.h"
#include "mario.h"
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

// ADC max & min
double adc_max = 3.3;
double adc_min = 0.0;

// ADC input value.
double adc_value = 0.2;

// DAC max & min.
double dac_max = 0.05;
short dac_min = 0;

short dac_value = 0;
double dac_factor = (4095.0 / 3.3);

// 0.0025 max rotation (Open), 0.0005 min rotation (Close)
double duty_min = 0.0005;
double duty_max = 0.0025;

// Variable duty
//double period = 0.004;

// Reading formatted for display.
char formatted[MAX];

adc_to_dac() {
	adc_value = adc_read();

	if (adc_value > adc_max) {
		adc_value = adc_max;
	}
	else if (adc_value < adc_min) {
		adc_value = adc_min;
	}

	double conversion = adc_value / adc_max;

	dac_value = dac_max * conversion * dac_factor;
}

int mario() {
	// Setup pins and registers
	uart_init();
	adc_init();
	pwm_init();
	ftm_init();
	gpio_init();
	dac_init();

	int note = 0;
	double period = 0.001;
	while(1) {
		// to calculate the note duration, take one second
		// divided by the note type.
		//e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
		double noteDuration = 1.0 / MARIO_TEMPO[note];
		adc_to_dac();
		if (MARIO_MELODY[note]) {
			period = 1.0 / MARIO_MELODY[note];
		} else {
			dac_value = 0;
		}

		double time_passed = 0;
		while (time_passed < noteDuration) {


			dac_out(dac_min);
			ftm_delay(period);

			dac_out(dac_value);
			ftm_delay(period);

			time_passed += period + period;
		}

		// to distinguish the notes, set a minimum time between them.
		// the note's duration + 30% seems to work well:
		double extraDelay = noteDuration * 0.3;
		ftm_delay(extraDelay);

		note = (note + 1) % MARIO_SIZE;
	}

	return 0;
}
