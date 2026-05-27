#ifndef BUTTONS_H
#define BUTTONS_H

#include <Arduino.h>

uint8_t buttons_getPressedButtons();
void buttons_checkInactivity();

#endif