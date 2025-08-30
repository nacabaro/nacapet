#include "menu.h"
#include "draw/draw.h"
#include "display/display.h"
#include "defs/defs.h"
#include "defs/chara_data.h"
#include "animations/animations.h"
#include "defs/sprite_data.h"

void menu_clearPoopScreen(
    TFT_eSprite &bg, TFT_eSprite &sprite, struct SpriteData* spriteData, struct SpriteData* bigUiElements, struct SpriteData* smallUiElements
) {
    printf("[AAAAAAA] pausing loop...\n");

    vTaskSuspend(secondLoop);

    printf("[AAAAAAA] loop paused...\n");

    int cleanerXPos = 174;

    lastUpdateTime = 0;

    printf("[AAAAAAA] drawing idle screen...\n");
  
    draw_drawBackground(bg, 90, 90, 3);
    uint8_t offsetX = menu_poopOverlay(bg, sprite, smallUiElements);
    
    printf("[AAAAAAA] drawing animation...\n");

    animate_performAnimation(sprite, spriteData, offsetX);

    printf("[AAAAAAA] drawing overlay...\n");

    menu_uiOverlay(sprite, bigUiElements);
    
    printf("[AAAAAAA] idle screen down...\n");

    tft_clearBuffer(sprite, TFT_TRANSPARENT);

    while (cleanerXPos > 18) {
        draw_drawSprite(sprite, cleanerXPos, 72, smallUiElements, CLEANER_ICON, 6);
        draw_drawSprite(sprite, cleanerXPos, 120, smallUiElements, CLEANER_ICON, 6);
        tft_drawBuffer();
        cleanerXPos -= 6;
        delay(50);
    }

    screenKey = HAPPY_SCREEN;
    charaData[currentCharacter].poopNumber = 0;

    return;
}