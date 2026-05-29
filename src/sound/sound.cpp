#include "sound.h"
#include "defs/defs.h"

static const Note* currentMelody = nullptr;

static uint16_t melodyLength = 0;
static uint16_t currentNote = 0;

static bool playing = false;

static unsigned long nextNoteTime = 0;

void sound_init() {
    pinMode(SPK_PIN, OUTPUT);
}

void sound_playTone(uint16_t frequency, uint16_t duration) {
    if (!soundEnabled) { return; }

    currentMelody = nullptr;
    melodyLength = 0;
    currentNote = 0;

    if (frequency == 0) {
        noTone(SPK_PIN);
    } else {
        tone(SPK_PIN, frequency, duration);
    }

    playing = true;
    nextNoteTime = millis() + duration;
}

void sound_playMelody(const Note* melody, uint16_t noteCount) {
    if (!soundEnabled) { return; }

    if (melody == nullptr || noteCount == 0) {
        return;
    }

    currentMelody = melody;
    melodyLength = noteCount;
    currentNote = 0;
    playing = true;
    nextNoteTime = 0;
}

void sound_stop() {
    noTone(SPK_PIN);
    playing = false;
    currentMelody = nullptr;
    melodyLength = 0;
    currentNote = 0;
}

bool sound_isPlaying() {
    return playing;
}

void sound_update() {
    if (!soundEnabled) {
        sound_stop();
        return;
    }

    if (!playing) {
        return;
    }

    if (currentMelody == nullptr) {
        if (millis() >= nextNoteTime) {
            sound_stop();
        }

        return;
    }

    if (millis() < nextNoteTime) {
        return;
    }

    if (currentNote >= melodyLength) {
        sound_stop();
        return;
    }

    const Note& note = currentMelody[currentNote];

    if (note.frequency == 0) {
        noTone(SPK_PIN);
    } else {
        tone(SPK_PIN, note.frequency, note.duration);
    }

    nextNoteTime = millis() + note.duration;

    currentNote++;
}
