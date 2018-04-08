/*
 * eeprom.c
 *
 * Created: 4/7/2018 9:26:54 PM
 *  Author: Nathan
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "eeprom.h"


// Description:
//      Writes a byte to the EEPROM at the address specified
// Arguments:
//      address (uint16_t): The address to write to in the eeprom
//      data (uint8_t): The data to store in the eeprom
void EepromWrite(uint16_t address, uint8_t data){
    cli();                      // disable interrupts
    while(EECR & (1 << EEPE));  // wait for completion of previous write
    EEAR = (address & 0x3FF);   // set address
    EEDR = data;                // set data
    EECR |= (1 << EEMPE);       // enable writing
    EECR |= (1 << EEPE);        // write data
    sei();                      // re-enable interrupts
}


// Description:
//      Reads a byte from the EEPROM at the address specified
// Arguments:
//      address (uint16_t): The address to read from
// Returns:
//      uint8_t: The data stored at the specified address
uint8_t EepromRead(uint16_t address){
    cli();                      // disable interrupts
    while(EECR & (1 << EEPE));  // wait for completion of previous write
    EEAR = (address & 0x3FF);   // set address
    EECR |= (1 << EERE);        // enable reading
    uint8_t data = EEDR;        // read the data
    sei();                      // re-enable interrupts
    return data;                // return the data
}


// Description:
//      Stores a 16 bit value in the eeprom
// Arguments:
//      address (uint16_t): The starting address to write to
//      data (uint16_t): The data to store in the eeprom
void EepromWrite16(uint16_t address, uint16_t data){
    address = address & 0x3FF;                  // limit address to 1k
    EepromWrite(address, (data >> 8));          // store high byte
    EepromWrite(address + 1, (data & 0xFF));    // store low byte
}


// Description:
//      Reads a 16 bit value from the EEprom at the address specified
// Arguments:
//      address (uint16_t): The starting address of the 16 bit data
// Returns:
//      uint16_t: The 16 bit data stored at the specified address
uint16_t EepromRead16(uint16_t address){
    address = address & 0x3FF;
    uint16_t highByte = EepromRead(address) << 8;
    uint16_t lowByte = EepromRead(address + 1);
    return (highByte & lowByte);
}