#include "MK64F12.h"
#include "console.h"

/**
 * Set the baud rate to 9600
 * Date bits -> 7
 * Stop bits -> 1
 * Disabled Parity & Flor control
 */
void uart_init() {
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

void uart_put(char c) {
	//  Poll the TDRE (transmit data register empty)
	while(!(UART0_S1 & UART_S1_TDRE_MASK)) {}
	// Write to output
	UART0_D = c;
}

char uart_get() {
	//  Poll the RDRF (transmit data register empty)
	while(!(UART0_S1 & UART_S1_RDRF_MASK)) {}
	// Read from output
	return UART0_D;
}

void uart_print(char* string) {
	while(*string) {
		uart_put(*string);
		string++;
	}
}
