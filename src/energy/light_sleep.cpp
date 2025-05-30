#include "energy.h"
#include <Arduino.h>
#include "defs/defs.h"
#include "display/display.h"
#include "driver/rtc_io.h"
#include "defs/screen_defs.h"

#define SLEEP_TIME_US 15000000ull

#define BUTTON_PIN_BITMASK(GPIO) (1ULL << GPIO)  // Macro for individual GPIO bitmask

void energy_setUpLightSleep() {
    // Plena confianza en manolo
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_32, 0);
    esp_sleep_enable_timer_wakeup(SLEEP_TIME_US);
}

void energy_startLightSleep() {
    esp_light_sleep_start();
    
    // Who woke you up???? TELL me!!
    auto cause = esp_sleep_get_wakeup_cause();

    runVpetTasks = true;
}
  