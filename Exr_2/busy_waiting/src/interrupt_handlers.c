#include <stdint.h>
#include <stdbool.h>

#include "../inc/interrupt_handlers.h"
#include "../inc/efm32gg.h"
#include "../inc/gpio.h"
#include "../inc/timer.h"
#include "../inc/rtc.h"
#include "../inc/dac.h"
#include "../inc/music.h"

// Timer1 interrupt handler
void __attribute__ ((interrupt)) TIMER1_IRQHandler()
{
	*TIMER1_IFC = 1;

	alternateDACValue();
}

// GPIO even pin interrupt handler
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler()
{
	uint32_t IF = *GPIO_IF;
	*GPIO_IFC = IF;

	onButtonPress(IF);
}

// GPIO odd pin interrupt handler
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler()
{
	uint32_t IF = *GPIO_IF;
	*GPIO_IFC = IF;

	onButtonPress(IF);
}

// RTC interrupt handler
void __attribute__ ((interrupt)) RTC_IRQHandler()
{
	*RTC_IFC = *RTC_IF;

	clearRTC();
	stopTimer();
	onNoteCleared();
}
