#ifndef LOOP_H
#define LOOP_H

// Idealmente deberia de usar la habilidad de pausar tareas de FreeRTOS, pero no fufa

extern bool pauseLoop;
extern bool loopPaused;

void loop_pauseLoop();
void loop_resumeLoop();

#endif