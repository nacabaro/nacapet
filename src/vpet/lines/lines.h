#ifndef LINES_H
#define LINES_H

#include "defs/file_chara.h"
#include <FS.h>

struct Egg_t* lines_getAvailableLines(uint8_t* count);
struct Line_t* lines_getSingleLine(const char* fileName, Egg_t* eggList);
void lines_getSingleEggSprites(fs::File &lineFile, Egg_t* eggList);
void lines_freeEggList(Egg_t* eggList, uint8_t eggNumber);
struct LineCare_t* lines_getLineCareMistakes(const char* fileName);
void lines_testLines();

#endif