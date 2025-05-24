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
#include "vpet/vpet.h"
#include "vpet/steps/steps.h"

const char* TAG = "[MAIN]";

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite composite = TFT_eSprite(&tft);
TFT_eSprite sprite = TFT_eSprite(&tft);
TFT_eSprite bg = TFT_eSprite(&tft);

struct BackgroundData backgroundData;
struct SpriteData mainCharacterSprites;
struct SpriteData menuElementsData;
struct SpriteData uiElementsData;

struct CharacterData charaData;

int screenKey = TITLE_SCREEN;
int menuKey = STATUS_SCREEN_MENU;
int submenuKey = -1;

uint32_t dayUnixTime = 0;
uint16_t stepCounter = 0;

struct tm timeInfo;

MPU6050 mpu;
ESP32Time rtc(0);
TaskHandle_t secondLoop = NULL;

void loop2();
void secondCoreTask(void*);

void setup() {
    Serial.begin(115200);
    delay(100); // Give MPU6050 and ESP32 time to power up
  
    Wire.begin(MPU_SDA_PIN, MPU_SCL_PIN);  // I2C init before MPU6050
    mpu.initialize();

    tft_initDisplay(tft, TFT_BLACK);
    tft_initScreenBuffer(composite, TFT_BLACK);

    storage_init();
    
    storage_readFile("/sprite.bin", &mainCharacterSprites);
    storage_readFile("/menu.bin", &menuElementsData);
    storage_readFile("/ui.bin", &uiElementsData);

    storage_initBackground("/bg.bin", bg);

    debug_printFreeMemory();

    pinMode(K1_PIN, INPUT_PULLUP);
    pinMode(K2_PIN, INPUT_PULLUP);
    pinMode(K3_PIN, INPUT_PULLUP);
    pinMode(K4_PIN, INPUT_PULLUP);

    charaData.hunger = 4;
    charaData.strength = 4;
    charaData.effort = 4;

    charaData.evoLeftTimer = 60;
    charaData.hungerCareMistakeTimer = 60;
    charaData.strengthCareMistakeTimer = 60;

    xTaskCreatePinnedToCore(secondCoreTask, "VPET_EVAL", 4096, NULL, 0, &secondLoop, 0);
}


void loop() { 
    switch (screenKey) {
        case TITLE_SCREEN:
            menu_drawTitle(composite, bg);
            break;

        case CLOCK_EDIT_SCREEN:
            menu_drawClockEdit(composite, bg);
            break;

        case CLOCK_SCREEN:
            menu_drawClock(composite, bg, menuKey);
            break;

        case IDLE_SCREEN:
            menu_drawIdleScreen(composite, bg, sprite, &mainCharacterSprites, &menuElementsData, &uiElementsData);
            break;

        case MENU_SCREEN:
            menu_drawCurrentMenuOption(composite, bg, sprite, &menuElementsData);
            break;

        case STATUS_SCREEN: 
            menu_statusScreen(composite, bg, sprite, &uiElementsData, &charaData);
            break;

        case OFF_SCREEN:
            menu_offScreen(composite);
            break;

        case TIMER_FINISHED_SCREEN:
            menu_timerFinishedScreen(composite, bg, sprite, &mainCharacterSprites);
            break;

        case FOOD_SCREEN:
            menu_foodScreen(composite, bg, sprite, &uiElementsData);
            break;

        case FEEDING_SCREEN:
            menu_feedingScreen(composite, bg, sprite, &uiElementsData, &mainCharacterSprites, submenuKey);
            break;

        case REFUSING_SCREEN:
            menu_refuseScreen(composite, bg, sprite, &mainCharacterSprites);
            break;

        case SLEEPY_SCREEN:
            menu_sleepyScreen(composite, bg, sprite, &mainCharacterSprites, &menuElementsData);
            break;

        case SLEEP_SCREEN:
            menu_sleepingScreen(composite, bg, sprite, &mainCharacterSprites, &menuElementsData, &uiElementsData);
            break;

        case CARE_MISTAKE_SCREEN:
            menu_careMistakeScreen(composite, bg, sprite, &mainCharacterSprites, &menuElementsData);
            break;

        case POOPING_SCREEN:
            menu_poopScreen(composite, bg, sprite, &mainCharacterSprites, &uiElementsData, &menuElementsData);
            break;

        case CLEAR_POOP_SCREEN:
            menu_clearPoopScreen(composite, bg, sprite, &mainCharacterSprites, &menuElementsData, &uiElementsData);
            break;

        case HAPPY_SCREEN:
            menu_drawHappyScreen(composite, bg, sprite, &mainCharacterSprites, &uiElementsData);
            break;
    }
}

void loop2() {
    steps_countSteps();
    buttons_checkInactivity();
    vpet_runVpetTasks();
}

void secondCoreTask(void*) {
    for (;;) { loop2(); }
}