/*
 * pid.h
 *
 * Created: 4/5/2018 9:12:08 PM
 *  Author: Nathan
 */ 


#ifndef PID_H_
#define PID_H_

// tuning constants
#define PID_KP      20
#define PID_KI      0.1
#define PID_KD      10

#define PID_MAX_VAL         1000
#define PID_WINDOW_SIZE     2000
#define PID_HEATER_TIMEOUT  5000
#define PID_REHEAT_TIME     5000


void PidSetTarget(int16_t target);
void PidReset(void);
int16_t PidUpdate(int16_t input);

#endif /* PID_H_ */