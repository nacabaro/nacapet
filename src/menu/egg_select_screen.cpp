#include "menu.h"
#include "draw/draw.h"
#include "defs/defs.h"
#include "vpet/vpet.h"
#include "display/display.h"
#include "defs/sprite_data.h"
#include "buttons/buttons.h"
#include "vpet/lines/lines.h"
#include "defs/chara_data.h"


void menu_lineSwitcher(TFT_eSprite &composite, TFT_eSprite &bg, TFT_eSprite &sprite, struct SpriteData* uiSmallSprite) {
    static uint8_t eggCounter = 0;

    uint8_t buttonsPressed = buttons_getPressedButtons();
    switch(buttonsPressed) {
        case K1_PRESSED:
            eggCounter = (eggCounter + 1) % eggNumber;
            break;
        
        case K2_PRESSED:
            menu_reloadEggs(eggCounter);
            eggCounter = 0;
            screenKey = EGG_HATCH_SCREEN;
            return;
            break;

        case K3_PRESSED:
            screenKey = IDLE_SCREEN;
            return;
            break;

        default:
            break;
    }

    draw_drawBackground(composite, bg, 90, 90, 3);
    tft_clearBuffer(sprite, TFT_TRANSPARENT);
    draw_drawSpriteCentered(composite, sprite, &eggSelection[eggCounter].eggSprite, 0, 6);
    tft_clearBuffer(sprite, TFT_TRANSPARENT);
    draw_drawSprite(composite, sprite, 194, 96, uiSmallSprite, ARROW_ICON, 6);

    tft_drawBuffer(composite);
}

void menu_reloadEggs(uint8_t selectedEgg) {
    char fileName[strlen(eggSelection[selectedEgg].fileName) + 1];
    strcpy(fileName, eggSelection[selectedEgg].fileName);

    lines_freeEggList();
    
    printf("[DEBUG] fileName=%s\n", fileName);

    lines_getSingleLine(fileName);
    lines_getLineCareMistakes(fileName);

    charaData.hatching = true;
}