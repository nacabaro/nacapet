#include "defs/defs.h"
#include "defs/chara_data.h"
#include "defs/screen_defs.h"
#include "buttons.h"
#include <Arduino.h>

uint64_t lastPressedButtonTime = esp_timer_get_time();

bool inactive = false;
bool screenOff = false;

bool k1_prev = HIGH;
bool k2_prev = HIGH;
bool k3_prev = HIGH;
bool k4_prev = HIGH;

void buttons_checkInactivity() {
    uint64_t currentTime = esp_timer_get_time();
    if (currentTime - lastPressedButtonTime > INACTIVITY_THRESHOLD_TIME_US && !screenOff && !alwaysOnEnabled) {
        #ifdef ALLOW_SLEEP
        digitalWrite(BL_PIN, LOW);
        screenKey = OFF_SCREEN;
        #endif
        
    } else if (currentTime - lastPressedButtonTime > LAST_PRESSED_BUTTON_THRESHOLD_TIME_US && !inactive) {        
        screenKey =  MAIN_SCREEN;
        inactive = true;
    }
}

uint8_t buttons_getPressedButtons() {
    bool k1_current = digitalRead(K1_PIN);
    bool k2_current = digitalRead(K2_PIN);
    bool k3_current = digitalRead(K3_PIN);
    bool k4_current = digitalRead(K4_PIN);

    uint8_t retV = (
        (k1_prev == LOW && k1_current == HIGH) << 3 | 
        (k2_prev == LOW && k2_current == HIGH) << 2 |
        (k3_prev == LOW && k3_current == HIGH) << 1 | 
        (k4_prev == LOW && k4_current == HIGH)    
    );

    if (retV != 0) {
        tone(SPK_PIN, BEEP_FREQ_HZ, BEEP_LEN_MS);
        lastPressedButtonTime = esp_timer_get_time();
        inactive = false;
        screenOff = false;
    }

    k1_prev = k1_current;
    k2_prev = k2_current;
    k3_prev = k3_current;
    k4_prev = k4_current;

    return retV;
}
