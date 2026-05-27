#ifndef MENU_H
#define MENU_H

#include <TFT_eSPI.h>
#include "defs/defs.h"
#include "defs/sprite_data.h"

void menu_drawCurrentMenuOption(TFT_eSprite &bg, TFT_eSprite &icon, struct SpriteData* spriteData);
void menu_drawClock(TFT_eSprite &bg);
void menu_drawClockEdit(TFT_eSprite &bg);
void menu_drawTitle(TFT_eSprite &bg, TFT_eSprite &composite);
void menu_drawIdleScreen(TFT_eSprite &bg, TFT_eSprite &sprite, struct SpriteData* spriteData, struct SpriteData* bigUiElements, struct SpriteData* smallUiElements);
void menu_offScreen();
void menu_statusScreen(TFT_eSprite &bg, TFT_eSprite &sprite, struct SpriteData* spriteData);
void menu_statusScreen_drawStat(TFT_eSprite &sprite, struct SpriteData* spriteData, int x, int y, const char* text, uint8_t statValue);
void menu_timerFinishedScreen(TFT_eSprite &bg, TFT_eSprite &sprite, struct SpriteData* spriteData);
void menu_uiOverlay(TFT_eSprite &charSprite, struct SpriteData* uiElements);
void menu_foodScreen(TFT_eSprite &bg, TFT_eSprite &mainChara, struct SpriteData* spriteData);
void menu_foodScreen_drawEntry(
    TFT_eSprite &mainChara, struct SpriteData* spriteData, 
    uint8_t entryId, uint8_t spriteNumber, const char* textEntry
);
void menu_feedingScreen(
    TFT_eSprite &bg, TFT_eSprite &charaSprite, 
    struct SpriteData* uiSpriteData, struct SpriteData* charaSpriteData, uint8_t item
);
void menu_refuseScreen(TFT_eSprite &bg, TFT_eSprite &mainChara, struct SpriteData* spriteData);
void menu_sleepyScreen(TFT_eSprite &bg, TFT_eSprite &sprite, struct SpriteData* charaSprites, struct SpriteData* uiSprites);
void menu_careMistakeScreen(TFT_eSprite &bg, TFT_eSprite &sprite, struct SpriteData* spriteData, struct SpriteData* uiSpritesBig);
void menu_sleepingScreen(
    TFT_eSprite &bg, TFT_eSprite &sprite, 
    struct SpriteData* mainCharaData, struct SpriteData* bigUiElements, struct SpriteData* smallUIElements
);
void menu_poopScreen(
    TFT_eSprite &bg, TFT_eSprite &sprite, 
    struct SpriteData* spriteData, struct SpriteData* smallUiElements, struct SpriteData* bigUiElements
);
uint8_t menu_poopOverlay(TFT_eSprite &bg, TFT_eSprite &sprite, struct SpriteData* smallUiElements);
void menu_clearPoopScreen(TFT_eSprite &bg, TFT_eSprite &sprite, struct SpriteData* spriteData, struct SpriteData* bigUiElements, struct SpriteData* smallUiElements);
void menu_drawHappyScreen(
    TFT_eSprite &bg, TFT_eSprite &sprite,
    struct SpriteData* spriteData, struct SpriteData* smallUiElements, const int returnScreen = MAIN_SCREEN
);
void menu_lineSwitcher(TFT_eSprite &bg, TFT_eSprite &sprite, struct SpriteData* uiSmallSprite);
void menu_eggHatchScreen(TFT_eSprite &bg, TFT_eSprite &sprite, struct SpriteData* uiBigSprite, struct SpriteData* uiSmallSprite);
void menu_reloadEggs(uint8_t selectedEgg);
void menu_drawDeathScreen(TFT_eSprite &bg, TFT_eSprite &sprite, struct SpriteData* uiBigSprite, struct SpriteData* uiSmallSprite);
void menu_evolutionScreen(TFT_eSprite &bg, TFT_eSprite &sprite, struct SpriteData* mainCharacterSprites);
void menu_mainScreen();
void menu_changeCharaScreen(TFT_eSprite &bg, TFT_eSprite &sprite, struct SpriteData* mainSpriteData, struct SpriteData* uiSpriteData);
void menu_drawAngryScreen(
    TFT_eSprite &bg, TFT_eSprite &sprite,
    struct SpriteData* spriteData, struct SpriteData* smallUiElements
);
void menu_drawFridgeScreen(TFT_eSprite &bg, TFT_eSprite& sprite, struct SpriteData* smallUiElements, struct SpriteData* bigUiElements);

void training_screenTraining2(
    TFT_eSprite &bg, TFT_eSprite &sprite,
    struct SpriteData* mainCharaData, struct SpriteData* attackSprites
);

void menu_sleepScreen_sleepAction();
void menu_sleepScreen_recalculateSleep();
void menu_freezeScreen_alternateFreeze();

#endif