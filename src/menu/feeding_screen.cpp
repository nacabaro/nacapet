#include "menu.h"
#include "animations/animations.h"
#include "buttons/buttons.h"
#include "draw/draw.h"
#include "display/display.h"

void menu_feedingScreen(
    TFT_eSprite &bg, TFT_eSprite &charaSprite, 
    struct SpriteData* uiSpriteData, struct SpriteData* charaSpriteData, uint8_t item
) {
    static int currentAnimationFrame = 0;
    
    uint64_t currentTime = esp_timer_get_time();
    
    if (currentTime - lastUpdateTime > ANIMATION_THRESHOLD_TIME_US) {
        draw_drawBackground(bg, 90, 90, 3);
        
        tft_clearBuffer(charaSprite, TFT_TRANSPARENT);
        animate_performEatingAnimation(charaSprite, charaSpriteData);
        
        tft_clearBuffer(charaSprite, TFT_TRANSPARENT);
        draw_drawSprite(charaSprite, 24, 120, uiSpriteData, item, 6);
        
        lastUpdateTime = currentTime;
        currentAnimationFrame++;
    }
    
    if (currentAnimationFrame > 6) {
        screenKey = FOOD_SCREEN;
        lastPressedButtonTime = currentTime;
        currentAnimationFrame = 0;
        submenuKey = -1;
    } 
    
    tft_drawBuffer();
    
    uint8_t pressedButtons = buttons_getPressedButtons();
    switch (pressedButtons) {
        case 8: 
        case 4:
            screenKey = FOOD_SCREEN;
            currentAnimationFrame = 0;
            lastPressedButtonTime = currentTime;
            submenuKey = -1;
            break;
        
        default:
            break;
    }
}