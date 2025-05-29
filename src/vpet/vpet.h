#ifndef VPET_H
#define VPET_H

#include <Arduino.h>

void IRAM_ATTR onActionTimerDelta();
void vpet_initTimer();
void vpet_computeCallLight();
bool vpet_evalSleep();
void vpet_evalTimers();
void vpet_runVpetTasks();
void vpet_evalHungerTimer();
void vpet_evalStrengthTimer();

#endif