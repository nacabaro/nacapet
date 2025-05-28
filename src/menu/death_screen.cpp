#include "menu.h"
#include "defs/defs.h"
#include "draw/draw.h"
#include "display/display.h"
#include "buttons/buttons.h"
#include "defs/sprite_data.h"
#include "vpet/lines/lines.h"

void menu_drawDeathScreen(TFT_eSprite &composite, TFT_eSprite &bg, TFT_eSprite &sprite, struct SpriteData* uiBigSprite, struct SpriteData* uiSmallSprite) {
    static uint8_t frameCounter = 0;
    uint64_t currentTime = esp_timer_get_time();

    uint8_t pressedButtons = buttons_getPressedButtons();
    switch (pressedButtons) {
        case K1_PRESSED:
            lines_getAvailableLines();
            screenKey = EGG_SELECT_SCREEN;
            return;
            break;
        
        case K2_PRESSED:
            screenKey = CLOCK_SCREEN;
            return;
            break;

        case K3_PRESSED:
            lastUpdateTime = currentTime;
            frameCounter = 0;

        default:
            break;
    }

    if (currentTime - lastUpdateTime > ANIMATION_THRESHOLD_TIME_US) {
        draw_drawBackground(composite, bg, 90, 90, 3);
        draw_drawSprite(composite, sprite, 72 + (6 * (frameCounter == 0)), 72, uiBigSprite, EMPTY_EGG, 6);
        tft_clearBuffer(sprite, TFT_TRANSPARENT);
        menu_uiOverlay(composite, sprite, uiSmallSprite);
        tft_clearBuffer(sprite, TFT_TRANSPARENT);

        frameCounter = (frameCounter + 1) % 32;
        lastUpdateTime = currentTime;
    }

    tft_drawBuffer(composite);
}