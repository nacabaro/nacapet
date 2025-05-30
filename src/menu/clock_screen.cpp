#include "menu.h"
#include "defs/defs.h"
#include "draw/draw.h"
#include "display/display.h"
#include "buttons/buttons.h"
#include "defs/screen_defs.h"

void menu_drawClock(TFT_eSprite &bg) {
    uint8_t pressedButtons = buttons_getPressedButtons();
    switch (pressedButtons) {
        case K2_PRESSED:
            screenKey = IDLE_SCREEN;
            break;
        
        default:
            break;
    }
    
    char hourBuffer[6];
    
    draw_drawBackground(bg, 90, 90, 3);
    
    snprintf(hourBuffer, 6, "%02d:%02d", timeInfo.tm_hour, timeInfo.tm_min);
    tft_drawText(hourBuffer, 4, 40, 90);

    sniprintf(hourBuffer, 3, "%02d", timeInfo.tm_sec);
    tft_drawText(hourBuffer, 2, 170, 104);

    tft_drawBuffer();
}

void menu_drawClockEdit(TFT_eSprite &bg) {
    char textBuffer[6];

    static int clockHourCount = 0;
    static int clockMinuteCount = 0;

    uint8_t pressedButtons = buttons_getPressedButtons();

    switch (pressedButtons) {
        case K1_PRESSED:
            clockHourCount = (clockHourCount + 1) % 24;
            break;

        case K2_PRESSED:
            clockMinuteCount = (clockMinuteCount + 1) % 60;
            break;
        
        case K3_PRESSED:
            // Es un dia random, nada significativo, ya pondre mas adelante que tenga dia del año
            rtc.setTime(0, clockMinuteCount, clockHourCount, 1, 11, 2024);
            getLocalTime(&timeInfo, 50);
            dayUnixTime = mktime(&timeInfo) % SECONDS_IN_DAY;

            coldBoot = false;

            screenKey = CLOCK_SCREEN;
            break; 

        default:
            break;
    }
    
    draw_drawBackground(bg, 90, 90, 3);

    snprintf(textBuffer, 6, "%02d:%02d", clockHourCount, clockMinuteCount);

    tft_drawText(textBuffer, 4, 40, 90);
    tft_drawText("SET", 4, 40, 122);

    tft_drawBuffer();
}
