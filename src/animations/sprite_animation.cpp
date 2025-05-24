#include "animations.h"
#include "draw/draw.h"
#include "display/display.h"
#include "defs/sprite_data.h"

const char* TAG_A = "[ANIMATION]";
const int spriteHeightOnScreen = 72;

int currentAnimationFrame = 0;

void animate_performAnimation(TFT_eSprite &buffer, TFT_eSprite &spr, struct SpriteData* spriteData, uint8_t offsetX) {
    draw_drawSprite(
        buffer, 
        spr,
        animationPositions[currentAnimationFrame] - offsetX, 
        spriteHeightOnScreen, 
        spriteData, 
        animationFrames[currentAnimationFrame], 
        6,
        animationFlipSprites[currentAnimationFrame]
    );

    currentAnimationFrame = (currentAnimationFrame + 1) % numFrames;
}

void animate_performAttentionAnimation(TFT_eSprite &buffer, TFT_eSprite &spr, struct SpriteData* spriteData) {
    currentAnimationFrame = (currentAnimationFrame + 1) % numFramesAngry;
    draw_drawSprite(
        buffer, 
        spr,
        spriteHeightOnScreen, 
        spriteHeightOnScreen, 
        spriteData, 
        angryAnimationFrames[currentAnimationFrame], 
        6,
        animationFlipSprites[currentAnimationFrame]
    );

}

void animate_performEatingAnimation(TFT_eSprite &buffer, TFT_eSprite &spr, struct SpriteData* spriteData) {
    currentAnimationFrame = (currentAnimationFrame + 1) % numFramesEating;
    draw_drawSprite(
        buffer,
        spr,
        spriteHeightOnScreen,
        spriteHeightOnScreen,
        spriteData,
        eatingAnimationFrames[currentAnimationFrame],
        6,
        false
    );
}

void animate_performRefuseAnimation(TFT_eSprite &buffer, TFT_eSprite &spr, struct SpriteData* spriteData) {
    currentAnimationFrame = (currentAnimationFrame + 1) % numFramesRefuse;
    draw_drawSprite(
        buffer,
        spr,
        spriteHeightOnScreen,
        spriteHeightOnScreen,
        spriteData,
        refuseAnimationFrames[currentAnimationFrame],
        6,
        refuseAnimationFlips[currentAnimationFrame]
    );
}

void animate_performSleepyAnimation(TFT_eSprite &buffer, TFT_eSprite &spr, struct SpriteData* spriteData) {
    currentAnimationFrame = (currentAnimationFrame + 1) % numFramesSleepy;
    draw_drawSprite(
        buffer,
        spr,
        spriteHeightOnScreen,
        spriteHeightOnScreen,
        spriteData,
        sleepyAnimationFrames[currentAnimationFrame],
        6,
        false
    );
}

void animate_performHappyAnimation(TFT_eSprite &buffer, TFT_eSprite &spr, struct SpriteData* spriteData) {
    currentAnimationFrame = (currentAnimationFrame + 1) % numFramesHappy;
    draw_drawSprite(
        buffer,
        spr,
        spriteHeightOnScreen,
        spriteHeightOnScreen,
        spriteData,
        happyAnimationFrames[currentAnimationFrame],
        6,
        false
    );
}