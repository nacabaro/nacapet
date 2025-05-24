#ifndef EVO_DATA_H
#define EVO_DATA_H

#include <stdint.h>

struct EvoData {
    uint8_t charaId;
    uint8_t charaNextId;
    uint8_t maxCareMistakes;
    uint8_t maxSleepInterruptions;
    uint8_t maxOverfeed;
    uint8_t minWonBattles;
    uint8_t minTotalBattles;
};

#endif