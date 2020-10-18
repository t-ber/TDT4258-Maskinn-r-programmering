#include <stdint.h>
#include <stdbool.h>

#include "../inc/efm32gg.h"
#include "../inc/gpio.h"
#include "../inc/dac.h"
#include "../inc/interrupt_handlers.h"
#include "../inc/timer.h"
#include "../inc/rtc.h"

/*
 * TODO calculate the appropriate sample period for the sound wave(s) you 
 * want to generate. The core clock (which the timer clock is derived
 * from) runs at 14 MHz by default. Also remember that the timer counter
 * registers are 16 bits. 
 */
/*
 * The period between sound samples, in clock cycles 
 */

/*
 * Declaration of peripheral setup functions 
 */
// void setupTimer(uint32_t period);
// void setupDAC();
void setupNVIC();
void setupPolling();

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

	/*
	 * Enable interrupt handling 
	 */
	
	setupPolling();

	//stopTimer();

	// turnOnLed();
	// turnOffLed();
	// startRTC(10000);

	// struct Note n = { .top = C4, .dur = 1000 };
	// playNote(n);
	// playLisa();

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

	*ISER0 |= (1 << 12); // Timer1
	//*ISER0 |= 0x802; // GPIO
	*ISER0 |= (1 << 30); // RTC
}

void setupPolling()
{
	uint32_t buttonPushPrevious = *GPIO_PC_DIN;
	buttonPushPrevious = buttonPushPrevious&0xff;
	//buttonPushPrevious = buttonPushPrevious&0xff;
	uint32_t prevTimerValue = *TIMER1_CNT;

	while(1)
	{
		uint32_t currentTimerValue = *TIMER1_CNT;
		uint32_t buttonPushCurrent = *GPIO_PC_DIN;
		buttonPushCurrent = buttonPushCurrent&0xff;
		uint32_t buttonChange = buttonPushPrevious^buttonPushCurrent; //XOR previous and current
		buttonChange = buttonChange&buttonPushPrevious; //Check which buttons have changed and were high at previous poll
		buttonPushPrevious= buttonPushCurrent;

		//If statements, handling music playing whenever correct buttons have changed. 
		if((buttonChange&0x10) != 0x0)//Check button SW7
		{
			playLisa();
		}
		else if((buttonChange&0x20) != 0x0)//Check button SW6
		{
			playPirates();
		}
		else if(prevTimerValue>currentTimerValue)
		{

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

		}

		uint32_t prevTimerValue = currentTimerValue;

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
