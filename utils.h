/*
 * utils.h
 *
 * Created: 3/8/2018 9:56:40 PM
 *  Author: Nathan
 */ 


#ifndef UTILS_H_
#define UTILS_H_

void delay(volatile uint32_t ticks);
void swap(uint8_t * a, uint8_t * b);
void reverse(uint8_t * str, uint8_t len);
uint8_t * int2str(int32_t num, uint8_t * buf);

#endif /* UTILS_H_ */