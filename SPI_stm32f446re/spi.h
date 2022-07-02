
#ifndef STM32F446XX_H
#include "stm32f446xx.h"
#endif

#ifndef STDIO_H
#include "stdio.h"
#endif

#ifndef USART_H
#include "usart.h"
#endif

void SPIConfig(void);

void SPI_Enable (void);

void SPI_Disable (void);

void CS_Enable (void);

void CS_Disable (void);


void SPI_Transmit (uint8_t *data, int size);

void SPI_Receive (uint8_t *data, int size);


void write (uint8_t address, uint8_t value);

void read (uint8_t address,uint8_t size);


void configureBME(void);
void readChipId(void);
void readTrim(void);
void readRawTemp(void);
void readRawPress(void);


int32_t BME280_compensate_T_int32(int32_t adc_T);
uint32_t BME280_compensate_P_int32(int32_t adc_P);




extern uint8_t RxData[30];

extern uint16_t dig_T1,dig_P1;
extern int16_t dig_T2,dig_T3,dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;


extern int32_t rawTemp;
extern int32_t rawPress;

extern int32_t t_fine;
extern float temparature,pressure;

