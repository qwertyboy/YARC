/*
 * encoder.c
 *
 * Created: 3/12/2018 5:55:12 PM
 *  Author: Nathan
 *
 *  Using the method described here to to decoding: http://www.mkesc.co.uk/ise.pdf
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "encoder.h"

uint8_t encPinA = 0;
uint8_t encPinB = 0;

// interrupt variables
volatile int32_t encPos = 0;
volatile uint8_t state = 0;
const int8_t stateTable[] = {0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0};


// Handles PORTB
ISR(PCINT0_vect){
    state = state << 2;                                 // move old data over
    state |= (PINB & (1 << encPinA)) >> encPinA;        // OR the A bit
    state |= ((PINB & (1 << encPinB)) >> encPinB) << 1; // OR the B bit
    encPos += stateTable[(state & 0x0F)];  // get the change from the entry table
}

// Handles PORTC
ISR(PCINT1_vect){
    state = state << 2;                                 // move old data over
    state |= (PINC & (1 << encPinA)) >> encPinA;        // OR the A bit
    state |= ((PINC & (1 << encPinB)) >> encPinB) << 1; // OR the B bit
    encPos += stateTable[(state & 0x0F)];  // get the change from the entry table
}

// Handles PORTD
ISR(PCINT2_vect){
    state = state << 2;                                 // move old data over
    state |= (PIND & (1 << encPinA)) >> encPinA;        // OR the A bit
    state |= ((PIND & (1 << encPinB)) >> encPinB) << 1; // OR the B bit
    encPos += stateTable[(state & 0x0F)];  // get the change from the entry table
}


// Description:
//      Initializes the appropriate pins and interrupts for a rotary encoder
// Arguments:
//      port (uint8_t *): A pointer to the port the encoder is connected to
//      pinA (uint8_t): The pin the A phase of the encoder is connected to
//      pinB (uint8_t): The pin the B phase of the encoder is connected to
void EncoderInit(volatile uint8_t * port, uint8_t pinA, uint8_t pinB){
    // disable interrupts
    SREG &= ~(1 << SREG_I);
    
    // pins
    encPinA = pinA;
    encPinB = pinB;
    
    // set pins as inputs, DDRx address is one less than PORTx
    *(port - 0x01) &= ~((1 << encPinA) | (1 << encPinB));
    // enable pull-ups
    *port |= (1 << encPinA) | (1 << encPinB);
    
    // enable appropriate pin change interrupt
    switch((intptr_t)port){
        case 0x25:  // PORTB - PCINT0-7
            PCICR |= (1 << PCIE0);
            PCMSK0 |= (1 << encPinA) | (1 << encPinB);
            break;
        case 0x28:  // PORTC - PCINT8-14
            PCICR |= (1 << PCIE1);
            PCMSK1 |= (1 << encPinA) | (1 << encPinB);
            break;
        case 0x2B:  // PORTD - PCINT16-23
            PCICR |= (1 << PCIE2);
            PCMSK2 |= (1 << encPinA) | (1 << encPinB);
            break;
        default:
            break;
    }
    
    // enable interrupts
    SREG |= (1 << SREG_I);
}


// Description:
//      Gets the current encoder position
// Arguments:
//      None
// Returns:
//      int32_t: The current encoder position
int32_t EncoderRead(void){
    if(encPos != 0){
        return (encPos - 2) / 4;
    }else{
        return encPos;
    }        
}


// Description:
//      Sets the encoder position manually
// Arguments:
//      pos (int32_t): The position to set the encoder to
void EncoderSetPos(int32_t pos){
    encPos = pos * 4;
}