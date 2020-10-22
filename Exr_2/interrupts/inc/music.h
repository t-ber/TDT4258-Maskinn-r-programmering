#ifndef MUSIC_H
#define MUSIC_H

typedef struct Note {
    float freq;
    uint16_t dur; // ms
} Note;

void playNote(Note n);

void onNoteCleared();
void stopPlaying();

void playImperial();
void playLisa();
void playPirates();

#endif