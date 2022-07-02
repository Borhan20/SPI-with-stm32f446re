#include "clock.h"


void clockConfig()
{
	//set HSEON 
	RCC->CR |= (1 << 16);
	
	//wait for HSERDY to be set
	while(!(RCC->CR & (1 << 17)));
	
	//set PWREN 
	RCC->APB1ENR |= (1 << 28);
	
	//study
	PWR->CR |= (3 << 14);
	
	//study
	FLASH->ACR |= FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_PRFTEN | FLASH_ACR_LATENCY_5WS;
	
	//configure AHB prescaler
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
	
	//configure APB Low speed prescaler (APB1)
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;
	
	//configure APB High speed prescaler (APB2)
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;
	
	//set values to PLLM,PLLN,PLLP and select HSE as the PLL source	
	RCC->PLLCFGR = (4 << 0) | (180 << 6) | (0 << 16) | RCC_PLLCFGR_PLLSRC_HSE;
	
	//turn on the PLL	
	RCC->CR |= RCC_CR_PLLON;
	
	//wait for PLL to be ready
	while(!(RCC->CR & (1 << 25)));
	
	//select PLL as the system clock source	
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	
	//wait for system clock source to be selected	
	while(!(RCC->CFGR & RCC_CFGR_SWS_PLL));
	
}

