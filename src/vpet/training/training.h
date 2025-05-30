#ifndef TRAINING_H
#define TRAINING_H

#include <TFT_eSPI.h>

void training_displayTrainingResult(
    TFT_eSprite &bg, TFT_eSprite &sprite,
    struct SpriteData* mainCharaData, struct SpriteData* attackSprites, uint8_t trainingResult
);
void training_trainingAttackSounds();


#endif