/*
 * utils.c
 *
 * Created: 3/8/2018 9:56:30 PM
 *  Author: Nathan
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "utils.h"


#define MILLIS_INC  2.048
#define FRACT_INC   6
#define FRACT_MAX   125

volatile uint32_t timerOverflowCount = 0;
volatile uint32_t timerMillis = 0;
static uint8_t timerFract = 0;


ISR(TIMER0_OVF_vect){
    uint32_t m = timerMillis;
    uint8_t f = timerFract;
    
    m += MILLIS_INC;
    f += FRACT_INC;
    if(f >= FRACT_MAX){
        f -= FRACT_MAX;
        m += 1;
    }
    
    timerFract = f;
    timerMillis = m;
    timerOverflowCount++;
}


// Description:
//      Initializes the timer for timekeeping. Must be called for delays to work!
// Arguments:
//      None
void timerInit(void){
    // enable interrupts
    sei();
    // fast pwm, probably not needed for this application (not arduino)
    TCCR0A |= (1 << WGM01) | (1 << WGM00);
    // clk/64
    TCCR0B |= (1 << CS01) | (1 << CS00);
    // enable overflow interrupt
    TIMSK0 |= (1 << TOIE0);
}


// Description:
//      Gets the current number of milliseconds since the program started
// Arguments:
//      None
// Returns:
//      The number of milliseconds since the program started
uint32_t millis(void){
    uint32_t m;
    uint8_t oldSREG = SREG;
    
    cli();
    m = timerMillis;
    SREG = oldSREG;
    
    return m;
}


// Description:
//      Gets the current number of microseconds since the program started
// Arguments:
//      None
// Returns:
//      The number of microseconds since the program started
uint32_t micros(void){
    uint32_t m;
    uint8_t oldSREG = SREG, t;
    
    cli();
    m = timerOverflowCount;
    t = TCNT0;
    
    if((TIFR0 & (1 << TOV0)) && (t < 255)){
        m++;
    }
    
    SREG = oldSREG;
    
    return ((m << 8) + t) * 8;
}


// Description:
//      Does nothing for the specified number of milliseconds
// Arguments:
//      ms (uint32_t): The number of milliseconds to wait
void delay(uint32_t ms){
    uint32_t start = micros();
    
    while(ms > 0){
        while(ms > 0 && (micros() - start) >= 1000){
            ms--;
            start += 1000;
        }
    }
}


// Description:
//      Delays the specified number of microseconds
// Arguments:
//      us (uint32_t): The number of microseconds to delay for
void delayMicro(uint32_t us){
    // for a 1 and 2 microsecond delay, simply return.  the overhead
    // of the function call takes 14 (16) cycles, which is 2us
    if (us <= 2) return; //  = 3 cycles, (4 when true)

    // the following loop takes 1/2 of a microsecond (4 cycles)
    // per iteration, so execute it twice for each microsecond of
    // delay requested.
    us <<= 1; //x2 us, = 2 cycles

    // account for the time taken in the preceeding commands.
    // we just burned 17 (19) cycles above, remove 4, (4*4=16)
    // us is at least 6 so we can substract 4
    us -= 4; // = 2 cycles
    
    // busy wait
    __asm__ __volatile__ (
    "1: sbiw %0,1" "\n\t" // 2 cycles
    "brne 1b" : "=w" (us) : "0" (us) // 2 cycles
    );
    // return = 4 cycles
}


//
uint8_t buttonRead(volatile uint8_t * pinx, uint8_t pin, uint16_t shortTimeout, uint16_t longTimeout){
    static uint8_t pressed = 0;
    static uint32_t downTime = 0;
    uint32_t upTime = 0;
    
    // check pin
    if((*pinx & (1 << pin)) == 0){
        if(!pressed){
            // set flag on press to get time
            pressed = 1;
            downTime = millis();
        }

        // return if passed long timeout and still held
        if(millis() - downTime >= longTimeout){
            return 2;
        }
    }else{
        if(pressed){
            // reset flag
            pressed = 0;
            // get release time if button was pressed
            upTime = millis();
            
            // determine what kind of press happened
            if(upTime - downTime >= longTimeout){
                // reset downTime to prevent returning long press
                downTime = 0;
                return 2;
            }else if(upTime - downTime >= shortTimeout){
                // reset downTime to prevent returning long press
                downTime = 0;
                return 1;
            }else{
                return 0;
            }
        }
    }
    // return 0 if nothing returned already
    return 0;
}    


// Description:
//      Swaps two numbers
// Arguments:
//      a (uint8_t *): Pointer to variable a
//      b (uint8_t *): Pointer to variable b
void swap(uint8_t * a, uint8_t * b){
    // dont do anything if identical
    if(a == b){
        return;
    }
    // use xor trick to swap
    *a = *a ^ *b;
    *b = *a ^ *b;
    *a = *a ^ *b;
}


// Description:
//      Reverses a string
// Arguments:
//      str (uint8_t *): A pointer to the string to reverse
//      len (uint8_t): The length of the string
void reverse(uint8_t * str, uint8_t len){
    uint8_t start = 0;
    uint8_t end = len - 1;
    while(start < end){
        swap(str + start, str + end);
        start++;
        end--;
    }
}


// Description:
//      Converts an integer to a character array
// Arguments:
//      num (int32_t): The number to convert
//      buf (uint8_t *): A pointer to a buffer to place the converted number
// Returns:
//      uint8_t *: A pointer to the converted number
uint8_t * int2str(int32_t num, uint8_t * buf){
    uint8_t len = 0;
    uint8_t isNeg = 0;
    
    // check if number is 0
    if(num == 0){
        buf[len++] = '0';
        buf[len] = '\0';
        return buf;
    }
    
    // check if negative
    if(num < 0){
        isNeg = 1;
        num = -num;
    }
    
    // process the number
    while(num != 0){
        uint8_t remainder = num % 10;
        buf[len++] = remainder + '0';
        num = num / 10;
    }
    
    // append '-' if negative
    if(isNeg){
        buf[len++] = '-';
    }
    
    // append null and reverse
    buf[len] = '\0';
    reverse(buf, len);
    return buf;
}