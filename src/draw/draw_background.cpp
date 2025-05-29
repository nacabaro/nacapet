#include "draw.h"
#include "defs/screen_defs.h"

const char* TAG_DB = "[DRAW BG]";


void draw_drawBackground(TFT_eSprite &bg, int spr_w, int spr_h, int factor) {
    int scaledWidth = spr_w * factor;
    int scaledHeight = spr_h * factor;
    
    for (int sy = 0; sy < scaledHeight; sy++) {
        for (int sx = 0; sx < scaledWidth; sx++) {
            int srcX = sx / factor;
            int srcY = sy / factor;

            uint16_t color = bg.readPixel(srcX, srcY);

            composite1.drawPixel(sx, sy, color);
            composite2.drawPixel(sx, sy - 120, color);
        }
    }
}