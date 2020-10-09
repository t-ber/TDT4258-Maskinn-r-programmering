#ifndef DAC_H
#define DAC_H

typedef struct Note {
    uint16_t top;
    uint16_t dur; // ms
} Note;

void setupDAC();
void playNote(Note n);
void onNoteCleared();

void playLisa();
void playPirates();

#endif