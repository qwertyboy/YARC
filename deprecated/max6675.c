/*
 * max6675.c
 *
 * Created: 3/9/2018 9:49:00 PM
 *  Author: Nathan
 */ 

#include <avr/io.h>
#include "max6675.h"
#include "../spi.h"

// spi pins
volatile uint8_t * maxPort;
uint8_t maxPin;


void Max6675Init(volatile uint8_t * port, uint8_t pin){
    // copy the port and pin
    maxPort = port;
    maxPin = pin;
}


// probably wont work until spiTransfer is modified for multiple bytes
int16_t Max6675Read(){
    *maxPort &= ~(1 << maxPin);
    uint16_t result = SpiTransfer16(0);
    *maxPort |= (1 << maxPin);
    
    if(result & THERMOCOUPLE_OPEN){
        return -1;
    }else{
        return (result >> 3) & 0xFFF;
    }
}