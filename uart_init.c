#include "stdint.h"
#include <stdlib.h>
#include "C:\Keil\TExaSware\tm4c123gh6pm.h"

// you have to write the initialization of this function because startup file need this function to build the project without any error..
void SystemInit() {};


/**************Delay Function ***********/
void Delay(unsigned long counter)
{
	unsigned long i = 0;
	for (i = 0; i < counter; i++)
	{
		/*busy wait*/
	};
}
/****************************************/


/************** Send Data to Data Register of UART5**********/
void UART0_Transmitter(char data)
{
	while ((UART0_FR_R & 0x20) != 0); // busy wait	
	UART0_DR_R = data;
}
/***********************************************************/


/************* Consume Data from Data Register of UART5 and store it dierctly to Data Register of UART2 **************/
char UART0_Receiver(void)
{
	while ((UART0_FR_R & 0x10) != 0); // busy wait	
	return ((char)(UART0_DR_R & 0xFF));
}
/**********************************************************************************************************************/

/****************************UART INITIALIZATION********************************/
void uart_initialization(void)
{
	/************* CLOCK ENABLE ***********************/
	/*enalbe clock for UART0*/
	SYSCTL_RCGCUART_R |= 0x01;
	/*enable clock for GPIOPORTA which used by UART0*/
	SYSCTL_RCGCGPIO_R |= 0x01;
	/**************************************************/

	Delay(1);

	/************* CLOCK ENABLE ***********************/
	/* enalbe clock for UART2 */
	SYSCTL_RCGCUART_R |= 0x04;
	/*enable clock for GPIOPORTD which used by UART2*/
	SYSCTL_RCGCGPIO_R |= 0x08;
	/*************************************************/

	Delay(1);


	/************* HANDLING UART0 REGISTERS **********************************************************************************/
	/*disable UART0 module*/
	UART0_CTL_R = 0;
	/*set the buad rate*/
	UART0_IBRD_R = 104;
	UART0_FBRD_R = 11;
	/* No parity - FIFO enabled - data length=8 bits */
	UART0_LCRH_R = 0x70;
	// finally enable the uart pins 4 and 5 and also set the UARTEN bit of this register, see CTL register of uart module..
	UART0_CTL_R = 0x301;
	/***************************************************************************************************************************/

	/************* HANDLING UART2 REGISTERS **********************************************************************************/
	UART2_CTL_R = 0;
	/*set the buad rate*/
	UART2_IBRD_R = 104;
	UART2_FBRD_R = 11;
	/* No parity - FIFO enabled - data length=8 bits */
	UART2_LCRH_R = 0x70;
	// finally enable the uart pins 4 and 5 and also set the UARTEN bit of this register, see CTL register of uart module..
	UART2_CTL_R = 0x301;
	/*************************************************************************************************************************/

	/************* CONFIGURE PORTA REGISTERS ***********************/
	/*set PA0 and PA1 as digital I/O*/
	GPIO_PORTE_DEN_R = 0x03;
	/*use PA0 and PA1 as UART, so handle their AFSEL and PCTL*/
	GPIO_PORTE_AFSEL_R = 0x03;
	GPIO_PORTE_PCTL_R = 0x11000000;
	/*set analog functionality by 0*/
	GPIO_PORTE_AMSEL_R = 0;
	/***************************************************************/

	/************* CONFIGURE PORTD REGISTERS ***********************/
	/*set PD6 and PD7 as digital I/O*/
	GPIO_PORTD_DEN_R = 0xC0;
	/*use PD6 and PD7 as UART, so handle their AFSEL and PCTL*/
	GPIO_PORTD_AFSEL_R = 0xC0;
	GPIO_PORTD_PCTL_R = 0x11000000;
	/*set analog functionality by 0*/
	GPIO_PORTD_AMSEL_R = 0;
	/***************************************************************/
}
/******************************************************************************/
