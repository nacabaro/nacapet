#include "lines.h"
#include "defs/defs.h"
#include "defs/file_chara.h"

Egg_t* currentEgg = NULL;
LineCare_t** currentLineCareInstr = NULL;
Line_t** currentLine = NULL;

void lines_initLineStorage() {
    currentLineCareInstr = (LineCare_t**) malloc(sizeof(LineCare_t*) * CHARA_COUNT_IN_DEVICE); 
    currentLine = (Line_t**) malloc(sizeof(Line_t*) * CHARA_COUNT_IN_DEVICE);
}