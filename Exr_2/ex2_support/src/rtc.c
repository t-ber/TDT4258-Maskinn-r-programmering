#include <stdint.h>
#include <stdbool.h>

#include "../inc/rtc.h"
#include "../inc/efm32gg.h"

void setupRTC()
{   
    *CMU_HFCORECLKEN0 |= (1 << 4); // Enable LF Clock

    *CMU_LFCLKSEL &= ~(0b11); // Clear LFA
    *CMU_LFCLKSEL |= (1 << 16); // Set LFAE, these two enable ULFRCO
    // *CMU_LFAPRESC0 |= 5 << 4;
    
    *CMU_LFACLKEN0 |= 0b10; // Enable clock

    *RTC_CTRL = 0b100; // Set top = COMP0
    *RTC_COMP0 = 1000; // Eksempel 1 sekund
    *RTC_IEN = 0b10; // Enable interrupt generation
}

void startRTC(uint16_t ms)
{
    *RTC_COMP0 = ms;
    *RTC_CTRL |= 1;
}

void clearRTC()
{
    *RTC_CTRL |= 0;
}