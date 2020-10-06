#include <stdint.h>
#include <stdbool.h>

#include "../inc/dac.h"
#include "../inc/efm32gg.h"
#include "../inc/timer.h"

void setupDAC()
{
	// Step 1
	*CMU_HFPERCLKEN0 |= (1 << 17);

	// Step 2
	*DAC0_CTRL = 0x50010;

	// Step 3
	*DAC0_CH0CTRL = 1;
	*DAC0_CH1CTRL = 1;
	
}

void startSoundAtPitch(float pitch)
{
	setTimerFrequency(pitch * 2);
	startTimer();
}

void stopSound()
{
	stopTimer();
}