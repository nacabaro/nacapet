#include "energy.h"
#include <Arduino.h>
#include "defs/defs.h"
#include "display/display.h"
#include "driver/rtc_io.h"
#include "defs/screen_defs.h"

#define SLEEP_TIME_US 15000000ull

#define BUTTON_PIN_BITMASK(GPIO) (1ULL << GPIO)  // Macro for individual GPIO bitmask

void energy_setUpLightSleep() {
    // Enable EXT1 wake-up source
    esp_err_t result = esp_sleep_enable_ext0_wakeup(GPIO_NUM_32, 0);

    if (result == ESP_OK) {
        Serial.println("EXT1 Wake-Up set successfully.");
    } else {
        Serial.println("Failed to set EXT1 Wake-Up as wake-up source.");
    }

    esp_sleep_enable_timer_wakeup(SLEEP_TIME_US);
}

void energy_startLightSleep() {
    if (skipSleep) { 
        skipSleep = false;
        return; 
    }

    esp_light_sleep_start();
    
    // 6) Figure out which woke you
    auto cause = esp_sleep_get_wakeup_cause();
    if (cause == ESP_SLEEP_WAKEUP_EXT0) {
      Serial.println("Woke by button on GPIO");
    } else if (cause == ESP_SLEEP_WAKEUP_TIMER) {
      Serial.println("Woke by timer");
    } else {
      Serial.printf("Other wakeup: %d\n", cause);
    }

    runVpetTasks = true;

    byte pinValue = digitalRead(26);
    Serial.println(pinValue);
}
  