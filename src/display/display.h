#ifndef DISPLAY_H
#define DISPLAY_H

#include <TFT_eSPI.h>

const int SCREEN_WIDTH = 240;   
const int SCREEN_HEIGHT = 240;

void tft_initDisplay(TFT_eSPI &tft, uint16_t color = TFT_WHITE);
void tft_initScreenBuffer(uint16_t color = TFT_WHITE);
void tft_drawBuffer();
void tft_clearBuffer(TFT_eSprite &buffer, uint16_t color = TFT_WHITE);
void tft_clearBuffer(uint16_t color = TFT_WHITE);
void tft_drawCenteredText(const char* text, int factor, int y);
void tft_drawText(const char* text, uint8_t size, uint8_t x, uint8_t y, uint16_t color = TFT_BLACK);
void tft_drawRectangle(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color = TFT_BLACK);

#endif