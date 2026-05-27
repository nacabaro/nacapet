#include "draw.h"
#include "defs/sprite_data.h"
#include "defs/screen_defs.h"

const char* TAG_DB = "[DRAW BG]";


void draw_drawBackground(TFT_eSprite& bg, int spr_w, int spr_h, int factor) {
    bg.pushToSprite(&composite, 0, 0, TFT_TRANSPARENT);
}

void draw_drawBackgroundSection(TFT_eSprite& bg, int x, int y, int w, int h) {
    // Get the raw 16-bit pixel buffer from the background sprite
    uint16_t* bgPtr = (uint16_t*)bg.getPointer();
    int bgStride = bg.width(); // Full row width — needed to advance between rows

    // pushImage assumes a contiguous (packed) source buffer, so feeding the
    // whole rectangle at once would read wrong pixels after the first row.
    // Instead, copy one row at a time, each time jumping by bgStride pixels.
    for (int row = 0; row < h; row++) {
        uint16_t* rowSrc = bgPtr + ((y + row) * bgStride) + x;
        composite.pushImage(x, y + row, w, 1, rowSrc);
    }
}
