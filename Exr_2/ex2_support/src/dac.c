#include <stdint.h>
#include <stdbool.h>

#include "../inc/dac.h"
#include "../inc/efm32gg.h"
#include "../inc/timer.h"
#include "../inc/rtc.h"
#include "../inc/gpio.h"

static struct Note currentNoteArray[128];
static uint32_t currentNoteIndex;
static uint32_t currentNoteArraySize;

void setupDAC()
{
	/*
	 * TODO enable and set up the Digital-Analog Converter
	 * 
	 * 1. Enable the DAC clock by setting bit 17 in CMU_HFPERCLKEN0 2.
	 * Prescale DAC clock by writing 0x50010 to DAC0_CTRL 3. Enable left
	 * and right audio channels by writing 1 to DAC0_CH0CTRL and
	 * DAC0_CH1CTRL 4. Write a continuous stream of samples to the DAC
	 * data registers, DAC0_CH0DATA and DAC0_CH1DATA, for example from a
	 * timer interrupt 
	 */

	// Step 1
	*CMU_HFPERCLKEN0 |= (1 << 17);

	// Step 2
	*DAC0_CTRL = 0x50010;

	// Step 3
	*DAC0_CH0CTRL = 1;
	*DAC0_CH1CTRL = 1;

	// Step 4 er nok best å gjøre i interrupt, evt main for busy-waiting?
}

void playNote(Note n)
{	
	if (n.top == 0) {
		stopTimer();
	} else {
		setTimerTop(n.top);
	}
	startRTC(n.dur);
}

void onNoteCleared()
{
	if (currentNoteIndex < currentNoteArraySize) {
		playNote(currentNoteArray[currentNoteIndex]);
		currentNoteIndex++;
	}
	else {
		stopTimer();
		clearRTC();
	}
}

void stopPlaying()
{
	currentNoteArraySize = 0;
	currentNoteIndex = 0;
	onNoteCleared();
}

void playImperial()
{
	struct Note imperial[] = {
		(struct Note) { .top = 2*A3, .dur = 750 },
		(struct Note) { .top = 0, .dur = 20 },
		(struct Note) { .top = 2*A3, .dur = 730 },
		(struct Note) { .top = 0, .dur = 20 },
		(struct Note) { .top = 2*A3, .dur = 730 },
		(struct Note) { .top = 2*F3, .dur = 500 },
		(struct Note) { .top = 2*C4, .dur = 250 },
		(struct Note) { .top = 2*A3, .dur = 750 },
		(struct Note) { .top = 2*F3, .dur = 500 },
		(struct Note) { .top = 2*C4, .dur = 250 },
		(struct Note) { .top = 2*A3, .dur = 1000 },
		(struct Note) { .top = 0, .dur = 500 },

		(struct Note) { .top = 2*E4, .dur = 750 },
		(struct Note) { .top = 0, .dur = 20 },
		(struct Note) { .top = 2*E4, .dur = 730 },
		(struct Note) { .top = 0, .dur = 20 },
		(struct Note) { .top = 2*E4, .dur = 730 },
		(struct Note) { .top = 2*F4, .dur = 500 },
		(struct Note) { .top = 2*C4, .dur = 250 },
		(struct Note) { .top = 2*G3S, .dur = 750 },
		(struct Note) { .top = 2*F3, .dur = 500 },
		(struct Note) { .top = 2*C4, .dur = 250 },
		(struct Note) { .top = 2*A3, .dur = 1000 },
		(struct Note) { .top = 0, .dur = 500 },

		(struct Note) { .top = 2*A4, .dur = 750 },
		(struct Note) { .top = 2*A3, .dur = 500 },
		(struct Note) { .top = 0, .dur = 20 },
		(struct Note) { .top = 2*A3, .dur = 230 },
		(struct Note) { .top = 2*A4, .dur = 750 },
		(struct Note) { .top = 2*G4S, .dur = 500 },
		(struct Note) { .top = 2*G4, .dur = 250 },
		(struct Note) { .top = 2*F4S, .dur = 125 },
		(struct Note) { .top = 2*F4, .dur = 250 },
		(struct Note) { .top = 2*F4S, .dur = 250 }

	};

	for (uint32_t i = 0; i < sizeof(imperial) && i < sizeof(currentNoteArray); i++) {
		currentNoteArray[i] = imperial[i];
	}
	currentNoteArraySize = sizeof(imperial) / sizeof(imperial[0]);
	currentNoteIndex = 0;

	onNoteCleared();

}

