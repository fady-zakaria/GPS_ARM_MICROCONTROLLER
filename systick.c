
#include"systick.h"
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
while((NVIC_ST_CTRL_R&0x00010000)==0){};// wait for flag

}

// Call this function to delay 1ms if given 1 and 2ms if given 2 and so on
void SysTick_Wait1ms(uint32_t delay){
unsigned long i;
for(i=0; i<delay; i++)
{
SysTick_Wait(16000); 
}
}
