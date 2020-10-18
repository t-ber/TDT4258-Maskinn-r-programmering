#include <stdint.h>
#include <stdbool.h>

#include "../inc/efm32gg.h"
#include "../inc/gpio.h"
#include "../inc/dac.h"
#include "../inc/interrupt_handlers.h"
#include "../inc/timer.h"
#include "../inc/rtc.h"
#include "../inc/music.h"

void setupNVIC();

// Program start in main
int main(void)
{
	// Set up peripherals
	setupGPIO();
	setupDAC();
	setupTimer();
	setupRTC();

	// Enable interrupt handling
	setupNVIC();

	// Set energy mode EM2 (deep sleep)
	*SCR = 6;

	// Play start-up melody
	playImperial();

	// Wait for interrupt
	__asm__("wfi");

	return 0;
}

// Enable interrupt handling
void setupNVIC()
{
	*ISER0 |= (1 << 12); // Timer1
	*ISER0 |= 0x802; // GPIO
	*ISER0 |= (1 << 30); // RTC
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
