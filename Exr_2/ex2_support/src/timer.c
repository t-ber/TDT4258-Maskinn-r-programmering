#include <stdint.h>
#include <stdbool.h>

#include "../inc/timer.h"
#include "../inc/efm32gg.h"
#include "../inc/interrupt_handlers.h"

/*
 * function to setup the timer 
 */
void setupTimer(uint16_t period)
{
	/*
	 * TODO enable and set up the timer
	 * 
	 * 1. Enable clock to timer by setting bit 6 in CMU_HFPERCLKEN0 2.
	 * Write the period to register TIMER1_TOP 3. Enable timer interrupt
	 * generation by writing 1 to TIMER1_IEN 4. Start the timer by writing 
	 * 1 to TIMER1_CMD
	 * 
	 * This will cause a timer interrupt to be generated every (period)
	 * cycles. Remember to configure the NVIC as well, otherwise the
	 * interrupt handler will not be invoked. 
	 */

	*TIMER1_CTRL |= (0x7 << 24);
	*CMU_HFPERCLKEN0 |= (1 << 6);
	*TIMER1_TOP = period;
	*TIMER1_IEN = 1;
	*TIMER1_CMD = 1;
}

void setTimerTop(uint16_t top)
{
	*SCR = 2; // Regular sleep mode
	*TIMER1_TOP = top;
	*TIMER1_CMD = 0b01;
}

void stopTimer()
{
	*SCR = 6; // Deep sleep mode
	*TIMER1_CMD = 0b10;
}