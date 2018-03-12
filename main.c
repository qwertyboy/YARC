/*
 * reflow-controller.c
 *
 * Created: 3/4/2018 11:13:09 PM
 * Author : Nathan
 */ 

#include <avr/io.h>
#include <stdlib.h>
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
    uint8_t degSymbol[] =  {0xc,0x12,0x12,0xc,0x0,0x0,0x0};
    lcdCreateChar(0, degSymbol);
    // enable max6675
    max6675Init(&PORTB, PORTB1);
    
    
    delay(500000);
    uint8_t printBuf[32];
    lcdPrint("Hello!");
    delay(500000);
    
    while(1){
        int16_t temp = max6675Read() / 4;
        lcdClear();
        lcdSetCursor(0, 0);
        itoa(temp, printBuf, 10);
        lcdPrint(printBuf);
        lcdWrite(0);
        lcdPrint("C");
        delay(200000);
    }
}