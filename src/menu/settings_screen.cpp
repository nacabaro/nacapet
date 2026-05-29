#include "buttons/buttons.h"
#include "defs/chara_data.h"
#include "defs/defs.h"
#include "display/display.h"
#include "draw/draw.h"
#include "menu.h"
#include "vpet/vpet/vpet.h"

enum SettingsItemType {
    SETTINGS_SUBMENU,
    SETTINGS_ACTION
};

struct SettingsMenuItem {
    const char* label;
    SettingsItemType type;
    uint8_t targetScreen;
    void (*actionFunction)();
    bool* stateValue;
};

void switchSoundState() {
    if (soundEnabled) {
        pinMode(SPK_PIN, INPUT);
    } else {
        pinMode(SPK_PIN, OUTPUT);
    }

    soundEnabled = !soundEnabled;
    printf("[SETTINGS] soundEnabled=%i\n", soundEnabled);
}


void switchScreenState() {
    alwaysOnEnabled = !alwaysOnEnabled;
    printf("[SETTINGS] alwaysOnEnabled=%i\n", alwaysOnEnabled);
}

void menu_settingsScreen(TFT_eSprite &bg, TFT_eSprite &mainChara, struct SpriteData *spriteData)
{
    static int8_t arrowPosition = 0;

    static SettingsMenuItem menuItems[] = {
        { "BG", SETTINGS_SUBMENU, BACKGROUND_CHANGE_SCREEN, nullptr, nullptr },
        { "SOUND", SETTINGS_ACTION, 0, switchSoundState, &soundEnabled },
        { "AOD", SETTINGS_ACTION, 0, switchScreenState, &alwaysOnEnabled },
        { "RESET", SETTINGS_SUBMENU, RESET_DATA_SCREEN, nullptr, nullptr }
    };

    const uint8_t numItems = sizeof(menuItems) / sizeof(menuItems[0]);

    uint8_t pressedButtons = buttons_getPressedButtons();
    switch (pressedButtons) {
        case K1_PRESSED:
        printf("[FOOD] arrowPosition=%i\n", arrowPosition);
        arrowPosition--;
        if (arrowPosition < 0) {
            arrowPosition = numItems - 1;
        }
        break;

        case K2_PRESSED:
        printf("[FOOD] arrowPosition=%i\n", arrowPosition);
        arrowPosition++;
        if (arrowPosition >= numItems) {
            arrowPosition = 0;
        } 
        break;

        case K3_PRESSED:
            screenKey = MENU_SCREEN;
            return;

        case K4_PRESSED:
            lastUpdateTime = 0;

            if (menuItems[arrowPosition].type == SETTINGS_ACTION) {
                if (menuItems[arrowPosition].actionFunction != nullptr) {
                    menuItems[arrowPosition].actionFunction();
                }
            } else if (menuItems[arrowPosition].type == SETTINGS_SUBMENU) {
                screenKey = menuItems[arrowPosition].targetScreen;
                return;
            }

            break;

        default:
            break;
    }

    draw_drawBackground(bg, 90, 90, 3);

    const int startX = 20;
    const int startY = 10;
    const int spacingY = 40;

    for (uint8_t i = 0; i < numItems; i++) {
        bool selected = (i == arrowPosition);
        int y = startY + (i * spacingY);

        tft_drawText(menuItems[i].label, 4, startX, y, selected ? TFT_BLACK : TFT_WHITE, selected ? TFT_WHITE : TFT_TRANSPARENT);

        if (menuItems[i].stateValue != nullptr) {
            const char* stateText = (*menuItems[i].stateValue) ? "ON" : "OFF";

            tft_drawText(stateText, 4, 150, y, selected ? TFT_BLACK : TFT_WHITE, selected ? TFT_WHITE : TFT_TRANSPARENT);
        }
    }

    tft_drawBuffer();
}
