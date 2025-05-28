#include "lines.h"
#include "defs/defs.h"
#include "defs/chara_data.h"

void lines_freeEggList() {
    for (int i = 0; i < eggNumber; i++) {
        for (int j = 0; j < eggSelection[i].eggSprite.spriteNumber; j++) {
            free(eggSelection[i].eggSprite.spriteData[j]);
        }

        free(eggSelection[i].eggSprite.spriteData);
    }

    free(eggSelection);
    
    eggSelection = NULL;
    eggNumber = 0;
}

void lines_freeCurrentEgg() {
    for (int i = 0; i < currentEgg->eggSprite.spriteNumber; i++) {
        free(currentEgg->eggSprite.spriteData[i]);
    } 

    free(currentEgg->eggSprite.spriteData);
    free(currentEgg);

    currentEgg = NULL;
}
