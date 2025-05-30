#include "menu.h"
#include "draw/draw.h"
#include "display/display.h"
#include "buttons/buttons.h"
#include "defs/defs.h"
#include "defs/sprite_data.h"
#include "defs/chara_data.h"
#include "animations/animations.h"
#include "vpet/vpet/vpet.h"
#include "vpet/lines/lines.h"

void menu_eggHatchScreen(TFT_eSprite &bg, TFT_eSprite &sprite, struct SpriteData* uiBigSprite, struct SpriteData* uiSmallSprite) {
    static bool eggSpriteFrame = false;
    
    uint8_t pressedButtons = buttons_getPressedButtons();
    switch (pressedButtons) {       
        case K2_PRESSED:
            screenKey = CLOCK_SCREEN;
            break;

        default:
            break;
    }

    uint64_t currentTime = esp_timer_get_time();
    if (currentTime - lastUpdateTime > ANIMATION_THRESHOLD_TIME_US) {
        if (charaData.hatchTimer <= currentLine[currentCharacter]->hatchTime) {
            draw_drawBackground(bg, 90, 90, 3);
            draw_drawSpriteCentered(sprite, &currentEgg->eggSprite, eggSpriteFrame, 6);
    
            eggSpriteFrame = !eggSpriteFrame;
            lastUpdateTime = currentTime;

            tft_drawBuffer();

            
        } else if (charaData.hatchTimer > currentLine[currentCharacter]->hatchTime && !charaData.hatched) {
            for (int i = 0; i < 30; i++) {
                tone(SPK_PIN, 4100, 35);
                tone(SPK_PIN, 3500, 35);

                draw_drawBackground(bg, 90, 90, 3);
                animate_performHatchingAnimation(sprite, &currentEgg->eggSprite);

                tft_drawBuffer();
            }

            draw_drawBackground(bg, 90, 90, 3);

            draw_drawSpriteCentered(sprite, &currentEgg->eggSprite, 2, 6);
            
            tft_drawBuffer();
            
            delay(2000);
                        
            lines_onHatchComplete();
            
            return;
        }
    }
}