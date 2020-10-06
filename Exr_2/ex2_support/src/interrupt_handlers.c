#include <stdint.h>
#include <stdbool.h>

#include "../inc/interrupt_handlers.h"
#include "../inc/efm32gg.h"
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
	} else {
		alternating_bool = 0;
		*DAC0_CH0DATA = 0;
		*DAC0_CH1DATA = 0;
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
