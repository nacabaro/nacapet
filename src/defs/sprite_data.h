#ifndef SPRITE_DATA_H
#define SPRITE_DATA_H

#include <stdint.h>

struct SpriteData {
    uint8_t spriteWidth;
    uint8_t spriteHeight;
    uint8_t spriteNumber;
    uint16_t** spriteData;
};

#endif