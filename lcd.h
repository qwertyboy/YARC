/*
 * lcd.h
 *
 * Created: 3/8/2018 9:02:28 PM
 *  Author: Nathan
 */ 


#ifndef LCD_H_
#define LCD_H_

// pins
#define ENABLE_PIN  2
#define RS_PIN      1

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00
// 4 bit mode, 2 line, 5x8 dots
#define DISPLAY_FUNCTION 0x08


// user commands
void LcdInit(volatile uint8_t * port, uint8_t pin);
void LcdPrint(const char * data);
void LcdClear();
void LcdHome();
void LcdSetCursor(uint8_t col, uint8_t row);
void LcdSetBacklight(uint8_t status);
void LcdDisplay();
void LcdCursor(uint8_t state);
void LcdCursorBlink(uint8_t state);
void LcdCreateChar(uint8_t location, uint8_t * charMap);

// internal commands
void LcdSetPin(uint8_t pin, uint8_t value);
void LcdPulse();
void LcdWrite4Bits(uint8_t value);
void LcdSend(uint8_t value, uint8_t mode);
void LcdCommand(uint8_t value);
void LcdWrite(uint8_t value);


#endif /* LCD_H_ */