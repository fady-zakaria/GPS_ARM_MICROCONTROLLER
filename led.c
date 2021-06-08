#include"led.h"
	
void SystemInit ()
{
	CPAC  |= 0X00F00000;
}

void led_init(void)
{
    SYSCTL_RCGCGPIO_R |= 0x20; 
    while ((SYSCTL_PRGPIO_R & 0x20 ) == 0) ;
    //GPIO_PORTF_CR_R = 0x1F;
    GPIO_PORTF_AFSEL_R = 0x00;
    //GPIO_PORTF_PCTL_R = 0;
    GPIO_PORTF_AMSEL_R = 0x00;
    GPIO_PORTF_DIR_R = 0x0E;
    GPIO_PORTF_DEN_R = 0x1F;
    GPIO_PORTF_PUR_R = 0x10;
			
}


void check_dist(float x)
{
	if(x>=100)
	{
		GPIO_PORTF_DATA_R = red;
	}
	else
	{
		GPIO_PORTF_DATA_R = blue;
	}
	
}