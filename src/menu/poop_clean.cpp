#include "menu.h"
#include "draw/draw.h"
#include "display/display.h"
#include "defs/defs.h"
#include "defs/chara_data.h"
#include "animations/animations.h"
#include "defs/sprite_data.h"

void menu_clearPoopScreen(
    TFT_eSprite &bg, TFT_eSprite &sprite, struct SpriteData* spriteData, struct SpriteData* bigUiElements, struct SpriteData* smallUiElements
) {
    static bool initialized = false;
    static int cleanerXPos = 174;

    if (!initialized) {
        cleanerXPos = 174;
        lastUpdateTime = 0;

        draw_drawBackground(bg, 90, 90, 3);
        uint8_t offsetX = menu_poopOverlay(bg, sprite, smallUiElements);

        animate_performAnimation(sprite, spriteData, offsetX);
        menu_uiOverlay(sprite, bigUiElements);
        tft_clearBuffer(sprite, TFT_TRANSPARENT);

        initialized = true;
    }

    if (cleanerXPos <= 18 - 48) {
        screenKey = HAPPY_SCREEN;
        menuKey = -1;
        charaData[currentCharacter].poopNumber = 0;
        initialized = false;

        return;
    }

    uint64_t currentTime = esp_timer_get_time();
    if (currentTime - lastUpdateTime > 50000) {
        draw_drawBackgroundSection(bg, cleanerXPos + 6, 72, 48, 96);

        draw_drawSprite(sprite, cleanerXPos, 72, smallUiElements, CLEANER_ICON);
        draw_drawSprite(sprite, cleanerXPos, 120, smallUiElements, CLEANER_ICON);

        draw_drawBackgroundSection(bg, 0, 72, 18, 96);

        tft_drawBuffer();

        cleanerXPos -= 6;
        lastUpdateTime = currentTime;
    }
}
