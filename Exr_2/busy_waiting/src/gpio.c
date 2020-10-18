#include <stdint.h>
#include <stdbool.h>

#include "../inc/gpio.h"
#include "../inc/efm32gg.h"
#include "../inc/dac.h"
#include "../inc/music.h"

// Set up GPIO peripheral
void setupGPIO()
{
	// Enable GPIO clock
	*CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_GPIO;

	// Set button pins to input
	*GPIO_PC_MODEL = 0x33333333;
	// Enable internal pull-up for buttons
	*GPIO_PC_DOUT = 0xff;

	// Set interrupts for falling edge on buttons
	*GPIO_EXTIPSELL = 0x22222222;
	*GPIO_EXTIFALL = 0xff;
	*GPIO_IEN = 0xff;
}

// Function called when buttons are pressed
void onButtonPress(uint32_t flags)
{
	// SW1 pressed
	if (flags & 0x01) {
		playLisa();
	}

	// SW2 pressed
	if (flags & 0x02) {
		playImperial();
	}

	// SW3 pressed
	if (flags & 0x04) {
		playPirates();
	}

	// SW8 pressed
	if (flags & 0x80) {
		stopPlaying();
	}
}