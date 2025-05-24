#include "menu.h"
#include "defs/defs.h"
#include "defs/sprite_data.h"
#include "display/display.h"
#include "draw/draw.h"
#include "animations/animations.h"

void menu_drawHappyScreen(
    TFT_eSprite &composite, TFT_eSprite &bg, TFT_eSprite &sprite,
    struct SpriteData* spriteData, struct SpriteData* smallUiElements
) {
    uint8_t frameCounter = 0;
    
    while (true) {
        uint64_t currentTime = esp_timer_get_time();
        if (currentTime - lastUpdateTime > ANIMATION_THRESHOLD_TIME_US) {
            if (frameCounter > 3) {
                screenKey = IDLE_SCREEN; // TODO: Change for while battling
                menuKey = STATUS_SCREEN;

                return;
            }

            draw_drawBackground(composite, bg, 90, 90, 3);

            tft_clearBuffer(sprite, TFT_TRANSPARENT);
            animate_performHappyAnimation(composite, sprite, spriteData);
            
            if (frameCounter % 2 == 0) {
                tone(SPK_PIN, 7500, 50);
                tone(SPK_PIN, 5000, 50);
                tone(SPK_PIN, 2500, 50);
                tone(SPK_PIN, 1000, 50);

                tft_clearBuffer(sprite, TFT_TRANSPARENT);
                draw_drawSprite(composite, sprite, 18, 72, smallUiElements, FIREWORKS_ICON, 6);
                draw_drawSprite(composite, sprite, 174, 72, smallUiElements, FIREWORKS_ICON, 6);
            }

            frameCounter++;            
            lastUpdateTime = currentTime;
        }
        
        tft_drawBuffer(composite);
    }
}