#include <stdint.h>
#include <stdbool.h>

#include "../inc/efm32gg.h"
#include "../inc/gpio.h"
#include "../inc/dac.h"
#include "../inc/interrupt_handlers.h"
#include "../inc/timer.h"

/*
 * TODO calculate the appropriate sample period for the sound wave(s) you 
 * want to generate. The core clock (which the timer clock is derived
 * from) runs at 14 MHz by default. Also remember that the timer counter
 * registers are 16 bits. 
 */
/*
 * The period between sound samples, in clock cycles 
 */
#define   SAMPLE_PERIOD   0
#define		TIMER1_BIT 1 // setting timer_1 bit in ISR0
#define		GPIO_ODD_BIT	//setting odd gpio bit in ISR0
#define		GPIO_EVEN_BIT	//setting even gpio bit in ISR0


/*
 * Declaration of peripheral setup functions 
 */
void setupTimer(uint32_t period);
void setupDAC();
void setupNVIC();

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
	setupTimer(SAMPLE_PERIOD);

	/*
	 * Enable interrupt handling 
	 */
	setupNVIC();

	/*
	 * TODO for higher energy efficiency, sleep while waiting for
	 * interrupts instead of infinite loop for busy-waiting 
	 */
	while (1) ;

	return 0;
}

void setupNVIC()
{
	/*
	 * TODO use the NVIC ISERx registers to enable handling of
	 * interrupt(s) remember two things are necessary for interrupt
	 * handling: - the peripheral must generate an interrupt signal - the
	 * NVIC must be configured to make the CPU handle the signal You will
	 * need TIMER1, GPIO odd and GPIO even interrupt handling for this
	 * assignment. 
	 */

	 //Enabling GPIO interrupts
	*GPIO_EXTIPSELL |= (0x22222222); 
	*GPIO_EXTIFALL |= (0xff); 	//1->0 transition
	*GPIO_EXTIRISE |= (0xff);		//0->1 transition
	*GPIO_IEN |= (0xff);		//enable interrupt generation

	//Enable TIMER1 clock
	*CMU_HFPERCLKEN0 |= (1<<5);//pp. 151 in EFM32GG RM
	
	*TIMER1_IEN |= (0xff); //Enable TIMER1 interrupts. more infor about this and other rimer registers at pp. 550 in EFM32GG RM
	

	//enable CPU interrupt handling
	*ISR0 |= (TIMER1_BIT<<11); //setting bit 12
	*ISR0 |= (GPIO_ODD_BIT<<10); //setting bit 11
	*ISR0 |= (GPIO_EVEN_BIT<<1); //setting bit 2
}

/*
 * if other interrupt handlers are needed, use the following names:
 * NMI_Handler HardFault_Handler MemManage_Handler BusFault_Handler
 * UsageFault_Handler Reserved7_Handler Reserved8_Handler
 * Reserved9_Handler Reserved10_Handler SVC_Handler DebugMon_Handler
 * Reserved13_Handler PendSV_Handler SysTick_Handler DMA_IRQHandler
 * GPIO_EVEN_IRQHandler TIMER0_IRQHandler USART0_RX_IRQHandler
 * USART0_TX_IRQHandler USB_IRQHandler ACMP0_IRQHandler ADC0_IRQHandler
 * DAC0_IRQHandler I2C0_IRQHandler I2C1_IRQHandler GPIO_ODD_IRQHandler
 * TIMER1_IRQHandler TIMER2_IRQHandler TIMER3_IRQHandler
 * USART1_RX_IRQHandler USART1_TX_IRQHandler LESENSE_IRQHandler
 * USART2_RX_IRQHandler USART2_TX_IRQHandler UART0_RX_IRQHandler
 * UART0_TX_IRQHandler UART1_RX_IRQHandler UART1_TX_IRQHandler
 * LEUART0_IRQHandler LEUART1_IRQHandler LETIMER0_IRQHandler
 * PCNT0_IRQHandler PCNT1_IRQHandler PCNT2_IRQHandler RTC_IRQHandler
 * BURTC_IRQHandler CMU_IRQHandler VCMP_IRQHandler LCD_IRQHandler
 * MSC_IRQHandler AES_IRQHandler EBI_IRQHandler EMU_IRQHandler 
 */
