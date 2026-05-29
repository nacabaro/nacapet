#ifndef SOUNDS_H
#define SOUNDS_H

#include "defs/defs.h"
#include "sound/sound.h"

#define SOUND_NOTE_COUNT(melody) (sizeof(melody) / sizeof((melody)[0]))

static const Note SOUND_BUTTON_BEEP[] = {
    {BEEP_FREQ_HZ, BEEP_LEN_MS},
};

static const Note SOUND_BOOT[] = {
    {1800, 55},
    {0, 20},
    {2600, 55},
    {0, 20},
    {3400, 70},
    {0, 35},
    {4600, 95},
};

static const Note SOUND_CARE_ALERT[] = {
    {1800, 80},
    {0, 35},
    {1800, 80},
    {0, 35},
    {5200, 120},
};

static const Note SOUND_HAPPY[] = {
    {2200, 45},
    {3300, 45},
    {4400, 55},
    {6600, 80},
    {0, 25},
    {5200, 60},
};

static const Note SOUND_ANGRY[] = {
    {900, 70},
    {0, 25},
    {700, 90},
    {0, 25},
    {520, 150},
};

static const Note SOUND_POOP_DROP[] = {
    {4200, 35},
    {3200, 35},
    {2200, 45},
    {0, 25},
    {900, 85},
};

static const Note SOUND_REFUSE[] = {
    {2600, 70},
    {0, 35},
    {1500, 80},
    {0, 30},
    {900, 120},
};

static const Note SOUND_EVOLUTION_WOBBLE[] = {
    {3200, 60},
    {3900, 60},
    {3000, 60},
    {4300, 60},
    {3400, 60},
    {4700, 70},
};

static const Note SOUND_EVOLUTION_COMPLETE[] = {
    {1600, 90},
    {2200, 90},
    {3000, 100},
    {4000, 110},
    {5200, 130},
    {6400, 220},
    {0, 60},
    {5200, 100},
    {6400, 180},
};

static const Note SOUND_TRAINING_START[] = {
    {3100, 55},
    {0, 25},
    {4100, 85},
};

static const Note SOUND_TRAINING_ATTACK[] = {
    {5200, 18},
    {0, 8},
    {4600, 18},
    {0, 8},
    {3900, 22},
    {0, 12},
    {3000, 28},
};

static const Note SOUND_HATCH[] = {
    {2200, 45},
    {0, 20},
    {2600, 45},
    {0, 20},
    {3000, 45},
    {0, 20},
    {3400, 45},
    {0, 20},
    {3800, 50},
    {0, 25},
    {4300, 55},
    {0, 30},
    {5000, 75},
    {0, 45},
    {3600, 55},
    {4300, 55},
    {5200, 110},
};

#endif
