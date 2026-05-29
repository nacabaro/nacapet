#include "training.h"
#include "draw/draw.h"
#include "defs/defs.h"
#include "defs/chara_data.h"
#include "defs/sounds.h"
#include "defs/sprite_data.h"
#include "display/display.h"

#define NUM_ROUNDS 5

uint8_t patternExcellent[NUM_ROUNDS] = {1, 1, 1, 1, 1};
uint8_t patternGreat[NUM_ROUNDS] = {1, 1, 0, 1, 0};
uint8_t patternGood[NUM_ROUNDS] = {0, 1, 0, 1, 1};
uint8_t patternBad[NUM_ROUNDS] = {1, 0, 1, 0, 0};
uint8_t patternMediocre[NUM_ROUNDS] = {0, 0, 0, 0, 0};

void training_displayTrainingResult(
    TFT_eSprite &bg, TFT_eSprite &sprite,
    struct SpriteData* mainCharaData, struct SpriteData* attackSprites, uint8_t trainingResult
) {
    charaData[currentCharacter].effort++;
    charaData[currentCharacter].strengthCareMistakeTimer = charaData[currentCharacter].initialChangeTimer;

    uint8_t* pattern = NULL;
    switch (trainingResult) {
        case ATTACK_PATTERN_EXCELLENT:
            pattern = patternExcellent;
            screenKey = HAPPY_SCREEN;

            charaData[currentCharacter].strength += 2;
            charaData[currentCharacter].weight -= 4;
            if (charaData[currentCharacter].weight < charaData[currentCharacter].minWeight) {
                charaData[currentCharacter].weight -= charaData[currentCharacter].minWeight;
            }

            break;

        case ATTACK_PATTERN_GREAT:
            pattern = patternGreat;
            screenKey = HAPPY_SCREEN;

            charaData[currentCharacter].strength += 2;
            charaData[currentCharacter].weight -= 2;
            if (charaData[currentCharacter].weight < charaData[currentCharacter].minWeight) {
                charaData[currentCharacter].weight = charaData[currentCharacter].minWeight;
            }

            break;

        case ATTACK_PATTERN_GOOD:
            pattern = patternGood;
            screenKey = HAPPY_SCREEN;

            charaData[currentCharacter].strength += 1;
            charaData[currentCharacter].weight -= 2;
            if (charaData[currentCharacter].weight < charaData[currentCharacter].minWeight) {
                charaData[currentCharacter].weight = charaData[currentCharacter].minWeight;
            }
            
            break;

        case ATTACK_PATTERN_BAD:
            pattern = patternBad;
            screenKey = ANGRY_SCREEN;

            charaData[currentCharacter].weight--;
            if (charaData[currentCharacter].weight < charaData[currentCharacter].minWeight) {
                charaData[currentCharacter].weight = charaData[currentCharacter].minWeight;
            }

            break;

        case ATTACK_PATTERN_MEDIOCRE:
        default:
            pattern = patternMediocre;
            screenKey = ANGRY_SCREEN;
            break;
    }

    draw_drawBackground(bg, 90, 90, 3);

    for (int i = 0; i < NUM_ROUNDS; i++) {
        training_trainingAttackSounds();
        
        for (int j = 78; j >= -48; j -= 6) {
            draw_drawSprite(sprite, 126, 72, mainCharaData, 11);

            draw_drawAttacks(bg, sprite, attackSprites, j, 72, pattern[i], charaData[currentCharacter].spriteAttackId);
            
            draw_drawBackgroundSection(bg, 0, 72, 18, 96);
            
            tft_drawBuffer();
        }

        delay(20);
    }


    lastPressedButtonTime = esp_timer_get_time();
}

// nOT FANCY
void training_trainingAttackSounds() {
    sound_playMelody(SOUND_TRAINING_ATTACK, SOUND_NOTE_COUNT(SOUND_TRAINING_ATTACK));
}
