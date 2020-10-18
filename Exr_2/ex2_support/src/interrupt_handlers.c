#include <stdint.h>
#include <stdbool.h>

#include "../inc/interrupt_handlers.h"
#include "../inc/efm32gg.h"
#include "../inc/gpio.h"
#include "../inc/timer.h"
#include "../inc/rtc.h"
#include "../inc/dac.h"
/*
 * TIMER1 interrupt handler 
 */

void __attribute__ ((interrupt)) TIMER1_IRQHandler()
{
	/*
	 * TODO feed new samples to the DAC remember to clear the pending
	 * interrupt by writing 1 to TIMER1_IFC 
	 */

	static volatile uint16_t alternating_bool = 0;

	if (alternating_bool == 0) {
		alternating_bool = 1;
		*DAC0_CH0DATA = AMPLITUDE;
		*DAC0_CH1DATA = AMPLITUDE;
		// turnOnLed();
	} else {
		alternating_bool = 0;
		*DAC0_CH0DATA = 0;
		*DAC0_CH1DATA = 0;
		// turnOffLed();
	}

	*TIMER1_IFC = 1;
}

/*
 * GPIO even pin interrupt handler 
 */
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler()
{
	/*
	 * TODO handle button pressed event, remember to clear pending
	 * interrupt 
	 */
	uint32_t IF = *GPIO_IF;
	*GPIO_IFC = IF;

	if (IF & 0x01) {
		// setTimerTop(C4);
		playLisa();
	}

	if (IF & 0x04) {
		// setTimerTop(E4);
		playPirates();
	}

	if (IF & 0x10) {
		// setTimerTop(G4);
	}

	if (IF & 0x40) {
		// setTimerTop(B4);
	}
}

/*
 * GPIO odd pin interrupt handler 
 */
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler()
{
	/*
	 * TODO handle button pressed event, remember to clear pending
	 * interrupt 
	 */
	uint32_t IF = *GPIO_IF;
	*GPIO_IFC = IF;

	if (IF & 0x02) {
		// setTimerTop(D4);
		playImperial();
	}

	if (IF & 0x08) {
		// setTimerTop(F4);
	}

	if (IF & 0x20) {
		// setTimerTop(A4);
	}

	if (IF & 0x80) {
		stopPlaying();
	}
	
}

void __attribute__ ((interrupt)) RTC_IRQHandler()
{
	*RTC_IFC = *RTC_IF;
	clearRTC();
	stopTimer();
	onNoteCleared();
}