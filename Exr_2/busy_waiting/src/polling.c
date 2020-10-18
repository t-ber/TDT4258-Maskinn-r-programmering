#include <stdint.h>
#include <stdbool.h>

#include "../inc/polling.h"
#include "../inc/efm32gg.h"
#include "../inc/gpio.h"
#include "../inc/dac.h"
#include "../inc/timer.h"
#include "../inc/rtc.h"
#include "../inc/music.h"

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

    onButtonPress(buttonFallingEdge);

    buttonPushPrevious = buttonPushCurrent;
}

void pollTimer1()
{
    uint32_t currentTimer1Value = *TIMER1_CNT;

    if (prevTimer1Value > currentTimer1Value) {
        alternateDACValue();
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
