#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include <TFT_eSPI.h>

const int numFrames = 39;
const int numFramesAngry = 2;
const int numFramesEating = 2;
const int numFramesHappy = 2;
const int numFramesRefuse = 4;
const int numFramesSleepy = 2;

// TODO: Cambiar a una animación mas atractiva
const int animationFrames[numFrames] = {
    0, 1, 0, 1, 0, 1, 0, 1, 0, 7, 3, 7, 
    0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 7, 3, 7,
    1, 0, 1, 0, 1, 0, 1
};
// Asumiendo que el buffer es 240x240, dejando margen y los sprites son 96x96 (6 de factor)
const int animationPositions[] = {
    72, 66, 60, 54, 48, 42, 36, 30, 24, 18, 24, 18, 
    24, 30, 36, 42, 48, 54, 60, 66, 72, 78, 84, 90, 96, 102, 108, 114, 120, 126, 120, 126, 
    114, 108, 102, 96, 90, 84, 78
};

const bool animationFlipSprites[numFrames] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
    0, 0, 0, 0, 0, 0, 0
};

const int angryAnimationFrames[] = {
    8, 9
};

const int eatingAnimationFrames[] = {
    3, 7
};

const int refuseAnimationFrames[] = {
    6, 6, 6, 6
};

const int refuseAnimationFlips[] = {
    0, 1, 0, 1
};

const int sleepyAnimationFrames[] = {
    4, 5
};

const int happyAnimationFrames[] = {
    3, 7
};

void animate_performAnimation(TFT_eSprite &buffer, TFT_eSprite &spr, struct SpriteData* spriteData, uint8_t offsetX);
void animate_performAttentionAnimation(TFT_eSprite &buffer, TFT_eSprite &spr, struct SpriteData* spriteData);
void animate_performEatingAnimation(TFT_eSprite &buffer, TFT_eSprite &spr, struct SpriteData* spriteData);
void animate_performRefuseAnimation(TFT_eSprite &buffer, TFT_eSprite &spr, struct SpriteData* spriteData);
void animate_performSleepyAnimation(TFT_eSprite &buffer, TFT_eSprite &spr, struct SpriteData* spriteData);
void animate_performHappyAnimation(TFT_eSprite &buffer, TFT_eSprite &spr, struct SpriteData* spriteData);

#endif