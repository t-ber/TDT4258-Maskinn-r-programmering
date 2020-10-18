#include <stdint.h>
#include <stdbool.h>

#include "../inc/timer.h"
#include "../inc/efm32gg.h"

// Set up the timer peripheral
void setupTimer()
{
	// Prescale clock
	*TIMER1_CTRL |= (0x7 << 24);

	// Enable clock for timer
	*CMU_HFPERCLKEN0 |= (1 << 6);
}

// Start the timer with the specified frequency
void startTimer(float freq)
{
	// Enable energy mode EM1 (sleep)
	// Because thet timer is not available in EM2
	*SCR = 2;

	// Calculate the needed top
	uint16_t top = freqToTop(freq);

	// Set the top, and start the counter
	*TIMER1_TOP = top;
	*TIMER1_CMD = 0b01;
}

// Stop the timer
void stopTimer()
{
	// Stop the counter
	*TIMER1_CMD = 0b10;

	// Enable energy mode EM2 (deep sleep)
	*SCR = 6;
}

// Convert a frequency to the corresponding TOP register value
uint16_t freqToTop(float freq)
{
	return (uint16_t) (CLOCK_SPEED / (16 * freq));
}