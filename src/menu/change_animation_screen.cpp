#include "menu.h"
#include "draw/draw.h"
#include "display/display.h"
#include "defs/screen_defs.h"
#include "vpet/evolution/evolution.h"
#include "loop/loop.h"

struct SpriteData* checkerboardPattern;

void menu_createCheckerboard() {
    // Build the pattern pre-scaled by SPRITE_SCALE (6) so that
    // draw_drawSprite can treat it identically to SPIFFS-loaded sprites.
    // Logical size: 34 wide × 1 tall  →  Scaled: 204 wide × 6 tall
    const uint8_t SCALE       = 6;
    const uint8_t logicalW    = 34;
    const uint8_t logicalH    = 1;
    const uint16_t scaledW    = logicalW * SCALE;   // 204
    const uint16_t scaledH    = logicalH * SCALE;   // 6
    const uint32_t bufferSize = scaledW * scaledH;

    checkerboardPattern = (SpriteData*) malloc(sizeof(SpriteData));
    checkerboardPattern->spriteWidth  = scaledW;
    checkerboardPattern->spriteHeight = scaledH;
    checkerboardPattern->spriteNumber = 1;
    checkerboardPattern->spriteData   = (uint16_t**) malloc(sizeof(uint16_t*));
    checkerboardPattern->spriteData[0] = (uint16_t*) malloc(sizeof(uint16_t) * bufferSize);

    uint16_t* buf = checkerboardPattern->spriteData[0];

    // Fill: repeat each logical pixel as a SCALE×SCALE block across all rows
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
    free(checkerboardPattern->spriteData[0]);
    free(checkerboardPattern->spriteData);
    free(checkerboardPattern);

}

// Don't worry, I hate this too
void menu_evolutionScreen(TFT_eSprite& bg, TFT_eSprite &sprite, struct SpriteData* mainCharacterSprites) {
    menu_createCheckerboard();
    TFT_eSprite checkerboard = TFT_eSprite(&tft);

    bool checkerboardShift = false;

    tft_clearBuffer(sprite, TFT_TRANSPARENT);

    for (int i = 0; i < 5;) {
        uint64_t currentTime = esp_timer_get_time();

        if (currentTime - lastUpdateTime > 500000) {
            tone(SPK_PIN, 4100, 50);
            tone(SPK_PIN, 3500, 50);

            draw_drawBackground(bg, 90, 90, 3);
            draw_drawSprite(sprite, 72 + ((i % 2 == 0) * 6), 72, mainCharacterSprites, 6);
            
            tft_drawBuffer();

            i++;
            lastUpdateTime = currentTime;
        }

    }

    draw_drawBackground(bg, 90, 90, 3);
    draw_drawSprite(sprite, 72, 72, mainCharacterSprites, 7);

    tft_clearBuffer(sprite, TFT_TRANSPARENT);

    for (int i = 0; i < 16;) {
        uint64_t currentTime = esp_timer_get_time();
        if (currentTime - lastUpdateTime > 100000) {
            uint8_t startYPos = 72 + (i * 6);
    
            tft_drawRectangle(18, startYPos, 204, 6, TFT_RED);
    
            draw_drawSprite(checkerboard, 18, startYPos, checkerboardPattern, 0, checkerboardShift);
            
            tft_drawBuffer();
    
            checkerboardShift = !checkerboardShift;
    
            i++;
            lastUpdateTime = currentTime;
        }
    }

    for (int i = 0; i < 16;) {
        uint64_t currentTime = esp_timer_get_time();
        if (currentTime - lastUpdateTime > 100000) {
            uint8_t startYPos = 72 + (i * 6);

            tft_drawRectangle(18, startYPos, 204, 6, TFT_BLACK);
            
            tft_drawBuffer();

            checkerboardShift = !checkerboardShift;

            i++;
            lastUpdateTime = currentTime;
        }
    }

    change_onChangeComplete();

    for (int i = 15; i >= 0;) {
        uint64_t currentTime = esp_timer_get_time();
        if (currentTime - lastUpdateTime > 100000) {
            uint8_t startYPos = 72 + (i * 6);

            tft_drawRectangle(18, startYPos, 204, 6, TFT_GREEN);
            draw_drawSprite(checkerboard, 18, startYPos, checkerboardPattern, 0, checkerboardShift);
            
            tft_drawBuffer();

            checkerboardShift = !checkerboardShift;

            i--;
            lastUpdateTime = currentTime;
        }
    }

    for (int i = 15; i >= 0;) {       
        uint64_t currentTime = esp_timer_get_time();
        if (currentTime - lastUpdateTime > 100000) {
            draw_drawBackground(bg, 90, 90, 3);
            draw_drawSprite(sprite, 72, 72, mainCharacterSprites, 7);

            uint8_t rectHeight = (6 * i);

            tft_drawRectangle(18, 72, 204, rectHeight, TFT_GREEN);
        
            for (int j = 0; j < i; j++) {
                uint8_t rectYPos = 72 + (6 * j);

                draw_drawSprite(checkerboard, 18, rectYPos, checkerboardPattern, 0, checkerboardShift);
                    
                checkerboardShift = !checkerboardShift;
            }

            tft_drawBuffer();
            i--;
            lastUpdateTime = currentTime;
        }
    }

    tone(SPK_PIN, 2100, 100);
    tone(SPK_PIN, 3500, 100);
    tone(SPK_PIN, 4100, 100);
    tone(SPK_PIN, 4650, 200);


    lastPressedButtonTime = esp_timer_get_time();

    menu_freeCheckerboard();

    vTaskResume(secondLoop);

    screenKey = MAIN_SCREEN;

    lastUpdateTime = 0; // Un pequeño empujoncito
}