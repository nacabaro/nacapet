#include "menu.h"
#include "display/display.h"
#include "draw/draw.h"
#include "buttons/buttons.h"
#include "defs/chara_data.h"
#include "vpet/vpet.h"

void menu_foodScreen(TFT_eSprite &composite, TFT_eSprite &bg, TFT_eSprite &mainChara, struct SpriteData* spriteData) {
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
        switch(arrowPosition) {
            case 0:
                if (charaData.hunger < 8) {
                    charaData.weight++;
                    charaData.hunger++;
                    screenKey = FEEDING_SCREEN;
                    submenuKey = FOOD_ICON;
                } else {
                    screenKey = REFUSING_SCREEN;
                }
                break;

            case 1:
                if (charaData.strength < 8) {
                    charaData.strength++;
                    charaData.weight += 2;
                    screenKey = FEEDING_SCREEN;
                    submenuKey = PILL_ICON;
                } else {
                    screenKey = REFUSING_SCREEN;
                }
                break;
            
            default:
                break;
        }

        vpet_computeCallLight();
    }

    composite.setTextSize(4);

    draw_drawBackground(composite, bg, 90, 90, 3);
    menu_foodScreen_drawEntry(composite, mainChara, spriteData, 0, FOOD_ICON, "Meat");
    menu_foodScreen_drawEntry(composite, mainChara, spriteData, 1, PILL_ICON, "Pill");
    
    draw_drawSprite(composite, mainChara, 5, (arrowPosition * 34) + 5, spriteData, ARROW_ICON, 4);

    tft_drawBuffer(composite);
}

void menu_foodScreen_drawEntry(
    TFT_eSprite &composite, TFT_eSprite &mainChara, struct SpriteData* spriteData, 
    uint8_t entryId, uint8_t spriteNumber, const char* textEntry
) {
    tft_clearBuffer(mainChara, TFT_TRANSPARENT);
    draw_drawSprite(composite, mainChara, 45, (entryId * 34) + 5, spriteData, spriteNumber, 4);
    composite.drawString(textEntry, 80, (entryId * 34) + 5);
}