#include "menu.h"
#include "buttons/buttons.h"
#include "animations/animations.h"
#include "display/display.h"
#include "defs/chara_data.h"
#include "draw/draw.h"

uint64_t lastBeepTime = esp_timer_get_time();
uint8_t beepCounter = 0;

void menu_careMistakeScreen(TFT_eSprite &bg, TFT_eSprite &sprite, struct SpriteData* spriteData, struct SpriteData* uiSpritesBig) {
    uint64_t currentTime = esp_timer_get_time();
    uint8_t pressedButtons = buttons_getPressedButtons();

    if (currentTime - lastBeepTime > ANIMATION_THRESHOLD_TIME_US * 2 && beepCounter < 10) {
        tone(SPK_PIN, 2500, 100);
        tone(SPK_PIN, 5000, 100);

        lastBeepTime = currentTime;

        beepCounter++;
    }
    
    if (currentTime - lastUpdateTime > ANIMATION_THRESHOLD_TIME_US) {
        draw_drawBackground(bg, 90, 90, 3);

        tft_clearBuffer(sprite, TFT_TRANSPARENT);
        animate_performAttentionAnimation(sprite, spriteData);
    
        tft_clearBuffer(sprite, TFT_TRANSPARENT);
        menu_uiOverlay(sprite, uiSpritesBig);
            
        lastUpdateTime = currentTime;
    }
    
    if (pressedButtons != 0) {
        screenKey = MAIN_SCREEN;
        beepCounter = 0;
    }

    tft_drawBuffer();
}