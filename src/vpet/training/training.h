#ifndef TRAINING_H
#define TRAINING_H

#include <TFT_eSPI.h>

void training_screenTraining1(
    TFT_eSprite &composite, TFT_eSprite &bg, TFT_eSprite &sprite,
    struct SpriteData* mainCharaData, struct SpriteData* attackSprites
);

#endif