#include "usart.h"


void usartConfig(void)
{
	// USART2 Enable
	RCC->APB1ENR |= (1<<17);
	
	// Enable Clock of GPIOA
	RCC->AHB1ENR |= 1;
	
	// MODER set for PA2, PA3
	GPIOA->MODER |= (2<<4) | (2<<6);
	
	//SPEED set for PA2, PA3
	GPIOA->OSPEEDR |= (3<<4) | (3<<6);
	
	// AF7 for PA2, PA3
	GPIOA->AFR[0] |= (7<<8);
	GPIOA->AFR[0] |= (7<<12);
	
	// USART2->CR1 = 0x00;
	
	//USART Enable
	USART2->CR1 = (1<<13);
	
	// Data length 8 bit
	USART2->CR1 &= ~(1U << 12);
	
	// Transmitter and receiver enable
	USART2->CR1 |= (1<<2)|(1<<3);
	
	// 1 Stop bit
	USART2->CR2 &= ~(3U<<12);
	
	// Baud rate select
	USART2->BRR |= (7 << 0) | (24 << 4);

	// receive interrupt enable
	USART2->CR1 |= (1<<5);
	
	
	// Interrupt enable 
	NVIC_EnableIRQ(USART2_IRQn);
}

void sendChar(uint8_t data){
	USART2->DR = data;
	while(!(USART2->SR & (1<<6)));
}


void sendString(uint8_t *string){
	sendChar(0x0A);
	uint8_t i = 0;
	while(string[i] != 0){
		sendChar(string[i]);
		i++;
	}
	sendChar(0x0A);
}

