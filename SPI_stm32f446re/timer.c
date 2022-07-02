#include "timer.h"

void timerConfig(){
	
	RCC->APB1ENR |= (1<<4);
	
	TIM6->PSC = (90 - 1);
	TIM6->ARR = 0xffff;
	
	TIM6->CR1 |= TIM_CR1_CEN;
	while(!(TIM6->SR & (1<<0)));
	
}

void Delay_us(uint16_t timeMicroSec){	
	
	TIM6->CNT = 0;
	
	while(TIM6->CNT < timeMicroSec);
}
void Delay_ms(uint16_t timeMilliSec){	
	
	for(int i=0; i<timeMilliSec; i++)
		Delay_us(1000);							
}


void Delay_s(uint16_t timeSec){					
	
	for(int i=0; i<timeSec; i++)
		Delay_ms(1000);								
}
