#include "menu.h"
#include "display/display.h"
#include "draw/draw.h"
#include "defs/defs.h"
#include "buttons/buttons.h"

void menu_drawTitle(TFT_eSprite &bg) {
    uint8_t pressedButtons = buttons_getPressedButtons();
    if (pressedButtons == 8 || pressedButtons == 4) {
        screenKey = CLOCK_EDIT_SCREEN;
        return;
    }

    draw_drawBackground(bg, 90, 90, 3);

    tft_drawCenteredText("NacaPet", 4, 40);

    tft_drawCenteredText(VERSION, 2, 80);

    tft_drawBuffer();
}