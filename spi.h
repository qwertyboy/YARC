/*
 * spi.h
 *
 * Created: 3/5/2018 9:55:11 PM
 *  Author: Nathan
 */ 


#ifndef SPI_H_
#define SPI_H_

#define SPI_CLKDIV_2    4
#define SPI_CLKDIV_4    0
#define SPI_CLKDIV_8    5
#define SPI_CLKDIV_16   1
#define SPI_CLKDIV_32   6
#define SPI_CLKDIV_64   2
#define SPI_CLKDIV_128  3

void SPIInit(uint8_t speed);
uint8_t SPITransfer(uint8_t data, volatile uint8_t * csPort, uint8_t csPin);

#endif /* SPI_H_ */