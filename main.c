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

int main(void){
    // enable spi
    spiInit(SPI_CLKDIV_2);
    // enable lcd
    lcdInit(&PORTB, PORTB2);
    lcdPrint("henlo :3c", 9);
    
    while(1){
    }
}