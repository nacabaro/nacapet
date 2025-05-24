#ifndef MEMORY_H
#define MEMORY_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

struct spriteData {
    uint8_t width;
    uint8_t height;
    uint8_t number;
    uint16_t** data;
};

uint16_t** memory_allocate(uint8_t numSprite, uint8_t width, uint8_t height);
void memory_free(uint8_t* ptr);

#endif