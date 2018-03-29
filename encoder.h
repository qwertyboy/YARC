/*
 * encoder.h
 *
 * Created: 3/12/2018 5:55:24 PM
 *  Author: Nathan
 */ 


#ifndef ENCODER_H_
#define ENCODER_H_

void EncoderInit(volatile uint8_t * port, uint8_t pinA, uint8_t pinB);
int32_t EncoderRead(void);
void EncoderSetPos(int32_t pos);

#endif /* ENCODER_H_ */