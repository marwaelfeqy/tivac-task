
#define NVIC_EN0_R                  *((volatile unsigned long*)0xE000E100UL)

void UART_UART1_Init(void);
void UART_UART1_WriteChar(char c);
char UART_UART1_ReadChar(void);
void UART_UART1_Enable_Int(void);
void UART1_Handler(void);