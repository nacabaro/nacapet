#include "menu.h"

int interruptKey = -1;

void menu_timerFinishedScreen(TFT_eSprite &bg, TFT_eSprite &sprite, struct SpriteData* spriteData) {
    screenOff = false;
    inactive = false;
    
    lastPressedButtonTime = esp_timer_get_time();
    digitalWrite(BL_PIN, HIGH);
    
    beepCounter = 0;
    
    screenKey = interruptKey;
}