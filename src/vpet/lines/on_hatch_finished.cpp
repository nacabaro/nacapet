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
    charaData.hp = currentLine[currentCharacter]->characters[0].hp;
    charaData.bp = currentLine[currentCharacter]->characters[0].bp;
    charaData.ap = currentLine[currentCharacter]->characters[0].ap;

    charaData.stage = currentLine[currentCharacter]->characters[0].stage;
    charaData.attribute = currentLine[currentCharacter]->characters[0].attribute;
    
    charaData.sleepTime = charaData.initialSleepTime = currentLine[currentCharacter]->characters[0].sleepTime;
    charaData.wakeupTime = charaData.initialWakeupTime = currentLine[currentCharacter]->characters[0].wakeTime;
    charaData.initialChangeTimer = charaData.changeTimerLeft = currentLine[currentCharacter]->characters[0].changeTime;

    charaData.initialStatsReductionTime = currentLine[currentCharacter]->characters[0].depleteTime;
    charaData.minWeight = currentLine[currentCharacter]->characters[0].minWeight;

    // Luego le obligamos a que nazca con hambre
    charaData.hungerCareMistakeTimer = CARE_MISTAKE_COUNTER_MAX;
    charaData.strengthCareMistakeTimer = CARE_MISTAKE_COUNTER_MAX;

    charaData.hatched = true;

    vpet_computeCallLight();

    interruptKey = CARE_MISTAKE_SCREEN;
    screenKey = TIMER_FINISHED_SCREEN;


}