#include "defs/defs.h"
#include "defs/chara_data.h"
#include "evolution.h"
#include "storage/storage.h"

bool change_onChangeTimerComplete() {
    for (int i = 0; i < currentLineCareInstr[currentCharacter]->numCareMistakesData; i++) {
        if (
            charaData[currentCharacter].idChara == currentLineCareInstr[currentCharacter]->careMistakeData[i].currentChara &&
            change_evalCharacter(i)
        ) {
            change_replaceCharaData(currentLineCareInstr[currentCharacter]->careMistakeData[i].nextChara);
            change_resetRuntimeStats();
            
            return true;
        }
    }

    return false;
}

bool change_evalCharacter(uint8_t nextCharaId) {
    CareMistakes_t* currentEvalCharacter = &(currentLineCareInstr[currentCharacter]->careMistakeData[nextCharaId]);
    printf("[EVAL] cm=%i, of=%i, sd=%i, ef=%i\n", charaData[currentCharacter].careMistakes, charaData[currentCharacter].overfeed, charaData[currentCharacter].sleepDisturbances, charaData[currentCharacter].effort);
    printf("[EVAL] MIN cm=%i, of=%i, sd=%i, ef=%i\n", currentEvalCharacter->minCareMistake, currentEvalCharacter->minOverfeeds, currentEvalCharacter->minSleepDist, currentEvalCharacter->minTraining);
    printf("[EVAL] MAX cm=%i, of=%i, sd=%i, ef=%i\n", currentEvalCharacter->maxCareMistake, currentEvalCharacter->maxOverfeeds, currentEvalCharacter->maxSleepDist, currentEvalCharacter->maxTraining);

    printf("[EVAL] stb=%i stw=%i\n", charaData[currentCharacter].stageTotalBattled, charaData[currentCharacter].stageTotalWon);
    printf("[EVAL] MIN stb=%i stw=%i\n", currentEvalCharacter->totalBattles, currentEvalCharacter->wonBattles);

    printf("[EVAL] NextID=%i\n", currentEvalCharacter->nextChara);

    bool retV = (
        (charaData[currentCharacter].careMistakes >= currentEvalCharacter->minCareMistake) && 
        (charaData[currentCharacter].careMistakes <= currentEvalCharacter->maxCareMistake) &&
        (charaData[currentCharacter].overfeed >= currentEvalCharacter->minOverfeeds) &&
        (charaData[currentCharacter].overfeed <= currentEvalCharacter->maxOverfeeds) &&
        (charaData[currentCharacter].sleepDisturbances >= currentEvalCharacter->minSleepDist) &&
        (charaData[currentCharacter].sleepDisturbances <= currentEvalCharacter->maxSleepDist) &&
        (charaData[currentCharacter].effort >= currentEvalCharacter->minTraining) &&
        (charaData[currentCharacter].effort <= currentEvalCharacter->maxTraining) &&
        (charaData[currentCharacter].stageTotalBattled >= currentEvalCharacter->totalBattles) &&
        (charaData[currentCharacter].stageTotalWon >= currentEvalCharacter->wonBattles)
    );

    printf("[EVAL] Res %i\n", retV);

    return retV;
}

void change_replaceCharaData(uint8_t nextCharaId) {
    LineChara_t* currentEvalCharacter = &(currentLine[currentCharacter]->characters[nextCharaId]);
    charaData[currentCharacter].idChara = nextCharaId;
    charaData[currentCharacter].hp = currentEvalCharacter->hp;
    charaData[currentCharacter].bp = currentEvalCharacter->bp;
    charaData[currentCharacter].ap = currentEvalCharacter->ap;
    charaData[currentCharacter].stage = currentEvalCharacter->stage;
    charaData[currentCharacter].attribute = currentEvalCharacter->attribute;
    charaData[currentCharacter].spriteAttackId = currentEvalCharacter->attackSprite;
    charaData[currentCharacter].initialSleepTime = currentEvalCharacter->sleepTime;
    charaData[currentCharacter].initialWakeupTime = currentEvalCharacter->wakeTime;
    charaData[currentCharacter].initialChangeTimer = currentEvalCharacter->changeTime;
    charaData[currentCharacter].initialStatsReductionTime = currentEvalCharacter->depleteTime;
    charaData[currentCharacter].minWeight = currentEvalCharacter->minWeight;
}

void change_resetRuntimeStats() {
    charaData[currentCharacter].careMistakes = 0;
    charaData[currentCharacter].effort = 0;
    charaData[currentCharacter].overfeed = 0;
    charaData[currentCharacter].sleepDisturbances = 0;
    charaData[currentCharacter].injuries = 0;
    
    charaData[currentCharacter].stageTotalBattled = 0;
    charaData[currentCharacter].stageTotalWon = 0;

    charaData[currentCharacter].dynamicSleepDists = 0;

    charaData[currentCharacter].sleepTime = charaData[currentCharacter].initialSleepTime;
    charaData[currentCharacter].wakeupTime = charaData[currentCharacter].initialWakeupTime;
    charaData[currentCharacter].changeTimerLeft = charaData[currentCharacter].initialChangeTimer;
}

void change_onChangeComplete() {
    char spriteFileName[30];
    snprintf(spriteFileName, 30, "/chara/%02x.bin", charaData[currentCharacter].idChara);

    storage_readFile(spriteFileName, &mainCharacterSprites);
}