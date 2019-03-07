#ifndef HEADER_CONSOLE
#define HEADER_CONSOLE

void UARTx_Interface_Init();
void UARTx_Putchar(char c);
char UARTx_Getchar();
void UARTx_Putstring(char* string);

#endif