#include "menu.h"
#include "display/display.h"
#include "draw/draw.h"
#include "defs/defs.h"
#include "defs/sprite_data.h"
#include "defs/chara_data.h"
#include "buttons/buttons.h"
#include "vpet/vpet.h"
#include <string.h>

const int textXPos = 10;
const int textYPos = 180;

void menu_drawCurrentMenuOption(TFT_eSprite &composite, TFT_eSprite &bg, TFT_eSprite &icon, struct SpriteData* spriteData) {
    draw_drawBackground(composite, bg, 90, 90, 3);

    uint8_t pressedButtons = buttons_getPressedButtons();

    switch (pressedButtons) {
        case K1_PRESSED:
            menuKey++;
            break;

        case K3_PRESSED:
            screenKey = IDLE_SCREEN;
            menuKey = STATUS_SCREEN_MENU;
            return;
            break;
        
        default:
            break;
    }

    // Separaíto mas guapito
    if (pressedButtons == K2_PRESSED) {
        switch (menuKey) {
            case STATUS_SCREEN_MENU:
                screenKey = STATUS_SCREEN;
                break;
                
            case FOOD_SCREEN_MENU:
                screenKey = FOOD_SCREEN;
                break;

            case SLEEP_SCREEN_MENU:
                menu_sleepScreen_sleepAction();
                break;

            case POOP_SCREEN_MENU:
                menuKey = STATUS_SCREEN;
                screenKey = CLEAR_POOP_SCREEN;
                return;
                break;

            default:
                break;
        }
        return;
    }

    composite.setTextSize(4);
    draw_drawSpriteCentered(composite, icon, spriteData, menuKey % 8, 6);

    switch(menuKey % 9) {
        case STATUS_SCREEN_MENU:
            tft_drawCenteredText(composite, "Status", 4, textYPos);
            break;

        case FOOD_SCREEN_MENU:
            tft_drawCenteredText(composite, "Food", 4, textYPos);
            break;

        case TRAIN_SCREEN_MENU:
            tft_drawCenteredText(composite, "Train", 4, textYPos);
            break;
            
        case BATTLE_SCREEN_MENU:
            tft_drawCenteredText(composite, "Battle", 4, textYPos);
            break;

        case POOP_SCREEN_MENU:
            tft_drawCenteredText(composite, "Cleanup", 4, textYPos);
            break;

        case MEDICAL_SCREEN_MENU:
            tft_drawCenteredText(composite, "Medical", 4, textYPos);
            break;

        case SLEEP_SCREEN_MENU:
            tft_drawCenteredText(composite, "Sleep", 4, textYPos);
            break;
            
        case SETTINGS_SCREEN_MENU:
            tft_drawCenteredText(composite, "Settings", 4, textYPos);
            break;
            
        case 8:
            menuKey = 0;
            screenKey = IDLE_SCREEN;
            return;
            break;
    }

    menu_uiOverlay(composite, icon, spriteData);

    tft_drawBuffer(composite);
}

void menu_sleepScreen_sleepAction() {
    if (charaData.asleep && charaData.sleepy) {
        menu_sleepScreen_recalculateSleep();

        charaData.sleepy = false;
        charaData.asleep = false;

        charaData.sleepDisturbances++;

        menuKey = STATUS_SCREEN;
        screenKey = IDLE_SCREEN;

    } else {
        charaData.asleep = true;

        vpet_computeCallLight();    // Lo hago por cortesia, no me gusta

        menuKey = STATUS_SCREEN;
        screenKey = SLEEP_SCREEN;
    }
}

void menu_sleepScreen_recalculateSleep() {
    uint32_t newSleepTime = (dayUnixTime + 3600) % SECONDS_IN_DAY;
    uint32_t newWakeUpTime = charaData.wakeupTime + 3600;

    charaData.sleepTime = newSleepTime;
    charaData.wakeupTime = newWakeUpTime;

    charaData.dynamicSleepDists++;
}