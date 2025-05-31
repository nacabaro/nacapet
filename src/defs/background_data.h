#ifndef BACKGROUND_DATA_H
#define BACKGROUND_DATA_H

#include <stdint.h>

struct BackgroundData {
    uint8_t backgroundWidth;
    uint8_t backgroundHeight;
    uint8_t scaleFactor;
    uint16_t* backgroundData = NULL;
};

#endif