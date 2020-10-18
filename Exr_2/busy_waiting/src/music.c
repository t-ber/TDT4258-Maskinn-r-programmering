#include <stdint.h>
#include <stdbool.h>

#include "../inc/music.h"
#include "../inc/dac.h"
#include "../inc/timer.h"
#include "../inc/rtc.h"
#include "../inc/pitch.h"

static struct Note currentNoteArray[128];
static uint32_t currentNoteIndex;
static uint32_t currentNoteArraySize;

// Play a note
void playNote(Note n)
{	
	// If 0 frequency, stop the timer (play nothing)
	if (n.freq == 0) {
		stopTimer();
	}
	// Else, start timer with desired frequency
	else {
		startTimer(n.freq);
	}
	// Start the RTC timer with the duration of the note
	startRTC(n.dur);
}

// Called when a note is cleared after a RTC interrupt
void onNoteCleared()
{
	// If song not done, play next note
	if (currentNoteIndex < currentNoteArraySize) {
		playNote(currentNoteArray[currentNoteIndex]);
		currentNoteIndex++;
	}
	// Else, stop playing
	else {
		stopTimer();
		clearRTC();
	}
}

// Stop playing music
void stopPlaying()
{
	currentNoteArraySize = 0;
	currentNoteIndex = 0;
	onNoteCleared();
}

// Play Lisa gikk til skolen
void playLisa()
{
	struct Note lisa[] = {
		(struct Note) { .freq = C4, .dur = 500 },
		(struct Note) { .freq = D4, .dur = 500 },
		(struct Note) { .freq = E4, .dur = 500 },
		(struct Note) { .freq = F4, .dur = 500 },
		(struct Note) { .freq = G4, .dur = 1000 },
		(struct Note) { .freq = 0, .dur = 10 },
		(struct Note) { .freq = G4, .dur = 990 },
		(struct Note) { .freq = A4, .dur = 500 },
		(struct Note) { .freq = 0, .dur = 10 },
		(struct Note) { .freq = A4, .dur = 490 },
		(struct Note) { .freq = 0, .dur = 10 },
		(struct Note) { .freq = A4, .dur = 490 },
		(struct Note) { .freq = 0, .dur = 10 },
		(struct Note) { .freq = A4, .dur = 490 },
		(struct Note) { .freq = G4, .dur = 1000 },
		(struct Note) { .freq = F4, .dur = 500 },
		(struct Note) { .freq = 0, .dur = 10 },
		(struct Note) { .freq = F4, .dur = 490 },
		(struct Note) { .freq = 0, .dur = 10 },
		(struct Note) { .freq = F4, .dur = 490 },
		(struct Note) { .freq = 0, .dur = 10 },
		(struct Note) { .freq = F4, .dur = 490 },
		(struct Note) { .freq = E4, .dur = 1000 },
		(struct Note) { .freq = 0, .dur = 10 },
		(struct Note) { .freq = E4, .dur = 990 },
		(struct Note) { .freq = D4, .dur = 500 },
		(struct Note) { .freq = 0, .dur = 10 },
		(struct Note) { .freq = D4, .dur = 490 },
		(struct Note) { .freq = 0, .dur = 10 },
		(struct Note) { .freq = D4, .dur = 490 },
		(struct Note) { .freq = 0, .dur = 10 },
		(struct Note) { .freq = D4, .dur = 490 },
		(struct Note) { .freq = C4, .dur = 500},
		(struct Note) { .freq = 0, .dur = 10}
	};
	
	for (uint32_t i = 0; i < sizeof(lisa) && i < sizeof(currentNoteArray); i++) {
		currentNoteArray[i] = lisa[i];
	}
	currentNoteArraySize = sizeof(lisa) / sizeof(lisa[0]);
	currentNoteIndex = 0;

	onNoteCleared();
}

