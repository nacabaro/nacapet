#ifndef DEBUG_H
#define DEBUG_H

#include <stdint.h>

void debug_printFreeMemory();
void debug_printAllSprites(uint16_t** sprite, uint8_t numSprites, uint8_t width, uint8_t height);

#endif