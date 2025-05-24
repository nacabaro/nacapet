#include "menu.h"
#include "draw/draw.h"
#include "display/display.h"
#include "defs/chara_data.h"

void menu_uiOverlay(TFT_eSprite &composite, TFT_eSprite &charSprite, struct SpriteData* uiElements) {
    struct tm timeInfo;
    char hourBuffer[6];

    composite.setTextSize(2);
    composite.setTextColor(TFT_WHITE);
    composite.fillRect(0, 0, 240, 24, TFT_BLACK);
    
    getLocalTime(&timeInfo, RTC_TIMEOUT_THRESHOLD_TIME_MS);
    snprintf(hourBuffer, 6, "%02d:%02d", timeInfo.tm_hour, timeInfo.tm_min); 

    composite.drawString(hourBuffer, 4, 4);
    
    snprintf(hourBuffer, 6, "%05d", stepCounter);

    composite.drawString(hourBuffer, 176, 4);
    
    composite.setTextColor(TFT_BLACK);
    if (charaData.careMistakeCallLight) {
        tft_clearBuffer(charSprite, TFT_TRANSPARENT);
        draw_drawSprite(composite, charSprite, 192, 192, uiElements, CARE_MISTAKE_CALL_LIGHT, 2);
    }
}