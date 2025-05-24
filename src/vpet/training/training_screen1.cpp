#include "training.h"
#include "defs/defs.h"
#include "defs/chara_data.h"
#include "defs/sprite_data.h"
#include "buttons/buttons.h"

void training_screenTraining1(
    TFT_eSprite &composite, TFT_eSprite &bg, TFT_eSprite &sprite,
    struct SpriteData* mainCharaData, struct SpriteData* attackSprites
) {
    uint8_t attackPower = 0;

    while (true) {
        uint8_t buttonsRead = buttons_getPressedButtons();
        switch (buttonsRead)
        {
        case 8:
            attackPower++;
            break;
        
        case 2:
            screenKey = IDLE_SCREEN;
            return;
            break;

        default:
            break;
        }

        uint64_t currentTime = esp_timer_get_time();
        if (currentTime - lastUpdateTime > 10000000) {
            // Display attack animation...
            // NOT YET SLOW DOWN
        }
    }
}