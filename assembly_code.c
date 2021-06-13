#include <stdint.h>
#include <math.h>
#include <string.h>
#include <stdio.h> // printf, scanf, NULL
#include <stdlib.h> // malloc, free, rand
#include "tm4c123gh6pm.h"



#define pi 3.14159265358979323846
#define R 6371e3
#define CPAC (*((volatile uint32_t *)0xE000ED88))

void SystemInit ()
{
	CPAC  |= 0X00F00000;
}

float cal_distance=0;
char GPSValues[100],parseValue[12][20],*token;
float latitude=0.0,longitude=0.0,seconds=0.0,minutes=0.0,degrees=0.0;
float l1 =0.0, lg1=0.0, l2 =0.0, lg2=0.0 ; 
const char comma[2] = ",";
char array[200];
char z=0;

////////gloal variable to count no of writes in eeprom////////////
int count_eeprom=0;
float array_of_eeprom[450];
////////////////////////////////////////////////////////////////


////////////////simple delay function////////////////////////////
void Delay(unsigned long counter)
{
unsigned long i = 0;
for(i=0; i< counter; i++);
}


void SysTick_Init(void){
NVIC_ST_CTRL_R = 0; // 1) disable SysTick during setup
NVIC_ST_RELOAD_R = 0x00FFFFFF; // 2) maximum reload value
NVIC_ST_CURRENT_R = 0; // 3) any write to CURRENT clears it
NVIC_ST_CTRL_R = 0x00000005; // 4) enable SysTick with core clock
}



// The delay parameter is in units of the 80 MHz core clock(12.5 ns)
void SysTick_Wait(uint32_t delay){
NVIC_ST_RELOAD_R = delay-1; // number of counts
NVIC_ST_CURRENT_R = 0; // any value written to CURRENT clears
while((NVIC_ST_CTRL_R&0x00010000)==0){};// wait for flag
}



// Call this routine to wait for delay*10ms
void SysTick_Wait1ms(uint32_t delay){
unsigned long i;
for(i=0; i<delay; i++)
{
SysTick_Wait(16000); // wait 10ms
}
}

void led_init(void)
{
    SYSCTL_RCGCGPIO_R |= 0x20; 
    while ((SYSCTL_PRGPIO_R & 0x20 ) == 0) ;
    //GPIO_PORTF_LOCK_R = 0x4C4F434B;///////////// REMOVE ON TIVA
    //GPIO_PORTF_CR_R = 0x1F;        //////////////REMOVE ON TIVA
    GPIO_PORTF_AFSEL_R = 0x00;
    //GPIO_PORTF_PCTL_R = 0;
    GPIO_PORTF_AMSEL_R = 0x00;
    GPIO_PORTF_DIR_R = 0x0E;
    GPIO_PORTF_DEN_R = 0x1F;
    GPIO_PORTF_PUR_R = 0x10;  // PF4 switch1
			
}


void LCD_Command(unsigned char cmd){



GPIO_PORTA_DATA_R &= ~0xE0; // Rs = 0 for command , Rw = 0 for write , E = 0 in the beginning
GPIO_PORTB_DATA_R = cmd;
GPIO_PORTA_DATA_R |= 0x20; //E=1
SysTick_Wait1ms(2);
GPIO_PORTA_DATA_R &= ~0x20; //E=0
}

// port B --> data (8 bit) from lcd (output) , port A (E,Rs,Rw) (output) (5,6,7)
void LCD_Init (void) /* LCD Initialize function */
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

//port A pin(5,6,7) output and digital
GPIO_PORTA_CR_R |= 0xE0; // 1110 0000
GPIO_PORTA_DEN_R |= 0xE0; // 1110 0000 digital
GPIO_PORTA_DIR_R |= 0xE0; // output
GPIO_PORTA_PCTL_R &= ~0xFFF00000; //32BIT
GPIO_PORTA_AMSEL_R &= ~0xE0; // not analog
GPIO_PORTA_AFSEL_R &= ~0xE0; //GPIO 6

//commands
SysTick_Wait1ms(50); /* LCD Power ON Initialization time >15ms */
LCD_Command (0x38); /* Initialization of 16X2 LCD in 8bit mode */
SysTick_Wait1ms(1);
LCD_Command (0x0E); /* Display ON Cursor OFF */
SysTick_Wait1ms(1);
LCD_Command (0x01); /* Clear display */
SysTick_Wait1ms(2);
LCD_Command (0x06); /* Auto Increment cursor */
SysTick_Wait1ms(2);
//LCD_Command (0x80); /* Cursor at home position */
}