// Play the imperial march
void playImperial()
{
	struct Note imperial[] = {
		(struct Note) { .freq = A3, .dur = 750 },
		(struct Note) { .freq = 0, .dur = 20 },
		(struct Note) { .freq = A3, .dur = 730 },
		(struct Note) { .freq = 0, .dur = 20 },
		(struct Note) { .freq = A3, .dur = 730 },
		(struct Note) { .freq = F3, .dur = 500 },
		(struct Note) { .freq = C4, .dur = 250 },
		(struct Note) { .freq = A3, .dur = 750 },
		(struct Note) { .freq = F3, .dur = 500 },
		(struct Note) { .freq = C4, .dur = 250 },
		(struct Note) { .freq = A3, .dur = 1000 },
		(struct Note) { .freq = 0, .dur = 500 },

		(struct Note) { .freq = E4, .dur = 750 },
		(struct Note) { .freq = 0, .dur = 20 },
		(struct Note) { .freq = E4, .dur = 730 },
		(struct Note) { .freq = 0, .dur = 20 },
		(struct Note) { .freq = E4, .dur = 730 },
		(struct Note) { .freq = F4, .dur = 500 },
		(struct Note) { .freq = C4, .dur = 250 },
		(struct Note) { .freq = G3S, .dur = 750 },
		(struct Note) { .freq = F3, .dur = 500 },
		(struct Note) { .freq = C4, .dur = 250 },
		(struct Note) { .freq = A3, .dur = 1000 },
		(struct Note) { .freq = 0, .dur = 500 },

		(struct Note) { .freq = A4, .dur = 750 },
		(struct Note) { .freq = A3, .dur = 500 },
		(struct Note) { .freq = 0, .dur = 20 },
		(struct Note) { .freq = A3, .dur = 230 },
		(struct Note) { .freq = A4, .dur = 750 },
		(struct Note) { .freq = G4S, .dur = 500 },
		(struct Note) { .freq = G4, .dur = 250 },
		(struct Note) { .freq = F4S, .dur = 125 },
		(struct Note) { .freq = F4, .dur = 250 },
		(struct Note) { .freq = F4S, .dur = 250 }

	};

	for (uint32_t i = 0; i < sizeof(imperial) && i < sizeof(currentNoteArray); i++) {
		currentNoteArray[i] = imperial[i];
	}
	currentNoteArraySize = sizeof(imperial) / sizeof(imperial[0]);
	currentNoteIndex = 0;

	onNoteCleared();

}

