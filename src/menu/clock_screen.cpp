#include "menu.h"
#include "defs/defs.h"
#include "draw/draw.h"
#include "display/display.h"
#include "buttons/buttons.h"
#include "vpet/vpet.h"

void menu_drawClock(TFT_eSprite &composite, TFT_eSprite &bg, int menuOption) {
    uint8_t pressedButtons = buttons_getPressedButtons();
    switch (pressedButtons) {
        case 4:
        screenKey = IDLE_SCREEN;
        break;
        
        default:
        break;
    }
    
    char hourBuffer[6];
    
    draw_drawBackground(composite, bg, 90, 90, 3);
    
    snprintf(hourBuffer, 6, "%02d:%02d", timeInfo.tm_hour, timeInfo.tm_min);
    composite.setTextSize(4); 
    composite.drawString(hourBuffer, 40, 90);

    sniprintf(hourBuffer, 3, "%02d", timeInfo.tm_sec);
    composite.setTextSize(2);
    composite.drawString(hourBuffer, 170, 104);

    tft_drawBuffer(composite);

}

void menu_drawClockEdit(TFT_eSprite &composite, TFT_eSprite &bg) {
    char textBuffer[6];

    static int clockHourCount = 0;
    static int clockMinuteCount = 0;

    uint8_t pressedButtons = buttons_getPressedButtons();
    lastPressedButtonTime = esp_timer_get_time();

    switch (pressedButtons) {
        case 8:
            clockHourCount = (clockHourCount + 1) % 24;
            break;

        case 4:
            clockMinuteCount = (clockMinuteCount + 1) % 60;
            break;
        
        case 2:
            rtc.setTime(0, clockMinuteCount, clockHourCount, 1, 11, 2024);
            getLocalTime(&timeInfo, 50);
            dayUnixTime = mktime(&timeInfo) % 86400;

            screenKey = CLOCK_SCREEN;
            onActionTimerDelta();
            vpet_initTimer();

            break; 

        default:
            break;
    }
    
    draw_drawBackground(composite, bg, 90, 90, 3);

    snprintf(textBuffer, 6, "%02d:%02d", clockHourCount, clockMinuteCount);

    composite.setTextSize(4);
    composite.drawString(textBuffer, 40, 90);
    composite.drawString("SET", 40, 122);

    tft_drawBuffer(composite);
}
