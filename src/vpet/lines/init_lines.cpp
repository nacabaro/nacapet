#include "lines.h"
#include "defs/defs.h"
#include "defs/file_chara.h"
#include "defs/chara_data.h"

Egg_t* currentEgg = NULL;
LineCare_t** currentLineCareInstr = NULL;
Line_t** currentLine = NULL;

void lines_initLineStorage() {
    charaData = (struct CharacterData*) calloc(CHARA_COUNT_IN_DEVICE, sizeof(struct CharacterData));
    currentLineCareInstr = (LineCare_t**) calloc(CHARA_COUNT_IN_DEVICE, sizeof(LineCare_t*)); 
    currentLine = (Line_t**) calloc(CHARA_COUNT_IN_DEVICE, sizeof(Line_t*));
}