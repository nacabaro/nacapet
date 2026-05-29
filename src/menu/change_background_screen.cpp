#include "menu.h"
#include "defs/sprite_data.h"
#include "buttons/buttons.h"
#include "draw/draw.h"
#include "storage/storage.h"
#include "display/display.h"
#include "defs/screen_defs.h"

void menu_changeBackgroundScreen(
    TFT_eSprite &bg, TFT_eSprite &sprite, struct SpriteData* uiSpriteData
) {
    static bool initialized = false;
    static int8_t selectedBackground = 0;
    static int8_t selectedPreviousBackground = -1;
    static uint8_t backgrounds = 0;
    static uint64_t screenLastActionTime = 0;

    uint64_t currentTime = esp_timer_get_time();

    if (!initialized) {
        fs::File bgFolder = SPIFFS.open("/bg");
        fs::File background = bgFolder.openNextFile();

        backgrounds = 0;
        while (background) {
            if (!background.isDirectory()) {
                backgrounds++;
            }

            background = bgFolder.openNextFile();
        }

        selectedBackground = currentBackground;
        selectedPreviousBackground = -1;
        screenLastActionTime = currentTime;
        initialized = true;

        printf("[BACKGROUNDS] numBackgrounds=%i\n", backgrounds);
    }

    uint8_t buttonsPressed = buttons_getPressedButtons();

    switch (buttonsPressed) {
        case K1_PRESSED:
            selectedBackground++;
            if (selectedBackground >= backgrounds) {
                selectedBackground = 0;
            }
            storage_initBackground(selectedBackground, bg);
            screenLastActionTime = currentTime;
            break;

        case K2_PRESSED:
            selectedBackground--;
            if (selectedBackground < 0) {
                selectedBackground = backgrounds - 1;
            }
            storage_initBackground(selectedBackground, bg);
            screenLastActionTime = currentTime;
            break;

        case K3_PRESSED:
            currentBackground = selectedBackground;
            initialized = false;
            screenKey = MAIN_SCREEN;
            return;

        case K4_PRESSED:
            storage_initBackground(currentBackground, bg);
            initialized = false;
            screenKey = MAIN_SCREEN;
            return;

        default:
            break;
    }

    if (selectedPreviousBackground != selectedBackground) {
        draw_drawBackground(bg, 90, 90, 3);
        draw_drawSprite(sprite, 174, 96, uiSpriteData, ARROW_ICON);
        tft_drawBuffer();
        selectedPreviousBackground = selectedBackground;
    }

    if (currentTime - screenLastActionTime > INACTIVITY_THRESHOLD_TIME_US) {
        printf("[BACKGROUND] Returning home cuz yes\n");
        storage_initBackground(currentBackground, bg);
        initialized = false;
        screenKey = MAIN_SCREEN;
    }
}
