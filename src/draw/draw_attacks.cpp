#include "draw.h"
#include "defs/sprite_data.h"


void draw_drawAttacks(TFT_eSprite &bg, TFT_eSprite &sprite, struct SpriteData* attackSpriteData, int x, int y, uint8_t attackType, uint8_t attackSprite, bool flipped) {
    // spriteWidth/Height are already pre-scaled at load time
    int cleanWidth = attackSpriteData->spriteWidth + 6; // +4 to be safe
    draw_drawBackgroundSection(bg, x, y, cleanWidth, attackSpriteData->spriteHeight * 2);

    switch(attackType) {
        case 1:
            draw_drawSprite(sprite, x, y, attackSpriteData, attackSprite, flipped);
            draw_drawSprite(sprite, x, y + 48, attackSpriteData, attackSprite, flipped);
            break;

        case 0:
        default:
            draw_drawSprite(sprite, x, y, attackSpriteData, attackSprite, flipped);
            break;
    }
}