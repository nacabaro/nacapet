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
    int8_t selectedBackground = currentBackground;

    fs::File bgFolder = SPIFFS.open("/bg");
    fs::File background = bgFolder.openNextFile();

    uint8_t backgrounds = 0;

    uint64_t currentTime = esp_timer_get_time();

    int8_t selectedPreviousBackground = 0;

    while (background) {
        if (!background.isDirectory()) {
            backgrounds++;
        }

        background = background.openNextFile();
    }

    for (;;) {
        uint8_t buttonsPressed = buttons_getPressedButtons();
        currentTime = esp_timer_get_time();

        switch (buttonsPressed) {
            case K1_PRESSED:
                selectedBackground++;
                if (selectedBackground > backgrounds) { 
                    selectedBackground = 0;
                }
                storage_initBackground(selectedBackground, bg);
                lastUpdateTime = currentTime;
                break;

            case K2_PRESSED:
                selectedBackground--;
                if (selectedBackground < 0) {
                    selectedBackground = backgrounds - 1;
                }
                storage_initBackground(selectedBackground, bg);
                lastUpdateTime = currentTime;
                break;
            
            case K3_PRESSED:
                currentBackground = selectedBackground;
                lastUpdateTime = currentTime;
                return;
            
            case K4_PRESSED:
                storage_initBackground(currentBackground, bg);
                lastUpdateTime = currentTime;
                return;
        }

        if (selectedPreviousBackground != selectedBackground) {
            draw_drawBackground(bg, 90, 90, 3);
            draw_drawSprite(sprite, 174, 96, uiSpriteData, ARROW_ICON);
            tft_drawBuffer();
        }

        if (currentTime - lastUpdateTime > INACTIVITY_THRESHOLD_TIME_US) {
            storage_initBackground(currentBackground, bg);
            return;
        }
    }

}