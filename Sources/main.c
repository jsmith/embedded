#include "console.h"
#include "adc.h"
#include "ftm.h"
#include "pwm.h"
#include "converter.h"

#define MAX 50

// Threshold of open / close action.
double threshold_max = 2.0;
double threshold_min = 1.0;

// Control state of hand open/close.
int p_hand_close = 0;

// ADC input value.
double value = 0.0;

// Reading formatted for display.
char formatted[MAX];

void check_hand_state() {
	if (p_hand_close == 0 && value >= threshold_max) {
		p_hand_close = 1;

		// Display that hand close enabled
		uart_print("Prosthetic Hand Close Detected: ");
		uart_print(formatted);
		uart_print("\n\r");
	}
	else if (p_hand_close == 1 && value <= threshold_min) {
		p_hand_close = 0;

		// Display that hand close enabled
		uart_print("Prosthetic Hand Open Detected: ");
		uart_print(formatted);
		uart_print("\n\r");
	}
}

int main() {
	// Setup pins and registers
	uart_init();
	adc_init();
	pwm_init();

	pwm_write();

	while (1) {
		value = adc_read();
		dtoa(value, formatted, 2);

//		check_hand_state();

		// Will output any user input.
		uart_print(formatted);
		uart_print("\n\r");
	}

	return 0;
}
