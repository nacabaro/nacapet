#ifndef SPRITE_DATA_H
#define SPRITE_DATA_H

#include <stdint.h>
#include <stddef.h>

struct SpriteData {
    uint8_t spriteWidth;
    uint8_t spriteHeight;
    uint8_t spriteNumber;
    uint16_t** spriteData = NULL;

    uint8_t lastX = 0;
    uint8_t lastY = 0;
    uint8_t lastW = 0;
    uint8_t lastH = 0;
};

#endif