void playLisa()
{
	struct Note lisa[] = {
		(struct Note) { .top = C4, .dur = 500 },
		(struct Note) { .top = D4, .dur = 500 },
		(struct Note) { .top = E4, .dur = 500 },
		(struct Note) { .top = F4, .dur = 500 },
		(struct Note) { .top = G4, .dur = 1000 },
		(struct Note) { .top = 0, .dur = 10 },
		(struct Note) { .top = G4, .dur = 990 },
		(struct Note) { .top = A4, .dur = 500 },
		(struct Note) { .top = 0, .dur = 10 },
		(struct Note) { .top = A4, .dur = 490 },
		(struct Note) { .top = 0, .dur = 10 },
		(struct Note) { .top = A4, .dur = 490 },
		(struct Note) { .top = 0, .dur = 10 },
		(struct Note) { .top = A4, .dur = 490 },
		(struct Note) { .top = G4, .dur = 1000 },
		(struct Note) { .top = F4, .dur = 500 },
		(struct Note) { .top = 0, .dur = 10 },
		(struct Note) { .top = F4, .dur = 490 },
		(struct Note) { .top = 0, .dur = 10 },
		(struct Note) { .top = F4, .dur = 490 },
		(struct Note) { .top = 0, .dur = 10 },
		(struct Note) { .top = F4, .dur = 490 },
		(struct Note) { .top = E4, .dur = 1000 },
		(struct Note) { .top = 0, .dur = 10 },
		(struct Note) { .top = E4, .dur = 990 },
		(struct Note) { .top = D4, .dur = 500 },
		(struct Note) { .top = 0, .dur = 10 },
		(struct Note) { .top = D4, .dur = 490 },
		(struct Note) { .top = 0, .dur = 10 },
		(struct Note) { .top = D4, .dur = 490 },
		(struct Note) { .top = 0, .dur = 10 },
		(struct Note) { .top = D4, .dur = 490 },
		(struct Note) { .top = C4, .dur = 500},
		(struct Note) { .top = 0, .dur = 10}
	};
	
	for (uint32_t i = 0; i < sizeof(lisa) && i < sizeof(currentNoteArray); i++) {
		currentNoteArray[i] = lisa[i];
	}
	currentNoteArraySize = sizeof(lisa) / sizeof(lisa[0]);
	currentNoteIndex = 0;

	onNoteCleared();
}

