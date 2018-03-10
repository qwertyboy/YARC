/*
 * lcd.c
 *
 * Created: 3/8/2018 9:02:17 PM
 *  Author: Nathan
 */ 

#include <avr/io.h>
#include "utils.h"
#include "lcd.h"
#include "spi.h"

// spi pins
volatile uint8_t * csPort;
uint8_t csPin;
uint8_t spiBuf = 0x80;

// lcd pins and vars
uint8_t dataPins[] = {6, 5, 4, 3};
#define DISPLAY_FUNCTION 0x08
uint8_t displayControl = 0;
uint8_t displayMode = 0;


// Description:
//      Initializes the lcd
// Arguments:
//      lcdPort (uint8_t *): A pointer to the port the lcd's cs pin is on
//      lcdPin (uint8_t): The pin the lcd's cs pin is connected to
void LCDInit(volatile uint8_t * lcdPort, uint8_t lcdPin){
    // copy the port and pin for cs
    csPort = lcdPort;
    csPin = lcdPin;
    
    // backlight command
    //spiBuf = 0x80;
    // delay roughly 50ms for lcd to start up
    delay(25000);
    // pull rs and enable low to send commands
    LCDSetPin(RS_PIN, 0);
    LCDSetPin(ENABLE_PIN, 0);
    
    // try 4 bit mode 3 times
    LCDWrite4Bits(0x03);
    delay(1);
    LCDWrite4Bits(0x03);
    delay(1);
    LCDWrite4Bits(0x03);
    delay(1);
    LCDWrite4Bits(0x02);
    
    // set line number
    LCDCommand(LCD_FUNCTIONSET | DISPLAY_FUNCTION);
    // turn on display with no cursor or blinking
    displayControl = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    LCDDisplay();
    // clear display
    LCDClear();
    
    // initialize text direction
    displayMode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
    LCDCommand(LCD_ENTRYMODESET | displayMode);
}


void LCDPrint(uint8_t * data, uint8_t length){
    while(length > 0){
        LCDWrite(*data++);
        length--;
    }
}

// Description:
//      Clears the lcd
void LCDClear(){
    LCDCommand(LCD_CLEARDISPLAY);
    delay(1000);
}

// Description:
//
void LCDHome(){
    LCDCommand(LCD_RETURNHOME);
    delay(1000);
}

void LCDSetCursor(uint8_t col, uint8_t row){
    uint8_t rowOffsets[] = {0x00, 0x40};
    LCDCommand(LCD_SETDDRAMADDR | (col + rowOffsets[row]));
}


// Description:
//      Turns the backlight on or off
// Arguments:
//      status (uint8_t): The value to set the backlight to
void LCDSetBacklight(uint8_t status){
    LCDSetPin(7, status);
}

void LCDDisplay(){
    displayControl |= LCD_DISPLAYON;
    LCDCommand(LCD_DISPLAYCONTROL | displayControl);
}

// Description:
//      Wrappers for sending commands or data to the lcd
// Arguments:
//      value (uint8_t): The command or data to send to the lcd
inline void LCDCommand(uint8_t value){
    LCDSend(value, 0);
}

inline void LCDWrite(uint8_t value){
    LCDSend(value, 1);
}

// Description:
//      Sets the value of a pin on the io expander on the lcd backpack
// Arguments:
//      pin (uint8_t): The pin to change the value of
//      value (uint8_t): The value to write to the pin
void LCDSetPin(uint8_t pin, uint8_t value){
    if(value == 1){
        spiBuf |= (1 << pin);
    }else{
        spiBuf &= ~(1 << pin);
    }
    SPITransfer(spiBuf, csPort, csPin);
}


// Description:
//      Pulses the enable pin on the lcd
// Arguments:
//      None
void LCDPulse(){
    LCDSetPin(ENABLE_PIN, 0);
    LCDSetPin(ENABLE_PIN, 1);
    LCDSetPin(ENABLE_PIN, 0);
    // delay roughly 45us
    delay(15);
}


// Description:
//      Writes a 4 bit value to the lcd
// Arguments:
//      value (uint8_t): The value to write to the pins
void LCDWrite4Bits(uint8_t value){
    for(uint8_t i = 0; i < 4; i++){
        LCDSetPin(dataPins[i], (value >> i) & 0x01);
    }
    LCDPulse();
}


// Description:
//      Sends either a command or data to the lcd
// Arguments:
//      value (uint8_t): The value to send to the lcd
//      mode (uint8_t): 0 for command or 1 for data
void LCDSend(uint8_t value, uint8_t mode){
    LCDSetPin(RS_PIN, mode);
    LCDWrite4Bits(value >> 4);
    LCDWrite4Bits(value);
}