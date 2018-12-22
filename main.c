/*****************************************************************************
​ ​* ​ ​ @file​ ​  main.c
​ * ​ ​ @brief​ ​ This is the main function which calls the function to initialize
 *			 the UART and the circular buffers. It also blinks an LED and
 *			 computes the fibonacci series.
 *   @Tools_Used GCC
​ * ​ ​ @author​ ​Devansh Mittal, Souvik De
​ * ​ ​ @date​ ​ November 27th, 2018
​ * ​ ​ @version​ ​ 1.0
*****************************************************************************/

#include "fsl_device_registers.h"
#include "uart.h"
#include "buffer.h"

int main(void)
{
	/* Initialize UART */
	UART_init(UART0, 115200);

	/* Initialize circular buffer for transmitting characters to Terminal Emulator */
	inita();
	/* Initialize circular buffer for receiving characters input by user */
	initb();

	MCG_BASE_PTR->C1 = MCG_C1_IREFS_MASK | MCG_C1_IRCLKEN_MASK;
		MCG_BASE_PTR->C2 = MCG_C2_IRCS_MASK; //Select fast internal clock
		SIM_BASE_PTR->SCGC6 |= SIM_SCGC6_TPM2_MASK; //Enable TPM2 clock
		SIM_BASE_PTR->SOPT2 |= SIM_SOPT2_TPMSRC(3);

		/* Set Timing to repeat approximately 3 times a second */
		TPM2_BASE_PTR->SC = TPM_SC_CMOD(1) | TPM_SC_PS(7);
		TPM2_BASE_PTR->MOD = 5000;

		SIM_BASE_PTR->SCGC5 |= SIM_SCGC5_PORTB_MASK;

		/*  PTB18 multiplexed with channel 0 of TPM2 module */
		PORTB_BASE_PTR->PCR[18] = PORT_PCR_MUX(3);

		PTB_BASE_PTR->PDDR = 1 << 18;

		/* Channel 0 of TPM2 is configured in PWM Mode */
		TPM2_BASE_PTR->CONTROLS[0].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;
		TPM2_BASE_PTR->CONTROLS[0].CnV = TPM2_BASE_PTR->MOD / 8;

		volatile int c = 0, first = 0, second = 1;

		while(1)
		{
			 /* Compute fibonacci series only when character is received */
			 if(flag)
			 {
				 report(); //Call the function to generate the report
				 if(c<=1) //To print 0 and 1
					 fib = c;
				 else
				 {
					 fib = first + second; //Calculate the next number in the series
					 first = second;
					 second = fib;
				 }
				 c++;
				 flag = 0;
			 }
		}
    return 0;
}
