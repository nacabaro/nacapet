#include "menu.h"
#include "draw/draw.h"
#include "defs/defs.h"
#include "defs/sprite_data.h"
#include "defs/chara_data.h"
#include "display/display.h"

uint8_t menu_poopOverlay(TFT_eSprite &composite, TFT_eSprite &sprite, struct SpriteData* smallUiElements) {
    static bool poopFlip = false;
    const uint8_t poopStartY = 120;
    uint8_t poopStartX = 174;
    bool poopTop = false;
    
    for (int i = 0; i < charaData.poopNumber; i++) {
        draw_drawSprite(composite, sprite, poopStartX, poopStartY - (48 * poopTop), smallUiElements, POOP_ICON, 6, poopFlip);
        poopStartX -= (i % 2) * 48;
        poopTop = !poopTop;
    }

    poopFlip = !poopFlip;

    return 222 - (poopStartX + ((charaData.poopNumber % 2 == 0) * 48));
}