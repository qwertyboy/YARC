/*
 * encoder.h
 *
 * Created: 3/12/2018 5:55:24 PM
 *  Author: Nathan
 */ 


#ifndef ENCODER_H_
#define ENCODER_H_

void encoderInit(volatile uint8_t * port, uint8_t pinA, uint8_t pinB);
int32_t encoderRead(void);

#endif /* ENCODER_H_ */