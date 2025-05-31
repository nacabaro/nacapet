#include "memory.h"
#include "defs/sprite_data.h"

const char* TAG_M = "[MEMORY]";


uint16_t** memory_allocate(uint8_t numSprite, uint8_t width, uint8_t height) {
    uint16_t** ptr = (uint16_t**) malloc(numSprite * sizeof(uint16_t*));
    if (ptr == NULL) {
        printf("%s Memory allocation failed\n", TAG_M);
        return NULL;
    }

    for (uint8_t i = 0; i < numSprite; i++) {
        ptr[i] = (uint16_t*) malloc(width * height * sizeof(uint16_t));

        if (ptr[i] == NULL) {
            printf("%s Memory allocation failed for sprite %d\n", TAG_M, i);
            for (uint8_t j = 0; j < i; j++) {
                free(ptr[j]);
            }

            free(ptr);

            return NULL;
        }
    }

    printf("%s Allocated %zu bytes at %p\n", TAG_M, numSprite * (width * height * 2), ptr);

    return ptr;
}


void memory_free(struct SpriteData* spriteData) {
    for (int i = 0; i < spriteData->spriteNumber; i++) {
        free(spriteData->spriteData[i]);
    }

    free(spriteData->spriteData);

    spriteData->spriteData = NULL;
}