#include "energy.h"
#include <Arduino.h>
#include "defs/defs.h"
#include "defs/chara_data.h"
#include "defs/sprite_data.h"
#include "display/display.h"
#include "driver/rtc_io.h"
#include "defs/screen_defs.h"
#include "storage/storage.h"
#include "vpet/vpet/vpet.h"

#define SLEEP_TIME_US 60000000ull  // 60 seconds periodic wakeup

void energy_setUpLightSleep() {
    // Left as a placeholder for backward compatibility
}

void energy_startLightSleep() {
    printf("[ENERGY] Entering light sleep...\n");
    
    // 1. Configure wake-up sources
    esp_sleep_enable_timer_wakeup(SLEEP_TIME_US);

    uint64_t mask = (1ULL << K1_PIN) | (1ULL << K2_PIN) | (1ULL << K3_PIN) | (1ULL << K4_PIN);
    #if BUTTON_MODE == INPUT_PULLUP
        #if defined(ESP_EXT1_WAKEUP_ANY_LOW)
            esp_sleep_enable_ext1_wakeup(mask, ESP_EXT1_WAKEUP_ANY_LOW);
        #else
            esp_sleep_enable_ext1_wakeup(mask, ESP_EXT1_WAKEUP_ALL_LOW);
        #endif
    #else
        esp_sleep_enable_ext1_wakeup(mask, ESP_EXT1_WAKEUP_ANY_HIGH);
    #endif

    // 2. Start light sleep (powers down CPU core and clocks, keeps RAM and state)
    esp_light_sleep_start();

    // 3. Woken up! Let's check why
    esp_sleep_wakeup_cause_t cause = esp_sleep_get_wakeup_cause();
    printf("[ENERGY] Woken up. Cause: %d\n", cause);

    // 4. Force immediate simulation update
    runVpetTasks = true;
    vpet_runVpetTasks();

    // 5. Evaluate the current character's state to see if any alert/event was triggered
    if (cause == ESP_SLEEP_WAKEUP_TIMER) {
        bool hasAlert = (
            charaData[currentCharacter].hunger == 0 ||
            charaData[currentCharacter].strength == 0 ||
            charaData[currentCharacter].careMistakeCallLight ||
            screenKey == TIMER_FINISHED_SCREEN
        );

        if (hasAlert) {
            // Wake up display and backlight to notify the user
            printf("[ENERGY] Periodic wakeup triggered a critical alert! Waking up completely.\n");
            digitalWrite(BL_PIN, HIGH);
            screenOff = false;
            inactive = false;
            if (screenKey != TIMER_FINISHED_SCREEN) {
                screenKey = TIMER_FINISHED_SCREEN;
            }
        } else {
            // Background checkpoint save, then let the loop put the CPU back to sleep
            printf("[ENERGY] Periodic simulation update successful. No alerts. Saving state...\n");
            storage_saveState();
        }
    } else if (cause == ESP_SLEEP_WAKEUP_EXT1) {
        // Woken up by a user button press
        printf("[ENERGY] User button press detected. Restoring UI...\n");
        digitalWrite(BL_PIN, HIGH);
        screenOff = false;
        inactive = false;
        screenKey = MAIN_SCREEN;
    }
}