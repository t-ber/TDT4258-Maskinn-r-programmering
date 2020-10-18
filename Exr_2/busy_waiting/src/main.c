#include <stdint.h>
#include <stdbool.h>

#include "../inc/efm32gg.h"
#include "../inc/gpio.h"
#include "../inc/dac.h"
#include "../inc/polling.h"
#include "../inc/timer.h"
#include "../inc/rtc.h"
#include "../inc/music.h"

// Program start in main
int main(void)
{
	// Set up peripherals
	setupGPIO();
	setupDAC();
	setupTimer();
	setupRTC();

	// Set up the polling process
	setupPolling();

	// Play start-up melody
	playImperial();

	// Start polling
	while (1) {
		pollButtons();
		pollTimer1();
		pollRTC();
	}

	return 0;
}
