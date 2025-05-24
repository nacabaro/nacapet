#ifndef CHARA_DATA_H
#define CHARA_DATA_H

#include <stdint.h>

struct CharacterData {
    // Calculated at runtime
    uint8_t hunger;
    uint8_t strength;
    uint8_t effort;
    uint8_t careMistakes;
    uint8_t weight;
    uint8_t age;
    uint8_t poopNumber;

    int32_t sleepCareMistakeCounter = 0;
    int32_t evoLeftTimer;
    int16_t hungerCareMistakeTimer;
    int16_t strengthCareMistakeTimer;

    bool gotLifeYearAdded = false;
    bool hungerCareMistakeObtained = false;
    bool strengthCareMistakeObtained = false;
    bool sleepCareMistakeObtained = false;
    bool careMistakeCallLight = false;

    bool sleepy = false;
    bool asleep = false;
    bool injured = false;
    bool dead = false;
    
    // Obtained from structure
    uint8_t idLine;
    uint8_t idChara;
    char charaName[40];
    uint8_t hp;
    uint8_t ap;
    uint8_t bp;
    uint8_t stage;
    uint8_t attribute;

    uint32_t sleepTime = 75600;
    uint32_t wakeupTime = 25200;

    uint32_t evoTime;

    uint16_t initialStatsReductionTime = 600;
};

#endif