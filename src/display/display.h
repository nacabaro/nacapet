#ifndef DISPLAY_H
#define DISPLAY_H

#include <TFT_eSPI.h>

const int SCREEN_WIDTH = 240;   
const int SCREEN_HEIGHT = 240;

void tft_initDisplay(TFT_eSPI &tft, uint16_t color = TFT_WHITE);
void tft_initScreenBuffer(TFT_eSprite &spr, uint16_t color = TFT_WHITE);
void tft_drawBuffer(TFT_eSprite &buffer);
void tft_clearBuffer(TFT_eSprite &buffer, uint16_t color = TFT_WHITE);
void tft_drawCenteredText(TFT_eSprite &buffer, const char* text, int factor, int y);

#endif