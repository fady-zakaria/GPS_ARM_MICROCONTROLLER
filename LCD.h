#include <stdlib.h> 
#include <stdio.h>
#include <stdint.h>
#include "D:/c/Keil/EE319Kware/inc/tm4c123gh6pm.h"

#define CLEAR_DISPLAY  ((unsigned char) 0X01 )
#define  DISPLAY_ON_CURSOR_OFF ((unsigned char) 0X0C )
#define FUNC_SET_8BIT_2LINE ((unsigned char) 0X038)
#define DISPLAY_ON_CURSOR_ON ((unsigned char) 0X0E )
#define SHIFT_CURSOR_RIGHT ((unsigned char) 0X06 )

void LCD_Init              (void);
void LCD_Command           (unsigned char);
void LCD_Data              (unsigned char);
void LCD_printString       (char*);