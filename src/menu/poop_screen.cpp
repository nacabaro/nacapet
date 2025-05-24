#include "menu.h"
#include "draw/draw.h"
#include "animations/animations.h"
#include "display/display.h"
#include "defs/defs.h"
#include "defs/chara_data.h"
#include "defs/sprite_data.h"

void menu_poopScreen(
    TFT_eSprite &composite, TFT_eSprite &bg, TFT_eSprite &sprite, 
    struct SpriteData* spriteData, struct SpriteData* smallUiElements, struct SpriteData* bigUiElements
) {
    uint8_t animationFrame = 0;
    bool animationPosition = 0;

    bool beepedAlready = false;

    while (1) {
        uint64_t currentTime = esp_timer_get_time();

        if (currentTime - lastUpdateTime > ANIMATION_THRESHOLD_TIME_US && animationFrame < 4) {
            draw_drawBackground(composite, bg, 90, 90, 3);
    
            tft_clearBuffer(sprite, TFT_TRANSPARENT);
            draw_drawSprite(composite, sprite, 72 + (animationPosition * 6), 72, spriteData, 6, 6);

            tft_clearBuffer(sprite, TFT_TRANSPARENT);
            menu_uiOverlay(composite, sprite, bigUiElements);
            
            animationFrame++;
            animationPosition = !animationPosition;

            lastUpdateTime = currentTime;

        } else if (currentTime - lastUpdateTime > ANIMATION_THRESHOLD_TIME_US && animationFrame < 6) {
            if (!beepedAlready) {
                tone(SPK_PIN, 2500, 50);
                tone(SPK_PIN, 5000, 50);
                tone(SPK_PIN, 2500, 50);
                tone(SPK_PIN, 5000, 50);

                beepedAlready = true;
            }

            draw_drawBackground(composite, bg, 90, 90, 3);
    
            tft_clearBuffer(sprite, TFT_TRANSPARENT);
            draw_drawSprite(composite, sprite, 174, 120, smallUiElements, POOP_ICON, 6);
            
            tft_clearBuffer(sprite, TFT_TRANSPARENT);
            menu_uiOverlay(composite, sprite, bigUiElements);

            tft_clearBuffer(sprite, TFT_TRANSPARENT);
            draw_drawSprite(composite, sprite, 72, 72, spriteData, 7, 6);
    
            animationFrame++;
            animationPosition = !animationPosition;

            lastUpdateTime = currentTime;

        } else if (animationFrame >= 6) {
            if (        
                (charaData.hunger == 0 && !charaData.hungerCareMistakeObtained) || 
                (charaData.strength == 0 && !charaData.strengthCareMistakeObtained) || 
                (charaData.sleepy && !charaData.asleep && !charaData.sleepCareMistakeObtained)
            ) { 
                screenKey = CARE_MISTAKE_SCREEN;
            } else  {
                screenKey = IDLE_SCREEN;
            }
            
            menuKey = 0;
            animationFrame = 0;
            animationPosition = 0;

            break;
        }

        tft_drawBuffer(composite);
    }
}