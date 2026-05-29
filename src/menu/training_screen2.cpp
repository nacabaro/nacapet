#include "menu.h"
#include "vpet/training/training.h"
#include "defs/defs.h"
#include "defs/chara_data.h"
#include "defs/sounds.h"
#include "draw/draw.h"
#include "display/display.h"
#include "buttons/buttons.h"

void training_screenTraining2(
    TFT_eSprite &bg, TFT_eSprite &sprite,
    struct SpriteData* mainCharaData, struct SpriteData* attackSprites
) {
    enum TrainingState {
        TRAINING_INIT,
        TRAINING_WAIT_INPUT,
        TRAINING_SHOW_BOTH,
        TRAINING_SHOW_HIT
    };

    static TrainingState state = TRAINING_INIT;
    static uint64_t inactivityTimer = 0;
    static uint64_t stageStartTime = 0;
    static uint8_t randomPosition = 0;
    static uint8_t projectilePosition = 0;

    uint64_t currentTime = esp_timer_get_time();

    if (state == TRAINING_INIT) {
        draw_drawBackground(bg, 90, 90 ,3);

        // Player
        draw_drawSprite(sprite, 174, 72, mainCharaData, 0, false);
        draw_drawBackgroundSection(bg, 222, 72, 18, 96);

        // Opponent
        draw_drawSprite(sprite, -30, 72, mainCharaData, 0, true);
        draw_drawBackgroundSection(bg, 0, 72, 18, 96);

        tft_drawBuffer();

        inactivityTimer = currentTime;
        state = TRAINING_WAIT_INPUT;
        return;
    }

    if (currentTime - lastUpdateTime > 500000) {
        sound_playMelody(SOUND_BUTTON_BEEP, SOUND_NOTE_COUNT(SOUND_BUTTON_BEEP));
        lastUpdateTime = currentTime;
    }

    if (state == TRAINING_WAIT_INPUT) {
        if (currentTime - inactivityTimer > 3000000) {
            screenKey = MAIN_SCREEN;
            submenuKey = STATUS_SCREEN_MENU;
            state = TRAINING_INIT;
            return;
        }

        uint8_t pressedButtons = buttons_getPressedButtons();
        if (pressedButtons == K1_PRESSED || pressedButtons == K2_PRESSED) {
            randomPosition = rand() % 2;
            projectilePosition =  pressedButtons >> 3; // pressedButtons >> 4 == 1 => K1; != 1 => K2
            printf("[TRAIN] projectilePosition=%i\n", projectilePosition);

            inactivityTimer = currentTime;

            // Player attack
            draw_drawSprite(sprite, 126, 72 + 48 * projectilePosition, attackSprites, POOP_ICON);

            // Opponent attack
            draw_drawSprite(sprite, 66, 72 + 48 * randomPosition, attackSprites, FOOD_ICON);

            tft_drawBuffer();

            stageStartTime = currentTime;
            state = TRAINING_SHOW_BOTH;

        } else if (pressedButtons == K3_PRESSED) {
            screenKey = MAIN_SCREEN;
            submenuKey = STATUS_SCREEN_MENU;
            state = TRAINING_INIT;
            return;
        }
    } else if (state == TRAINING_SHOW_BOTH && currentTime - stageStartTime > 500000) {
        if (projectilePosition != randomPosition) {
            draw_drawBackgroundSection(bg, 126, 72 + 48 * projectilePosition, 48, 48);
            draw_drawSprite(sprite, 66, 72 + 48 * projectilePosition, attackSprites, POOP_ICON);

            tft_drawBuffer();

            stageStartTime = currentTime;
            state = TRAINING_SHOW_HIT;
        } else {
            draw_drawBackgroundSection(bg, 66, 72, 108, 96);
            tft_drawBuffer();
            printf("[TRAIN] Train failed\n");
            screenKey = ANGRY_SCREEN;
            state = TRAINING_INIT;
        }
    } else if (state == TRAINING_SHOW_HIT && currentTime - stageStartTime > 500000) {
        draw_drawBackgroundSection(bg, 66, 72, 108, 96);
        tft_drawBuffer();
        charaData[currentCharacter].strength++;
        screenKey = HAPPY_SCREEN;
        state = TRAINING_INIT;
    }
}
