#include "defs/defs.h"
#include "defs/chara_data.h"
#include "evolution.h"
#include "storage/storage.h"

bool change_onChangeTimerComplete() {
    for (int i = 0; i < currentLineCareInstr[currentCharacter]->numCareMistakesData; i++) {
        if (
            charaData.idChara == currentLineCareInstr[currentCharacter]->careMistakeData[i].currentChara &&
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
    printf("[EVAL] cm=%i, of=%i, sd=%i, ef=%i\n", charaData.careMistakes, charaData.overfeed, charaData.sleepDisturbances, charaData.effort);
    printf("[EVAL] MIN cm=%i, of=%i, sd=%i, ef=%i\n", currentEvalCharacter->minCareMistake, currentEvalCharacter->minOverfeeds, currentEvalCharacter->minSleepDist, currentEvalCharacter->minTraining);
    printf("[EVAL] MAX cm=%i, of=%i, sd=%i, ef=%i\n", currentEvalCharacter->maxCareMistake, currentEvalCharacter->maxOverfeeds, currentEvalCharacter->maxSleepDist, currentEvalCharacter->maxTraining);

    printf("[EVAL] stb=%i stw=%i\n", charaData.stageTotalBattled, charaData.stageTotalWon);
    printf("[EVAL] MIN stb=%i stw=%i\n", currentEvalCharacter->totalBattles, currentEvalCharacter->wonBattles);

    printf("[EVAL] NextID=%i\n", currentEvalCharacter->nextChara);

    bool retV = (
        (charaData.careMistakes >= currentEvalCharacter->minCareMistake) && 
        (charaData.careMistakes <= currentEvalCharacter->maxCareMistake) &&
        (charaData.overfeed >= currentEvalCharacter->minOverfeeds) &&
        (charaData.overfeed <= currentEvalCharacter->maxOverfeeds) &&
        (charaData.sleepDisturbances >= currentEvalCharacter->minSleepDist) &&
        (charaData.sleepDisturbances <= currentEvalCharacter->maxSleepDist) &&
        (charaData.effort >= currentEvalCharacter->minTraining) &&
        (charaData.effort <= currentEvalCharacter->maxTraining) &&
        (charaData.stageTotalBattled >= currentEvalCharacter->totalBattles) &&
        (charaData.stageTotalWon >= currentEvalCharacter->wonBattles)
    );

    printf("[EVAL] Res %i\n", retV);

    return retV;
}

void change_replaceCharaData(uint8_t nextCharaId) {
    LineChara_t* currentEvalCharacter = &(currentLine[currentCharacter]->characters[nextCharaId]);
    charaData.idChara = nextCharaId;
    charaData.hp = currentEvalCharacter->hp;
    charaData.bp = currentEvalCharacter->bp;
    charaData.ap = currentEvalCharacter->ap;
    charaData.stage = currentEvalCharacter->stage;
    charaData.attribute = currentEvalCharacter->attribute;
    charaData.initialSleepTime = currentEvalCharacter->sleepTime;
    charaData.initialWakeupTime = currentEvalCharacter->wakeTime;
    charaData.initialChangeTimer = currentEvalCharacter->changeTime;
    charaData.initialStatsReductionTime = currentEvalCharacter->depleteTime;
    charaData.minWeight = currentEvalCharacter->minWeight;
}

void change_resetRuntimeStats() {
    charaData.careMistakes = 0;
    charaData.effort = 0;
    charaData.overfeed = 0;
    charaData.sleepDisturbances = 0;
    charaData.injuries = 0;
    
    charaData.stageTotalBattled = 0;
    charaData.stageTotalWon = 0;

    charaData.dynamicSleepDists = 0;

    charaData.sleepTime = charaData.initialSleepTime;
    charaData.wakeupTime = charaData.initialWakeupTime;
    charaData.changeTimerLeft = charaData.initialChangeTimer;
}

void change_onChangeComplete() {
    char spriteFileName[30];
    snprintf(spriteFileName, 30, "/chara/%02x.bin", charaData.idChara);

    storage_readFile(spriteFileName, &mainCharacterSprites);
}