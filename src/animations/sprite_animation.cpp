#include "animations.h"
#include "draw/draw.h"
#include "display/display.h"
#include "defs/sprite_data.h"

const char* TAG_A = "[ANIMATION]";
const int spriteHeightOnScreen = 72;

int currentAnimationFrame = 0;

void animate_performAnimation(TFT_eSprite &spr, struct SpriteData* spriteData, uint8_t offsetX) {
    draw_drawSprite(
        spr,
        animationPositions[currentAnimationFrame] - offsetX,
        spriteHeightOnScreen,
        spriteData,
        animationFrames[currentAnimationFrame],
        animationFlipSprites[currentAnimationFrame]
    );

    currentAnimationFrame = (currentAnimationFrame + 1) % numFrames;
}

void animate_performAttentionAnimation(TFT_eSprite &spr, struct SpriteData* spriteData) {
    currentAnimationFrame = (currentAnimationFrame + 1) % numFramesAngry;
    draw_drawSprite(
        spr,
        spriteHeightOnScreen,
        spriteHeightOnScreen,
        spriteData,
        angryAnimationFrames[currentAnimationFrame],
        animationFlipSprites[currentAnimationFrame]
    );
}

void animate_performEatingAnimation(TFT_eSprite &spr, struct SpriteData* spriteData) {
    currentAnimationFrame = (currentAnimationFrame + 1) % numFramesEating;
    draw_drawSprite(
        spr,
        spriteHeightOnScreen,
        spriteHeightOnScreen,
        spriteData,
        eatingAnimationFrames[currentAnimationFrame],
        false
    );
}

void animate_performRefuseAnimation(TFT_eSprite &spr, struct SpriteData* spriteData) {
    currentAnimationFrame = (currentAnimationFrame + 1) % numFramesRefuse;
    draw_drawSprite(
        spr,
        spriteHeightOnScreen,
        spriteHeightOnScreen,
        spriteData,
        refuseAnimationFrames[currentAnimationFrame],
        refuseAnimationFlips[currentAnimationFrame]
    );
}

void animate_performSleepyAnimation(TFT_eSprite &spr, struct SpriteData* spriteData) {
    currentAnimationFrame = (currentAnimationFrame + 1) % numFramesSleepy;
    draw_drawSprite(
        spr,
        spriteHeightOnScreen,
        spriteHeightOnScreen,
        spriteData,
        sleepyAnimationFrames[currentAnimationFrame],
        false
    );
}

void animate_performHappyAnimation(TFT_eSprite &spr, struct SpriteData* spriteData) {
    currentAnimationFrame = (currentAnimationFrame + 1) % numFramesHappy;
    draw_drawSprite(
        spr,
        spriteHeightOnScreen,
        spriteHeightOnScreen,
        spriteData,
        happyAnimationFrames[currentAnimationFrame],
        false
    );
}


void animate_performAngryAnimation(TFT_eSprite &spr, struct SpriteData* spriteData) {
    currentAnimationFrame = (currentAnimationFrame + 1) % numFramesAngry;
    draw_drawSprite(
        spr,
        spriteHeightOnScreen,
        spriteHeightOnScreen,
        spriteData,
        angryAnimationFrames[currentAnimationFrame],
        false
    );
}

void animate_performHatchingAnimation(TFT_eSprite &spr, struct SpriteData* spriteData) {
    currentAnimationFrame = (currentAnimationFrame + 1) % numFramesHatching;
    draw_drawSprite(
        spr,
        hatchingAnimationPositions[currentAnimationFrame],
        spriteHeightOnScreen,
        spriteData,
        0,
        false
    );
}
