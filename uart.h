/*****************************************************************************
​ ​* ​ ​ @file​ ​  buffer.h
​ * ​ ​ @brief​ ​ Contains the functions prototypes essential for uart.c
 *   @Tools_Used GCC
​ * ​ ​ @author​ ​Devansh Mittal, Souvik De
​ * ​ ​ @date​ ​ November 27th, 2018
​ * ​ ​ @version​ ​ 1.0
*****************************************************************************/

#include "fsl_device_registers.h"

/**
* @Brief This function initializes the UART.
*
* @Param  UART number to be initialized, and baud rate to be used.
*
* @Return void
**/
extern void UART_init(void *UARTx, uint32_t Baud);

#ifdef BLOCKING
/**
* @Brief This function transmits character through UART using blocking mechanism.
*
* @Param  Character to be transmitted.
*
* @Return void
**/
void UART_PutChar(char c);

/**
* @Brief This function receives character through UART using blocking mechanism.
*
* @Param  void
*
* @Return Character received
**/
char UART_GetChar(void);

#else
/**
* @Brief This function handles the interrupt operations for UART0.
*
* @Param  void
*
* @Return void
**/
void UART0_IRQHandler (void);

/**
* @Brief This function enables UART0.
*
* @Param interrupt number.
*
* @Return void
**/
void enable_irq (int irq);
#endif

/**
* @Brief This function writes data to be transmitted out of UART to the circular buffer.
*
* @Param pointer to character to be transmitted out of UART.
*
* @Return void
**/
void uart_write(char *p);
