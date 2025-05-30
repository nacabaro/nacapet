#include "draw.h"
#include "defs/sprite_data.h"


void draw_drawAttacks(TFT_eSprite &sprite, struct SpriteData* attackSpriteData, uint8_t x, uint8_t y, uint8_t attackType, uint8_t attackSprite, uint8_t factor, bool flipped) {
    switch(attackType) {
        case 1:
            draw_drawSprite(sprite, x, y, attackSpriteData, attackSprite, factor, flipped);
            draw_drawSprite(sprite, x, y + 48, attackSpriteData, attackSprite, factor, flipped);
            break;
        
        case 0:
        default:
            draw_drawSprite(sprite, x, y, attackSpriteData, attackSprite, factor, flipped);
            break;
    }
}