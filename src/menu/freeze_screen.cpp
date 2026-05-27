#include "menu.h"
#include "buttons/buttons.h"
#include "draw/draw.h"
#include "display/display.h"

void menu_drawFridgeScreen(TFT_eSprite &bg, TFT_eSprite& sprite, struct SpriteData* smallUiElements, struct SpriteData* bigUiElements) {
    uint8_t pressedButtons = buttons_getPressedButtons();
    switch (pressedButtons) {
        case K1_PRESSED:
            screenKey = MENU_SCREEN;
            menuKey = STATUS_SCREEN_MENU;
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

        draw_drawSpriteCentered(sprite, bigUiElements, FRIDGE_DOOR);
        
        menu_uiOverlay(sprite, bigUiElements);

        lastUpdateTime = currentTime;
    }
    
    tft_drawBuffer();    
}