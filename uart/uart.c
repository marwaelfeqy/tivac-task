
#include <stdint.h>
#include "uart.h"
#include "TM4C123GH6PM.h"



void UART_UART1_Init(void)
{
	
	
	// Provide clock to UART1
	SYSCTL->RCGCUART |= 0x0002;
		
	// Enable clock to PORT B
	SYSCTL->RCGCGPIO |= 0x002;
	
	// Disable while configuring.
	UART1->CTL=0;
	// Baud rate will be configured for 9600.  Note: this assumes an 80MHz bus clock!
	// 	The High-Speed Enable feature will be disabled, so the system clock will be 
	//	divided by 16. Therefore:
	//
	//	Integer Part (16 bits) = Floor(80MHz/(16 * 9600)) = Floor(520.83333) = 520 = 0x208
	//  Fraction Part (6 bits) = Floor((0.83333 * 64) + 0.5) = Floor(53.83312) = 53 = 0x35
	//
	// TODO:  Make it possible to pass the baud rate in as a parameter.
	//
	UART1->IBRD = 0x208; 
	UART1->FBRD = 0x35;
	
	// Use the system clock for the clock source.
	UART1->CC = 0;
	
	// 8-bit, no parity, 1 stop bit, no FIFO.
	UART1->LCRH = 0x72;
	
	// Enable UART along with RX and TX.
	UART1->CTL = 0x301;
	
	// Now we enable TX5 and RX5 on PORTE pins 4 & 5.
	// U5TX == PE5
	// U5RX == PE4

	// Enable pins 4 & 5 for digital and disable for analog.
	GPIOB->DEN|= 0x30;
	GPIOB->AMSEL &= (uint32_t)(~0x0003);
	
	// Enable pins 4 & 5 for alternate function.
	GPIOB->AFSEL |= 0x03;
	
	
	// Set the pin control function to 0x1 for each pin.
	//	PE4 bits are 19:16
	//	PE5 bits are 23:20
	GPIOB->PCTL |= 0x00000011;
	
}

void UART_UART1_Enable_Int(void)
{
	UART1->IM|=0x10;
	UART1->RIS|=0x10;
	NVIC_EN0_R|=0x40;
	 
}

void UART_UART1_WriteChar(char c)
{
	// Monitor the Transmit FIFO Full flag until it clears.
	//TXFF
	while (UART1->FR & 0x20) {}
	
	// Write character to the data register.
	UART1->DR = c;	

}


char UART_UART1_ReadChar(void)
{
	// Monitor the Receive FIFO Empty flag until it clears.
	// NOTE that this is blocking.
	while (UART1->FR & 0x10) {}
		
	// Return the character in the data register.
	return (uint8_t) UART1->DR;
}


void UART1_Handler(void)
{
	char c;
	if(UART1->MIS & 0x10)
	{
		c= (uint8_t) UART1->DR;
		UART1->ICR=0x10;
		if(c>='a'&&c<='z')
		{
			UART_UART1_WriteChar(c-32);//make it capital
		}
		else
			UART_UART1_WriteChar(c+1);
		
	}
}

