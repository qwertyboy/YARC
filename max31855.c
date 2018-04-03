/*
 * max31855.c
 *
 * Created: 3/28/2018 9:35:50 PM
 *  Author: Nathan
 */ 

#include <avr/io.h>
#include "max31855.h"
#include "spi.h"

// spi port and pin
volatile uint8_t * max31855CsPort;
uint8_t max31855CsPin;


// Description:
//      Initializes the MAX31855
// Arguements:
//      port (uint8_t *): A pointer to the port the CS pin is connected to
//      pin (uint8_t): The pin number the CS pin is connected to
void Max31855Init(volatile uint8_t * port, uint8_t pin){
    // copy cs port and pin
    max31855CsPort = port;
    max31855CsPin = pin;
    // set cs pin as output, DDRx is one address below PORTx
    *(max31855CsPort - 0x01) |= (1 << max31855CsPin);
}


//
float Max31855Read(void){
    int16_t result = 0;
    
    // drive cs low
    *max31855CsPort &= ~(1 << max31855CsPin);
    // read high and low words
    uint16_t highWord = SpiTransfer16(0x00);
    uint16_t lowWord = SpiTransfer16(0x00);
    // drive cs high
    *max31855CsPort |= (1 << max31855CsPin);
    
    // check if negative
    if(highWord & 0x8000){
        // shift over and sign extend
        result = (highWord >> 2) | 0xC000;
    }else{
        result = highWord >> 2;
    }
    
    float num = result * 0.25;
    return num;
}