// Play the Pirates of the Caribbean theme
void playPirates()
{
	struct Note pirates[] = {
		(struct Note) { .freq = A3, .dur = 250 },
		(struct Note) { .freq = C4, .dur = 250 },
		(struct Note) { .freq = D4, .dur = 490 },
		(struct Note) { .freq = 0, .dur = 10 },
		(struct Note) { .freq = D4, .dur = 490 },
		(struct Note) { .freq = 0, .dur = 10 },
		(struct Note) { .freq = D4, .dur = 250 },
		(struct Note) { .freq = E4, .dur = 250 },
		(struct Note) { .freq = F4, .dur = 490 },
		(struct Note) { .freq = 0, .dur = 10 },
		(struct Note) { .freq = F4, .dur = 490 },
		(struct Note) { .freq = 0, .dur = 10 },
		(struct Note) { .freq = F4, .dur = 250 },
		(struct Note) { .freq = G4, .dur = 250 },
		(struct Note) { .freq = E4, .dur = 500 },
		(struct Note) { .freq = 0, .dur = 10 },
		(struct Note) { .freq = E4, .dur = 500 },
		(struct Note) { .freq = D4, .dur = 250 },
		(struct Note) { .freq = C4, .dur = 240 },
		(struct Note) { .freq = 0, .dur = 10 },
		(struct Note) { .freq = C4, .dur = 250 },
		(struct Note) { .freq = D4, .dur = 500 },
		(struct Note) { .freq = 0, .dur = 250 },

		(struct Note) { .freq = A3, .dur = 250 },
		(struct Note) { .freq = C4, .dur = 250 },
		(struct Note) { .freq = D4, .dur = 490 },
		(struct Note) { .freq = 0, .dur = 10 },
		(struct Note) { .freq = D4, .dur = 490 },
		(struct Note) { .freq = 0, .dur = 10 },
		(struct Note) { .freq = D4, .dur = 250 },
		(struct Note) { .freq = E4, .dur = 250 },
		(struct Note) { .freq = F4, .dur = 490 },
		(struct Note) { .freq = 0, .dur = 10 },
		(struct Note) { .freq = F4, .dur = 490 },
		(struct Note) { .freq = 0, .dur = 10 },
		(struct Note) { .freq = F4, .dur = 250 },
		(struct Note) { .freq = G4, .dur = 250 },
		(struct Note) { .freq = E4, .dur = 500 },
		(struct Note) { .freq = 0, .dur = 10 },
		(struct Note) { .freq = E4, .dur = 500 },
		(struct Note) { .freq = D4, .dur = 250 },
		(struct Note) { .freq = C4, .dur = 240 },
		(struct Note) { .freq = D4, .dur = 500 },
		(struct Note) { .freq = 0, .dur = 500 },

		(struct Note) { .freq = A3, .dur = 250 },
		(struct Note) { .freq = C4, .dur = 250 },
		(struct Note) { .freq = D4, .dur = 490 },
		(struct Note) { .freq = 0, .dur = 10 },
		(struct Note) { .freq = D4, .dur = 490 },
		(struct Note) { .freq = 0, .dur = 10 },
		(struct Note) { .freq = D4, .dur = 250 },
		(struct Note) { .freq = F4, .dur = 250 },
		(struct Note) { .freq = G4, .dur = 490 },
		(struct Note) { .freq = 0, .dur = 10 },
		(struct Note) { .freq = G4, .dur = 490 },
		(struct Note) { .freq = 0, .dur = 10 },
		(struct Note) { .freq = G4, .dur = 250 },
		(struct Note) { .freq = A4, .dur = 250 },
		(struct Note) { .freq = A4S, .dur = 490 },
		(struct Note) { .freq = 0, .dur = 10 },
		(struct Note) { .freq = A4S, .dur = 500 },
		(struct Note) { .freq = A4, .dur = 250 },
		(struct Note) { .freq = G4, .dur = 250 },
		(struct Note) { .freq = A4, .dur = 250 },
		(struct Note) { .freq = D4, .dur = 490 },
		(struct Note) { .freq = 0, .dur = 250 },

		(struct Note) { .freq = D4, .dur = 250 },
		(struct Note) { .freq = E4, .dur = 250 },
		(struct Note) { .freq = F4, .dur = 490 },
		(struct Note) { .freq = 0, .dur = 10 },
		(struct Note) { .freq = F4, .dur = 500 },
		(struct Note) { .freq = G4, .dur = 500 },
		(struct Note) { .freq = A4, .dur = 250 },
		(struct Note) { .freq = D4, .dur = 490 },
		(struct Note) { .freq = 0, .dur = 250 },

		(struct Note) { .freq = D4, .dur = 250 },
		(struct Note) { .freq = F4, .dur = 250 },
		(struct Note) { .freq = E4, .dur = 490 },
		(struct Note) { .freq = 0, .dur = 10 },
		(struct Note) { .freq = E4, .dur = 500 },
		(struct Note) { .freq = F4, .dur = 250 },
		(struct Note) { .freq = E4, .dur = 250 },
		(struct Note) { .freq = D4, .dur = 500 }

	};
	
	for (uint32_t i = 0; i < sizeof(pirates) && i < sizeof(currentNoteArray); i++) {
		currentNoteArray[i] = pirates[i];
	}
	currentNoteArraySize = sizeof(pirates) / sizeof(pirates[0]);
	currentNoteIndex = 0;

	onNoteCleared();
}
