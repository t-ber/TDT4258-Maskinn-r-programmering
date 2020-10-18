#include <stdint.h>
#include <stdbool.h>

#include "../inc/rtc.h"
#include "../inc/efm32gg.h"

// Set up the RTC peripheral
void setupRTC()
{   
    // Enable Low Frequency Clock
    *CMU_HFCORECLKEN0 |= (1 << 4); // Enable LF Clock

    // Set ULFRCO (~1 kHz) as Low Frequency Clock
    *CMU_LFCLKSEL &= ~(0b11); // Clear LFA
    *CMU_LFCLKSEL |= (1 << 16); // Set LFAE
    
    // Enable clock for RTC
    *CMU_LFACLKEN0 |= 0b10;

    // Set up RTC
    *RTC_CTRL = 0b100; // Set top = COMP0
    *RTC_IEN = 0b10; // Enable interrupt generation
}

// Start RTC timer with specified number of milliseconds
void startRTC(uint16_t ms)
{
    // Set the top to the specified number of milliseconds
    *RTC_COMP0 = ms;

    // Reset the counter
    *RTC_CNT = 0;

    // Start the counter
    *RTC_CTRL |= 1;
}

// Stop the RTC timer
void clearRTC()
{
    // Stop counting
    *RTC_CTRL |= 0;
}