void LCD_Data (unsigned char data){
//port A (E,Rs,Rw) (output) (5,6,7)
GPIO_PORTA_DATA_R &= ~0xE0; // Rw = 0 for write , E = 0 in the beginning //000
GPIO_PORTA_DATA_R |= 0x40; // Rs = 1 for data //010
GPIO_PORTB_DATA_R = data;
GPIO_PORTA_DATA_R |= 0x20; //E=1
SysTick_Wait1ms(2);
GPIO_PORTA_DATA_R &= ~0x20; //E=0
}
void uart_initialization(void){
/************* CLOCK ENABLE ***********************/
/*enalbe clock for UART5*/
SYSCTL_RCGCUART_R |=SYSCTL_RCGCUART_R5;
/*enable clock for GPIOPORTE which used by UART5*/
SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R4;
/**************************************************/



Delay(1);




SYSCTL_RCGCUART_R |=SYSCTL_RCGCUART_R2;
/*enable clock for GPIOPORTE which used by UART5*/
SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R3;
Delay(1);




/************* HANDLING UART REGISTERS **********************************************************************************/
/*disable UART5 module*/
UART5_CTL_R = 0;
/*set the buad rate*/
UART5_IBRD_R = 104;
UART5_FBRD_R = 11;
/* No parity - FIFO enabled - data length=8 bits */
UART5_LCRH_R = (UART_LCRH_WLEN_8 |UART_LCRH_FEN);
// finally enable the uart pins 4 and 5 and also set the UARTEN bit of this register, see CTL register of uart module..
UART5_CTL_R = 0x301;
/***************************************************************************************************************************/
UART2_CTL_R = 0;
/*set the buad rate*/
UART2_IBRD_R = 104;
UART2_FBRD_R = 11;
/* No parity - FIFO enabled - data length=8 bits */
UART2_LCRH_R = (UART_LCRH_WLEN_8 |UART_LCRH_FEN);
// finally enable the uart pins 4 and 5 and also set the UARTEN bit of this register, see CTL register of uart module..
UART2_CTL_R = 0x301;




/************* CONFIGURE PORTE REGISTERS ***********************/
/*set PE4 and PE5 as digital I/O*/
GPIO_PORTE_DEN_R = 0x30;
/*use PE4 and PE5 as UART, so handle their AFSEL and PCTL*/
GPIO_PORTE_AFSEL_R= 0x30;
GPIO_PORTE_PCTL_R=0x00110000;
/*set analog functionality by 0*/
GPIO_PORTE_AMSEL_R=0;
/***************************************************************/
/*set PE4 and PE5 as digital I/O*/
GPIO_PORTD_DEN_R = 0xC0;
/*use PE4 and PE5 as UART, so handle their AFSEL and PCTL*/
GPIO_PORTD_AFSEL_R= 0xC0;
GPIO_PORTD_PCTL_R=0x11000000;
/*set analog functionality by 0*/
GPIO_PORTD_AMSEL_R=0;
//Delay(1);
}
void UART2_Receiver(char* data){
//char data;
while((UART2_FR_R & 0x0010) != 0){}; /* wait until Rx buffer is not empty */
/* before giving it another byte */
//UART5_DR_R = UART2_DR_R&0xFF ;/////////////////////////zyada
// return (unsigned char) UART2_DR_R&0xFF;
*data=UART2_DR_R;
}



// this function takes array of characters read from gps arr[200]
void readarray(char array[])
{
	
	int index;
 	int k=0;
	int i=0;
	while(k<100)//hwa el mafrod el rakam da yb2a < mn 100 bkter 34an adman en lsa fe data
	{
		if((array[k]=='G')&&(array[k+1]=='P')&&(array[k+2]=='R')&&(array[k+3]=='M')&&(array[k+4]=='C'))
		{
			index=k+6;
			while(array[index]!='*')
			{
				GPSValues[i]=array[index];
				index++;
				i++;
		        }
			index=0;
			token = strtok(GPSValues, comma);
			while( token != NULL )
			{
				strcpy(parseValue[index], token);
				token = strtok(NULL, comma);
				index++;
			}
			//finally lets take our latitude and longitude..
			// if 2 passed string are equale, strcmp will return 0.
			if(strcmp(parseValue[1],"A")==0)
			{
				latitude=atof(parseValue[2]);
				longitude=atof(parseValue[4]);
				
				//double atof(const char *str)
				
				//latitude Calculation
				degrees=latitude/100;
				minutes=latitude-(float)(degrees*100);
				seconds=minutes/60;
				l2=degrees+seconds;

				////////////////////////////////////////////
				if(count_eeprom<450)
				{
				array_of_eeprom[count_eeprom]=l2;
				count_eeprom++;
				}
				////////////////////////////////////////////

				//longitude Calculation
				degrees=longitude/100;
				minutes=longitude-(float)(degrees*100);
				seconds=minutes/60;
				lg2=degrees+seconds;
				
				////////////////////////////////////////////
				if(count_eeprom<450)
				{
				array_of_eeprom[count_eeprom]=lg2;
				count_eeprom++;
				}
				////////////////////////////////////////////
				
				if (l2 <30 || lg2 <30)
				{
					l2 =l1;
					lg2 = lg1;
					
				}
				
				break;	
		  }
		 }
		k++;
  }
	
}


