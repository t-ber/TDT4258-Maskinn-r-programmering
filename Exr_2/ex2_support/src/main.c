#include <stdint.h>
#include <stdbool.h>

#include "../inc/efm32gg.h"
#include "../inc/gpio.h"
#include "../inc/dac.h"
#include "../inc/timer.h"
#include "../inc/rtc.h"
#include "../inc/polling.h"

/*
 * Your code will start executing here 
 */
int main(void)
{
	/*
	 * Call the peripheral setup functions 
	 */
	setupGPIO();
	setupDAC();
	setupTimer(C4);
	setupRTC();

	stopTimer();

	setupPolling();

	while (1) {
		pollButtons();
		pollTimer1();
		pollRTC();
	}

	return 0;
}
