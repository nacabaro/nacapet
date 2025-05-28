#include "menu.h"
#include "draw/draw.h"
#include "display/display.h"
#include "buttons/buttons.h"
#include "defs/defs.h"
#include "defs/sprite_data.h"
#include "defs/chara_data.h"
#include "animations/animations.h"
#include "vpet/vpet.h"
#include "vpet/lines/lines.h"

void menu_eggHatchScreen(TFT_eSprite &composite, TFT_eSprite &bg, TFT_eSprite &sprite, struct SpriteData* uiBigSprite, struct SpriteData* uiSmallSprite) {
    static bool eggSpriteFrame = false;
    uint8_t frameCounter = 0;

    uint8_t pressedButtons = buttons_getPressedButtons();
    switch (pressedButtons) {
        case K3_PRESSED:
        case K1_PRESSED:
            break;
        
        case K2_PRESSED:
            screenKey = CLOCK_SCREEN;
            break;

        default:
            break;
    }

    uint64_t currentTime = esp_timer_get_time();
    if (currentTime - lastUpdateTime > ANIMATION_THRESHOLD_TIME_US) {
        if (charaData.hatchTimer <= currentLine[currentCharacter]->hatchTime) {
            draw_drawBackground(composite, bg, 90, 90, 3);
            draw_drawSpriteCentered(composite, sprite, &currentEgg->eggSprite, eggSpriteFrame, 6);
    
            eggSpriteFrame = !eggSpriteFrame;
            lastUpdateTime = currentTime;

            tft_drawBuffer(composite);

            
        } else if (charaData.hatchTimer > currentLine[currentCharacter]->hatchTime && !charaData.hatched) {
            for (int i = 0; i < 30; i++) {
                tone(SPK_PIN, 4100, 35);
                tone(SPK_PIN, 3500, 35);

                draw_drawBackground(composite, bg, 90, 90, 3);
                animate_performHatchingAnimation(composite, sprite, &currentEgg->eggSprite);

                tft_drawBuffer(composite);
            }

            draw_drawBackground(composite, bg, 90, 90, 3);
            draw_drawSpriteCentered(composite, sprite, &currentEgg->eggSprite, 2, 6);
            
            tft_drawBuffer(composite);
            
            delay(2000);
                        
            lines_onHatchComplete();
            
            vpet_computeCallLight();

            interruptKey = CARE_MISTAKE_SCREEN;
            screenKey = TIMER_FINISHED_SCREEN;

            return;
        }
    }
}