/**
 * Jacob Smith
 * Mar. 7th, 2019
 */
#include "console.h"

int main() {
	// Setup pins and registers
	uart_init();

	// Test direct putchar() Functionality
    uart_put('t');
	uart_put('e');
	uart_put('s');
	uart_put('t');
	uart_put(' ');
	
	uart_print("TEST UPPERCASE");

	// Test user input getchar() functionality
	while(1){
		// Will output any user input.
		uart_put(uart_get());
	}

    return 0;
}
