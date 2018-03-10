/*
 * max6675.h
 *
 * Created: 3/9/2018 9:49:15 PM
 *  Author: Nathan
 */ 


#ifndef MAX6675_H_
#define MAX6675_H_

#define THERMOCOUPLE_OPEN 0x04

void max6675Init(volatile uint8_t * maxPort, uint8_t maxPin);
int16_t max6675Read();

#endif /* MAX6675_H_ */