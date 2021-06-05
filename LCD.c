#include <stdlib.h> 
#include <stdio.h>
#include <stdint.h>
#include "D:/c/Keil/EE319Kware/inc/tm4c123gh6pm.h"

#include "systick.h"

void LCD_Command(unsigned char cmd){

	GPIO_PORTA_DATA_R &= ~0xE0; // Rs = 0 for command , Rw = 0 for write , E = 0 in the beginning
	GPIO_PORTB_DATA_R = cmd; 
	GPIO_PORTA_DATA_R |= 0x20; //E=1
	SysTick_Wait1ms(2);
	GPIO_PORTA_DATA_R &= ~0x20; //E=0
}	

void LCD_Init (void)		/* LCD Initialize function */
{
	SYSCTL_RCGCGPIO_R |= 0x03; //port A , B
	while((SYSCTL_PRGPIO_R & 0X03) == 0){};	
	//port B		
	GPIO_PORTB_CR_R |= 0xFF; // 1111 1111
	GPIO_PORTB_DEN_R |= 0xFF; // all digital
	GPIO_PORTB_DIR_R |= 0xFF; // all output 
	GPIO_PORTB_PCTL_R &= ~0xFFFFFFFF; //32BIT 
	GPIO_PORTB_AMSEL_R = 0; // not analog
	GPIO_PORTB_AFSEL_R = 0; //GPIO
}