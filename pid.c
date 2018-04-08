/*
 * pid.c
 *
 * Created: 4/5/2018 9:11:56 PM
 *  Author: Nathan
 */ 

#include <avr/io.h>
#include "pid.h"

// PID variables
int16_t pidTarget = 0;
int16_t pidError = 0;
int16_t pidLastError = 0;
int16_t pidIntegral = 0;
int16_t pidDerivative = 0;
int16_t pidOut = 0;


// Description:
//      Sets the target for the PID controller to reach
// Arguments:
//      target (int16_t): The target to reach
void PidSetTarget(int16_t target){
    pidTarget = target;
}


// Description:
//      Resets all the PID control variables
// Arguments:
//      None
void PidReset(void){
    pidError = 0;
    pidLastError = 0;
    pidIntegral = 0;
    pidDerivative = 0;
    pidOut = 0;
}


// Description:
//      Updates the PID output
// Arguments:
//      input (int16_t): The input to the PID controller
// Returns:
//      int16_t: The newly calculated output from the PID controller
int16_t PidUpdate(int16_t input){
    // update PID parameters
    // calc error
    pidError = pidTarget - input;
    if(pidError < -PID_MAX_VAL){
        pidError = -PID_MAX_VAL;
        }else if(pidError > PID_MAX_VAL){
        pidError = PID_MAX_VAL;
    }
            
    // calc integral
    pidIntegral += pidError;
    if(pidIntegral < -PID_MAX_VAL){
        pidIntegral = -PID_MAX_VAL;
        }else if(pidIntegral > PID_MAX_VAL){
        pidIntegral = PID_MAX_VAL;
    }
            
    // calc derivative
    pidDerivative = pidError - pidLastError;
    if(pidDerivative < -PID_MAX_VAL){
        pidDerivative = -PID_MAX_VAL;
        }else if(pidDerivative > PID_MAX_VAL){
        pidDerivative = PID_MAX_VAL;
    }
            
    // update control variable
    pidOut = (PID_KP * pidError) + (PID_KI * pidIntegral) + (PID_KD * pidDerivative);
    if(pidOut < 0){
        pidOut = 0;
        }else if(pidOut > PID_MAX_VAL){
        pidOut = PID_MAX_VAL;
    }
    
    return pidOut;
}    