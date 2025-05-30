#include <Arduino.h>
#include "display/display.h"
#include "memory/memory.h"
#include "storage/storage.h"
#include "animations/animations.h"
#include "debug/debug.h"
#include "defs/defs.h"
#include "defs/chara_data.h"
#include "menu/menu.h"
#include "buttons/buttons.h"
#include "vpet/vpet/vpet.h"
#include "vpet/steps/steps.h"
#include "vpet/lines/lines.h"
#include "energy/energy.h"
#include "driver/rtc_io.h"


const char* TAG = "[MAIN]";

// TFT_eSPI stuff, important
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite composite1 = TFT_eSprite(&tft);
TFT_eSprite composite2 = TFT_eSprite(&tft);
TFT_eSprite sprite = TFT_eSprite(&tft);
TFT_eSprite bg = TFT_eSprite(&tft);

// External devices stuff
MPU6050 mpu;
ESP32Time rtc(0);

// Sprite data and background data
struct BackgroundData backgroundData;
struct SpriteData mainCharacterSprites;
struct SpriteData menuElementsData;
struct SpriteData uiElementsData;

// Active character data
// TODO: Split into CHARA_COUNT_IN_DEVICE times
struct CharacterData charaData;
uint8_t currentCharacter = 0;

// Boot flag, tells if the device clock has been initialized
bool coldBoot = true;

// Screen keys, this tells which screen is being shown the screens state machine
int screenKey = TITLE_SCREEN;
int menuKey = STATUS_SCREEN_MENU;
int submenuKey = -1;

// Step counter, counts steps, duh
// TODO: Reset with each day, maybe have a log of steps
uint16_t stepCounter = 0;

// Time stuff, timeInfo is the time structure used by everything, updated in loop2
// dayUnixTime is used to tell the day time, in UNIX seconds
struct tm timeInfo;
uint32_t dayUnixTime = 0;

// Egg stuff, initializes it to zero, used by the algorithm to tell if something has
// been malloc'd into memory,
Egg_t* eggSelection = NULL;
uint8_t eggNumber = 0;

// Tasks
TaskHandle_t secondLoop = NULL;
bool pauseLoop = false;

void loop2();
void secondCoreTask(void*);

void setup() {
    Serial.begin(115200);
    delay(100); // Give MPU6050 and ESP32 time to power up

    Wire.begin(MPU_SDA_PIN, MPU_SCL_PIN);  // I2C init before MPU6050
    mpu.initialize();

    tft_initDisplay(tft, TFT_BLACK);
    tft_initScreenBuffer(TFT_BLACK);

    storage_init();
    
    storage_readFile("/menu.bin", &menuElementsData);
    storage_readFile("/ui.bin", &uiElementsData);

    storage_initBackground("/bg.bin", bg);

    pinMode(K1_PIN, INPUT_PULLDOWN);
    pinMode(K2_PIN, INPUT_PULLDOWN);
    pinMode(K3_PIN, INPUT_PULLDOWN);
    pinMode(K4_PIN, INPUT_PULLDOWN);

    xTaskCreatePinnedToCore(secondCoreTask, "VPET_EVAL", 4096, NULL, 0, &secondLoop, 0);
    
    lines_initLineStorage();

    vpet_initTimer();

    energy_setUpLightSleep();
}


void loop() { 
    switch (screenKey) {
        case TITLE_SCREEN:
            menu_drawTitle(bg);
            break;

        case CLOCK_EDIT_SCREEN:
            menu_drawClockEdit(bg);
            break;

        case CLOCK_SCREEN:
            menu_drawClock(bg);
            break;

        case IDLE_SCREEN:
            menu_drawIdleScreen(bg, sprite, &mainCharacterSprites, &menuElementsData, &uiElementsData);
            break;

        case MENU_SCREEN:
            menu_drawCurrentMenuOption(bg, sprite, &menuElementsData);
            break;

        case STATUS_SCREEN: 
            menu_statusScreen(bg, sprite, &uiElementsData, &charaData);
            break;

        case OFF_SCREEN:
            menu_offScreen();
            break;

        case TIMER_FINISHED_SCREEN:
            menu_timerFinishedScreen(bg, sprite, &mainCharacterSprites);
            break;

        case FOOD_SCREEN:
            menu_foodScreen(bg, sprite, &uiElementsData);
            break;

        case FEEDING_SCREEN:
            menu_feedingScreen(bg, sprite, &uiElementsData, &mainCharacterSprites, submenuKey);
            break;

        case REFUSING_SCREEN:
            menu_refuseScreen(bg, sprite, &mainCharacterSprites);
            break;

        case SLEEPY_SCREEN:
            menu_sleepyScreen(bg, sprite, &mainCharacterSprites, &menuElementsData);
            break;

        case SLEEP_SCREEN:
            menu_sleepingScreen(bg, sprite, &mainCharacterSprites, &menuElementsData, &uiElementsData);
            break;

        case CARE_MISTAKE_SCREEN:
            menu_careMistakeScreen(bg, sprite, &mainCharacterSprites, &menuElementsData);
            break;

        case POOPING_SCREEN:
            menu_poopScreen(bg, sprite, &mainCharacterSprites, &uiElementsData, &menuElementsData);
            break;

        case CLEAR_POOP_SCREEN:
            menu_clearPoopScreen(bg, sprite, &mainCharacterSprites, &menuElementsData, &uiElementsData);
            break;

        case HAPPY_SCREEN:
            menu_drawHappyScreen(bg, sprite, &mainCharacterSprites, &uiElementsData);
            break;

        case EGG_HATCH_SCREEN:
            menu_eggHatchScreen(bg, sprite, &menuElementsData, &uiElementsData);
            break;
        
        case EGG_SELECT_SCREEN:
            menu_lineSwitcher(bg, sprite, &uiElementsData);
            break;

        case EGG_EMPTY_SCREEN:
            menu_drawDeathScreen( bg, sprite, &menuElementsData, &uiElementsData);
            break;       

        case EVOLUTION_SCREEN:
            menu_evolutionScreen(bg, sprite, &mainCharacterSprites);
            break;
    }

    if (screenKey == IDLE_SCREEN || screenKey == OFF_SCREEN) {
        steps_countSteps();
    }
}

void loop2() {
    if (!pauseLoop) {
        buttons_checkInactivity();
        vpet_runVpetTasks();
        
        getLocalTime(&timeInfo, 50);
        dayUnixTime = mktime(&timeInfo) % SECONDS_IN_DAY;
    
        if (screenOff) { energy_startLightSleep(); }
    } else {
        lastPressedButtonTime = esp_timer_get_time();
        buttons_getPressedButtons();
    }

}

void secondCoreTask(void*) {
    for (;;) {
        loop2(); 
    }
}