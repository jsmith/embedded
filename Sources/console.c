#include "fsl_device_registers.h"
#include "console.h";

/**
 * Set the baud rate to 9600
 * Date bits -> 7
 * Stop bits -> 1
 * Disabled Parity & Flor control
 */
void UARTx_Interface_Init() {
	// Enable the clock module
	SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;

	// Enable port E and B
	SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK | SIM_SCGC5_PORTB_MASK;

	// Connect the UART0 to the PORTE bits PC4 -TX, PC5 â€“RX
	PORTB_PCR17 |= PORT_PCR_MUX(3); // PB17 TX
	PORTB_PCR16 |= PORT_PCR_MUX(3); // PB16 RX

	// Disable transmit enable and receive enable
	UART0_C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK);

	// Configure UART3 for 8 bits, parity disabled
	UART0_C1 = 0b00;

	// Set the baud rate to 9600
	// Must be 1/16 the clock rate
	UART0_BDH = 0;
	UART0_BDL = 0x88;

	// Enable transmit and receive
	UART0_C2 |= UART_C2_TE_MASK;
	UART0_C2 |= UART_C2_RE_MASK;
}

void UARTx_Putchar(char c) {
	//  Poll the TDRE (transmit data register empty)
	while(!((UART0_S1 >> 7) & 1)) {}
	// Write to output
	UART0_D = c;
}

char UARTx_Getchar() {
	//  Poll the RDRF (transmit data register empty)
	while(!((UART0_S1 >> 5) & 1)) {}
	// Read from output
	return UART0_D;
}

void UARTx_Putstring(char* string) {
	while(*string) {
		UARTx_Putchar(*string);

		string++;
	}
}