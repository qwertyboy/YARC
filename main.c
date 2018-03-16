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
    // enable timer0 for timekeeping
    timerInit();
    
    // set cs pins as output
    DDRB |= (1 << DDB2) | (1 << DDB1);
    // led on PD0
    DDRD |= (1 << DDD0);
    
    // enable spi
    spiInit(SPI_CLKDIV_2);
    // enable lcd
    lcdInit(&PORTB, PORTB2);
    // create a degree symbol
    uint8_t degSymbol[] =  {0x0C, 0x12, 0x12, 0x0C, 0x00, 0x00, 0x00};
    lcdCreateChar(0, degSymbol);
    // enable max6675
    max6675Init(&PORTB, PORTB1);
    // initialize encoder
    encoderInit(&PORTC, PORTC1, PORTC0);
    
    
    uint8_t printBuf[32];
    lcdClear();
    lcdClear();
    lcdPrint("Hello!");
    delay(1000);
    lcdCursor(1);
    
    while(1){
        int32_t pos = encoderRead();
        
        lcdClear();
        lcdPrint("position: ");
        itoa(pos, printBuf, 10);
        lcdPrint(printBuf);
        if(pos >= 0 && pos <= 15){
            lcdSetCursor(pos, 0);
        }
        delay(100);
    }
}