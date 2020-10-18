#ifndef TIMER_H
#define TIMER_H

#define CLOCK_SPEED 14000000

void setupTimer();
void startTimer(float freq);
void stopTimer();

uint16_t freqToTop(float freq);

#endif