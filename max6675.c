/*
 * max6675.c
 *
 * Created: 3/9/2018 9:49:00 PM
 *  Author: Nathan
 */ 

#include <avr/io.h>
#include "max6675.h"
#include "spi.h"

// spi pins
volatile uint8_t * csPort;
uint8_t csPin;


void max6675Init(volatile uint8_t * maxPort, uint8_t maxPin){
    // copy the port and pin
    csPort = maxPort;
    csPin = maxPin;
}


// probably wont work until spiTransfer is modified for multiple bytes
int16_t max6675Read(){
    uint8_t highByte = spiTransfer(0x00, csPort, csPin);
    uint8_t lowByte = spiTransfer(0x00, csPort, csPin);
    uint16_t result = ((highByte | lowByte) >> 3) & 0xFFF;
    if(lowByte & THERMOCOUPLE_OPEN){
        return -1;
    }else{
        return result;
    }
}