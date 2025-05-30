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
void draw_drawAttacks(TFT_eSprite &sprite, struct SpriteData* attackSpriteData, uint8_t x, uint8_t y, uint8_t attackType, uint8_t attackSprite, uint8_t factor, bool flipped = false);

#endif