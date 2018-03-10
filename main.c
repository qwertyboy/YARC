/*
 * reflow-controller.c
 *
 * Created: 3/4/2018 11:13:09 PM
 * Author : Nathan
 */ 

#include <avr/io.h>
#include "utils.h"
#include "spi.h"
#include "lcd.h"
#include "max6675.h"

int main(void){
    // set cs pins as output
    DDRB |= (1 << DDB2) | (1 << DDB1);
    // enable spi
    spiInit(SPI_CLKDIV_2);
    // enable lcd
    lcdInit(&PORTB, PORTB2);
    // enable max6675
    max6675Init(&PORTB, PORTB1);
    
    lcdPrint("henlo :3c", 9);
    
    while(1){
    }
}