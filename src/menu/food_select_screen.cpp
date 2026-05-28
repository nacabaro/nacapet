#include "buttons/buttons.h"
#include "defs/chara_data.h"
#include "display/display.h"
#include "draw/draw.h"
#include "menu.h"
#include "vpet/vpet/vpet.h"

void menu_foodScreen(TFT_eSprite &bg, TFT_eSprite &mainChara,
                     struct SpriteData *spriteData) {
  if (charaData[currentCharacter].sleepy) {
    tone(SPK_PIN, BEEP_FREQ_HZ, BEEP_LEN_MS);
    delay(100);
    tone(SPK_PIN, BEEP_FREQ_HZ, BEEP_LEN_MS);
    screenKey = MENU_SCREEN;

    return;
  }

  static uint8_t arrowPosition = 0;

  uint8_t pressedButtons = buttons_getPressedButtons();
  switch (pressedButtons) {
    case K1_PRESSED:
      arrowPosition = (arrowPosition + 1) % 2;
      break;

    case K3_PRESSED:
      screenKey = MENU_SCREEN;
      break;

    default:
      break;
  }

  if (pressedButtons == K2_PRESSED) {
    lastUpdateTime = 0;
    switch (arrowPosition) {
    case 0:
      if (charaData[currentCharacter].hunger < 8) {
        charaData[currentCharacter].hungerCareMistakeTimer =
            charaData[currentCharacter].initialStatsReductionTime;
        charaData[currentCharacter].hungerCareMistakeObtained = false;
        charaData[currentCharacter].weight++;
        charaData[currentCharacter].hunger++;
        screenKey = FEEDING_SCREEN;
        submenuKey = FOOD_ICON;
      } else {
        screenKey = REFUSING_SCREEN;
        if (!charaData[currentCharacter].overfeedHappened) {
          charaData[currentCharacter].overfeed++;
          charaData[currentCharacter].overfeedHappened = true;
        }
      }
      return;
      break;

    case 1:
      if (charaData[currentCharacter].strength < 8) {
        charaData[currentCharacter].strengthCareMistakeTimer =
            charaData[currentCharacter].initialStatsReductionTime;
        charaData[currentCharacter].strength++;
        charaData[currentCharacter].weight += 2;
        screenKey = FEEDING_SCREEN;
        submenuKey = PILL_ICON;
      } else {
        screenKey = REFUSING_SCREEN;
      }
      return;
      break;

    default:
      break;
    }

    vpet_computeCallLight();
  }

  draw_drawBackground(bg, 90, 90, 3);
  menu_foodScreen_drawEntry(mainChara, spriteData, 0, FOOD_ICON, "Meat");
  menu_foodScreen_drawEntry(mainChara, spriteData, 1, PILL_ICON, "Pill");

  draw_drawSprite(mainChara, 5, (arrowPosition * 34) + 5, spriteData,
                  ARROW_ICON);

  tft_drawBuffer();
}

void menu_foodScreen_drawEntry(TFT_eSprite &mainChara,
                               struct SpriteData *spriteData, uint8_t entryId,
                               uint8_t spriteNumber, const char *textEntry) {
  tft_clearBuffer(mainChara, TFT_TRANSPARENT);
  draw_drawSprite(mainChara, 45, (entryId * 34) + 5, spriteData, spriteNumber);
  tft_drawText(textEntry, 4, 80, (entryId * 34) + 5);
}