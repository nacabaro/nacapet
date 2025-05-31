#include "menu.h"
#include "defs/defs.h"
#include "defs/sprite_data.h"
#include "defs/chara_data.h"
#include "buttons/buttons.h"
#include "display/display.h"
#include "draw/draw.h"

void menu_statusScreen(TFT_eSprite &bg, TFT_eSprite &sprite, struct SpriteData* spriteData) {
    tft_clearBuffer(sprite, TFT_TRANSPARENT);

    uint8_t pressedButtons = buttons_getPressedButtons();
    switch (pressedButtons) {
        case 2:
            screenKey = MENU_SCREEN;
            break;

        default:
            break;
    }

    draw_drawBackground(bg, 90, 90, 3);

    menu_statusScreen_drawStat(sprite, spriteData, 10, 10, "Hunger", charaData[currentCharacter].hunger);
    menu_statusScreen_drawStat(sprite, spriteData, 10, 80, "Strength", charaData[currentCharacter].strength);
    menu_statusScreen_drawStat(sprite, spriteData, 10, 150, "Effort", charaData[currentCharacter].effort);

    tft_drawBuffer();
}

void menu_statusScreen_drawStat(TFT_eSprite &sprite, struct SpriteData* spriteData, int x, int y, const char* text, uint8_t statValue) {
    uint8_t icon;
    
    tft_drawText(text, 4, x, y);
    
    for (int i = 0; i < 4; i++) {
        if (i < statValue) { icon = FULL_HEART_ICON; } 
        else { icon = EMPTY_HEART_ICON; }

        draw_drawSprite(
            sprite, 
            15 + (i * 32),
            y + 30, 
            spriteData, 
            icon, 
            4, 
            false
        );
    }
}