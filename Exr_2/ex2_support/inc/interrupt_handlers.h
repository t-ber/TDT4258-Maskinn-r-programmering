#ifndef INTERRUPT_HANDLERS_H
#define INTERRUPT_HANDLERS_H

#define AMPLITUDE 25

void __attribute__ ((interrupt)) TIMER1_IRQHandler();

void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler();

void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler();

void __attribute__ ((interrupt)) RTC_IRQHandler();

#endif