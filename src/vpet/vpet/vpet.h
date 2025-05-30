#ifndef VPET_H
#define VPET_H

#include <Arduino.h>

extern uint64_t vpetLastEvaluationTime;

void IRAM_ATTR onActionTimerDelta();
void vpet_initTimer();
void vpet_computeCallLight();
bool vpet_evalSleep(uint8_t diff_sec);
void vpet_evalTimers();
void vpet_debugTimers(uint8_t diffSec);
void vpet_runVpetTasks();
void vpet_reduceTimers(uint8_t diff_sec);
void vpet_evalHungerTimer();
void vpet_evalStrengthTimer();

#endif