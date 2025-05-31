#include "lines.h"
#include "defs/defs.h"
#include "defs/chara_data.h"
#include "storage/storage.h"
#include "vpet/vpet/vpet.h"

void lines_onHatchComplete() {
    // Cuando se nace, lo que se va a hacer es poner el sprite 0, es decir,
    // La primera entrada de la linea es el sprite que sale.
    lines_freeCurrentEgg();

    char spriteFileName[30];
    snprintf(spriteFileName, 30, "/chara/%02x.bin", currentLine[currentCharacter]->characters[0].id);

    storage_readFile(spriteFileName, &mainCharacterSprites);

    // Primero los datos del bicho actual en nuestra estructura de datos de confianza
    charaData[currentCharacter].hp = currentLine[currentCharacter]->characters[0].hp;
    charaData[currentCharacter].bp = currentLine[currentCharacter]->characters[0].bp;
    charaData[currentCharacter].ap = currentLine[currentCharacter]->characters[0].ap;

    charaData[currentCharacter].stage = currentLine[currentCharacter]->characters[0].stage;
    charaData[currentCharacter].attribute = currentLine[currentCharacter]->characters[0].attribute;
    charaData[currentCharacter].spriteAttackId = currentLine[currentCharacter]->characters[0].attackSprite;
    
    charaData[currentCharacter].sleepTime = charaData[currentCharacter].initialSleepTime = currentLine[currentCharacter]->characters[0].sleepTime;
    charaData[currentCharacter].wakeupTime = charaData[currentCharacter].initialWakeupTime = currentLine[currentCharacter]->characters[0].wakeTime;
    charaData[currentCharacter].initialChangeTimer = charaData[currentCharacter].changeTimerLeft = currentLine[currentCharacter]->characters[0].changeTime;

    charaData[currentCharacter].initialStatsReductionTime = currentLine[currentCharacter]->characters[0].depleteTime;
    charaData[currentCharacter].minWeight = currentLine[currentCharacter]->characters[0].minWeight;

    // Luego le obligamos a que nazca con hambre
    charaData[currentCharacter].hungerCareMistakeTimer = CARE_MISTAKE_COUNTER_MAX;
    charaData[currentCharacter].strengthCareMistakeTimer = CARE_MISTAKE_COUNTER_MAX;

    charaData[currentCharacter].hatched = true;

    vpet_computeCallLight();

    interruptKey = CARE_MISTAKE_SCREEN;
    screenKey = TIMER_FINISHED_SCREEN;


}