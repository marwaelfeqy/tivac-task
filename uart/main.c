#include <stdint.h>
#include "uart.h"
#include "TM4C123GH6PM.h"
int main(void)
{
	
	UART_UART1_Init();
	UART_UART1_Enable_Int();
	__enable_irq();
	while (1)
	{
	}
}
	
	