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
//*************************************** SYSTICK_TIMER(DELAY) *******************************

// this function initializes systick timer
void SysTick_Init(void) {
	NVIC_ST_CTRL_R = 0; // 1) disable SysTick during setup
	NVIC_ST_RELOAD_R = 0x00FFFFFF; // 2) maximum reload value
	NVIC_ST_CURRENT_R = 0; // 3) clear CURRENT 
	NVIC_ST_CTRL_R = 0x00000005; // 4) enable SysTick with core clock
}

// this function wait for 'count' bit to be equal 1
void SysTick_Wait(uint32_t delay) {
	NVIC_ST_RELOAD_R = delay - 1; // number of counts
	NVIC_ST_CURRENT_R = 0; // any value written to CURRENT clears
	while ((NVIC_ST_CTRL_R & 0x00010000) == 0) {};// wait for count flag
}

// Call this function to delay 1ms if given 1 and 2ms if given 2 and so on
void SysTick_Wait1ms(uint32_t delay) {
	unsigned long i;
	for (i = 0; i < delay; i++)
	{
		SysTick_Wait(16000);
	}
}
//*******************************************************************************************

//////////////////////////////////// Parsing /////////////////////////////////////////
void parse_float(float dist)
{
	char out[20]; int k = 0;
	int count = 0, i = 0;
	int n_dist = (int)dist;
	int f_dist = ((dist - n_dist) * 100000);
	char n;
	if (f_dist == 0)
	{
		n = '0';
		out[i] = n;
		i++;
		count++;
	}

	while (f_dist)
	{

		n = (f_dist % 10) + '0';
		f_dist /= 10;
		out[i] = n;
		count++;
		i++;
	}

	n = '.';
	out[i] = n;
	i++;
	count++;
	if (n_dist == 0)
	{
		n = '0';
		out[i] = n;
		count++;
	}

	while (n_dist)
	{

		n = (n_dist % 10) + '0';
		n_dist /= 10;
		out[i] = n;
		count++;
		i++;
	}


	for (k = count - 1; k >= 0; k--)
	{
		UART0_Transmitter(out[k]);
	}

	UART0_Transmitter('\n');

}

//*************************************** UART0 ***********************************************
void UART0_Init(void)
{
	SYSCTL_RCGCUART_R |= (1 << 0); // UART 0 --> 0000 0001
	SYSCTL_RCGCGPIO_R |= (1 << 0); // Port A (0000 0001) ( Pin 0 (PA0) --> U0Rx , Pin 1 (PA1) --> U0Tx )
	SysTick_Wait1ms(100);

	//PORT A
	GPIO_PORTA_AFSEL_R |= 0x03; //enable alt function to PA0 ,PA1
	GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R & 0xFFFFFF00) + 0x00000011;
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
void UART0_Transmitter(char data)
{
	while ((UART0_FR_R & 0x0020) != 0);
	UART0_DR_R = data;
}
/////////////////////////recieve u command ///////////////////////////

char UART0_Receiver(void)
{
	while ((UART0_FR_R & 0x0010) != 0);
	return (char)(UART0_DR_R & 0xFF);
}

///////////////////////////////////////////////////////////////////////



///////////////////////////UART 7////////////////////////////////////////

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

