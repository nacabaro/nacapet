#include "draw.h"
#include "defs/screen_defs.h"

const char* TAG_DB = "[DRAW BG]";


void draw_drawBackground(TFT_eSprite &bg, int spr_w, int spr_h, int factor) {
    // spr_w, spr_h = dimensions of the source sprite “bg”
    // factor       = integer scale factor

    for (int srcY = 0; srcY < spr_h; srcY++) {
        // Compute the top row in the scaled image that corresponds to srcY
        int destRowBase  = srcY * factor;

        // For each “vertical tile” row within [0 .. factor-1]:
        for (int dy = 0; dy < factor; dy++) {
            int outY1 = destRowBase + dy;     // Y in composite1
            int outY2 = outY1 - 120;          // Y in composite2

            // Now loop over each source column
            for (int srcX = 0; srcX < spr_w; srcX++) {
                // Fetch the color exactly once for this source pixel
                uint16_t color = bg.readPixel(srcX, srcY);

                // Compute the leftmost column in the scaled image that corresponds to srcX
                int destColBase = srcX * factor;

                // For each “horizontal tile” column within [0 .. factor-1]:
                for (int dx = 0; dx < factor; dx++) {
                    int outX = destColBase + dx;
                    composite1.drawPixel(outX, outY1, color);
                    composite2.drawPixel(outX, outY2, color);
                }
            }
        }
    }
}

// USO FUTURO
void draw_drawBackgroundSection(TFT_eSprite &bg, int spr_w, int spr_h, int tft_x, int tft_y, int tft_w, int tft_h, int factor) {
    int srcX_start = tft_x / factor;
    int srcX_end = (tft_x + tft_w - 1) / factor;
    int srcY_start = tft_y / factor;
    int srcY_end = (tft_y + tft_h - 1) / factor;

    if (srcX_start < 0) srcX_start = 0;
    if (srcY_start < 0) srcY_start = 0;
    if (srcX_end >= spr_w) srcX_end = spr_w - 1;
    if (srcY_end >= spr_h) srcY_end = spr_h - 1;

    if (srcX_start > srcX_end || srcY_start > srcY_end) return;

    for (int srcY = srcY_start; srcY <= srcY_end; srcY++) {
        int destY_base = srcY * factor;
        int blockY0 = destY_base;
        int blockY1 = destY_base + (factor - 1);
        
        int y0 = (blockY0 < tft_y) ? tft_y : blockY0;
        int y1 = (blockY1 > (tft_y + tft_h - 1)) ? (tft_y + tft_h - 1) : blockY1;
        
        if (y0 > y1) continue;

        for (int srcX = srcX_start; srcX <= srcX_end; srcX++) {
            int destX_base = srcX * factor;
            int blockX0 = destX_base;
            int blockX1 = destX_base + (factor - 1);

            int x0 = (blockX0 < tft_x) ? tft_x : blockX0;
            int x1 = (blockX1 > (tft_x + tft_w - 1)) ? (tft_x + tft_w - 1) : blockX1;

            if (x0 > x1) continue;

            uint16_t color = bg.readPixel(srcX, srcY);

            for (int y = y0; y <= y1; y++) {
                for (int x = x0; x <= x1; x++) {
                    composite1.drawPixel(x, y, color);
                    composite2.drawPixel(x, y - 120, color);
                }
            }
        }
    }
}