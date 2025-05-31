#include "vpet/training/training.h"
#include "defs/defs.h"
#include "defs/chara_data.h"
#include "defs/sprite_data.h"
#include "buttons/buttons.h"
#include "display/display.h"
#include "draw/draw.h"
#include "loop/loop.h"

void training_screenTraining1(
    TFT_eSprite &bg, TFT_eSprite &sprite,
    struct SpriteData* mainCharaData, struct SpriteData* attackSprites
) {
    
    draw_drawBackground(bg, 90, 90, 3);
    draw_drawSpriteCentered(sprite, mainCharaData, 11, 6);
    
    tone(SPK_PIN, 4100, 100);
    tone(SPK_PIN, 3500, 100);

    tft_drawBuffer();

    delay(500);

    draw_drawBackground(bg, 90, 90, 3);
    draw_drawSpriteCentered(sprite, mainCharaData, 0, 6);
    tft_drawCenteredText("PUSH!!", 4, 190);

    tft_drawBuffer();

    uint8_t attackPower = 0;
    uint64_t currentTime = lastUpdateTime = esp_timer_get_time();
    int16_t lineYPos = 0;
    bool counted = false;
    
    while (currentTime - lastUpdateTime < 5000000) {
        currentTime = esp_timer_get_time();

        uint8_t buttonsRead = buttons_getPressedButtons();

        switch (buttonsRead) {
            case 8:
            attackPower++;
            if (attackPower % 4 == 0) {
                tft_drawRectangle(18, 168 - lineYPos, 18, 6, TFT_BLACK);
                tft_drawBuffer();
                lineYPos += 8;
            } else {
                delay(15);
            }
            break;
        
        case 2:
            screenKey = MENU_SCREEN;
            menuKey = TRAIN_SCREEN_MENU;
            return;
            break;

        default:
            break;
        }
    }

    uint8_t attackResult;
    if (attackPower > 20) {
        attackResult = ATTACK_PATTERN_EXCELLENT;
    } else if (attackPower <= 20 && attackPower > 16) {
        attackResult = ATTACK_PATTERN_GREAT;
    } else if (attackPower <= 16 && attackPower > 10) {
        attackResult = ATTACK_PATTERN_GOOD;
    } else if (attackPower <= 10 && attackPower > 6) {
        attackResult = ATTACK_PATTERN_BAD;
    } else {
        attackResult = ATTACK_PATTERN_MEDIOCRE;
    }

    loop_pauseLoop();

    training_displayTrainingResult(bg, sprite, mainCharaData, attackSprites, attackResult);
    
    loop_resumeLoop();
}