#include "menu.h"
#include "defs/defs.h"
#include "buttons/buttons.h"
#include "display/display.h"

void menu_offScreen(TFT_eSprite &buffer) {
    uint8_t buttons = buttons_getPressedButtons();
    if (buttons != 0) {
        tft_drawBuffer(buffer);
        digitalWrite(BL_PIN, HIGH);
        screenKey = IDLE_SCREEN;
    }
}