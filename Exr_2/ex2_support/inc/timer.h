#ifndef TIMER_H
#define TIMER_H

#define   G3   4464
#define   A3   3977
#define   B3   3543
#define   C4   3344
#define   D4   2980
#define   E4   2665
#define   F4   2506
#define   G4   2232
#define   A4   1989
#define   B4   1772

#define   A4S  1877

void setupTimer(uint16_t period);
void setTimerTop(uint16_t top);
void stopTimer();

#endif