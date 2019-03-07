#ifndef HEADER_CONSOLE
#define HEADER_CONSOLE

void uart_init();
void uart_put(char c);
char uart_get();
void uart_print(char* string);

#endif