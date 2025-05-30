#include "menu.h"
#include "display/display.h"
#include "draw/draw.h"
#include "buttons/buttons.h"
#include "defs/chara_data.h"
#include "vpet/vpet/vpet.h"

void menu_foodScreen(TFT_eSprite &bg, TFT_eSprite &mainChara, struct SpriteData* spriteData) {
    if (charaData.sleepy) {
        tone(SPK_PIN, BEEP_FREQ_HZ, BEEP_LEN_MS);
        delay(100);
        tone(SPK_PIN, BEEP_FREQ_HZ, BEEP_LEN_MS);
        screenKey = MENU_SCREEN;
        
        return;
    }

    static uint8_t arrowPosition = 0;
    
    uint8_t pressedButtons = buttons_getPressedButtons();
    switch (pressedButtons) {
        case 8:
            arrowPosition = (arrowPosition + 1) % 2;
            break;

        case 2:
            screenKey = MENU_SCREEN;
            break;
        
        default:
            break;
    }
    
    if (pressedButtons == 4) {
        lastUpdateTime = 0;
        switch(arrowPosition) {
            case 0:
                if (charaData.hunger < 8) {
                    charaData.hungerCareMistakeTimer = charaData.initialStatsReductionTime;
                    charaData.hungerCareMistakeObtained = false;
                    charaData.weight++;
                    charaData.hunger++;
                    screenKey = FEEDING_SCREEN;
                    submenuKey = FOOD_ICON;
                } else {
                    screenKey = REFUSING_SCREEN;
                    if (!charaData.overfeedHappened) {
                        charaData.overfeed++;
                        charaData.overfeedHappened = true;
                    }
                }
                return;
                break;

            case 1:
                if (charaData.strength < 8) {
                    charaData.strengthCareMistakeTimer = charaData.initialStatsReductionTime;
                    charaData.strength++;
                    charaData.weight += 2;
                    screenKey = FEEDING_SCREEN;
                    submenuKey = PILL_ICON;
                } else {
                    screenKey = REFUSING_SCREEN;
                }
                return;
                break;
            
            default:
                break;
        }

        vpet_computeCallLight();
    }


    draw_drawBackground(bg, 90, 90, 3);
    menu_foodScreen_drawEntry(mainChara, spriteData, 0, FOOD_ICON, "Meat");
    menu_foodScreen_drawEntry(mainChara, spriteData, 1, PILL_ICON, "Pill");
    
    draw_drawSprite(mainChara, 5, (arrowPosition * 34) + 5, spriteData, ARROW_ICON, 4);

    tft_drawBuffer();
}

void menu_foodScreen_drawEntry(
    TFT_eSprite &mainChara, struct SpriteData* spriteData, 
    uint8_t entryId, uint8_t spriteNumber, const char* textEntry
) {
    tft_clearBuffer(mainChara, TFT_TRANSPARENT);
    draw_drawSprite(mainChara, 45, (entryId * 34) + 5, spriteData, spriteNumber, 4);
    tft_drawText(textEntry, 4, 80, (entryId * 34) + 5);
}