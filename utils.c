/*
 * utils.c
 *
 * Created: 3/8/2018 9:56:30 PM
 *  Author: Nathan
 */ 

#include <avr/io.h>

// Description:
//      Does nothing for the specified number of ticks. Does not seem to be linear
// Arguments:
//      ticks (uint32_t): The number of times to loop
void delay(volatile uint32_t ticks){
    uint32_t i;
    for(i = 0; i < ticks; i++);
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