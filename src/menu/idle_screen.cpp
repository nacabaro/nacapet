#include "menu.h"
#include "draw/draw.h"
#include "defs/defs.h"
#include "display/display.h"
#include "buttons/buttons.h"
#include "animations/animations.h"
#include "defs/chara_data.h"

uint64_t lastUpdateTime = esp_timer_get_time();

void menu_drawIdleScreen(TFT_eSprite &bg, TFT_eSprite &sprite, struct SpriteData* spriteData, struct SpriteData* bigUiElements, struct SpriteData* smallUiElements) {
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
        uint8_t offsetX = menu_poopOverlay(bg, sprite, smallUiElements);
        
        animate_performAnimation(sprite, spriteData, offsetX);
        menu_uiOverlay(sprite, bigUiElements);
            
        lastUpdateTime = currentTime;
    }
    
    tft_drawBuffer();
}
