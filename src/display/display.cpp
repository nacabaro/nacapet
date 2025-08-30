#include "display.h"
#include "defs/screen_defs.h"
#include "defs/defs.h"

int xPos = 0;
int yPos = 0;

static const int BUF_W = 240;
static const int BUF_H = 120;


void tft_initDisplay(TFT_eSPI &tft, uint16_t color) {
    tft.begin();
    tft.initDMA();
    tft.setRotation(0);
    tft.fillScreen(TFT_RED);
}

void tft_initScreenBuffer(uint16_t color) {
    bool status = composite1.createSprite(240, 120);
    composite1.fillSprite(TFT_BLUE);
    composite1.setTextColor(TFT_BLACK);
    composite1.setTextSize(4);
    composite1.pushSprite(0, 0);

    status = composite2.createSprite(240, 120);
    composite2.fillSprite(TFT_BLUE);
    composite2.setTextColor(TFT_BLACK);
    composite2.setTextSize(4);
    composite2.pushSprite(0, 120);
}

void tft_drawBuffer() {
    tft.startWrite();
    composite1.pushSprite(0, 0);
    composite2.pushSprite(0, 0 + BUF_H);
    tft.endWrite();
}

/*void tft_drawBuffer() {
    tft.startWrite();
    tft.pushImageDMA(0, 0, 240, 120, (uint16_t*) composite1.getPointer());
    tft.pushImageDMA(0, 120, 240, 120, (uint16_t*) composite2.getPointer());
    tft.endWrite();
}*/

void tft_clearBuffer(TFT_eSprite &composite, uint16_t color) {
    composite.fillSprite(color);
}

void tft_clearBuffer(uint16_t color) {
    composite1.fillSprite(color);
    composite2.fillSprite(color);
}

void tft_drawCenteredText(const char* text, int size, int yGlobal) {
    int textW = strlen(text) * size * 6;
    int x = (SCREEN_WIDTH - textW) / 2;

    composite1.setTextSize(size);
    composite1.setTextColor(TFT_BLACK);
    composite1.drawString(text, x, yGlobal);

    // only bottom half, adjust local Y
    int yLocal = yGlobal - BUF_H;
    composite2.setTextSize(size);
    composite2.setTextColor(TFT_BLACK);
    composite2.drawString(text, x, yLocal);
}

void tft_drawText(const char* text, int size, int x, int y, uint16_t color) {
    composite1.setTextSize(size);
    composite1.setTextColor(color);
    composite1.drawString(text, x, y);

    composite2.setTextSize(size);
    composite2.setTextColor(color);
    composite2.drawString(text, x, y - BUF_H);
}

void tft_drawRectangle(int x, int y, int w, int h, uint16_t color) {
    composite1.fillRect(x, y, w, h, color);
    composite2.fillRect(x, y - BUF_H, w, h, color);
}