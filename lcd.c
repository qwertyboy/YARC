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
volatile uint8_t * lcdPort;
uint8_t lcdPin;
uint8_t spiBuf = 0x80;

// lcd pins
uint8_t dataPins[] = {6, 5, 4, 3};
// status variables
uint8_t displayControl = 0;
uint8_t displayMode = 0;


//------------------------------ User Commands ------------------------------//

// Description:
//      Initializes the lcd
// Arguments:
//      lcdPort (uint8_t *): A pointer to the port the lcd's cs pin is on
//      lcdPin (uint8_t): The pin the lcd's cs pin is connected to
void LcdInit(volatile uint8_t * port, uint8_t pin){
    // copy the port and pin for cs
    lcdPort = port;
    lcdPin = pin;
    
    // backlight command
    //spiBuf = 0x80;
    // delay roughly 50ms for lcd to start up
    Delay(50);
    // pull rs and enable low to send commands
    LcdSetPin(RS_PIN, 0);
    LcdSetPin(ENABLE_PIN, 0);
    
    // set 4 bit mode
    LcdWrite4Bits(0x03);
    Delay(4);
    LcdWrite4Bits(0x03);
    Delay(4);
    LcdWrite4Bits(0x03);
    DelayMicro(150);
    LcdWrite4Bits(0x02);
    
    // set line number
    LcdCommand(LCD_FUNCTIONSET | DISPLAY_FUNCTION);
    // turn on display with no cursor or blinking
    displayControl = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    LcdDisplay();
    // clear display
    LcdClear();
    
    // initialize text direction
    displayMode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
    LcdCommand(LCD_ENTRYMODESET | displayMode);
}


// Description:
//      Prints text to the lcd
// Arguments:
//      data (uint8_t *): A pointer to the text to print
void LcdPrint(const char * data){
    uint8_t i = 0;
    while(data[i] != '\0'){
        if(data[i] == '\n'){
            LcdSetCursor(0, 1);
            i++;
        }else{
            LcdWrite(data[i++]);
        }        
    }
}


// Description:
//      Clears the lcd
// Arguments:
//      None
void LcdClear(){
    LcdCommand(LCD_CLEARDISPLAY);
    Delay(2);
}


// Description:
//      Sends the cursor to the home position
// Arguments:
//      None
void LcdHome(){
    LcdCommand(LCD_RETURNHOME);
    Delay(2);
}


// Description:
//      Sets the cursor to the desired position
// Arguments:
//      col (uint8_t): The column to set the cursor to
//      row (uint8_t): The row to set the cursor to
void LcdSetCursor(uint8_t col, uint8_t row){
    uint8_t rowOffsets[] = {0x00, 0x40};
    LcdCommand(LCD_SETDDRAMADDR | (col + rowOffsets[row]));
}


// Description:
//      Turns the backlight on or off
// Arguments:
//      status (uint8_t): The value to set the backlight to
void LcdSetBacklight(uint8_t status){
    LcdSetPin(7, status);
}


// Description:
//      Turns the display on
// Arguments:
//      None
void LcdDisplay(){
    displayControl |= LCD_DISPLAYON;
    LcdCommand(LCD_DISPLAYCONTROL | displayControl);
}


// Description:
//      Turns on or off the underline cursor
// Arguments:
//      state (uint8_t): 1 to enable the cursor, 0 to disable
void LcdCursor(uint8_t state){
    if(state == 1){
        displayControl |= LCD_CURSORON;
    }else{
        displayControl &= ~(LCD_CURSORON);
    }
    LcdCommand(LCD_DISPLAYCONTROL | displayControl);
}


// Description:
//      Turns on or off the blinking cursor
// Arguments:
//      state (uint8_t): 1 to enable the cursor, 0 to disable
void LcdCursorBlink(uint8_t state){
    if(state == 1){
        displayControl |= LCD_BLINKON;
    }else{
        displayControl &= ~(LCD_BLINKON);
    }
    LcdCommand(LCD_DISPLAYCONTROL | displayControl);
}


// Description:
//      Creates a custom character for the lcd
// Arguments:
//      location (uint8_t): The location to store the character
//      charMap (uint8_t *): An array holding the custom character data. Can be
//                           generated here: https://www.quinapalus.com/hd44780udg.html
// charMap Layout:
//      0x10 0x08 0x04 0x02 0x01
//         x    x    x    x    x    charMap[0]
//         x    x    x    x    x    CharMap[1]
//         x    x    x    x    x    ...
//         x    x    x    x    x    ...
//         x    x    x    x    x    ...
//         x    x    x    x    x    ...
//         x    x    x    x    x    ...
//         x    x    x    x    x    charMap[7]
void LcdCreateChar(uint8_t location, uint8_t * charMap){
    // only 7 locations available so mask
    location &= 0x07;
    LcdCommand(LCD_SETCGRAMADDR | (location << 3));
    uint8_t i;
    for(i = 0; i < 8; i++){
        LcdWrite(charMap[i]);
    }
}


//---------------------------- Internal Commands ----------------------------//

// Description:
//      Wrappers for sending commands or data to the lcd
// Arguments:
//      value (uint8_t): The command or data to send to the lcd
inline void LcdCommand(uint8_t value){
    LcdSend(value, 0);
}
inline void LcdWrite(uint8_t value){
    LcdSend(value, 1);
}

// Description:
//      Sets the value of a pin on the io expander on the lcd backpack
// Arguments:
//      pin (uint8_t): The pin to change the value of
//      value (uint8_t): The value to write to the pin
void LcdSetPin(uint8_t pin, uint8_t value){
    if(value == 1){
        spiBuf |= (1 << pin);
    }else{
        spiBuf &= ~(1 << pin);
    }
    *lcdPort &= ~(1 << lcdPin);
    SpiTransfer(spiBuf);
    *lcdPort |= (1 << lcdPin);
}


// Description:
//      Pulses the enable pin on the lcd
// Arguments:
//      None
void LcdPulse(){
    LcdSetPin(ENABLE_PIN, 0);
    DelayMicro(1);
    LcdSetPin(ENABLE_PIN, 1);
    DelayMicro(1);
    LcdSetPin(ENABLE_PIN, 0);
    // Delay roughly 45us
    DelayMicro(50);
}


// Description:
//      Writes a 4 bit value to the lcd
// Arguments:
//      value (uint8_t): The value to write to the pins
void LcdWrite4Bits(uint8_t value){
    for(uint8_t i = 0; i < 4; i++){
        LcdSetPin(dataPins[i], (value >> i) & 0x01);
    }
    LcdPulse();
}


// Description:
//      Sends either a command or data to the lcd
// Arguments:
//      value (uint8_t): The value to send to the lcd
//      mode (uint8_t): 0 for command or 1 for data
void LcdSend(uint8_t value, uint8_t mode){
    LcdSetPin(RS_PIN, mode);
    LcdWrite4Bits(value >> 4);
    LcdWrite4Bits(value);
}