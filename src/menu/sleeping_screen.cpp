#include "menu.h"
#include "defs/defs.h"
#include "draw/draw.h"
#include "display/display.h"
#include "buttons/buttons.h"
#include "animations/animations.h"
#include "defs/chara_data.h"

void menu_sleepingScreen(
    TFT_eSprite &composite, TFT_eSprite &bg, TFT_eSprite &sprite, 
    struct SpriteData* mainCharaData, struct SpriteData* bigUiElements, struct SpriteData* smallUIElements
) {
    if (charaData.sleepy && !charaData.asleep) {
        screenKey = SLEEPY_SCREEN;
        return;
    } else if (!charaData.sleepy && !charaData.asleep) {
        screenKey = IDLE_SCREEN;
        return;
    }

    uint64_t currentTime = esp_timer_get_time();
    static uint8_t frameCounter = 0;

    uint8_t buttonsPressed = buttons_getPressedButtons();
    switch (buttonsPressed) {
        case 8:
            screenKey = MENU_SCREEN;
            break;

        case 4:
            screenKey = CLOCK_SCREEN;
            break;

        default:
            break;
    }

    if (currentTime - lastUpdateTime > ANIMATION_SLEEPY_THRESHOLD_TIME_US) {
        draw_drawBackground(composite, bg, 90, 90, 3);
        
        tft_clearBuffer(sprite, TFT_TRANSPARENT);
        animate_performSleepyAnimation(composite, sprite, mainCharaData);

        tft_clearBuffer(sprite, TFT_TRANSPARENT);
        draw_drawSprite(composite, sprite, 72, 72, bigUiElements, BED_SPRITE, 6);
            
        tft_clearBuffer(sprite, TFT_TRANSPARENT);
        draw_drawSprite(composite, sprite, 172, 72, smallUIElements, ZZZ_ICON, 6);

        tft_clearBuffer(sprite, TFT_TRANSPARENT);
        menu_uiOverlay(composite, sprite, bigUiElements);

        lastUpdateTime = currentTime;
    }
    
    tft_drawBuffer(composite);
}