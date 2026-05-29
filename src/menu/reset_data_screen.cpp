#include <Arduino.h>

#include "buttons/buttons.h"
#include "defs/defs.h"
#include "display/display.h"
#include "draw/draw.h"
#include "menu.h"
#include "storage/storage.h"

void menu_resetDataScreen(TFT_eSprite &bg) {
    static uint8_t selectedOption = 0;

    uint8_t pressedButtons = buttons_getPressedButtons();
    switch (pressedButtons) {
        case K1_PRESSED:
            selectedOption = (selectedOption + 1) % 2;
            break;

        case K2_PRESSED:
            if (selectedOption == 1) {
                draw_drawBackground(bg, 90, 90, 3);
                tft_drawCenteredText("RESETTING", 3, 104);
                tft_drawBuffer();

                if (storage_deleteState()) {
                    ESP.restart();
                }

                draw_drawBackground(bg, 90, 90, 3);
                tft_drawCenteredText("FAILED", 4, 95);
                tft_drawBuffer();
                return;
            }

            selectedOption = 0;
            screenKey = SETTINGS_SCREEN;
            return;

        case K3_PRESSED:
            selectedOption = 0;
            screenKey = SETTINGS_SCREEN;
            return;

        default:
            break;
    }

    draw_drawBackground(bg, 90, 90, 3);

    tft_drawCenteredText("RESET DATA?", 3, 35);

    const char* labels[] = { "NO", "YES" };
    const int startX = 72;
    const int startY = 95;
    const int spacingY = 42;

    for (uint8_t i = 0; i < 2; i++) {
        bool selected = (i == selectedOption);
        int y = startY + (i * spacingY);

        tft_drawText(labels[i], 4, startX, y, selected ? TFT_BLACK : TFT_WHITE, selected ? TFT_WHITE : TFT_TRANSPARENT);
    }

    tft_drawBuffer();
}
