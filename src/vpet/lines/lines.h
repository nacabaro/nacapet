#ifndef LINES_H
#define LINES_H

#include "defs/file_chara.h"
#include <FS.h>

void lines_getAvailableLines();
void lines_getSingleLine(const char* fileName);
void lines_getLineCareMistakes(const char* fileName);
void lines_freeEggList();
void lines_freeCurrentEgg();
void lines_initLineStorage();
void lines_getSingleEggSprites(fs::File &lineFile, Egg_t* eggList);
void lines_onHatchComplete();

#endif