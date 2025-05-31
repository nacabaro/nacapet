#include "menu.h"
#include "buttons/buttons.h"
#include "draw/draw.h"
#include "loop/loop.h"
#include "defs/chara_data.h"
#include "storage/storage.h"
#include "display/display.h"
#include "defs/defs.h"


void menu_changeCharaScreen(TFT_eSprite &bg, TFT_eSprite &sprite, struct SpriteData* mainSpriteData, struct SpriteData* uiSpriteData) {
    loop_pauseLoop();

    uint8_t selectedChara = currentCharacter;
    CharacterData* selectedCharaData = &charaData[selectedChara];

    bool updateScreen = true;
    
    for (;;) {
        uint8_t pressedButtons = buttons_getPressedButtons();
        switch (pressedButtons) {
            case K1_PRESSED:
                selectedChara++;
                if (selectedChara >= CHARA_COUNT_IN_DEVICE) {
                    selectedChara = 0;
                }
                selectedCharaData = &charaData[selectedChara];
                updateScreen = true;
                break;
            
            case K2_PRESSED:
                currentCharacter = selectedChara;
                loop_resumeLoop();

                screenKey = MAIN_SCREEN;
                menuKey = STATUS_SCREEN;
                return;
                break;

            case K3_PRESSED:
                char fileName[20];
                sprintf(fileName, "/chara/%02x.bin", charaData[currentCharacter].idChara);

                storage_readFile(fileName, mainSpriteData);       
                loop_resumeLoop();

                screenKey = MAIN_SCREEN;
                menuKey = STATUS_SCREEN;
                return;
                break;

            default:
                break;
        }

        delay(15);

        if (updateScreen) {
            draw_drawBackground(bg, 90, 90, 3);
            
            if (selectedCharaData->hatched) {
                char fileName[20];
                sprintf(fileName, "/chara/%02x.bin", selectedCharaData->idChara);
                
                storage_readFile(fileName, mainSpriteData);
                draw_drawSprite(sprite, 18, 72, mainSpriteData, 0, 6);
            } else {
                tft_drawCenteredText("EMPTY", 4, 120);
            }

            draw_drawSprite(sprite, 174, 96, uiSpriteData, ARROW_ICON, 6);
            
            tft_drawBuffer();

            updateScreen = false;
        }

        uint64_t currentTime = esp_timer_get_time();
        if (currentTime - lastPressedButtonTime > INACTIVITY_THRESHOLD_TIME_US) {
            char fileName[20];
            sprintf(fileName, "/chara/%02x.bin", charaData[currentCharacter].idChara);

            storage_readFile(fileName, mainSpriteData);       
            loop_resumeLoop();

            screenKey = MAIN_SCREEN;
            menuKey = STATUS_SCREEN;
        }
    }
}