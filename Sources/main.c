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

// Reading formatted for display.
char formatted[MAX];

// NOTE:  UART disabled as causes weird jitter glitch with servo.
void check_hand_state() {
	if (value <= threshold_1_low && (p_hand_close != 0 || p_hand_open != 1)) {
		p_hand_close = 0;
		p_hand_open = 1;

		// Display that hand opening enabled
//		uart_print("Prosthetic Hand Beginning to Open: ");
//		uart_print(formatted);
//		uart_print("\n\r");
	}
	else if(value > threshold_1_high && value <= threshold_2_low && (p_hand_close != 0 || p_hand_open != 0)) {
		p_hand_close = 0;
		p_hand_open = 0;

		// Display that hand movement suspended
//		uart_print("Prosthetic Hand Movement Suspended: ");
//		uart_print(formatted);
//		uart_print("\n\r");
	}
	else if(value > threshold_2_high && (p_hand_close != 1 || p_hand_open != 0)) {
		p_hand_close = 1;
		p_hand_open = 0;

		// Display that hand closing enabled
//		uart_print("Prosthetic Hand Beginning to Close: ");
//		uart_print(formatted);
//		uart_print("\n\r");
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

	// Loop to check and sync all I/O
	while(1) {
		value = adc_read();
		dtoa(value, formatted, 2);
		dac_out(value * 1241);

		// Will output any user input.
//		uart_print(formatted);
//		uart_print("\n\r");

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


	return 0;
}
