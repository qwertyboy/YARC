/*
 * spi.c
 *
 * Created: 3/5/2018 9:54:49 PM
 *  Author: Nathan
 */ 

#include <avr/io.h>
#include "spi.h"

// SPIInit
// Description:
//      Initializes the USI module in SPI master mode
// Arguments:
//      speed (uint8_t): The clock speed to run the bus at
// Returns:
//      none
void spiInit(uint8_t speed){
    // disable interrupts
    //SREG &= ~(1 << SREG_I);
    
    // set mosi, sck, ss as output, miso as input
    DDRB |= (1 << DDB3) | (1 << DDB5) | (1 << DDB2);
    DDRB &= ~(1 << DDB4);
    // set ss high
    PORTB |= (1 << PORTB2);
    
    // enable spi, msb first, master, mode 0, set speed
    SPCR = (1 << SPE) | (1 << MSTR) | (speed & 0x03);
    // set spi2x bit if needed
    if(speed == SPI_CLKDIV_2 || speed == SPI_CLKDIV_8 || speed == SPI_CLKDIV_32){
        SPSR |= (1 << SPI2X);
    }
    
    // enable interrupts
    //SREG |= (1 << SREG_I);
}


// SPITransfer
// Description:
//      Sends a byte over SPI and waits for a return
// Arguments:
//      data (uint8_t): The data to send
//      csPort (uint8_t *): The address of the cs port
//      csPin (uint8_t): The cs pin number
// Returns:
//      uint8_t: The data received from the slave device
uint8_t spiTransfer(uint8_t data, volatile uint8_t * csPort, uint8_t csPin){
    // set cs pin low
    *csPort &= ~(1 << csPin);
    // send data
    SPDR = data;
    // wait for transmission to complete
    while(!(SPSR & (1 << SPIF)));
    // set cs pin high
    *csPort |= (1 << csPin);
    return SPDR;
}

