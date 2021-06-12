//*************************************** UART_CONNECTION_WITH_PC *******************************
#include <stdlib.h> 
#include <stdio.h>
#include <stdint.h>
#include "D:/c/Keil/EE319Kware/inc/tm4c123gh6pm.h"

#define NVIC_ST_CTRL_R (*((volatile uint32_t *)0xE000E010))
#define NVIC_ST_RELOAD_R (*((volatile uint32_t *)0xE000E014))
#define NVIC_ST_CURRENT_R (*((volatile uint32_t *)0xE000E018))

#define CPAC (*((volatile uint32_t *)0xE000ED88))

void SystemInit ()
{
	CPAC  |= 0X00F00000;
}

//*************************************** SYSTICK_TIMER(DELAY) *******************************

// this function initializes systick timer
void SysTick_Init(void){
NVIC_ST_CTRL_R = 0; // 1) disable SysTick during setup
NVIC_ST_RELOAD_R = 0x00FFFFFF; // 2) maximum reload value
NVIC_ST_CURRENT_R = 0; // 3) clear CURRENT 
NVIC_ST_CTRL_R = 0x00000005; // 4) enable SysTick with core clock
}

// this function wait for 'count' bit to be equal 1
void SysTick_Wait(uint32_t delay){
NVIC_ST_RELOAD_R = delay-1; // number of counts
NVIC_ST_CURRENT_R = 0; // any value written to CURRENT clears
while((NVIC_ST_CTRL_R&0x00010000)==0){};// wait for count flag
}

// Call this function to delay 1ms if given 1 and 2ms if given 2 and so on
void SysTick_Wait1ms(uint32_t delay){
unsigned long i;
for(i=0; i<delay; i++)
{SysTick_Wait(16000);}
}
//*******************************************************************************************

//*************************************** UART7 ***********************************************

void UART7_Transmitter(char data){
while((UART7_FR_R & 0x0020) != 0);
UART7_DR_R = data;
} 

char UART7_Receiver(void){
while((UART7_FR_R & 0x0010) != 0);	
return (char)(UART7_DR_R & 0xFF);
}

void UART7_Init(void)
{
	SYSCTL_RCGCUART_R |= 0x0080; // UART 7 --> 1000 0000
	SYSCTL_RCGCGPIO_R |= 0x0010; // Port E (0001 0000) ( Pin 0 (PE0) --> U7Rx , Pin 1 (PE1) --> U7Tx )
	SysTick_Wait1ms(100);
	
	//PORT E
	GPIO_PORTE_AFSEL_R |= 0x03; //enable alt function to PE0 ,PE1
	GPIO_PORTE_PCTL_R = (GPIO_PORTE_PCTL_R & 0xFFFFFF00)+0x00000011;
	GPIO_PORTE_DEN_R |= 0x03; //enable digital
	GPIO_PORTE_AMSEL_R &= ~0x03; //disable analog
	
	UART7_CTL_R &= ~0x80; //disable UART7 module (Reset)
	/*set the buad rate*/
	UART7_IBRD_R = 104;
	UART7_FBRD_R = 11;
	UART7_LCRH_R = 0x0070; // No parity - FIFO enabled - data length = 8 bits - one stop bit
	UART7_CC_R = 0x0; // to enable system clock to uart7
	UART7_CTL_R = 0x301; // enable RXE,TXE and UART
	
}

//*******************************************************************************************
//function to change from float to char to send it to UART7
void Data_Transmit_Float(float no)
{
//  char toprint[10];
//  sprintf(toprint, "%f", no);
//  itoa(no, toprint);
  
  int len = snprintf('\0', 0, "%f", no);
  char* transmit_no = (char *)malloc(len + 1);
  snprintf(transmit_no, len + 1, "%f", no);
  int j = 0;
  while (j < len - 3 )
  {
    UART7_Transmitter(transmit_no[j]);
    j++;
  }
  free(transmit_no);
}

//*******************************************************************************************

//function to transmit string to check the UART7
void	transmit_string(char * str)
{
	while(*str)
	{
		UART7_Transmitter(*(str++));
	}
}

//*******************************************************************************************

//*************************************** UART0 (if UART7 didn't work) ***********************************************
/*

void UART0_Transmitter(char data)
{
	while((UART0_FR_R & 0x0020) != 0);
	UART0_DR_R = data;
} 

char UART0_Receiver(void)
{
	while((UART0_FR_R & 0x0010) != 0);	
	return (char)(UART0_DR_R & 0xFF);
}

void UART0_Init(void)
{
	SYSCTL_RCGCUART_R |= (1<<0); // UART 0 --> 0000 0001
	SYSCTL_RCGCGPIO_R |= (1<<0); // Port A (0000 0001) ( Pin 0 (PA0) --> U0Rx , Pin 1 (PA1) --> U0Tx )
	SysTick_Wait1ms(100);
	
	//PORT A
	GPIO_PORTA_AFSEL_R |= 0x03; //enable alt function to PA0 ,PA1
	GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R & 0xFFFFFF00)+0x00000011;
	GPIO_PORTA_DEN_R |= 0x03; //enable digital
	GPIO_PORTA_AMSEL_R &= ~0x03; //disable analog

	UART0_CTL_R &= ~0x01; //disable UART0 module (Reset)
	//set the buad rate
	UART0_IBRD_R = 104;
	UART0_FBRD_R = 11;
	UART0_LCRH_R = 0x0070; // No parity - FIFO enabled - data length = 8 bits - one stop bit
	UART0_CC_R = 0x0; // to enable system clock to uart0
	UART0_CTL_R = 0x301; // enable RXE,TXE and UART

}

// *******************************************************************************************
//function to change from float to char to send it to UART0
void Data_Transmit_Float(float no)
{
//  char toprint[10];
//  sprintf(toprint, "%f", no);
//  itoa(no, toprint);
  
  int len = snprintf('\0', 0, "%f", no);
  char* transmit_no = (char *)malloc(len + 1);
  snprintf(transmit_no, len + 1, "%f", no);
  int j = 0;
  while (j < len - 3 )
  {
    UART0_Transmitter(transmit_no[j]);
    j++;
  }
  free(transmit_no);
}

// *******************************************************************************************

//function to transmit string to check the UART0
void	transmit_string(char * str)
{
	while(*str)
	{
		UART0_Transmitter(*(str++));
	}
}

// *******************************************************************************************

*/

//*******************************************************************************************


int main()
{
	float Coordinates_points[30];
	char check;
	int i;
	
	SysTick_Init();
	UART7_Init();
	
	//UART0_Init();
	
	
	while(1)
	{
		transmit_string("Enter :\n\r"); // To check the Connection
		check = UART7_Receiver();
		if(check=='U'){
			for(i=0;i<30;i++)
			{
				Data_Transmit_Float(Coordinates_points[i]);
				//UART7_Transmitter(Coordinates_points[i]);
				SysTick_Wait1ms(10);
			}
			break;
		}
	}

	return 0;
}	


