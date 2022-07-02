#include "spi.h"



void SPIConfig (void)
{
  RCC->APB2ENR |= (1<<12);  // Enable SPI1 CLock
	
  SPI1->CR1 |= (1<<0)|(1<<1);   // CPOL=1, CPHA=1
	
  SPI1->CR1 |= (1<<2);  // Master Mode
	
  SPI1->CR1 |= (3<<3);  // BR[2:0] = 100: fPCLK/32, PCLK2 = 90MHz, SPI clk < 5MHz
	
  SPI1->CR1 &= ~(1u<<7);  // LSBFIRST = 0, MSB first
	
  SPI1->CR1 |= (1<<8) | (1<<9);  // SSM=1, SSi=1 -> Software Slave Management
	
  SPI1->CR1 &= ~(1u<<10);  // RXONLY = 0, full-duplex
	
  SPI1->CR1 &= ~(1u<<11);  // DFF=0, 8 bit data
	
  SPI1->CR2 = 0;
}


void SPI_Enable (void)
{
	SPI1->CR1 |= (1<<6);   // SPE=1, Peripheral enabled
}

void SPI_Disable (void)
{
	SPI1->CR1 &= ~(1U<<6);   // SPE=0, Peripheral Disabled
}

void CS_Enable (void)
{
	GPIOA->BSRR |= (1<<9)<<16;
}

void CS_Disable (void)
{
	GPIOA->BSRR |= (1<<9);
}


void SPI_Transmit (uint8_t *data, int size)
{
	/************** STEPS TO FOLLOW *****************
	1. Wait for the TXE bit to set in the Status Register
	2. Write the data to the Data Register
	3. After the data has been transmitted, wait for the BSY bit to reset in Status Register
	4. Clear the Overrun flag by reading DR and SR
	************************************************/		
	int i=0;
	while (i<size)
	{
	   while (!((SPI1->SR)&(1<<1)));  // wait for TXE bit to set -> This will indicate that the buffer is empty
		 SPI1->DR = data[i]; 		// load the data into the Data Register
		 i++;
	}	
	
/*During discontinuous communications, there is a 2 APB clock period delay between the
write operation to the SPI_DR register and BSY bit setting. As a consequence it is
mandatory to wait first until TXE is set and then until BSY is cleared after writing the last
data.
*/
	while (!((SPI1->SR)&(1<<1)));  // wait for TXE bit to set -> This will indicate that the buffer is empty		
	while (((SPI1->SR)&(1<<7)));  // wait for BSY bit to Reset -> This will indicate that SPI is not busy in communication	
	
		
	//  Clear the Overrun flag by reading DR and SR
	uint8_t temp = (uint8_t)SPI1->DR;
					temp = (uint8_t)SPI1->SR;
		
}


void SPI_Receive (uint8_t *data, int size)
{
	/************** STEPS TO FOLLOW *****************
	1. Wait for the BSY bit to reset in Status Register
	2. Send some Dummy data before reading the DATA
	3. Wait for the RXNE bit to Set in the status Register
	4. Read data from Data Register
	************************************************/		

	while (size)
	{
		while (((SPI1->SR)&(1<<7)));  // wait for BSY bit to Reset -> This will indicate that SPI is not busy in communication
		SPI1->DR = 0;  // send dummy data
		while (!((SPI1->SR) &(1<<0)));  // Wait for RXNE to set -> This will indicate that the Rx buffer is not empty
		*data++ = (uint8_t) SPI1->DR;		
		size--;
	}	
}







	
void write (uint8_t address, uint8_t value)
{
	uint8_t data[2];
	data[0] = address & (~(1<<7));  // multibyte write
	data[1] = value;
	CS_Enable ();  // pull the cs pin low
	SPI_Transmit (data, 2);  // write data to register
	CS_Disable ();  // pull the cs pin high
}
	

void read (uint8_t address,uint8_t size)
{
	CS_Enable ();  // pull the pin lowF
	SPI_Transmit (&address, 1);  // send address
	SPI_Receive (RxData, size);  // receive 6 bytes data
	CS_Disable ();  // pull the pin high
}



void configureBME(){
	write(0xF4,0x57);
	write(0xF5,0x10);
}

void readChipId(){
	read((0xD0|0x80),1);
	char chipId[10];
	sprintf(chipId,"Chip ID %x",RxData[0]);
	sendString(chipId);
}

void readTrim(){
	read((0x88|0x80),24);
	dig_T1 =  ((uint16_t)(RxData[1]<<8) | RxData[0]);
	dig_T2 = (int16_t) ((RxData[3]<<8)|RxData[2]);
	dig_T3 = (int16_t) ((RxData[5]<<8)|RxData[4]);
	dig_P1 =  ((uint16_t) (RxData[7]<<8) | RxData[6]);
	dig_P2 = (int16_t) ((RxData[9]<<8) | RxData[8]);
	dig_P3 = (int16_t) ((RxData[11]<<8) | RxData[10]);
	dig_P4 = (int16_t) ((RxData[13]<<8) | RxData[12]);
	dig_P5 = (int16_t) ((RxData[15]<<8) | RxData[14]);
	dig_P6 = (int16_t) ((RxData[17]<<8) | RxData[16]);
	dig_P7 = (int16_t) ((RxData[19]<<8) | RxData[18]);
	dig_P8 = (int16_t) ((RxData[21]<<8) | RxData[20]);
	dig_P9 = (int16_t) ((RxData[23]<<8) | RxData[22]);
}


void readRawTemp(){
	read((0xFA|0x80),3);
	rawTemp = (RxData[0]<<12)|(RxData[1]<<4)|(RxData[2]>>4);
}

void readRawPress(){
	read((0xF7|0x80),3);
	rawPress = (RxData[0]<<12)|(RxData[1]<<4)|(RxData[2]>>4);
}

int32_t BME280_compensate_T_int32(int32_t adc_T)
{
	int32_t var1, var2, T;
	var1 = ((((adc_T>>3) - ((int32_t)dig_T1<<1))) * ((int32_t)dig_T2)) >> 11;
	var2 = (((((adc_T>>4) - ((int32_t)dig_T1)) * ((adc_T>>4) - ((int32_t)dig_T1)))>> 12) *((int32_t)dig_T3)) >> 14;
	t_fine = var1 + var2;
	T = (t_fine * 5 + 128) >> 8;
	return T;
}

uint32_t BME280_compensate_P_int32(int32_t adc_P)
{
	int64_t var1, var2, p;
	var1 = ((int64_t)t_fine) - 128000;
	var2 = var1 * var1 * (int64_t)dig_P6;
	var2 = var2 + ((var1*(int64_t)dig_P5)<<17);
	var2 = var2 + (((int64_t)dig_P4)<<35);
	var1 = ((var1 * var1 * (int64_t)dig_P3)>>8) + ((var1 * (int64_t)dig_P2)<<12);
	var1 = (((((int64_t)1)<<47)+var1))*((int64_t)dig_P1)>>33;
	if (var1 == 0)
	{
		return 0; // avoid exception caused by division by zero
	}
	p = 1048576-adc_P;
	p = (((p<<31)-var2)*3125)/var1;
	var1 = (((int64_t)dig_P9) * (p>>13) * (p>>13)) >> 25;
	var2 = (((int64_t)dig_P8) * p) >> 19;
	p = ((p + var1 + var2) >> 8) + (((int64_t)dig_P7)<<4);
	return (uint32_t)p;
}



