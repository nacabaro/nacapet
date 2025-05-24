#include "menu.h"
#include "display/display.h"
#include "draw/draw.h"
#include "defs/defs.h"
#include "buttons/buttons.h"

void menu_drawTitle(TFT_eSprite &composite, TFT_eSprite &bg) {
    uint8_t pressedButtons = buttons_getPressedButtons();
    lastPressedButtonTime = esp_timer_get_time();

    if (pressedButtons == 8 || pressedButtons == 4) {
        screenKey = CLOCK_EDIT_SCREEN;
    }

    draw_drawBackground(composite, bg, 90, 90, 3);

    composite.setTextSize(4);
    tft_drawCenteredText(composite, "NacaPet", 4, 40);

    composite.setTextSize(2);
    tft_drawCenteredText(composite, VERSION, 2, 80);

    tft_drawBuffer(composite);
}