#ifndef STORAGE_H
#define STORAGE_H

#include <SPIFFS.h>
#include <stdio.h>
#include <TFT_eSPI.h>

#include "defs/sprite_data.h"

void storage_init();
void storage_readFile(const char* path, struct SpriteData* spriteData);
void storage_initBackground(const int id, TFT_eSprite &bg);

void storage_saveState(); 
void storage_loadState();
bool storage_deleteState();

#endif
