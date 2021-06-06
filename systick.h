
#include"Main_Header.h"

#define NVIC_ST_CTRL_R (*((volatile uint32_t *)0xE000E010))
#define NVIC_ST_RELOAD_R (*((volatile uint32_t *)0xE000E014))
#define NVIC_ST_CURRENT_R (*((volatile uint32_t *)0xE000E018))

void SysTick_Init        (void);
void SysTick_Wait        (uint32_t);
void SysTick_Wait1ms     (uint32_t);
