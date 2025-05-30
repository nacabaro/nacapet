#ifndef TRAINING_SCREENS_H
#define TRAINING_SCREENS_H

#include <TFT_eSPI.h>

void training_screenTraining1(
    TFT_eSprite &bg, TFT_eSprite &sprite,
    struct SpriteData* mainCharaData, struct SpriteData* attackSprites
);

#endif