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


// Description:
//      Reads the temperature from the attached thermocouple
// Arguments:
//      None
// Returns:
//      float: The temperature in degrees Celsius (maybe remove float)
//             9999 if a fault is detected
float Max31855Read(void){
    int16_t result = 0;
    
    // drive cs low
    *max31855CsPort &= ~(1 << max31855CsPin);
    // read high word and discard low word
    uint16_t highWord = SpiTransfer16(0x00);
    SpiTransfer16(0x00);
    // drive cs high
    *max31855CsPort |= (1 << max31855CsPin);
    
    // check if fault
    if(highWord & THERMOCOUPLE_FAULT_BIT){
        return THERMOCOUPLE_FAULT_RETURN;
    }
    
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


// Description:
//      Reads the internal reference junction temperature
// Arguments:
//      None
// Returns:
//      float: The temperature in degrees Celsius (maybe remove float)
//             9999 if a fault is detected
float Max31855ReadReference(void){
    int16_t result = 0;
        
    // drive cs low
    *max31855CsPort &= ~(1 << max31855CsPin);
    // discard high word and read low word
    uint16_t highWord = SpiTransfer16(0x00);
    uint16_t lowWord = SpiTransfer16(0x00);
    // drive cs high
    *max31855CsPort |= (1 << max31855CsPin);
    
    // check if fault
    if(highWord & THERMOCOUPLE_FAULT_BIT){
        return THERMOCOUPLE_FAULT_RETURN;
    }
        
    // check if negative
    if(lowWord & 0x8000){
        // shift over and sign extend
        result = (lowWord >> 4) | 0xF000;
    }else{
        result = lowWord >> 4;
    }
    
    float num = result * 0.0625;
    return num;
}


// Description:
//      Reads the fault status
// Arguments:
//      None
// Returns:
//      uint8_t: The fault bits. Check with definitions in max31855.h
uint8_t Max31855ReadFault(void){
    // drive cs low
    *max31855CsPort &= ~(1 << max31855CsPin);
    // read high word and read low word
    uint16_t highWord = SpiTransfer16(0x00);
    uint16_t lowWord = SpiTransfer16(0x00);
    // drive cs high
    *max31855CsPort |= (1 << max31855CsPin);
    
    if(!(highWord & THERMOCOUPLE_FAULT_BIT)){
        return 0;
    }else{
        return (lowWord & 0x07);
    }
}