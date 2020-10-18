#include <stdint.h>
#include <stdbool.h>

#include "../inc/dac.h"
#include "../inc/efm32gg.h"
#include "../inc/timer.h"
#include "../inc/rtc.h"
#include "../inc/gpio.h"

// Set up the DAC peripheral
void setupDAC()
{
	// Enable clock for DAC
	*CMU_HFPERCLKEN0 |= (1 << 17);

	// Prescale DAC clock
	*DAC0_CTRL = 0x50010;

	// Enable both channels of the DAC
	*DAC0_CH0CTRL = 1;
	*DAC0_CH1CTRL = 1;
}

// Alternate the sample value in the DAC
// (We do this every half period to generate the desired frequency)
void alternateDACValue() {
	static volatile uint16_t alternating_bool = 0;

	if (alternating_bool == 0) {
		alternating_bool = 1;
		*DAC0_CH0DATA = AMPLITUDE;
		*DAC0_CH1DATA = AMPLITUDE;
	}
	else {
		alternating_bool = 0;
		*DAC0_CH0DATA = 0;
		*DAC0_CH1DATA = 0;
	}
}
