#include "menu.h"
#include "draw/draw.h"
#include "display/display.h"
#include "defs/screen_defs.h"
#include "defs/sounds.h"
#include "vpet/evolution/evolution.h"
#include "loop/loop.h"

struct SpriteData* checkerboardPattern;

void menu_createCheckerboard() {
    if (checkerboardPattern != NULL) {
        return;
    }

    const uint8_t SCALE       = 6;
    const uint8_t logicalW    = 34;
    const uint8_t logicalH    = 1;
    const uint16_t scaledW    = logicalW * SCALE;
    const uint16_t scaledH    = logicalH * SCALE;
    const uint32_t bufferSize = scaledW * scaledH;

    checkerboardPattern = (SpriteData*) malloc(sizeof(SpriteData));
    checkerboardPattern->spriteWidth  = scaledW;
    checkerboardPattern->spriteHeight = scaledH;
    checkerboardPattern->spriteNumber = 1;
    checkerboardPattern->spriteData   = (uint16_t**) malloc(sizeof(uint16_t*));
    checkerboardPattern->spriteData[0] = (uint16_t*) malloc(sizeof(uint16_t) * bufferSize);

    uint16_t* buf = checkerboardPattern->spriteData[0];

    for (uint16_t row = 0; row < scaledH; row++) {
        for (uint8_t col = 0; col < logicalW; col++) {
            uint16_t color = (col % 2 == 0) ? TFT_BLACK : TFT_TRANSPARENT;
            uint16_t* dst  = buf + row * scaledW + col * SCALE;
            for (uint8_t dx = 0; dx < SCALE; dx++) {
                dst[dx] = color;
            }
        }
    }
}

void menu_freeCheckerboard() {
    if (checkerboardPattern == NULL) {
        return;
    }

    free(checkerboardPattern->spriteData[0]);
    free(checkerboardPattern->spriteData);
    free(checkerboardPattern);
    checkerboardPattern = NULL;
}

void menu_evolutionScreen(TFT_eSprite& bg, TFT_eSprite &sprite, struct SpriteData* mainCharacterSprites) {
    enum EvolutionPhase {
        EVOLUTION_INIT,
        EVOLUTION_WOBBLE,
        EVOLUTION_RED_FILL,
        EVOLUTION_BLACK_FILL,
        EVOLUTION_APPLY_CHANGE,
        EVOLUTION_GREEN_REVEAL,
        EVOLUTION_FINAL_REVEAL,
        EVOLUTION_DONE
    };

    static EvolutionPhase phase = EVOLUTION_INIT;
    static TFT_eSprite checkerboard = TFT_eSprite(&tft);
    static bool checkerboardShift = false;
    static int frameIndex = 0;

    sound_update();

    uint64_t currentTime = esp_timer_get_time();

    if (phase == EVOLUTION_INIT) {
        menu_createCheckerboard();
        checkerboardShift = false;
        frameIndex = 0;
        tft_clearBuffer(sprite, TFT_TRANSPARENT);
        lastUpdateTime = 0;
        phase = EVOLUTION_WOBBLE;
        return;
    }

    if (phase == EVOLUTION_WOBBLE) {
        if (frameIndex >= 5) {
            draw_drawBackground(bg, 90, 90, 3);
            draw_drawSprite(sprite, 72, 72, mainCharacterSprites, 7);
            tft_clearBuffer(sprite, TFT_TRANSPARENT);
            frameIndex = 0;
            lastUpdateTime = 0;
            phase = EVOLUTION_RED_FILL;
            return;
        }

        if (currentTime - lastUpdateTime > 500000) {
            sound_playMelody(SOUND_EVOLUTION_WOBBLE, SOUND_NOTE_COUNT(SOUND_EVOLUTION_WOBBLE));

            draw_drawBackground(bg, 90, 90, 3);
            draw_drawSprite(sprite, 72 + ((frameIndex % 2 == 0) * 6), 72, mainCharacterSprites, 6);

            tft_drawBuffer();

            frameIndex++;
            lastUpdateTime = currentTime;
        }

        return;
    }

    if (phase == EVOLUTION_RED_FILL) {
        if (frameIndex >= 16) {
            frameIndex = 0;
            lastUpdateTime = 0;
            phase = EVOLUTION_BLACK_FILL;
            return;
        }

        if (currentTime - lastUpdateTime > 100000) {
            uint8_t startYPos = 72 + (frameIndex * 6);

            tft_drawRectangle(18, startYPos, 204, 6, TFT_RED);
            draw_drawSprite(checkerboard, 18, startYPos, checkerboardPattern, 0, checkerboardShift);
            tft_drawBuffer();

            checkerboardShift = !checkerboardShift;
            frameIndex++;
            lastUpdateTime = currentTime;
        }

        return;
    }

    if (phase == EVOLUTION_BLACK_FILL) {
        if (frameIndex >= 16) {
            frameIndex = 15;
            lastUpdateTime = 0;
            phase = EVOLUTION_APPLY_CHANGE;
            return;
        }

        if (currentTime - lastUpdateTime > 100000) {
            uint8_t startYPos = 72 + (frameIndex * 6);

            tft_drawRectangle(18, startYPos, 204, 6, TFT_BLACK);
            tft_drawBuffer();

            checkerboardShift = !checkerboardShift;
            frameIndex++;
            lastUpdateTime = currentTime;
        }

        return;
    }

    if (phase == EVOLUTION_APPLY_CHANGE) {
        change_onChangeComplete();
        lastUpdateTime = 0;
        phase = EVOLUTION_GREEN_REVEAL;
        return;
    }

    if (phase == EVOLUTION_GREEN_REVEAL) {
        if (frameIndex < 0) {
            frameIndex = 15;
            lastUpdateTime = 0;
            phase = EVOLUTION_FINAL_REVEAL;
            return;
        }

        if (currentTime - lastUpdateTime > 100000) {
            uint8_t startYPos = 72 + (frameIndex * 6);

            tft_drawRectangle(18, startYPos, 204, 6, TFT_GREEN);
            draw_drawSprite(checkerboard, 18, startYPos, checkerboardPattern, 0, checkerboardShift);
            tft_drawBuffer();

            checkerboardShift = !checkerboardShift;
            frameIndex--;
            lastUpdateTime = currentTime;
        }

        return;
    }

    if (phase == EVOLUTION_FINAL_REVEAL) {
        if (frameIndex < 0) {
            phase = EVOLUTION_DONE;
            return;
        }

        if (currentTime - lastUpdateTime > 100000) {
            draw_drawBackground(bg, 90, 90, 3);
            draw_drawSprite(sprite, 72, 72, mainCharacterSprites, 7);

            uint8_t rectHeight = 6 * frameIndex;

            tft_drawRectangle(18, 72, 204, rectHeight, TFT_GREEN);

            for (int j = 0; j < frameIndex; j++) {
                uint8_t rectYPos = 72 + (6 * j);

                draw_drawSprite(checkerboard, 18, rectYPos, checkerboardPattern, 0, checkerboardShift);
                checkerboardShift = !checkerboardShift;
            }

            tft_drawBuffer();
            frameIndex--;
            lastUpdateTime = currentTime;
        }

        return;
    }

    sound_playMelody(SOUND_EVOLUTION_COMPLETE, SOUND_NOTE_COUNT(SOUND_EVOLUTION_COMPLETE));

    lastPressedButtonTime = esp_timer_get_time();

    menu_freeCheckerboard();

    vTaskResume(secondLoop);

    phase = EVOLUTION_INIT;
    screenKey = MAIN_SCREEN;

    lastUpdateTime = 0;
}