void playPirates()
{
	struct Note pirates[] = {
		(struct Note) { .top = A3, .dur = 250 },
		(struct Note) { .top = C4, .dur = 250 },
		(struct Note) { .top = D4, .dur = 490 },
		(struct Note) { .top = 0, .dur = 10 },
		(struct Note) { .top = D4, .dur = 490 },
		(struct Note) { .top = 0, .dur = 10 },
		(struct Note) { .top = D4, .dur = 250 },
		(struct Note) { .top = E4, .dur = 250 },
		(struct Note) { .top = F4, .dur = 490 },
		(struct Note) { .top = 0, .dur = 10 },
		(struct Note) { .top = F4, .dur = 490 },
		(struct Note) { .top = 0, .dur = 10 },
		(struct Note) { .top = F4, .dur = 250 },
		(struct Note) { .top = G4, .dur = 250 },
		(struct Note) { .top = E4, .dur = 500 },
		(struct Note) { .top = 0, .dur = 10 },
		(struct Note) { .top = E4, .dur = 500 },
		(struct Note) { .top = D4, .dur = 250 },
		(struct Note) { .top = C4, .dur = 240 },
		(struct Note) { .top = 0, .dur = 10 },
		(struct Note) { .top = C4, .dur = 250 },
		(struct Note) { .top = D4, .dur = 500 },
		(struct Note) { .top = 0, .dur = 250 },

		(struct Note) { .top = A3, .dur = 250 },
		(struct Note) { .top = C4, .dur = 250 },
		(struct Note) { .top = D4, .dur = 490 },
		(struct Note) { .top = 0, .dur = 10 },
		(struct Note) { .top = D4, .dur = 490 },
		(struct Note) { .top = 0, .dur = 10 },
		(struct Note) { .top = D4, .dur = 250 },
		(struct Note) { .top = E4, .dur = 250 },
		(struct Note) { .top = F4, .dur = 490 },
		(struct Note) { .top = 0, .dur = 10 },
		(struct Note) { .top = F4, .dur = 490 },
		(struct Note) { .top = 0, .dur = 10 },
		(struct Note) { .top = F4, .dur = 250 },
		(struct Note) { .top = G4, .dur = 250 },
		(struct Note) { .top = E4, .dur = 500 },
		(struct Note) { .top = 0, .dur = 10 },
		(struct Note) { .top = E4, .dur = 500 },
		(struct Note) { .top = D4, .dur = 250 },
		(struct Note) { .top = C4, .dur = 240 },
		(struct Note) { .top = D4, .dur = 500 },
		(struct Note) { .top = 0, .dur = 500 },

		(struct Note) { .top = A3, .dur = 250 },
		(struct Note) { .top = C4, .dur = 250 },
		(struct Note) { .top = D4, .dur = 490 },
		(struct Note) { .top = 0, .dur = 10 },
		(struct Note) { .top = D4, .dur = 490 },
		(struct Note) { .top = 0, .dur = 10 },
		(struct Note) { .top = D4, .dur = 250 },
		(struct Note) { .top = F4, .dur = 250 },
		(struct Note) { .top = G4, .dur = 490 },
		(struct Note) { .top = 0, .dur = 10 },
		(struct Note) { .top = G4, .dur = 490 },
		(struct Note) { .top = 0, .dur = 10 },
		(struct Note) { .top = G4, .dur = 250 },
		(struct Note) { .top = A4, .dur = 250 },
		(struct Note) { .top = A4S, .dur = 490 },
		(struct Note) { .top = 0, .dur = 10 },
		(struct Note) { .top = A4S, .dur = 500 },
		(struct Note) { .top = A4, .dur = 250 },
		(struct Note) { .top = G4, .dur = 250 },
		(struct Note) { .top = A4, .dur = 250 },
		(struct Note) { .top = D4, .dur = 490 },
		(struct Note) { .top = 0, .dur = 250 },

		(struct Note) { .top = D4, .dur = 250 },
		(struct Note) { .top = E4, .dur = 250 },
		(struct Note) { .top = F4, .dur = 490 },
		(struct Note) { .top = 0, .dur = 10 },
		(struct Note) { .top = F4, .dur = 500 },
		(struct Note) { .top = G4, .dur = 500 },
		(struct Note) { .top = A4, .dur = 250 },
		(struct Note) { .top = D4, .dur = 490 },
		(struct Note) { .top = 0, .dur = 250 },

		(struct Note) { .top = D4, .dur = 250 },
		(struct Note) { .top = F4, .dur = 250 },
		(struct Note) { .top = E4, .dur = 490 },
		(struct Note) { .top = 0, .dur = 10 },
		(struct Note) { .top = E4, .dur = 500 },
		(struct Note) { .top = F4, .dur = 250 },
		(struct Note) { .top = E4, .dur = 250 },
		(struct Note) { .top = D4, .dur = 500 }

	};
	
	for (uint32_t i = 0; i < sizeof(pirates) && i < sizeof(currentNoteArray); i++) {
		currentNoteArray[i] = pirates[i];
	}
	currentNoteArraySize = sizeof(pirates) / sizeof(pirates[0]);
	currentNoteIndex = 0;

	onNoteCleared();
}

void playCDE()
{
	struct Note cde[] = {
		(Note) { .top = C4, .dur = 500 },
		(Note) { .top = D4, .dur = 500 },
		(Note) { .top = E4, .dur = 500 }
	};
	
	for (uint32_t i = 0; i < sizeof(cde) && i < sizeof(currentNoteArray); i++) {
		currentNoteArray[i] = cde[i];
	}
	currentNoteArraySize = sizeof(cde) / sizeof(cde[0]);
	currentNoteIndex = 0;

	onNoteCleared();
}