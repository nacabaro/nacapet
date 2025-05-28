#ifndef CHARA_DATA_H
#define CHARA_DATA_H

#include <stdint.h>

struct CharacterData {
    // Calculated at runtime
    bool hatching = false; 
    bool hatched = false;
    uint16_t hatchTimer = 0;

    uint8_t hunger;
    uint8_t strength;
    uint8_t weight;
    uint8_t age;
    uint8_t poopNumber;
    
    uint8_t careMistakes;
    uint8_t effort;
    uint8_t overfeed;
    uint8_t sleepDisturbances;

    uint16_t stageTotalBattled;
    uint16_t stageTotalWon;

    uint16_t charaTotalBattled;
    uint16_t charaTotalWon;

    int32_t sleepCareMistakeCounter = 0;
    int32_t evoLeftTimer;
    int16_t hungerCareMistakeTimer;
    int16_t strengthCareMistakeTimer;

    bool gotLifeYearAdded = false;
    bool hungerCareMistakeObtained = false;
    bool strengthCareMistakeObtained = false;
    bool sleepCareMistakeObtained = false;
    bool careMistakeCallLight = false;
    bool overfeedHappened = false;

    bool sleepy = false;
    bool asleep = false;
    bool injured = false;
    bool dead = false;
    bool traited = true;
    
    // Obtained from structure
    uint8_t idChara;

    char charaName[40];

    uint8_t hp;
    uint8_t ap;
    uint8_t bp;

    uint8_t stage;
    uint8_t attribute;

    uint32_t sleepTime;
    uint32_t wakeupTime;

    uint32_t evoTime;

    uint16_t initialStatsReductionTime = 600;
    uint8_t minWeight;
};

#endif