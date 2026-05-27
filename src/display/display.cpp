#include "display.h"
#include "defs/screen_defs.h"
#include "defs/defs.h"

int xPos = 0;
int yPos = 0;

static const int BUF_W = 240;
static const int BUF_H = 120;


void tft_initDisplay(TFT_eSPI &tft, uint16_t color) {
    #ifdef ANDREW_UNIT
        pinMode(BL_PIN, OUTPUT);
        digitalWrite(BL_PIN, HIGH);
    #endif

    tft.init(); 
    tft.setRotation(0);
    tft.fillScreen(color);
}

void tft_initScreenBuffer(uint16_t color) {
    composite.setAttribute(PSRAM_ENABLE, true);

    
    // 2. Try to create the sprite
    if (composite.createSprite(240, 240)) {
        printf("SUCCESS: Composite sprite created.\n");
        composite.fillSprite(TFT_RED); // If this works, screen should turn RED
    } else {
        printf("FATAL: Composite sprite failed! No RAM/PSRAM.\n");
        return; // Stop here so we don't draw "lines"
    }

    composite.setFreeFont(NULL); // Reset to default GLCD font
    composite.setTextFont(1);    // Use the standard small font (scaled by size 4)
    composite.setTextColor(TFT_BLUE);
    composite.setTextSize(4);
    composite.pushSprite(0, 0);
}

void tft_drawBuffer() {
    composite.pushSprite(0, 0, TFT_TRANSPARENT);
}

void tft_clearBuffer(TFT_eSprite &composite, uint16_t color) {
    composite.fillSprite(color);
}

void tft_clearBuffer(uint16_t color) {
    composite.fillSprite(color);
}

void tft_drawCenteredText(const char* text, int size, int yGlobal) {
    int textW = strlen(text) * size * 6;
    int x = (SCREEN_WIDTH - textW) / 2;

    composite.setTextSize(size);
    composite.setTextColor(TFT_BLACK);
    composite.drawString(text, x, yGlobal);
}

void tft_drawText(const char* text, int size, int x, int y, uint16_t color) {
    composite.setTextSize(size);
    composite.setTextColor(color);
    composite.drawString(text, x, y);
}

void tft_drawRectangle(int x, int y, int w, int h, uint16_t color) {
    composite.fillRect(x, y, w, h, color);
}