#include <stdint.h>
#include <stdbool.h>

#include "../inc/polling.h"
#include "../inc/efm32gg.h"
#include "../inc/gpio.h"
#include "../inc/dac.h"
#include "../inc/timer.h"
#include "../inc/rtc.h"

static volatile uint32_t buttonPushPrevious;
static volatile uint32_t prevTimer1Value;
static volatile uint32_t prevRTCValue;

void setupPolling()
{
    buttonPushPrevious = *GPIO_PC_DIN & 0xff;
    prevTimer1Value = *TIMER1_CNT;
    prevRTCValue = *RTC_CNT;
}

void pollButtons()
{
    uint32_t buttonPushCurrent = *GPIO_PC_DIN & 0xff;
    uint32_t buttonChange = buttonPushCurrent ^ buttonPushPrevious;
    uint32_t buttonFallingEdge = buttonChange & buttonPushPrevious;

    if (buttonFallingEdge & 0x01) {
        playLisa();
    }

    if (buttonFallingEdge & 0x02) {
        playImperial();
    }

    if (buttonFallingEdge & 0x04) {
        playPirates();
    }

    if (buttonFallingEdge & 0x80) {
        stopPlaying();
    }

    buttonPushPrevious = buttonPushCurrent;
}

void pollTimer1()
{
    uint32_t currentTimer1Value = *TIMER1_CNT;

    if (prevTimer1Value > currentTimer1Value) {
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
    }

    prevTimer1Value = currentTimer1Value;
}

void pollRTC()
{
    uint32_t currentRTCValue = *RTC_CNT;

    if (currentRTCValue != prevRTCValue && currentRTCValue == *RTC_COMP0) {
        clearRTC();
        stopTimer();
        onNoteCleared();
    }

    prevRTCValue = currentRTCValue;
}