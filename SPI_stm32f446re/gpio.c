#include "gpio.h"

void GPIOConfig (void)
{	
	RCC->AHB1ENR |= (1<<0);  // Enable GPIO Clock
	
	GPIOA->MODER |= (2<<10)|(2<<12)|(2<<14)|(1<<18);  // Alternate functions for PA5, PA6, PA7 and Output for PA9
	
	GPIOA->OSPEEDR |= (3<<10)|(3<<12)|(3<<14)|(3<<18);  // HIGH Speed for PA5, PA6, PA7, PA9
	
	GPIOA->AFR[0] |= (5<<20)|(5<<24)|(5<<28);   // AF5(SPI1) for PA5, PA6, PA7
}
