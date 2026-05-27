#include "energy.h"
#include <Arduino.h>
#include <esp_sleep.h>
#include <driver/gpio.h>
#include "defs/defs.h"
#include "defs/chara_data.h"
#include "defs/sprite_data.h"
#include "display/display.h"
#include "driver/rtc_io.h"
#include "defs/screen_defs.h"
#include "storage/storage.h"
#include "vpet/vpet/vpet.h"

#define SLEEP_TIME_US 60000000ull  // 60 seconds periodic wakeup

void energy_startLightSleep() {
    printf("[ENERGY] Entering light sleep...\n");
    
    // Keep RTC peripherals powered on during sleep so internal pullups/pulldowns stay active
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON);

    // EXT1 wakeup uses the RTC IO subsystem. We must explicitly configure the RTC pull resistors
    // so the pins do not float and cause immediate wake-ups.
    // NOTE: We must initialize the pins as RTC GPIOs using rtc_gpio_init() first!
    #if BUTTON_MODE == INPUT_PULLUP
        rtc_gpio_init((gpio_num_t)K1_PIN); rtc_gpio_pullup_en((gpio_num_t)K1_PIN); rtc_gpio_pulldown_dis((gpio_num_t)K1_PIN);
        rtc_gpio_init((gpio_num_t)K2_PIN); rtc_gpio_pullup_en((gpio_num_t)K2_PIN); rtc_gpio_pulldown_dis((gpio_num_t)K2_PIN);
        rtc_gpio_init((gpio_num_t)K3_PIN); rtc_gpio_pullup_en((gpio_num_t)K3_PIN); rtc_gpio_pulldown_dis((gpio_num_t)K3_PIN);
        rtc_gpio_init((gpio_num_t)K4_PIN); rtc_gpio_pullup_en((gpio_num_t)K4_PIN); rtc_gpio_pulldown_dis((gpio_num_t)K4_PIN);
    #else
        rtc_gpio_init((gpio_num_t)K1_PIN); rtc_gpio_pulldown_en((gpio_num_t)K1_PIN); rtc_gpio_pullup_dis((gpio_num_t)K1_PIN);
        rtc_gpio_init((gpio_num_t)K2_PIN); rtc_gpio_pulldown_en((gpio_num_t)K2_PIN); rtc_gpio_pullup_dis((gpio_num_t)K2_PIN);
        rtc_gpio_init((gpio_num_t)K3_PIN); rtc_gpio_pulldown_en((gpio_num_t)K3_PIN); rtc_gpio_pullup_dis((gpio_num_t)K3_PIN);
        rtc_gpio_init((gpio_num_t)K4_PIN); rtc_gpio_pulldown_en((gpio_num_t)K4_PIN); rtc_gpio_pullup_dis((gpio_num_t)K4_PIN);
    #endif

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

    digitalWrite(48, LOW);

    esp_light_sleep_start();

    digitalWrite(48, HIGH);

    // Release the pins from RTC control after waking up so digitalRead works normally
    rtc_gpio_deinit((gpio_num_t)K1_PIN);
    rtc_gpio_deinit((gpio_num_t)K2_PIN);
    rtc_gpio_deinit((gpio_num_t)K3_PIN);
    rtc_gpio_deinit((gpio_num_t)K4_PIN);

    esp_sleep_wakeup_cause_t cause = esp_sleep_get_wakeup_cause();
    printf("[ENERGY] Woken up. Cause: %d\n", cause);

    runVpetTasks = true;
    vpet_runVpetTasks();

    if (cause == ESP_SLEEP_WAKEUP_TIMER) {
        bool hasAlert = (
            charaData[currentCharacter].hunger == 0 ||
            charaData[currentCharacter].strength == 0 ||
            charaData[currentCharacter].careMistakeCallLight ||
            screenKey == TIMER_FINISHED_SCREEN
        );

        if (hasAlert) {
            printf("[ENERGY] Periodic wakeup triggered, maybe care mistake.\n");
            digitalWrite(BL_PIN, HIGH);
            screenOff = false;
            inactive = false;
            if (screenKey != TIMER_FINISHED_SCREEN) {
                screenKey = TIMER_FINISHED_SCREEN;
            }
        } else {
            printf("[ENERGY] Periodic wake up triggered, no alerts. Sampling steps...\n");
            isSamplingSteps = true;
            sampleStartTime = esp_timer_get_time();
            initialSteps = stepCounter;
            storage_saveState();
        }
    } else if (cause == ESP_SLEEP_WAKEUP_EXT1) {
        printf("[ENERGY] User button press detected.\n");
        digitalWrite(BL_PIN, HIGH);
        screenOff = false;
        inactive = false;
        screenKey = MAIN_SCREEN;
    } else {
        // Fallback for other wake-up causes to prevent high-speed loop thrashing
        delay(200);
    }
}