#include "draw.h"

#include "defs/sprite_data.h"
#include "defs/screen_defs.h"

#define BUFFER_X 240
#define BUFFER_Y 240

const char* TAG_D = "[DRAW]";

void draw_drawSprite(
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

    //printf("%s: Sprite %d drawn at (%d, %d) %s\n", TAG_D, spriteNumber, x, y, (flipHorizontal ? "flipped" : ""));
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