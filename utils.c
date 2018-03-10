/*
 * utils.c
 *
 * Created: 3/8/2018 9:56:30 PM
 *  Author: Nathan
 */ 

#include <avr/io.h>

void delay(volatile uint32_t ticks){
    uint32_t i;
    for(i = 0; i < ticks; i++);
}