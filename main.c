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

// process states
#define STATE_PREHEAT   0
#define STATE_SOAK      1
#define STATE_REFLOW    2
#define STATE_COOLDOWN  3

// PID constants
#define PID_KP 1
#define PID_KI 1
#define PID_KD 1

// misc variables
char printBuf[16];

int main(void){
    //-----------------------------Initialization----------------------------//
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
    
    //---------------------------End Initialization--------------------------//
    
    lcdPrint("   YetAnother\nReflowController");
    delay(2000);
    lcdClear();
    
    // profile vars
    Profile_t profile = Profiles[0];    // default to lead
    
    // wait for button to select a profile.
    lcdPrint("Select Profile:");
    while(buttonRead(&PINC, PINC2, 50, 1000) != 1){
        // read encoder
        int16_t encoderPos = encoderRead();
        static int16_t lastEncPos = -1;
        
        // check bounds
        if(encoderPos < 0){
            encoderPos = 0;
            encoderSetPos(0);
        }else if(encoderPos > NUM_PROFILES - 1){
            encoderPos = NUM_PROFILES - 1;
            encoderSetPos(NUM_PROFILES);
        }
        
        // get the profile selected by the encoder
        profile = Profiles[encoderPos];
        // update display if encoder changed
        if(encoderPos != lastEncPos){
            lastEncPos = encoderPos;
            lcdSetCursor(1, 1);
            lcdPrint("                ");  // clear line
            lcdSetCursor(1, 1);
            lcdPrint(profile.profileName);
        }
    }
    
    // display selected profile and wait for button to be held
    lcdClear();
    lcdPrint("Profile: ");
    lcdPrint(profile.profileName);
    lcdSetCursor(0, 1);
    lcdPrint("Hold start...");
    
    // blink button while waiting
    uint32_t blinkTime = 0;
    while(buttonRead(&PINC, PINC2, 50, 1000) != 2){
        if(millis() - blinkTime >= 1000){
            blinkTime = millis();
            PIND |= (1 << PIND0);
        }
    }
    
    lcdClear();
    lcdPrint("Phase: ");
    while(1){
        static uint8_t phase = 0;
        static uint32_t loop10hz = 0;
        // pid variables
        static uint16_t ovenTemp = 0;
        static int16_t pidError = 0;
        static int16_t pidLastError = 0;
        static int16_t pidIntegral = 0;
        static int16_t pidDerivative = 0;
        static int16_t pidOut = 0;
        
        // 10 hz loop
        if(millis() - loop10hz >= 100){
            loop10hz = millis();
            // update lcd
            lcdSetCursor(7, 0);
            lcdPrint("        ");
            lcdSetCursor(7, 0);
            lcdPrint(PhaseText[phase]);
            
            // update PID parameters
            ovenTemp = max6675Read() / 4;
            pidError = profile.preHeatTemp - ovenTemp;
            pidIntegral += pidError;
            pidDerivative = pidError - pidLastError;
            // update control variable
            pidOut = (PID_KP * pidError) + (PID_KI * pidIntegral) + (PID_KD * pidDerivative);
            lcdSetCursor(0, 1);
            lcdPrint("p:");
            itoa(pidOut, printBuf, 10);
            lcdPrint(printBuf);
            lcdPrint(" t:");
            itoa(ovenTemp, printBuf, 10);
            lcdPrint(printBuf);
        }
    }
}