#include "C:\Keil\TExaSware\tm4c123gh6pm.h"
#include <stdint.h>

void SystemInit (){}

int cal_distance =300;	

	
void led_init(void)
{
    SYSCTL_RCGCGPIO_R |= 0x20; 
    while ((SYSCTL_PRGPIO_R & 0x20 ) == 0) ;
		GPIO_PORTF_LOCK_R = 0x4C4F434B;///////////// REMOVE ON TIVA
		GPIO_PORTF_CR_R = 0x1F;        //////////////REMOVE ON TIVA
    GPIO_PORTF_AFSEL_R = 0x00;
    //GPIO_PORTF_PCTL_R = 0;
    GPIO_PORTF_AMSEL_R = 0x00;
    GPIO_PORTF_DIR_R = 0x0E;
    GPIO_PORTF_DEN_R = 0x1F;
    GPIO_PORTF_PUR_R = 0x10;  // PF4
			
}

void interrupt_init (void)
{
	GPIO_PORTF_IS_R &= ~0x10;     // edge sensitive
	GPIO_PORTF_IBE_R &= ~0x10;    // only one edge not both 
	GPIO_PORTF_IEV_R &= ~0x10;   //falling edge event
	GPIO_PORTF_ICR_R = 0x10;     //CLEAR         
	GPIO_PORTF_IM_R = 0x10;      //ARM INTERRUPT
	NVIC_PRI7_R =(NVIC_PRI7_R&0xFF00FFFF)|0x00200000; //PRIORITY =1
	NVIC_EN0_R = (1<<30);      //ENABLE INTERRUPT FOR PORT(F) 
	
	EnableInterrupts();
}


void GPIOPortF_Handler(void)
{
	GPIO_PORTF_ICR_R = 0x10;
	//if(cal_distance >100)
	//{
		GPIO_PORTF_DATA_R = 0x08;  // Green led 
	//}
	
	/////////LCD CODE TO PRINT DISTANCE
	//
	//
	//
	
}

int __main()
{
	
	led_init();
	interrupt_init();
	
	while (1)
	{};
		
	return 0;
	
}



