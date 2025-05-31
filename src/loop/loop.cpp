#include "loop.h"
#include "defs/defs.h"
#include "vpet/vpet/vpet.h"

bool pauseLoop = false;
bool loopPaused = false;

void loop_pauseLoop() {
    pauseLoop = true;
    while (!loopPaused) { 
        delay(10);
    };
}

void loop_resumeLoop() {
    lastPressedButtonTime = vpetLastEvaluationTime = esp_timer_get_time();

    pauseLoop = false;
    while (loopPaused) { 
        printf("[LOOP] Loop paused\n");
    };
}