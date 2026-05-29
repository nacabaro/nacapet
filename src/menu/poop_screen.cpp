#include "menu.h"
#include "draw/draw.h"
#include "animations/animations.h"
#include "display/display.h"
#include "defs/defs.h"
#include "defs/chara_data.h"
#include "defs/sounds.h"
#include "defs/sprite_data.h"

void menu_poopScreen(
    TFT_eSprite &bg, TFT_eSprite &sprite,
    struct SpriteData* spriteData, struct SpriteData* smallUiElements, struct SpriteData* bigUiElements
) {
    static uint8_t animationFrame = 0;
    static bool animationPosition = 0;
    static bool beepedAlready = false;

    uint64_t currentTime = esp_timer_get_time();

    if (currentTime - lastUpdateTime > ANIMATION_THRESHOLD_TIME_US && animationFrame < 4) {
        draw_drawBackground(bg, 90, 90, 3);

        tft_clearBuffer(sprite, TFT_TRANSPARENT);
        draw_drawSprite(sprite, 72 + (animationPosition * 6), 72, spriteData, 6);

        tft_clearBuffer(sprite, TFT_TRANSPARENT);
        menu_uiOverlay(sprite, bigUiElements);

        animationFrame++;
        animationPosition = !animationPosition;

        lastUpdateTime = currentTime;
        tft_drawBuffer();

    } else if (currentTime - lastUpdateTime > ANIMATION_THRESHOLD_TIME_US && animationFrame < 6) {
        if (!beepedAlready) {
            sound_playMelody(SOUND_POOP_DROP, SOUND_NOTE_COUNT(SOUND_POOP_DROP));

            beepedAlready = true;
        }

        draw_drawBackground(bg, 90, 90, 3);

        tft_clearBuffer(sprite, TFT_TRANSPARENT);
        draw_drawSprite(sprite, 174, 120, smallUiElements, POOP_ICON);

        tft_clearBuffer(sprite, TFT_TRANSPARENT);
        menu_uiOverlay(sprite, bigUiElements);

        tft_clearBuffer(sprite, TFT_TRANSPARENT);
        draw_drawSprite(sprite, 72, 72, spriteData, 7);

        animationFrame++;
        animationPosition = !animationPosition;

        lastUpdateTime = currentTime;
        tft_drawBuffer();

    } else if (animationFrame >= 6) {
        if (
            (charaData[currentCharacter].hunger == 0 && !charaData[currentCharacter].hungerCareMistakeObtained) ||
            (charaData[currentCharacter].strength == 0 && !charaData[currentCharacter].strengthCareMistakeObtained) ||
            (charaData[currentCharacter].sleepy && !charaData[currentCharacter].asleep && !charaData[currentCharacter].sleepCareMistakeObtained)
        ) {
            screenKey = CARE_MISTAKE_SCREEN;
        } else  {
            screenKey = MAIN_SCREEN;
        }

        menuKey = -1;
        animationFrame = 0;
        animationPosition = 0;
        beepedAlready = false;
    }
}
