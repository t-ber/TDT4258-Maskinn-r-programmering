#include <stdint.h>
#include <stdbool.h>

#include "../inc/interrupt_handlers.h"
#include "../inc/gpio.h"
#include "../inc/efm32gg.h"
#include "../inc/sound.h"

/*
 * TIMER1 interrupt handler 
 */
void __attribute__ ((interrupt)) TIMER1_IRQHandler()
{	
	*TIMER1_IFC = 1;
	static volatile bool flank = true;

	uint16_t data;
	if (flank) {
		// data = DAC_MIDDLE + AMPLITUDE / 2;
		data = 1000;
	}
	else {
		// data = DAC_MIDDLE - AMPLITUDE / 2;
		data = 0;
	}

	*DAC0_COMBDATA = (data << 16) | data;
	flank = !flank;
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
}
