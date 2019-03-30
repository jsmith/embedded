#include "console.h"
#include "adc.h"
#include "converter.h"
#define MAX 50


int main_adc() {
	// Setup pins and registers
	uart_init();
	adc_init();

	char formatted[MAX];
	while (1) {
		double value = adc_read();
		dtoa(value, formatted, 2);

		// Will output any user input.
		uart_print(formatted);
		uart_print("\n\r");
	}

	return 0;
}
