#ifndef EVOLUTION_H
#define EVOLUTION_H

#include <stdint.h>

bool change_onChangeTimerComplete();
bool change_evalCharacter(uint8_t nextCharaId);
void change_replaceCharaData(uint8_t nextCharaId);
void change_resetRuntimeStats();
void change_onChangeComplete();

#endif