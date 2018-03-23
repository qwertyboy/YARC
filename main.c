/*
 * reflow-controller.c
 *
 * Created: 3/4/2018 11:13:09 PM
 * Author : Nathan
 */ 

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include "utils.h"
#include "spi.h"
#include "lcd.h"
#include "max6675.h"
#include "encoder.h"
#include "menu.h"

int main(void){
    // enable timer0 for timekeeping
    timerInit();
    
    // set cs pins as output
    DDRB |= (1 << DDB2) | (1 << DDB1);
    // led on PD0
    DDRD |= (1 << DDD0);
    // button on PC2, set as input and enable pull-up
    DDRC &= ~(1 << DDC2);
    PORTC |= (1 << PORTC2);
    
    // enable spi
    spiInit(SPI_CLKDIV_2);
    // enable lcd
    lcdInit(&PORTB, PORTB2);
    // create a degree symbol
    uint8_t degSymbol[] = {0x0C, 0x12, 0x12, 0x0C, 0x00, 0x00, 0x00, 0x00};
    uint8_t boxSymbol[] = {0x1F, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x1F};
    lcdCreateChar(0, degSymbol);
    lcdCreateChar(1, boxSymbol);
    lcdClear();
    lcdClear();
    // enable max6675
    max6675Init(&PORTB, PORTB1);
    // initialize encoder
    encoderInit(&PORTC, PORTC1, PORTC0);
    
    
    lcdPrint("   YetAnother\nReflowController");
    delay(2000);
    lcdClear();
    
    // misc loop variables
    uint8_t printBuf[32];   // buffer for printing to lcd
    int16_t encoderPos = 0;
    int16_t lastEncPos = -1;
    
    // profile vars
    Profile_t profile;
    
    // wait for button to select a profile.
    lcdPrint("Select Profile:");
    while(buttonRead(&PINC, PINC2, 50, 1000) != 1){
        // read encoder
        encoderPos = encoderRead();
        // check lower bound
        if(encoderPos < 0){
            encoderPos = 0;
            encoderSetPos(0);
        }
        
        // get the profile selected by the encoder
        profile = Profiles[encoderPos % 2];
        // update display if encoder changed
        if(encoderPos != lastEncPos){
            lastEncPos = encoderPos;
            lcdSetCursor(1, 1);
            lcdPrint("         ");  // clear line
            lcdSetCursor(1, 1);
            lcdPrint(profile.profileName);
        }
    }
    
    while(1){           
        
        //delay(100);
    }
}