#include "debug.h"

#include <freertos/FreeRTOS.h>

void debug_printFreeMemory() {
    size_t freeMemory = esp_get_free_heap_size();
    printf("[DEBUG] Free memory: %zu bytes\n", freeMemory);
}

void debug_printAllSprites(uint16_t** sprite, uint8_t numSprites, uint8_t width, uint8_t height) {
    for (uint8_t i = 0; i < numSprites; i++) {
        printf("[DEBUG] Sprite %d:\n", i);
        for (uint8_t y = 0; y < height; y++) {
            printf("[DEBUG] Row %d: ", y);
            for (uint8_t x = 0; x < width; x++) {
                printf("%04X ", sprite[i][y * width + x]);
            }
            printf("\n");
        }
    }
}