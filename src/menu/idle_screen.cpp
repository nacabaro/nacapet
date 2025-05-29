#include "menu.h"
#include "draw/draw.h"
#include "defs/defs.h"
#include "display/display.h"
#include "buttons/buttons.h"
#include "animations/animations.h"
#include "defs/chara_data.h"

uint64_t lastUpdateTime = esp_timer_get_time();

void menu_drawIdleScreen(TFT_eSprite &bg, TFT_eSprite &sprite, struct SpriteData* spriteData, struct SpriteData* bigUiElements, struct SpriteData* smallUiElements) {
    if (coldBoot) { 
        screenKey = TITLE_SCREEN;
        return;
    } else if (!charaData.hatched && !charaData.hatching) {
        screenKey = EGG_EMPTY_SCREEN;
        return;
    } else if (!charaData.hatched && charaData.hatching) {
        screenKey = EGG_HATCH_SCREEN;
        return;
    } else if (charaData.sleepy && !charaData.asleep) {
        screenKey = SLEEPY_SCREEN;
        return;
    } else if ((charaData.sleepy && charaData.asleep) || charaData.asleep) {
        screenKey = SLEEP_SCREEN;
        return;
    }

    uint8_t pressedButtons = buttons_getPressedButtons();
    
    switch (pressedButtons) {
        case K1_PRESSED:
            screenKey = MENU_SCREEN;
            menuKey = 0;
            break;
        
        case K2_PRESSED:
            screenKey = CLOCK_SCREEN;
            break;
        
        default:
            break;
    }

    uint64_t currentTime = esp_timer_get_time();

    if (currentTime - lastUpdateTime > ANIMATION_THRESHOLD_TIME_US) {
        draw_drawBackground(bg, 90, 90, 3);
        
        tft_clearBuffer(sprite, TFT_TRANSPARENT);
        uint8_t offsetX = menu_poopOverlay(sprite, smallUiElements);
        
        tft_clearBuffer(sprite, TFT_TRANSPARENT);
        animate_performAnimation(sprite, spriteData, offsetX);
        
        tft_clearBuffer(sprite, TFT_TRANSPARENT);
        menu_uiOverlay(sprite, bigUiElements);
            
        lastUpdateTime = currentTime;
    }
    
    tft_drawBuffer();
}
