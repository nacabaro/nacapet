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
    
    uint8_t careMistakes = 0;
    uint8_t effort = 0;
    uint8_t overfeed = 0;
    uint8_t sleepDisturbances = 0;
    uint8_t injuries = 0;

    uint16_t stageTotalBattled;
    uint16_t stageTotalWon;

    uint16_t charaTotalBattled;
    uint16_t charaTotalWon;

    int32_t sleepCareMistakeCounter = 0;
    int32_t changeTimerLeft;
    int16_t hungerCareMistakeTimer;
    int16_t strengthCareMistakeTimer;

    bool gotLifeYearAdded = false;
    bool hungerCareMistakeObtained = false;
    bool strengthCareMistakeObtained = false;
    bool sleepCareMistakeObtained = false;
    bool careMistakeCallLight = false;
    bool overfeedHappened = false;

    uint32_t sleepTime;
    uint32_t wakeupTime;

    bool sleepy = false;
    bool asleep = false;
    bool injured = false;
    bool dead = false;
    bool traited = true;

    // Usado para recalcular cuantos dias van a tardar en recuperarse, max 5 pls
    uint8_t dynamicSleepDists; 
    
    // Obtained from structure
    uint8_t idChara;

    char charaName[40];

    uint8_t hp;
    uint8_t ap;
    uint8_t bp;

    uint8_t stage;
    uint8_t attribute;
    uint8_t spriteAttackId;

    uint32_t initialSleepTime;
    uint32_t initialWakeupTime;

    uint32_t initialChangeTimer;

    uint16_t initialStatsReductionTime = 600;
    uint8_t minWeight;
};

#endif