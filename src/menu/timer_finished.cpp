#include "menu.h"

int interruptKey = -1;

void menu_timerFinishedScreen(TFT_eSprite &bg, TFT_eSprite &sprite, struct SpriteData* spriteData) {
    lastPressedButtonTime = esp_timer_get_time();
    digitalWrite(BL_PIN, HIGH);
    
    screenOff = false;
    inactive = false;
    
    beepCounter = 0;
    
    screenKey = interruptKey;
}