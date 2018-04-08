/*
 * eeprom.h
 *
 * Created: 4/7/2018 9:27:10 PM
 *  Author: Nathan
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_

void EepromWrite(uint16_t address, uint8_t data);
uint8_t EepromRead(uint16_t address);
void EepromWrite16(uint16_t address, uint16_t data);
uint16_t EepromRead16(uint16_t address);

#endif /* EEPROM_H_ */