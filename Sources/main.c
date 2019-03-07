#include "console.h"
#include <stdlib.h>
#define MAX 50

int main() {
	// Setup pins and registers
	uart_init();
	adc_init();

	uart_print("TEST UPPERCASE");

	// Test user input getchar() functionality
	char formatted[MAX];
	while(1){
		double value = adc_read();
		snprintf(output, MAX, "%f", value);

		// Will output any user input.
		uart_put(formatted);
	}

	return 0;
}
