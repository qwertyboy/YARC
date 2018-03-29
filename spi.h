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

void SpiInit(uint8_t speed);
uint8_t SpiTransfer(uint8_t data);
uint16_t SpiTransfer16(uint16_t data);

#endif /* SPI_H_ */