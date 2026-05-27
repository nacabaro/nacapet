#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>


void utils_upscaleSprite(
    const uint16_t* src,
    uint8_t srcWidth,
    uint8_t srcHeight,
    uint16_t* dst
);

#endif
