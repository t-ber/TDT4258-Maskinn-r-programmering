#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include "../inc/timer.h"
#include "../inc/efm32gg.h"

void setupTimer(uint16_t initial_top)
{	
	// Set prescaling
	*TIMER1_CTRL |= TIMER1_PRESC << 24; // Prescaling factor of 8

	// Normal setup steps
	*CMU_HFPERCLKEN0 = (1 << 6);
	*TIMER1_TOP = initial_top;
	*TIMER1_IEN = 1;
	*ISER0 = (1 << 12);
	*TIMER1_CMD = 0b10; // Don't start counting immediately
}

void setTimerFrequency(float freq)
{
	uint16_t new_top = HF_FREQ / (pow(2, TIMER1_PRESC) * freq);

	if (new_top < 0) {
		new_top = 0;
	}
	else if (new_top >= pow(2, 16)) {
		new_top = pow(2, 16) - 1;
	}

	*TIMER1_TOPB = new_top;
}

void startTimer()
{
	*TIMER1_CMD = 0b01;
}

void stopTimer()
{
	*TIMER1_CMD = 0b10;
}