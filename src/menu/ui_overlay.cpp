#include "menu.h"
#include "draw/draw.h"
#include "display/display.h"
#include "defs/chara_data.h"
#include "defs/screen_defs.h"

void menu_uiOverlay(TFT_eSprite &charSprite, struct SpriteData* uiElements) {
    struct tm timeInfo;
    char hourBuffer[6];

    getLocalTime(&timeInfo, RTC_TIMEOUT_THRESHOLD_TIME_MS);
    snprintf(hourBuffer, 6, "%02d:%02d", timeInfo.tm_hour, timeInfo.tm_min);     

    composite1.fillRect(0, 0, 240, 24, TFT_BLACK);
    tft_drawText(hourBuffer, 2, 4, 4, TFT_WHITE);
    
    snprintf(hourBuffer, 6, "%05d", stepCounter);
    tft_drawText(hourBuffer, 2, 176, 4, TFT_WHITE);
    
    if (charaData[currentCharacter].careMistakeCallLight) {
        tft_clearBuffer(charSprite, TFT_TRANSPARENT);
        draw_drawSprite(charSprite, 192, 192, uiElements, CARE_MISTAKE_CALL_LIGHT, 2);
    }
}