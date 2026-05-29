#pragma once

#include <Arduino.h>

struct Note {
    uint16_t frequency;
    uint16_t duration;
};

void sound_init();
void sound_update();
void sound_playTone(uint16_t frequency, uint16_t duration);
void sound_playMelody(const Note* melody, uint16_t noteCount);
void sound_stop();
bool sound_isPlaying();