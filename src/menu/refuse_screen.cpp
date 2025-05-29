#include "menu.h"
#include "buttons/buttons.h"
#include "draw/draw.h"
#include "display/display.h"
#include "defs/defs.h"
#include "defs/sprite_data.h"
#include "animations/animations.h"

void menu_refuseScreen(TFT_eSprite &bg, TFT_eSprite &mainChara, struct SpriteData* spriteData) {
    static uint8_t currentAnimationFrame = 0;
    static bool soundPlayed = false;

    uint64_t currentTime = esp_timer_get_time();
    uint8_t pressedButtons = buttons_getPressedButtons();
    switch (pressedButtons) {
        case 8:
        case 4:
            currentAnimationFrame = 0;
            screenKey = FOOD_SCREEN;
            soundPlayed = false;
            return;
            break;
    
        default:
            break;
    }

    if (currentTime - lastUpdateTime > ANIMATION_THRESHOLD_TIME_US) {
        draw_drawBackground(bg, 90, 90, 3);
        animate_performRefuseAnimation(mainChara, spriteData);

        lastUpdateTime = currentTime;
        currentAnimationFrame++;
    }

    if (!soundPlayed) {
        tone(SPK_PIN, 3000, 100);
        tone(SPK_PIN, 1000, 100);

        soundPlayed = true;
    }

    if (currentAnimationFrame > 4) {
        currentAnimationFrame = 0;
        soundPlayed = false;
        screenKey = IDLE_SCREEN;
    }

    tft_drawBuffer();
}