#include "menu.h"
#include "defs/defs.h"
#include "defs/sprite_data.h"
#include "display/display.h"
#include "draw/draw.h"
#include "animations/animations.h"

void menu_drawAngryScreen(
    TFT_eSprite &bg, TFT_eSprite &sprite,
    struct SpriteData* spriteData, struct SpriteData* smallUiElements
) {
    uint8_t frameCounter = 0;
    
    while (true) {
        uint64_t currentTime = esp_timer_get_time();
        if (currentTime - lastUpdateTime > ANIMATION_THRESHOLD_TIME_US) {
            if (frameCounter > 3) {
                screenKey = MAIN_SCREEN; // TODO: Change for while battling
                menuKey = STATUS_SCREEN;

                vTaskResume(secondLoop);

                return;
            }

            draw_drawBackground(bg, 90, 90, 3);

            tft_clearBuffer(sprite, TFT_TRANSPARENT);
            animate_performAngryAnimation(sprite, spriteData);
            
            if (frameCounter % 2 == 0) {
                tone(SPK_PIN, 1000, 100);
                tone(SPK_PIN, 1000, 200);

                tft_clearBuffer(sprite, TFT_TRANSPARENT);
                draw_drawSprite(sprite, 18, 72, smallUiElements, FIREWORKS_ICON, 6);
                draw_drawSprite(sprite, 174, 72, smallUiElements, FIREWORKS_ICON, 6);
            }

            frameCounter++;            
            lastUpdateTime = currentTime;
        }
        
        tft_drawBuffer();
    }
}