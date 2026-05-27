#include "menu.h"
#include "vpet/training/training.h"
#include "defs/defs.h"
#include "defs/chara_data.h"
#include "draw/draw.h"
#include "display/display.h"
#include "buttons/buttons.h"

void training_screenTraining2(
    TFT_eSprite &bg, TFT_eSprite &sprite,
    struct SpriteData* mainCharaData, struct SpriteData* attackSprites
) {
    vTaskSuspend(secondLoop);

    draw_drawBackground(bg, 90, 90 ,3);
    
    // Player
    draw_drawSprite(sprite, 174, 72, mainCharaData, 0, false);
    draw_drawBackgroundSection(bg, 222, 72, 18, 96);

    // Opponent
    draw_drawSprite(sprite, -30, 72, mainCharaData, 0, true);
    draw_drawBackgroundSection(bg, 0, 72, 18, 96);

    tft_drawBuffer();

    uint64_t inactivityTimer = esp_timer_get_time();
    while (true) {
        uint64_t currentTime = esp_timer_get_time();
        if (currentTime - lastUpdateTime > 500000) {
            tone(SPK_PIN, BEEP_FREQ_HZ, BEEP_LEN_MS);
            lastUpdateTime = currentTime;
        }

        if (currentTime - inactivityTimer > 3000000) {
            screenKey = MAIN_SCREEN;
            submenuKey = STATUS_SCREEN_MENU;

            vTaskResume(secondLoop);
            
            return;
        } 

        uint8_t randomPosition = rand() % 2;
        uint8_t projectilePosition = 0;

        uint8_t pressedButtons = buttons_getPressedButtons();
        if (pressedButtons == K1_PRESSED || pressedButtons == K2_PRESSED) {
            projectilePosition =  pressedButtons >> 3; // pressedButtons >> 4 == 1 => K1; != 1 => K2
            printf("[TRAIN] projectilePosition=%i\n", projectilePosition);

        } else if (pressedButtons == K3_PRESSED) {
            screenKey = MAIN_SCREEN;
            submenuKey = STATUS_SCREEN_MENU;

            vTaskResume(secondLoop);
            
            return;
        }

        if (pressedButtons != 0) {
            inactivityTimer = esp_timer_get_time();

            // Player attack
            draw_drawSprite(sprite, 126, 72 + 48 * projectilePosition, attackSprites, POOP_ICON);
    
            // Opponent attack
            draw_drawSprite(sprite, 66, 72 + 48 * randomPosition, attackSprites, FOOD_ICON);

            tft_drawBuffer();

            if (projectilePosition != randomPosition) {
                delay(500);

                draw_drawBackgroundSection(bg, 126, 72 + 48 * projectilePosition, 48, 48);
                draw_drawSprite(sprite, 66, 72 + 48 * projectilePosition, attackSprites, POOP_ICON);

                tft_drawBuffer();

                delay(500);
            } else {
                delay(500);
            }

            draw_drawBackgroundSection(bg, 66, 72, 108, 96);

            if (projectilePosition != randomPosition) {
                charaData[currentCharacter].strength++;
                menu_drawHappyScreen(bg, sprite, mainCharaData, attackSprites);
            } else {
                printf("[TRAIN] Train failed\n");
                menu_drawAngryScreen(bg, sprite, mainCharaData, attackSprites);
            }
        }
    }
}