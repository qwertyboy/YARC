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
#include "encoder.h"

int main(void){
    // set cs pins as output
    DDRB |= (1 << DDB2) | (1 << DDB1);
    // led on PD0
    DDRD |= (1 << DDD0);
    
    // enable spi
    spiInit(SPI_CLKDIV_2);
    // enable lcd
    lcdInit(&PORTB, PORTB2);
    uint8_t degSymbol[] =  {0xc,0x12,0x12,0xc,0x0,0x0,0x0};
    lcdCreateChar(0, degSymbol);
    // enable max6675
    max6675Init(&PORTB, PORTB1);
    // initialize encoder
    encoderInit(&PORTC, PORTC0, PORTC1);
    
    
    uint8_t printBuf[32];
    lcdClear();
    lcdClear();
    lcdPrint("Hello!");
    delay(500000);
    
    while(1){
        int32_t pos = encoderRead();
        
        lcdClear();
        lcdSetCursor(0, 0);
        lcdPrint("position: ");
        itoa(pos, printBuf, 10);
        lcdPrint(printBuf);
        delay(200000);
    }
}