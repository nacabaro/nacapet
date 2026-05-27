#include "storage.h"
#include "memory/memory.h"
#include "defs/defs.h"
#include "utils/utils.h"


const char* TAG_SR = "[STORAGE]";


void storage_readFile(const char* path, struct SpriteData* spriteData) {
    File file = SPIFFS.open(path, "r");
    if (!file) {
        printf("%s Failed to open file for reading\n", TAG_SR);
        return;
    }

    uint8_t width, height, spriteNumber;

    file.read(&width, 1);
    file.read(&height, 1);
    file.read(&spriteNumber, 1);

    if (spriteData->spriteData != NULL) {
        memory_free(spriteData);
    }

    const uint8_t scaledW = width  * SPRITE_SCALE;
    const uint8_t scaledH = height * SPRITE_SCALE;

    uint16_t** scaled =
        (uint16_t**) ps_malloc(spriteNumber * sizeof(uint16_t*));

    if (!scaled) {
        printf("%s PSRAM alloc failed for pointer table\n", TAG_SR);
        file.close();
        return;
    }

    for (uint8_t i = 0; i < spriteNumber; i++) {

        scaled[i] =
            (uint16_t*) ps_malloc(scaledW * scaledH * sizeof(uint16_t));

        if (!scaled[i]) {
            printf("%s PSRAM alloc failed for sprite %d\n", TAG_SR, i);

            for (uint8_t j = 0; j < i; j++) {
                free(scaled[j]);
            }

            free(scaled);
            file.close();
            return;
        }
    }

    uint16_t* spriteBuf =
        (uint16_t*) malloc(width * height * sizeof(uint16_t));

    if (!spriteBuf) {
        printf("%s scratch alloc failed\n", TAG_SR);

        for (uint8_t i = 0; i < spriteNumber; i++) {
            free(scaled[i]);
        }

        free(scaled);
        file.close();
        return;
    }

    printf(
        "%s Read header: width=%d, height=%d, numSprites=%d -> scaled to %dx%d\n",
        TAG_SR,
        width,
        height,
        spriteNumber,
        scaledW,
        scaledH
    );

    for (int sprN = 0; sprN < spriteNumber; sprN++) {

        // Read original sprite into temporary buffer
        for (int y = 0; y < height; y++) {

            for (int x = 0; x < width; x++) {

                uint8_t hi, lo;

                file.read(&hi, 1);
                file.read(&lo, 1);

                spriteBuf[y * width + x] =
                    (lo << 8) | hi;
            }
        }

        // Upscale sprite
        utils_upscaleSprite(
            spriteBuf,
            width,
            height,
            scaled[sprN]
        );
    }

    free(spriteBuf);
    file.close();

    spriteData->spriteWidth  = scaledW;
    spriteData->spriteHeight = scaledH;
    spriteData->spriteNumber = spriteNumber;
    spriteData->spriteData   = scaled;

    printf(
        "%s Loaded & upscaled %s (%d sprites, each %dx%d px)\n",
        TAG_SR,
        path,
        spriteNumber,
        scaledW,
        scaledH
    );
}