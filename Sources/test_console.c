/**
 * Jacob Smith
 * Mar. 7th, 2019
 */
#include "console.h";

int main() {
	// Setup pins and registers
	UARTx_Interface_Init();

	// Test direct putchar() Functionality
    UARTx_Putchar('t');
	UARTx_Putchar('e');
	UARTx_Putchar('s');
	UARTx_Putchar('t');
	UARTx_Putchar(' ');
	
	UARTx_Putstring("TEST UPPERCASE");

	// Test user input getchar() functionality
	while(1){
		// Will output any user input.
		UARTx_Putchar(UARTx_Getchar());
	}

    return 0;
}