#include "menu.h"
#include "draw/draw.h"
#include "display/display.h"
#include "defs/defs.h"
#include "defs/chara_data.h"
#include "defs/sprite_data.h"

void menu_clearPoopScreen(
    TFT_eSprite &composite, TFT_eSprite &bg, TFT_eSprite &sprite, struct SpriteData* spriteData, struct SpriteData* bigUiElements, struct SpriteData* smallUiElements
) {
    int cleanerXPos = 174;

    menu_drawIdleScreen(composite, bg, sprite, spriteData, bigUiElements, smallUiElements);
    
    tft_clearBuffer(sprite, TFT_TRANSPARENT);
    while (cleanerXPos > 18) {
        draw_drawSprite(composite, sprite, cleanerXPos, 72, smallUiElements, CLEANER_ICON, 6);
        draw_drawSprite(composite, sprite, cleanerXPos, 120, smallUiElements, CLEANER_ICON, 6);
        tft_drawBuffer(composite);
        cleanerXPos -= 6;
        delay(50);
    }

    screenKey = HAPPY_SCREEN;
    charaData.poopNumber = 0;

    return;
}