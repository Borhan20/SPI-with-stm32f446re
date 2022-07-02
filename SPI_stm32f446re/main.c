#ifndef STM32F446XX_H
#include "stm32f446xx.h"
#endif

#include "clock.h"
#include "gpio.h"
#include "usart.h"
#include "timer.h"
#include "spi.h"


uint8_t RxData[30];

uint16_t dig_T1,dig_P1;
int16_t dig_T2,dig_T3,dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;


int32_t rawTemp, rawPress;

int32_t t_fine;
float temparature,pressure;


int main()
{
	clockConfig();
	
	GPIOConfig();
	
	SPIConfig();
	
	usartConfig();
	
	CS_Disable();
	
	timerConfig();
		
	SPI_Enable();
	
	readChipId();
	
	configureBME();
	
	readTrim();
	
	char tempStr[50];
	char pressStr[50];
	
	
	while(1) {
		readRawTemp();	
		temparature = (float)((BME280_compensate_T_int32((int32_t)rawTemp))/100.0);
		
		sprintf(tempStr,"Temperature %f C", (double)temparature);
		sendString((uint8_t*) tempStr);
		
		readRawPress();
		pressure = (float)((BME280_compensate_P_int32(rawPress))/256.0);
		sprintf(pressStr,"Pressure %f Pa", (double) pressure);
		sendString((uint8_t*) pressStr);
		Delay_s(4);
	}
}



