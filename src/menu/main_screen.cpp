#include "menu.h"
#include "defs/defs.h"
#include "defs/chara_data.h"

void menu_mainScreen() {
    printf("[MAINSCR] on main screen\n");

    if (coldBoot) {  
        screenKey = TITLE_SCREEN;
        return;
    } else if (!charaData[currentCharacter].hatched && !charaData[currentCharacter].hatching) {
        screenKey = EGG_EMPTY_SCREEN;
        return;
    } else if (!charaData[currentCharacter].hatched && charaData[currentCharacter].hatching) {
        screenKey = EGG_HATCH_SCREEN;
        return;
    } else if (charaData[currentCharacter].sleepy && !charaData[currentCharacter].asleep) {
        screenKey = SLEEPY_SCREEN;
        return;
    } else if ((charaData[currentCharacter].sleepy && charaData[currentCharacter].asleep) || charaData[currentCharacter].asleep) {
        screenKey = SLEEP_SCREEN;
        return;
    } else {
        screenKey = IDLE_SCREEN;
        return;
    }
}