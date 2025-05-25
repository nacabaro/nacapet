#ifndef FILE_CHARA_H
#define FILE_CHARA_H

#include <stdint.h>
#include "sprite_data.h"

struct Egg_t {
    uint8_t id;
    char fileName[20];
    char name[16];
    SpriteData eggSprite;
};

typedef struct __attribute__((packed)) {
    uint8_t id;
    char name[16];
    uint8_t hp, ap, bp;
    uint8_t stage;
    uint8_t attribute;
    uint8_t attackSprite;
    uint32_t sleepTime;
    uint32_t wakeTime;
    uint32_t changeTime;
    uint16_t depleteTime;
    uint8_t minWeight;
}  LineChara_t;


typedef struct __attribute__((packed)) {
    uint8_t currentChara, nextChara;
    uint8_t minCareMistake, maxCareMistake;
    uint8_t minSleepDist, maxSleepDist;
    uint8_t minOverfeeds, maxOverfeeds;
    uint8_t minTraining, maxTraining;
    uint8_t totalBattles, wonBattles;
} CareMistakes_t;


struct LineCare_t {
    uint8_t lineId;
    uint8_t numCareMistakesData;
    CareMistakes_t* careMistakeData;
};

struct Line_t {
    uint8_t id;
    char name[16];
    uint8_t charaNumber;
    uint16_t hatchTime;
    LineChara_t* characters;
};

#endif