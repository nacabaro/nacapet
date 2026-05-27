#include "draw.h"

#include "defs/sprite_data.h"
#include "defs/screen_defs.h"

#define BUFFER_X 240
#define BUFFER_Y 240

const char* TAG_D = "[DRAW]";


void draw_drawSprite(
    TFT_eSprite &spr,
    int x, int y,
    struct SpriteData* spriteData,
    uint8_t spriteNumber,
    bool flipHorizontal
) {
    // Sprites are pre-scaled at load time; width/height are already final.
    const int W = spriteData->spriteWidth;
    const int H = spriteData->spriteHeight;

    if (spr.width() != W || spr.height() != H) {
        spr.deleteSprite();
        spr.createSprite(W, H);
    }

    uint16_t* sprBuf = (uint16_t*) spr.getPointer();
    uint16_t* srcBuf = spriteData->spriteData[spriteNumber];

    if (!flipHorizontal) {
        // Fast path: one memcpy of the whole frame
        memcpy(sprBuf, srcBuf, W * H * sizeof(uint16_t));
    } else {
        // Mirror each row horizontally
        for (int row = 0; row < H; row++) {
            const uint16_t* src = srcBuf + row * W;
            uint16_t*       dst = sprBuf + row * W;
            for (int col = 0; col < W; col++) {
                dst[col] = src[W - 1 - col];
            }
        }
    }

    spr.pushToSprite(&composite, x, y, TFT_TRANSPARENT);

    spriteData->lastX = x;
    spriteData->lastY = y;
    spriteData->lastW = W;
    spriteData->lastH = H;
}


void draw_drawSpriteCentered(
    TFT_eSprite &spr,
    struct SpriteData* spriteData, uint8_t spriteNumber, bool flipped, int y
) {
    int x = (BUFFER_X - spriteData->spriteWidth)  / 2;
    int new_y = (y == -1)
        ? (BUFFER_Y - spriteData->spriteHeight) / 2
        : y;

    draw_drawSprite(spr, x, new_y, spriteData, spriteNumber, flipped);
}