#include <stdlib.h> 
#include <stdio.h>
#include <stdint.h>
#include "D:/c/Keil/EE319Kware/inc/tm4c123gh6pm.h"

#define CLEAR_DISPLAY  ((unsigned char) 0X01 )
#define  DISPLAY_ON_CURSOR_OFF ((unsigned char) 0X0C )
#define FUNC_SET_8BIT_2LINE ((unsigned char) 0X038)
#define DISPLAY_ON_CURSOR_ON ((unsigned char) 0X0E )
#define SHIFT_CURSOR_RIGHT ((unsigned char) 0X06 )

//#include"lcd.h"
#include "systick.h"

void LCD_Command(unsigned char cmd) {

	GPIO_PORTA_DATA_R &= ~0xE0; // Rs = 0 for command , Rw = 0 for write , E = 0 in the beginning
	GPIO_PORTB_DATA_R = cmd;
	GPIO_PORTA_DATA_R |= 0x20; //E=1
	SysTick_Wait1ms(2);
	GPIO_PORTA_DATA_R &= ~0x20; //E=0
}


// port B --> data (8 bit) from lcd (output) , port A (E,Rs,Rw) (output) (5,6,7) 
void LCD_Init(void)		/* LCD Initialize function */
{
	SYSCTL_RCGCGPIO_R |= 0x03; //port A , B
	while ((SYSCTL_PRGPIO_R & 0X03) == 0) {};
	//port B		
	GPIO_PORTB_CR_R |= 0xFF; // 1111 1111
	GPIO_PORTB_DEN_R |= 0xFF; // all digital
	GPIO_PORTB_DIR_R |= 0xFF; // all output 
	GPIO_PORTB_PCTL_R &= ~0xFFFFFFFF; //32BIT 
	GPIO_PORTB_AMSEL_R = 0; // not analog
	GPIO_PORTB_AFSEL_R = 0; //GPIO

	//port A pin(5,6,7) output and digital
	GPIO_PORTA_CR_R |= 0xE0; // 1110 0000
	GPIO_PORTA_DEN_R |= 0xE0; // 1110 0000 digital
	GPIO_PORTA_DIR_R |= 0xE0; // output 
	GPIO_PORTA_PCTL_R &= ~0xFFF00000; //32BIT 
	GPIO_PORTA_AMSEL_R &= ~0xE0; // not analog
	GPIO_PORTA_AFSEL_R &= ~0xE0; //GPIO	6

	//commands
	SysTick_Wait1ms(50);		/* LCD Power ON Initialization time >15ms */
	LCD_Command(FUNC_SET_8BIT_2LINE);	/* Initialization of 16X2 LCD in 8bit mode */
	SysTick_Wait1ms(1);
	LCD_Command(DISPLAY_ON_CURSOR_ON);	/* Display ON Cursor ON */
	SysTick_Wait1ms(1);
	LCD_Command(CLEAR_DISPLAY);	/* Clear display */
	SysTick_Wait1ms(2);
	LCD_Command(SHIFT_CURSOR_RIGHT);	/* Auto Increment cursor */
	SysTick_Wait1ms(2);
	//LCD_Command (0x80);	/* Cursor at home position */
	//LCD_Command (0x80);	/* Cursor at home position */
}

void LCD_Data(unsigned char data) {
	//port A (E,Rs,Rw) (output) (5,6,7)
	GPIO_PORTA_DATA_R &= ~0xE0; // Rw = 0 for write , E = 0 in the beginning //000
	GPIO_PORTA_DATA_R |= 0x40; // Rs = 1 for data //010
	GPIO_PORTB_DATA_R = data;
	GPIO_PORTA_DATA_R |= 0x20; //E=1
	SysTick_Wait1ms(2);
	GPIO_PORTA_DATA_R &= ~0x20; //E=0
}

void LCD_printString(char* str) {
	int i = 0;
	LCD_Command(DISPLAY_ON_CURSOR_OFF);

	while (str[i] != '\0')
	{
		LCD_Data(str[i]);
		i++;
	}
}
