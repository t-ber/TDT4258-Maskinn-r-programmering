#include <stdint.h>
#include <stdbool.h>

#include "../inc/gpio.h"
#include "../inc/efm32gg.h"



/*
 * function to set up GPIO mode and interrupts
 */
void setupGPIO()
{
	/*
	 * TODO set input and output pins for the joystick 
	 */
		
	/*
	 * Example of HW access from C code: turn on joystick LEDs D4-D8 check 
	 * efm32gg.h for other useful register definitions 
	 */
	*CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_GPIO;	/* enable GPIO clock */
	*GPIO_PA_CTRL = 2;	/* set high drive strength */
	*GPIO_PA_MODEH = 0x55555555;	/* set pins A8-15 as output */
	*GPIO_PA_DOUT = 0x0700;	/* turn on LEDs D4-D8 (LEDs are active
				 * low) */
}	  

void turnOnLed(uint8_t led_num){
	uint8_t regOffset = led_num + REG_LED_OFFSET;
	*GPIO_PA_DOUT &= ~(1 << regOffset);
}

void turnOffLed(uint8_t led_num){
	uint8_t regOffset = led_num + REG_LED_OFFSET;
	*GPIO_PA_DOUT |= (1 << regOffset);
}

void toggleLed(uint8_t led_num){
	uint8_t regOffset = led_num + REG_LED_OFFSET;
	*GPIO_PA_DOUT ^= (1 << regOffset);
}

