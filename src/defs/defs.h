#ifndef DEFS_H
#define DEFS_H

#include <ESP32Time.h>
#include <MPU6050.h>

#include "defs/background_data.h"
#include "defs/sprite_data.h"
#include "defs/file_chara.h"

#define VERSION "Alpha v0.1"

// SCREEN PINOUT
#define SCL_PIN 34
#define SDA_PIN 35
#define RST_PIN 32
#define BL_PIN 25

// BUTTONS PINOUT
#define K1_PIN 27
#define K2_PIN 26
#define K3_PIN 33
#define K4_PIN 32

// SPEAKER PINOUT
#define SPK_PIN 21

// MPU6050 PINOUT
#define MPU_SCL_PIN 4
#define MPU_SDA_PIN 16
#define MPU_INT_PIN 12

// SPECIAL SCREEN THAT OPENS WHEN TIMERS ARE DONE
// RECEIVES AN EXTRA PARAMETER (INTERRUPTKEY)
#define TIMER_FINISHED_SCREEN 999

// ANIMATION FRAMERATE DEFINITION IN µS
#define ANIMATION_THRESHOLD_TIME_US 500000
#define ANIMATION_SLEEPY_THRESHOLD_TIME_US 2500000

// INACTIVITY TIMERS (LAST TIME A BUTTON WAS PRESSED)
#define LAST_PRESSED_BUTTON_THRESHOLD_TIME_US 10000000
#define INACTIVITY_THRESHOLD_TIME_US 30000000

// RTC TIMEOUT WHEN TIME NOT DEFINED
#define RTC_TIMEOUT_THRESHOLD_TIME_MS 100

// STANDARD BEEP WHEN PRESSING BUTTON
#define BEEP_FREQ_HZ 4100
#define BEEP_LEN_MS 35

// MENU ENTRIES
#define STATUS_SCREEN_MENU 0
#define FOOD_SCREEN_MENU 1
#define TRAIN_SCREEN_MENU 2
#define BATTLE_SCREEN_MENU 3
#define POOP_SCREEN_MENU 4
#define MEDICAL_SCREEN_MENU 5
#define SLEEP_SCREEN_MENU 6
#define SETTINGS_SCREEN_MENU 7

// SCREENS THAT OPEN AFTER CLICKING ON A MENU ENTRY
#define STATUS_SCREEN 10
#define FOOD_SCREEN 11
#define TRAIN_SCREEN 12
#define BATTLE_SCREEN 13
#define CLEAR_POOP_SCREEN 14
#define MEDICAL_SCREEN 15
#define SLEEP_SCREEN 16
#define SETTINGS_SCREEN 17

// ICONS FOR EACH MENU ENTRY (MENU.BIN)
#define STATUS_SCREEN_ICON 0
#define FOOD_SCREEN_ICON 1
#define TRAIN_SCREEN_ICON 2
#define BATTLE_SCREEN_ICON 3
#define CLEAR_POOP_ICON 4
#define MEDICAL_SCREEN_ICON 5
#define SLEEP_SCREEN_ICON 6
#define SETTINGS_SCREEN_ICON 7
#define CARE_MISTAKE_CALL_LIGHT 8
#define BED_SPRITE 9
#define EMPTY_EGG 10

// SCREENS
#define OFF_SCREEN -1
#define TITLE_SCREEN 0
#define CLOCK_EDIT_SCREEN 1
#define CLOCK_SCREEN 2
#define IDLE_SCREEN 3
#define MENU_SCREEN 4
#define EGG_SELECT_SCREEN 5
#define EGG_HATCH_SCREEN 6
#define EGG_EMPTY_SCREEN 7
#define FEEDING_SCREEN 20
#define REFUSING_SCREEN 21
#define SLEEPY_SCREEN 22
#define CARE_MISTAKE_SCREEN 23
#define POOPING_SCREEN 24
#define HAPPY_SCREEN 25

// SMALL UI ICONS (UI.BIN)
#define POOP_ICON 0
#define FOOD_ICON 1
#define PILL_ICON 2
#define ZZZ_ICON 3
#define AGE_ICON 4
#define SCALE_ICON 5
#define ARROW_ICON 6
#define FIREWORKS_ICON 7
#define COMPLAIN_ICON 8
#define FULL_HEART_ICON 9
#define EMPTY_HEART_ICON 10
#define CLEANER_ICON 11

#define SECONDS_IN_DAY 86400

// STANDARD VPET PARAMETER (CARE MISTAKES)
#define CARE_MISTAKE_COUNTER_MAX 60
#define SLEEP_CARE_MISTAKE_COUNTER_MAX 60
#define SLEEP_COUNTER_MAX 120

// ATTACK PATTERNS
#define ATTACK_PATTERN_MEDIOCRE 0
#define ATTACK_PATTERN_BAD 1
#define ATTACK_PATTERN_GOOD 2
#define ATTACK_PATTERN_GREAT 3
#define ATTACK_PATTERN_EXCELLENT 4

// BUTTON PRESSES DEFINITIONS
#define K1_PRESSED 8
#define K2_PRESSED 4
#define K3_PRESSED 2
#define K4_PRESSED 1
#define NONE_PRESSED 0

#define SCALE_FACTOR 3

#define CHARA_COUNT_IN_DEVICE 5

extern int screenKey;
extern int menuKey;
extern int submenuKey;
extern int interruptKey;

extern bool k1_prev;
extern bool k2_prev;
extern bool k3_prev;
extern bool k4_prev;

extern bool inactive;
extern bool screenOff;
extern bool skipSleep;

extern uint64_t lastPressedButtonTime;
extern uint64_t lastUpdateTime;
extern uint64_t lastBeepTime;

extern struct CharacterData charaData;
extern struct tm timeInfo;
extern uint32_t dayUnixTime; 

extern ESP32Time rtc;
extern MPU6050 mpu;

extern hw_timer_t *actionTimerDelta;

extern bool runVpetTasks;

extern uint8_t beepCounter;

extern uint16_t stepCounter;

extern bool coldBoot;

extern uint8_t eggNumber;
extern Egg_t* eggSelection;

extern uint8_t currentCharacter;

extern Egg_t* currentEgg;
extern LineCare_t** currentLineCareInstr;
extern Line_t** currentLine;

extern struct SpriteData mainCharacterSprites;

#endif