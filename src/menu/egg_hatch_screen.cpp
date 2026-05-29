#include "menu.h"
#include "draw/draw.h"
#include "display/display.h"
#include "buttons/buttons.h"
#include "defs/defs.h"
#include "defs/sprite_data.h"
#include "defs/chara_data.h"
#include "animations/animations.h"
#include "vpet/vpet/vpet.h"
#include "vpet/lines/lines.h"
#include "defs/sounds.h"

const uint64_t HATCH_ANIMATION_FRAME_TIME_US = 100000;
const uint64_t HATCH_COMPLETE_HOLD_TIME_US = 2000000;

void menu_eggHatchScreen(TFT_eSprite& bg, TFT_eSprite &sprite, struct SpriteData* uiBigSprite, struct SpriteData* uiSmallSprite) {
    static bool eggSpriteFrame = false;
    static bool hatchSoundStarted = false;
    static uint8_t hatchingFrame = 0;
    static uint64_t hatchHoldStartTime = 0;
    static uint64_t hatchAnimationLastFrameTime = 0;

    sound_update();

    uint8_t pressedButtons = buttons_getPressedButtons();
    switch (pressedButtons) {
        case K2_PRESSED:
            screenKey = CLOCK_SCREEN;
            hatchSoundStarted = false;
            hatchingFrame = 0;
            hatchHoldStartTime = 0;
            hatchAnimationLastFrameTime = 0;
            break;

        default:
            break;
    }

    uint64_t currentTime = esp_timer_get_time();
    if (hatchHoldStartTime > 0) {
        if (currentTime - hatchHoldStartTime > HATCH_COMPLETE_HOLD_TIME_US) {
            hatchHoldStartTime = 0;
            hatchSoundStarted = false;
            hatchingFrame = 0;
            hatchAnimationLastFrameTime = 0;
            lines_onHatchComplete();
        }

        return;
    }

    if (charaData[currentCharacter].hatchTimer > currentLine[currentCharacter]->hatchTime && !charaData[currentCharacter].hatched) {
        if (!hatchSoundStarted) {
            sound_playMelody(SOUND_HATCH, SOUND_NOTE_COUNT(SOUND_HATCH));
            hatchSoundStarted = true;
            hatchAnimationLastFrameTime = 0;
        }

        if (hatchingFrame < 30 && currentTime - hatchAnimationLastFrameTime > HATCH_ANIMATION_FRAME_TIME_US) {
            draw_drawBackground(bg, 90, 90, 3);
            animate_performHatchingAnimation(sprite, &currentEgg->eggSprite);

            tft_drawBuffer();

            hatchingFrame++;
            hatchAnimationLastFrameTime = currentTime;

            return;
        }

        if (hatchingFrame < 30) {
            return;
        }

        draw_drawBackground(bg, 90, 90, 3);

        draw_drawSpriteCentered(sprite, &currentEgg->eggSprite, 2);

        tft_drawBuffer();
        hatchHoldStartTime = currentTime;

        return;
    }

    if (currentTime - lastUpdateTime > ANIMATION_THRESHOLD_TIME_US) {
        draw_drawBackground(bg, 90, 90, 3);
        draw_drawSpriteCentered(sprite, &currentEgg->eggSprite, eggSpriteFrame);

        eggSpriteFrame = !eggSpriteFrame;
        lastUpdateTime = currentTime;

        tft_drawBuffer();
    }
}
