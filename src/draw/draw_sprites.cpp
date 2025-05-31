#include "draw.h"

#include "defs/sprite_data.h"
#include "defs/screen_defs.h"

#define BUFFER_X 240
#define BUFFER_Y 240

const char* TAG_D = "[DRAW]";


/*void draw_drawSprite(
    TFT_eSprite &spr, int x, int y, 
    struct SpriteData* spriteData, uint8_t spriteNumber, uint8_t factor, bool flipHorizontal 
) {
    int scaledWidth = spriteData->spriteWidth * factor;
    int scaledHeight = spriteData->spriteHeight * factor;
    
    spr.createSprite(scaledWidth, scaledHeight);

    for (int sy = 0; sy < scaledHeight; sy++) {
        for (int sx = 0; sx < scaledWidth; sx++) {
            int srcX = sx / factor;
            if (flipHorizontal) {
                srcX = (spriteData->spriteWidth - 1) - srcX;
            } 

            int srcY = sy / factor;

            uint16_t color = spriteData->spriteData
                [spriteNumber]
                [srcY * spriteData->spriteWidth + srcX];
            
                spr.drawPixel(sx, sy, color);
        }
    }

    spr.pushToSprite(&composite1, x, y, TFT_TRANSPARENT);
    spr.pushToSprite(&composite2, x, y - 120, TFT_TRANSPARENT);

    spriteData->lastX = x;
    spriteData->lastY = y;
    spriteData->lastW = scaledWidth;
    spriteData->lastH = scaledWidth;

    //printf("%s: Sprite %d drawn at (%d, %d) %s\n", TAG_D, spriteNumber, x, y, (flipHorizontal ? "flipped" : ""));
}*/


void draw_drawSprite(
    TFT_eSprite &spr,
    int x, int y,
    struct SpriteData* spriteData,
    uint8_t spriteNumber,
    uint8_t factor,
    bool flipHorizontal
) {
    int srcW = spriteData->spriteWidth;
    int srcH = spriteData->spriteHeight;
    int scaledW = srcW * factor;
    int scaledH = srcH * factor;

    if (spr.width() != scaledW || spr.height() != scaledH) {
        spr.deleteSprite();
        spr.createSprite(scaledW, scaledH);
    }

    uint16_t *sprBuf = (uint16_t *)spr.getPointer();
    uint16_t *srcBuf = spriteData->spriteData[spriteNumber];

    for (int srcY = 0; srcY < srcH; srcY++) {
        int destYBase = srcY * factor;
        for (int srcX = 0; srcX < srcW; srcX++) {
            int useX = flipHorizontal ? (srcW - 1 - srcX) : srcX;
            uint16_t raw  = srcBuf[srcY * srcW + useX];
            uint16_t color = (raw << 8) | (raw >> 8);

            int destXBase = srcX * factor;
            for (int dy = 0; dy < factor; dy++) {
                int rowStart = (destYBase + dy) * scaledW + destXBase;
                for (int dx = 0; dx < factor; dx++) {
                    sprBuf[rowStart + dx] = color;
                }
            }
        }
    }

    spr.pushToSprite(&composite1, x, y, TFT_TRANSPARENT);
    spr.pushToSprite(&composite2, x, y - 120, TFT_TRANSPARENT);

    spriteData->lastX = x;
    spriteData->lastY = y;
    spriteData->lastW = scaledW;
    spriteData->lastH = scaledH;
}


void draw_drawSpriteCentered(
    TFT_eSprite &spr,
    struct SpriteData* spriteData, uint8_t spriteNumber, uint8_t factor, bool flipped, int y
) {
    int x = (BUFFER_X - (spriteData->spriteWidth * factor)) / 2;
    int new_y; 
    if (y == -1) {
        new_y = (BUFFER_Y - (spriteData->spriteHeight * factor)) / 2;
    } else {
        new_y = y;
    }

    draw_drawSprite(
        spr, x, new_y, 
        spriteData, spriteNumber, factor, flipped
    );
}