#include "utils.h"
#include "defs/defs.h"


/**
 * Upscale a sprite using nearest-neighbor scaling.
 *
 * @param src          Pointer to source sprite pixels
 * @param srcWidth     Original sprite width
 * @param srcHeight    Original sprite height
 * @param dst          Destination buffer for upscaled sprite
 */
void utils_upscaleSprite(
    const uint16_t* src,
    uint8_t srcWidth,
    uint8_t srcHeight,
    uint16_t* dst
) {
    const uint8_t scaledW = srcWidth * SPRITE_SCALE;

    for (int srcY = 0; srcY < srcHeight; srcY++) {

        for (int dy = 0; dy < SPRITE_SCALE; dy++) {

            uint16_t* dstRow =
                dst + ((srcY * SPRITE_SCALE + dy) * scaledW);

            for (int srcX = 0; srcX < srcWidth; srcX++) {

                uint16_t color = src[srcY * srcWidth + srcX];

                uint16_t* dstPixel =
                    dstRow + (srcX * SPRITE_SCALE);

                for (int dx = 0; dx < SPRITE_SCALE; dx++) {
                    dstPixel[dx] = color;
                }
            }
        }
    }
}