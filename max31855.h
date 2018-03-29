/*
 * max31855.h
 *
 * Created: 3/28/2018 9:36:19 PM
 *  Author: Nathan
 */ 


#ifndef MAX31855_H_
#define MAX31855_H_

#define THERMOCOUPLE_SHORT_VCC  4
#define THERMOCOUPLE_SHORT_GND  2
#define THERMOCOUPLE_OPEN       1


void Max31855Init(volatile uint8_t * port, uint8_t pin);
int16_t Max31855Read(void);

#endif /* MAX31855_H_ */