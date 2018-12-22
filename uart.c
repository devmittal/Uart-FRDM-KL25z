/*****************************************************************************
​ ​* ​ ​ @file​ ​  uart.c
​ * ​ ​ @brief​ ​ This function initializes the UART and contains the blocking and
 *		     non-blocking method to communicate with the UART. It also contains
 *		     the interrupt initialization.
 *   @Tools_Used GCC
​ * ​ ​ @author​ ​Devansh Mittal, Souvik De
​ * ​ ​ @date​ ​ November 27th, 2018
​ * ​ ​ @version​ ​ 1.0
*****************************************************************************/

#include "fsl_device_registers.h"
#include "uart.h"
#include "buffer.h"

/* Structure to store port number and the mode in which it is used */
typedef struct _PORT_PIN_t
{
	PORT_Type *PORTx;
	uint8_t Bit;
	uint32_t Alt;
} PORT_PIN_t;

/* Initializes PORTA0 and PORTA1 to the 2nd alternate function - UART0 */
PORT_PIN_t UART_Port_Pin[] = {
        						{ PORTA, 1, PORT_PCR_ISF_MASK | PORT_PCR_MUX(0x2) },
								{ PORTA, 2, PORT_PCR_ISF_MASK | PORT_PCR_MUX(0x2) }
							 };

/* Initializes UART */
void UART_init(void *UARTx, uint32_t Baud)
{
	uint8_t i;
	uint32_t calculated_baud = 0;
	uint32_t baud_diff = 0;
	uint32_t osr_val = 0;
	uint32_t sbr_val, uart0clk;
	uint32_t baud_rate;
	uint32_t reg_temp = 0;
	uint32_t temp = 0;

	if ((UARTLP_Type *) UARTx == UART0)
	{
		SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;			/* Enable GPIOA clock	*/
		SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;			/* Enable UART0 clock	*/

		/* Configure the GPIOs */
		UART_Port_Pin[0].PORTx->PCR[UART_Port_Pin[0].Bit] =
		        UART_Port_Pin[0].Alt;
		UART_Port_Pin[1].PORTx->PCR[UART_Port_Pin[1].Bit] =
		        UART_Port_Pin[1].Alt;

		/* Disable the transmit and receive enable bits while configuring UART0 */
		UART0_C2 &= ~(UART0_C2_TE_MASK | UART0_C2_RE_MASK);

		((UARTLP_Type *) UARTx)->C1 = 0x00; //default
		((UARTLP_Type *) UARTx)->C2 = UARTLP_C2_RE_MASK | UARTLP_C2_TE_MASK; //Enable transmitter and reciever
		((UARTLP_Type *) UARTx)->C3 = 0x00; //defualt
		((UARTLP_Type *) UARTx)->C4 = 0x00; //default
		((UARTLP_Type *) UARTx)->C5 = 0x00; //default
		SIM->SOPT2 &= ~(SIM_SOPT2_PLLFLLSEL_MASK);	/* Clock for UART0 - MCGFLLCLK */
		SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);		/* Using MCGFLLPLL */

		 baud_rate = Baud;
		 uart0clk = SystemCoreClock;

		/* Calculate first baud rate using lowest osr value */
		i = 4;
		sbr_val = (uint32_t)(uart0clk/(baud_rate * i));
		calculated_baud = (uart0clk / (i * sbr_val));

		/* Calculate difference between calculated baud rate and desired baud rate */
		if (calculated_baud > baud_rate)
			baud_diff = calculated_baud - baud_rate;
		else
			baud_diff = baud_rate - calculated_baud;

		osr_val = i;

		/* Select the best OSR value */
		for (i = 5; i <= 32; i++)
		{
			sbr_val = (uint32_t)(uart0clk/(baud_rate * i));
			calculated_baud = (uart0clk / (i * sbr_val));

			if (calculated_baud > baud_rate)
				temp = calculated_baud - baud_rate;
			else
				temp = baud_rate - calculated_baud;

			/* Choose that osr value which has the lowest difference between calculated and desired baud rate */
			if (temp <= baud_diff)
			{
				baud_diff = temp;
				osr_val = i;
			}
		}

		if (baud_diff < ((baud_rate / 100) * 3))
		{
			/* If the OSR is between 4 and 8 then edge sampling should be turned on */
			if ((osr_val >3) && (osr_val < 9))
				UART0_C5|= UART0_C5_BOTHEDGE_MASK;

			/* Setup OSR value */
			reg_temp = UART0_C4;
			reg_temp &= ~UART0_C4_OSR_MASK;
			reg_temp |= UART0_C4_OSR(osr_val-1);

			/* Write reg_temp to C4 register */
			UART0_C4 = reg_temp;

			reg_temp = (reg_temp & UART0_C4_OSR_MASK) + 1;
			sbr_val = (uint32_t)((uart0clk)/(baud_rate * (reg_temp)));

			 /* Save off the current value of the uart0_BDH except for the SBR field */
			reg_temp = UART0_BDH & ~(UART0_BDH_SBR(0x1F));

			/* Write the calculated baud rate divisor value to the uart0_BDH and uart0_BDL register */
			UART0_BDH = reg_temp |  UART0_BDH_SBR(((sbr_val & 0x1F00) >> 8));
			UART0_BDL = (uint8_t)(sbr_val & UART0_BDL_SBR_MASK);

			/* Enable the receiver interrupt */
			UART0_C2 |= UART_C2_RIE_MASK;

			/* Enable the transmitter and receiver */
			UART0_C2 |= (UART0_C2_TE_MASK
								| UART0_C2_RE_MASK);

			/* Enable Uart0 interrupt */
			enable_irq(INT_UART0 - 16);
		}
	}
}

#ifdef BLOCKING
/* Transmitting character using blocking mechanism */
void UART_PutChar(char c)
{
	while(!(UART0_S1 & UART_S1_TDRE_MASK));  // Wait until transmit buffer empty
	UART0_D = c;
}

/* Receiving character using blocking mechanism */
char UART_GetChar(void)
{
	while(!(UART0_S1 & UART_S1_RDRF_MASK));  // Wait until receive buffer full
	return UART0_D;
}
#else

void UART0_IRQHandler (void)
{
  char c = 0;

 if ((UART0_S1 & UART_S1_TDRE_MASK) && !isempty(ringtx))
  {
	 UART0_D = remove_data(ringtx);
	 if(isempty(ringtx))
	 UART0_C2 &= ~UART_C2_TIE_MASK;
  }

 if (UART0_S1&UART_S1_RDRF_MASK && !isfull(ringrx))
 {
	 flag = 1;
	 c = UART0_D;
	 insert_data(ringrx, c);
	if(isfull(ringrx))
		remove_data(ringrx);
 }
}

/* Enabling the UART0 interrupt */
void enable_irq (int irq)
{
      /* Set the ICPR and ISER registers */
      NVIC_ICPR = 1 << (irq%32);
      NVIC_ISER = 1 << (irq%32);
}
#endif

/* Writing data to be transmitted out of UART to the circular buffer */
void uart_write(char *p)
{
	while(*p) //Loop till data is valid
	{
		while(isfull(ringtx)); //Check if the transmit buffer is full
		insert_data(ringtx, *p++); //Insert data to the transmit buffer
		UART0_C2 |= UART_C2_TIE_MASK; //Enable the transmit interrupt
	}
}
