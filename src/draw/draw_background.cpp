#include "draw.h"
#include "defs/sprite_data.h"
#include "defs/screen_defs.h"

const char* TAG_DB = "[DRAW BG]";


void draw_drawBackground(TFT_eSprite& bg, int spr_w, int spr_h, int factor) {
    bg.pushToSprite(&composite, 0, 0, TFT_TRANSPARENT);
}

void draw_drawBackgroundSection(TFT_eSprite& bg, int x, int y, int w, int h) {
    uint16_t* bgPtr = (uint16_t*)bg.getPointer();
    int bgStride = bg.width(); 

    for (int row = 0; row < h; row++) {
        uint16_t* rowSrc = bgPtr + ((y + row) * bgStride) + x;
        composite.pushImage(x, y + row, w, 1, rowSrc);
    }
}
