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
    SPIInit(SPI_CLKDIV_2);
    // enable lcd
    LCDInit(&PORTB, PORTB2);
    LCDPrint("henlo :3c", 9);
    
    while(1){
    }
}