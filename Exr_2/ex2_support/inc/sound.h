#ifndef SOUND_H
#define SOUND_H

#define AMPLITUDE 100
#define DAC_MIDDLE 0x800

#define NO_NOTE 0

#define C2 65.406
#define D2 73.416
#define E2 82.407
#define F2 87.307
#define G2 97.999
#define A2 110.000
#define B2 123.470
#define C3 130.813
#define D3 146.832
#define E3 164.814
#define F3 174.614
#define G3 195.998
#define A3 220.000
#define B3 246.942
#define C4 261.626
#define D4 293.665
#define E4 329.628
#define F4 349.228
#define G4 391.995
#define A4 440.000
#define B4 493.883
#define C5 523.251
#define D5 587.330
#define E5 659.255
#define F5 698.457
#define G5 783.991
#define A5 880.000
#define B5 987.767
#define C6 1046.502

struct Note {
    float freq;
    uint16_t dur; // duration in milliseconds
}

#endif