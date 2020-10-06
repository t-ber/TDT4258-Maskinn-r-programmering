#ifndef TIMER_H
#define TIMER_H

#define HF_FREQ 14000000
#define TIMER1_PRESC 0x3

void setupTimer(uint16_t period);
void setTimerFrequency(float freq);
void startTimer();
void stopTimer();

#endif