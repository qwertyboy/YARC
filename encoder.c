/*
 * encoder.c
 *
 * Created: 3/12/2018 5:55:12 PM
 *  Author: Nathan
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "encoder.h"

uint8_t encPinA, encPinB = 0;
volatile int32_t encPos = 0;

// Handles PORTB
ISR(PCINT0_vect){
    // check which pin interrupt occurred on
    if(PINB & (1 << encPinA)){
        // pin A went low first so encoder is turning clockwise
        encPos++;
    }else{
        // pin B went low first so ccw
        encPos--;
    }
}

// Handles PORTC
ISR(PCINT1_vect){
    // check which pin interrupt occurred on
    if(PINC & (1 << encPinA)){
        // pin A went low first so encoder is turning clockwise
        encPos++;
        }else{
        // pin B went low first so ccw
        encPos--;
    }
}

// Handles PORTD
ISR(PCINT2_vect){
    // check which pin interrupt occurred on
    if(PIND & (1 << encPinA)){
        // pin A went low first so encoder is turning clockwise
        encPos++;
        }else{
        // pin B went low first so ccw
        encPos--;
    }
}


// Description:
//      Initializes the appropriate pins and interrupts for a rotary encoder
// Arguments:
//      port (uint8_t *): A pointer to the port the encoder is connected to
//      pinA (uint8_t): The pin the A phase of the encoder is connected to
//      pinB (uint8_t): The pin the B phase of the encoder is connected to
void encoderInit(volatile uint8_t * port, uint8_t pinA, uint8_t pinB){
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
int32_t encoderRead(void){
    return encPos;
}