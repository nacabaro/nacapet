#ifndef DRAW_H
#define DRAW_H

#include <TFT_eSPI.h>

void draw_drawSprite(
    TFT_eSprite &tft, int x, int y, 
    struct SpriteData* spriteData, uint8_t spriteNumber,
    uint8_t factor, bool flipHorizontal = false
);
void draw_drawBackground(TFT_eSprite &bg, int spr_w, int spr_h, int factor);
void draw_drawSpriteCentered(
    TFT_eSprite &spr, struct SpriteData* spriteData, uint8_t spriteNumber, uint8_t factor, bool flipped = false, int y = -1
);

#endif