#include <stdint.h>
#include <stdbool.h>

#include "../inc/dac.h"
#include "../inc/efm32gg.h"

void setupDAC()
{
	/*
	 * TODO enable and set up the Digital-Analog Converter
	 * 
	 * 1. Enable the DAC clock by setting bit 17 in CMU_HFPERCLKEN0 2.
	 * Prescale DAC clock by writing 0x50010 to DAC0_CTRL 3. Enable left
	 * and right audio channels by writing 1 to DAC0_CH0CTRL and
	 * DAC0_CH1CTRL 4. Write a continuous stream of samples to the DAC
	 * data registers, DAC0_CH0DATA and DAC0_CH1DATA, for example from a
	 * timer interrupt 
	 */

	// Step 1
	*CMU_HFPERCLKEN0 |= (1 << 17);

	// Step 2
	*DAC0_CTRL = 0x50010;

	// Step 3
	*DAC0_CH0CTRL = 1;
	*DAC0_CH1CTRL = 1;

	// Step 4 er nok best å gjøre i interrupt, evt main for busy-waiting?
}
