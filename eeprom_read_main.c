#include "stdint.h"
#include "E:/keil/EE319Kware/inc/tm4c123gh6pm.h" 

#define NVIC_ST_CTRL_R (*((volatile uint32_t *)0xE000E010))
#define NVIC_ST_RELOAD_R (*((volatile uint32_t *)0xE000E014))
#define NVIC_ST_CURRENT_R (*((volatile uint32_t *)0xE000E018))

#define CPAC (*((volatile uint32_t *)0xE000ED88))


void SystemInit()
{
	CPAC  |= 0X00F00000;
}


///////////////////////////uart7////////////////////////////////////////



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


////////////////////////////////EEPROM///////////////////////////////////////////////


void delay(uint32_t no)
{
	while(no)
		no -- ;
}


void eeprom_init()
{
	
	// 1- enable eeprom usage
	SYSCTL_RCGCEEPROM_R  |= SYSCTL_RCGCEEPROM_R0;  //set bit 0 by 1 
	delay(1000);
	while ((SYSCTL_PREEPROM_R & SYSCTL_RCGCEEPROM_R0 ) == 0) ;    //wait until ready
	
	
	//3- check for done 
	while((EEPROM_EEDONE_R  & 0x01 ) != 0x00) ;  //wait for the task to be completed
	
	//4- check for errors
	while((EEPROM_EESUPP_R & 0x0C) == 0x01) ;  //continue when they are clear
	
	//5- reset
	SYSCTL_SREEPROM_R = 0x01;
	//delay
	delay(3000);
	SYSCTL_SREEPROM_R = 0x00;
	while(SYSCTL_PREEPROM_R == 0x00);  //wait until ready
	
	//delay
	delay(1000);
	//6- check for done 
	while((EEPROM_EEDONE_R & 0x01 ) != 0x00) ;  //wait for the task to be completed
	
	//7- check for errors
	while((EEPROM_EESUPP_R & 0x0C) == 0x01) ;  //continue when they are clear
 
}


void read(void)  //uint32_t data_size
{
	uint32_t i=0x00;
//	uint32_t j=0x00;
	float data;
	
	
	
	while(1)
	{
		
			
			//while (i<=(0xF))
			//{
		EEPROM_EEBLOCK_R = 0x00;
		EEPROM_EEOFFSET_R = i;  data=EEPROM_EERDWR_R ;
				parse_float(data);
		i+=0x01;
				if(i==0x10) 
				{
					EEPROM_EEBLOCK_R=EEPROM_EEBLOCK_R+0x01;
					 i=0x00;
				}
	//}
			//j+=0x01;
	   // i=0x00;
	
	}
	
	
}
	


/////////////////////////////////////////////////////////////////////////////////////


int main()
{
	char receive_command;
	eeprom_init();
	SysTick_Init();
  UART0_Init();
	//receive_command = UART0_Receiver();
	//if (receive_command =='U') read();
		read();
	
return 0;
}

