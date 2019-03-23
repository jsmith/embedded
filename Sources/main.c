#include "console.h"
#include "adc.h"
#include "ftm.h"
#include "pwm.h"
#include "gpio.h"
#include "dac.h"
#include "mario.h"
#include "converter.h"

#define MAX 50
#define PWM_PERIOD 0.02

// 0.0025 max rotation (Open), 0.0005 min rotation (Close)
#define DUTY_MIN 0.0005
#define DUTY_MAX 0.0025
#define DUTY_STEP 0.00001

#define TIME_STEP 0.0002

#define UART_ENABLE 0

// Threshold of open / close action.
#define THRESHOLD_1_LOW 1.1
#define THRESHOLD_1_HIGH 1.1
#define THRESHOLD_2_LOW 2.2
#define THRESHOLD_2_HIGH 2.2

// ADC max & min
#define ADC_MAX 3.3
#define ADC_MIN 0.0

#define DAC_MAX 0.05
#define DAC_MIN 0

// Amount of 1 volt in DAC
#define DAC_FACTOR (4095.0 / 3.3);

// Control state of hand open/close.
int p_hand_close = 0;
int p_hand_open = 0;


// Variable duty
// double duty = 0.0005;

// Reading formatted for display.
char formatted[MAX];

// NOTE:  UART disabled as causes weird jitter glitch with servo.
void check_hand_state(double value) {
	if (value <= THRESHOLD_1_LOW && (p_hand_close != 0 || p_hand_open != 1)) {
		p_hand_close = 0;
		p_hand_open = 1;

		// Display that hand opening enabled
		if (UART_ENABLE) {
			uart_print("Prosthetic Hand Beginning to Open: ");
			uart_print(formatted);
			uart_print("\n\r");
		}
	}
	else if(value > THRESHOLD_1_HIGH && value <= THRESHOLD_2_LOW && (p_hand_close != 0 || p_hand_open != 0)) {
		p_hand_close = 0;
		p_hand_open = 0;

		if (UART_ENABLE) {
			uart_print("Prosthetic Hand Movement Suspended: ");
			uart_print(formatted);
			uart_print("\n\r");
		}
	}
	else if(value > THRESHOLD_2_HIGH && (p_hand_close != 1 || p_hand_open != 0)) {
		p_hand_close = 1;
		p_hand_open = 0;

		if (UART_ENABLE) {
			uart_print("Prosthetic Hand Beginning to Close: ");
			uart_print(formatted);
			uart_print("\n\r");
		}
	}
}

double adc_to_dac() {
	double adc_value = adc_read();

	if (adc_value > ADC_MAX) {
		adc_value = ADC_MAX;
	}
	else if (adc_value < ADC_MIN) {
		adc_value = ADC_MIN;
	}

	double conversion = adc_value / ADC_MAX;

	return DAC_MAX * conversion * DAC_FACTOR;
}

int main() {
	// Setup pins and registers
	uart_init();
	adc_init();
	pwm_init();
	ftm_init();
	gpio_init();
	dac_init();

	// PWM
	int duty_set = 0;
	double duty_time = 0;

	// DAC Mario Song
	int note = 0;
	double note_change_time = 0;
	double half_period = 0.002; // This is just a default and will change

	// DAC
	int dac_set = 0;
	int set_dac_zero = 0;
	double dac_change_time = 0.0;

	// Variable duty
	double duty = 0.0005;

	// Time since program start (running time).
	double time = 0;
	double dac_value = 0;
	while (1) {

//		check_hand_state(dac_value);
//
//		if(p_hand_close == 1 &&  p_hand_open == 0) {
//			duty -= DUTY_STEP;
//			if (duty < DUTY_MIN) {
//				duty = DUTY_MIN;
//			}
//		} else if(p_hand_close == 0 &&  p_hand_open == 1){
//			duty += DUTY_STEP;
//			if (duty > DUTY_MAX) {
//				duty = DUTY_MAX;
//			}
//		}
//
//		// Check if time to change PWM high/low
//		if (time >= duty_time) {
//			if (duty_set) {
//				duty_time = time + duty;
//				pwm_set_output();
//				duty_set = 0;
//			} else {
//				duty_time = time + PWM_PERIOD - duty;
//				pwm_clear_output();
//				duty_set = 1;
//			}
//		}

		// Check if time to change Mario Song Note
		if (time >= note_change_time) {
			dac_value = adc_to_dac();

			if (MARIO_MELODY[note] != 0) {
				half_period = 1.0 / MARIO_MELODY[note];
				set_dac_zero = 0;
			} else {
				// If the frequency is zero, we don't want to play anything
				set_dac_zero = 1;
			}

			note_change_time += 1.0 / MARIO_BEATS[note];
			note = (note + 1) % MARIO_SIZE;
		}


		if (time >= dac_change_time) {
			if (dac_set) {
				double new_value = 0;
				if (!set_dac_zero) {
					new_value = dac_value;
				}

				dac_out(new_value);
				dac_set = 0;
			} else {
				dac_out(DAC_MIN);
				dac_set = 1;
			}

			dac_change_time += half_period;
		}


		// Increase running time.
		ftm_delay(TIME_STEP);
		time += TIME_STEP;
	}

	return 0;
}
