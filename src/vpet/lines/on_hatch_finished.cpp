#include "lines.h"
#include "defs/defs.h"
#include "defs/chara_data.h"
#include "storage/storage.h"
#include "vpet/vpet.h"

void lines_onHatchComplete() {
    // Cuando se nace, lo que se va a hacer es poner el sprite 0, es decir,
    // La primera entrada de la linea es el sprite que sale.
    lines_freeCurrentEgg();

    char spriteFileName[30];
    snprintf(spriteFileName, 30, "/chara/%02x.bin", currentLine[currentCharacter]->characters[0].id);

    storage_readFile(spriteFileName, &mainCharacterSprites);

    charaData.hp = currentLine[currentCharacter]->characters[0].hp;
    charaData.bp = currentLine[currentCharacter]->characters[0].bp;
    charaData.ap = currentLine[currentCharacter]->characters[0].ap;

    charaData.stage = currentLine[currentCharacter]->characters[0].stage;
    charaData.attribute = currentLine[currentCharacter]->characters[0].attribute;

    charaData.sleepTime = currentLine[currentCharacter]->characters[0].sleepTime;
    charaData.wakeupTime = currentLine[currentCharacter]->characters[0].wakeTime;
    charaData.evoLeftTimer = currentLine[currentCharacter]->characters[0].changeTime;
    charaData.evoTime = currentLine[currentCharacter]->characters[0].changeTime;

    charaData.hungerCareMistakeTimer = CARE_MISTAKE_COUNTER_MAX;
    charaData.strengthCareMistakeTimer = CARE_MISTAKE_COUNTER_MAX;

    charaData.initialStatsReductionTime = currentLine[currentCharacter]->characters[0].depleteTime;
    charaData.minWeight = currentLine[currentCharacter]->characters[0].minWeight;

    charaData.hatched = true;
}