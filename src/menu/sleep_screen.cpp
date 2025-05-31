#include "menu.h"
#include "draw/draw.h"
#include "display/display.h"
#include "defs/defs.h"
#include "buttons/buttons.h"
#include "animations/animations.h"
#include "defs/chara_data.h"

void menu_sleepyScreen(TFT_eSprite &bg, TFT_eSprite &sprite, struct SpriteData* charaSprites, struct SpriteData* uiSprites) {
    if (!charaData[currentCharacter].asleep && !charaData[currentCharacter].sleepy) {
        screenKey = MAIN_SCREEN;
        return;
    } else if (charaData[currentCharacter].asleep && charaData[currentCharacter].sleepy) {
        screenKey = SLEEP_SCREEN;
        return;
    }
    
    uint64_t currentTime = esp_timer_get_time();
    uint8_t pressedButtons = buttons_getPressedButtons();
    switch (pressedButtons) {
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
        draw_drawBackground(bg, 90, 90, 3);

        tft_clearBuffer(sprite, TFT_TRANSPARENT);
        animate_performSleepyAnimation(sprite, charaSprites);
        
        tft_clearBuffer(sprite, TFT_TRANSPARENT);
        menu_uiOverlay(sprite, uiSprites);

        tft_clearBuffer(sprite, TFT_TRANSPARENT);
        menu_uiOverlay(sprite, uiSprites);

        lastUpdateTime = currentTime;
    }

    if (currentTime - lastBeepTime > ANIMATION_THRESHOLD_TIME_US * 2 && beepCounter < 10) {
        tone(SPK_PIN, 2500, 100);
        tone(SPK_PIN, 5000, 100);

        lastBeepTime = currentTime;

        beepCounter++;
    }

    tft_drawBuffer();
}