float dist(float lat1, float lat2, float lon1, float lon2)
{
float d=0; //the returned value
if (l1!=0 && lg1!=0)
{
float phi1 = lat1 * (pi/ 180); // in radians
float phi2 = lat2 * (pi / 180);
float delta_phi = (lat2 - lat1) * (pi / 180);
float delta_lamda = (lon2 - lon1) * (pi / 180);
float a = (sin(delta_phi / 2) * sin(delta_phi / 2)) +
cos(phi1) * cos(phi2) *sin(delta_lamda / 2) * sin(delta_lamda / 2);
float c = 2 * atan2(sqrt(a), sqrt(1 - a));
d = (R) * (c); // in metres
}
// here we put the values of l2 and lg2 in l1 and lg1 respectively so that
// the next time we call this function it will calulate distance between the previous point and the
// point just read from gps
l1=l2;
lg1=lg2;
return d;
}
void float_hex(float array_of_eeprom[], uint32_t count_eeprom)
{
	float point;
	uint32_t i = 0;
	while (i < count_eeprom)
	{
		if ((i % 2) == 0)
		{
			point = (array_of_eeprom[i] - 30) * (10000000);
			array_of_eeprom_tohex[i] = point;
		}
		else
		{
			point = (array_of_eeprom[i] - 31) * (10000000);
			array_of_eeprom_tohex[i] = point;
		}
		i += 1;
	}
}





void check_dist(float x)
{

	if (x >= 66 && x <= 70)
	{
		//cal_distance_right=((-0.0477)*cal_distance*cal_distance*cal_distance)+((9.6932)*cal_distance*cal_distance)-((753.84)*cal_distance)+19511;
		cal_distance_right = ((-0.0477)*cal_distance*cal_distance) + ((8.8391)*cal_distance) - (276.87);
		GPIO_PORTF_DATA_R |= red;
		LCD_Command(0xc0);
		parse_float(cal_distance_right);
	}

}


int main()
{
	int cnt =199;
        SysTick_Init();
        uart_initialization();
	while (1)
	{
	   cnt =199;
	   z=0;
	   while (cnt !=0)
	   {
	      UART2_Receiver(array+z);
	      z+=1;
	      //if (z ==199);
	      cnt -=1;
	      //break;	
	   }
	   readarray(array);
	   cal_distance += dist(l1,l2,lg1,lg2);
  
	}

//eeprom_init();
	//	UART5_Init();

//if led is green this means we pressed switch
	GPIO_PORTF_DATA_R = 0x08;  // Green led 
	//array_of_eeprom[count_eeprom]='\0';
	//count_eeprom+=1;
	//float_hex(array_of_eeprom, count_eeprom);
	//write(array_of_eeprom_tohex ,count_eeprom);
	//for(l=0;l<count_eeprom;l+=1)
	//{
		//temp_transmit=array_of_eeprom[l];
		//parse_float_transmit_uart(temp_transmit);
	//}
	//when led turns off this means we finished writing to eeprom
	
while(1)
	{
		UART0_Transmitter('E');
		transmit_string("Enter :\n\r"); // To check the Connection
		check = UART0_Receiver();
		if(check=='U'){
			for(l=0;l<count_eeprom;l+=1)
			{
				temp_transmit=array_of_eeprom[l];
				parse_float_transmit_uart(temp_transmit);
				//UART7_Transmitter(Coordinates_points[i]);
				SysTick_Wait1ms(10);
			}
			transmit_string("Finish\n"); // To check the Connection
			break;
		
	}
}
	GPIO_PORTF_DATA_R &=~ 0x0E;
	
	return 0;
}

