#include "display.h"

void tft_initDisplay(TFT_eSPI &tft, uint16_t color) {
    tft.init();
    tft.setRotation(0);
    tft.fillScreen(TFT_RED);
}

void tft_initScreenBuffer(TFT_eSprite &buffer, uint16_t color) {
    buffer.createSprite(SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
    buffer.fillSprite(color);
    buffer.setTextColor(TFT_BLACK);
    buffer.setTextSize(4);
    buffer.pushSprite(0, 0);
}

void tft_drawBuffer(TFT_eSprite &buffer) {
    buffer.pushSprite(0, 0);
}

void tft_clearBuffer(TFT_eSprite &buffer, uint16_t color) {
    buffer.fillSprite(color);
}

void tft_drawCenteredText(TFT_eSprite &buffer, const char* text, int factor, int y) {
    size_t textWidth = strlen(text) * factor * 6;
    int x = (SCREEN_WIDTH - textWidth) / 2;
    buffer.setTextSize(factor);
    buffer.drawString(text, x, y);
}