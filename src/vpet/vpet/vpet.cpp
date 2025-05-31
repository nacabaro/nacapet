#include "vpet.h"
#include "defs/defs.h"
#include "defs/chara_data.h"
#include "vpet/evolution/evolution.h"
#include "loop/loop.h"

hw_timer_t *actionTimerDelta = NULL;
bool runVpetTasks = false;

uint64_t vpetLastEvaluationTime = esp_timer_get_time();

void vpet_initTimer() {
    printf("[TIMER] Timer Init.\n");
    actionTimerDelta = timerBegin(0, 80, true);
    timerAttachInterrupt(actionTimerDelta, &onActionTimerDelta, true);
    timerAlarmWrite(actionTimerDelta, 1000000, true);
    timerAlarmEnable(actionTimerDelta);
}

void vpet_computeCallLight() {
    charaData[currentCharacter].careMistakeCallLight = (
        (charaData[currentCharacter].hunger == 0 && !charaData[currentCharacter].hungerCareMistakeObtained) || 
        (charaData[currentCharacter].strength == 0 && !charaData[currentCharacter].strengthCareMistakeObtained) || 
        (charaData[currentCharacter].sleepy && !charaData[currentCharacter].asleep && !charaData[currentCharacter].sleepCareMistakeObtained)
    );
}

bool vpet_evalSleep(uint8_t diff_sec) {
    // Se devuelve true si quieres pausar los otros contadores
    // False ejecutara los contadores correspondientes
    if (
        dayUnixTime < charaData[currentCharacter].sleepTime && 
        dayUnixTime > charaData[currentCharacter].wakeupTime &&
        charaData[currentCharacter].sleepy
        // Esto se ejecuta cuando ya es hora de despertarse
        // Resultado el personaje se despierta
    ) {
        charaData[currentCharacter].sleepCareMistakeCounter = 0;
        charaData[currentCharacter].sleepCareMistakeObtained = false;
        charaData[currentCharacter].gotLifeYearAdded = false;

        charaData[currentCharacter].sleepy = false;
        charaData[currentCharacter].asleep = false;
        
        if (charaData[currentCharacter].dynamicSleepDists > 0) {
            // Primero, el sleep time ahora está reventado, hay que restaurarlo
            // fácil, simplemente recalcula a la hora de despertar
            charaData[currentCharacter].dynamicSleepDists--;

            charaData[currentCharacter].sleepTime = (charaData[currentCharacter].initialSleepTime + (charaData[currentCharacter].dynamicSleepDists * 3600)) % SECONDS_IN_DAY;
            charaData[currentCharacter].wakeupTime = (charaData[currentCharacter].initialWakeupTime + (charaData[currentCharacter].dynamicSleepDists * 3600)) % SECONDS_IN_DAY; 
        }

        return false;
        
    } else if (
        dayUnixTime < charaData[currentCharacter].sleepTime && 
        dayUnixTime > charaData[currentCharacter].wakeupTime &&
        charaData[currentCharacter].asleep &&
        charaData[currentCharacter].sleepCareMistakeCounter < SLEEP_COUNTER_MAX
        // Esto se ejecuta cuando mandamos a dormir al personaje
        // durante el dia. 
        // Resultado, el personaje deberia de dormir una siesta
    ) {
        charaData[currentCharacter].sleepCareMistakeCounter += diff_sec;

        return true;

    } else if (
        dayUnixTime < charaData[currentCharacter].sleepTime && 
        dayUnixTime > charaData[currentCharacter].wakeupTime &&
        charaData[currentCharacter].asleep &&
        charaData[currentCharacter].sleepCareMistakeCounter >= SLEEP_COUNTER_MAX
        // Esto se ejecuta cuando la siesta del personaje acaba
        // Resultado, el personaje se despierta
    ) {
        charaData[currentCharacter].sleepCareMistakeCounter = 0;
        charaData[currentCharacter].asleep = false;

        return false;
        
    } else if (
        (
            dayUnixTime > charaData[currentCharacter].sleepTime || 
            dayUnixTime < charaData[currentCharacter].wakeupTime
        ) && 
        !charaData[currentCharacter].sleepy
        // Esto se ejecuta cuando la hora actual del sistema
        // está en el intervalo temporal de las horas el las que el
        // personaje duerme
        // Resultado: el personaje se duerme, y se llama a la pantalla
        // de se acabo el temporizador, ademas activa la call light
    ) {
        charaData[currentCharacter].sleepy = true;
        charaData[currentCharacter].careMistakeCallLight = true;

        screenKey = TIMER_FINISHED_SCREEN;
        interruptKey = SLEEPY_SCREEN;

        return true; 

    } else if (
        charaData[currentCharacter].sleepy && !charaData[currentCharacter].asleep &&
        charaData[currentCharacter].sleepCareMistakeCounter < SLEEP_COUNTER_MAX &&
        !charaData[currentCharacter].sleepCareMistakeObtained
        // Esto se ejecuta cuando el personaje debería de estar durmiendo
        // pero no se le ha mandado a dormir, empieza a contar para pasar
        // un care mistake
    ) {
        charaData[currentCharacter].sleepCareMistakeCounter += diff_sec;

        return true;

    } else if (
        charaData[currentCharacter].sleepy && !charaData[currentCharacter].asleep &&
        charaData[currentCharacter].sleepCareMistakeCounter >= SLEEP_COUNTER_MAX &&
        !charaData[currentCharacter].sleepCareMistakeObtained
        // Esto se ejecuta cuando el personaje deberia de estar durmiendo 
        // pero no se le ha mandado a dormir, y el contador ya ha llegado
        // al tiempo maximo
        // Resultado: se añade el care mistake y se activa la flag para
        // evitar otro care mistake
    ) {
        charaData[currentCharacter].sleepCareMistakeObtained = true;
        charaData[currentCharacter].careMistakes++;

        return true;

    } else if (
        !charaData[currentCharacter].gotLifeYearAdded &&
        dayUnixTime < 60 // This stinks
        // Esto se ejecuta cuando es media noche.
        // Resultado: se incrementa la edad por 1
    ) {
        charaData[currentCharacter].age++;
        charaData[currentCharacter].gotLifeYearAdded = true;

        return true;

    } else if (
        charaData[currentCharacter].sleepy
    ) {
        return true;
    }

    return false;

}

void vpet_evalTimers() {
    vpet_evalHungerTimer();
    vpet_evalStrengthTimer();
}

void vpet_reduceTimers(uint8_t diff_sec) {
    if (charaData[currentCharacter].hungerCareMistakeTimer > 0) {
        charaData[currentCharacter].hungerCareMistakeTimer -= diff_sec;
    } 

    if (charaData[currentCharacter].strengthCareMistakeTimer > 0) {
        charaData[currentCharacter].strengthCareMistakeTimer -= diff_sec;
    } 

    if (charaData[currentCharacter].changeTimerLeft > 0) {
        charaData[currentCharacter].changeTimerLeft -= diff_sec;
    }
}

void vpet_evalHungerTimer() {
    if (
        charaData[currentCharacter].hungerCareMistakeTimer <= 0 &&
        charaData[currentCharacter].hunger > 0
    ) {
        charaData[currentCharacter].hunger--;

        if (charaData[currentCharacter].hunger > 0) {
            charaData[currentCharacter].hungerCareMistakeTimer = charaData[currentCharacter].initialStatsReductionTime;
            screenKey = TIMER_FINISHED_SCREEN;
            interruptKey = POOPING_SCREEN;
        } else {
            charaData[currentCharacter].hungerCareMistakeTimer = CARE_MISTAKE_COUNTER_MAX;
            screenKey = TIMER_FINISHED_SCREEN;
            interruptKey = CARE_MISTAKE_SCREEN;
        }

        if (charaData[currentCharacter].poopNumber < 8) {
            charaData[currentCharacter].poopNumber++;
        } else {
            charaData[currentCharacter].injured = true;
            charaData[currentCharacter].injuries++;
        }

    } else if (
        charaData[currentCharacter].hungerCareMistakeTimer <= 0 &&
        charaData[currentCharacter].hunger == 0 &&
        !charaData[currentCharacter].hungerCareMistakeObtained
    ) {
        charaData[currentCharacter].hungerCareMistakeObtained = true;
        charaData[currentCharacter].careMistakes++;

    }
}

void vpet_evalStrengthTimer() {
    if (
        charaData[currentCharacter].strengthCareMistakeTimer <= 0 &&
        charaData[currentCharacter].strength > 0
    ) {
        charaData[currentCharacter].strength--;
        if (charaData[currentCharacter].strength > 0) {
            charaData[currentCharacter].strengthCareMistakeTimer = charaData[currentCharacter].initialStatsReductionTime;
        } else {
            charaData[currentCharacter].strengthCareMistakeTimer = CARE_MISTAKE_COUNTER_MAX;
            if (interruptKey != POOPING_SCREEN) {
                interruptKey = CARE_MISTAKE_SCREEN;
                screenKey = TIMER_FINISHED_SCREEN;
            }
        }

    } else if (
        charaData[currentCharacter].strengthCareMistakeTimer <= 0 &&
        charaData[currentCharacter].strength == 0 &&
        !charaData[currentCharacter].strengthCareMistakeObtained
    ) {
        charaData[currentCharacter].strengthCareMistakeObtained = true;
        charaData[currentCharacter].careMistakes++;

    }
}

void vpet_evalChangeTimer() {
    if (charaData[currentCharacter].changeTimerLeft <= 0) {
        if (change_onChangeTimerComplete()) {
            screenKey = TIMER_FINISHED_SCREEN;
            interruptKey = EVOLUTION_SCREEN;

            pauseLoop = true;
        }
    }
}


void IRAM_ATTR onActionTimerDelta() {
    runVpetTasks = true;
}

void vpet_runVpetTasks() {
    if (runVpetTasks) {
        uint64_t currentEvaluationTime = esp_timer_get_time();

        uint64_t deltaUs   = currentEvaluationTime - vpetLastEvaluationTime;
        uint8_t  diffSec  = (deltaUs + 1000000 - 1000) / 1000000;

        if (charaData[currentCharacter].hatched) {
            vpet_computeCallLight();

            if (!vpet_evalSleep(diffSec)) {
                vpet_reduceTimers(diffSec);
                vpet_evalTimers();
            }
            
            vpet_evalChangeTimer();
        
        } else if (!charaData[currentCharacter].hatched && charaData[currentCharacter].hatching) {
            charaData[currentCharacter].hatchTimer += diffSec;
            
            if (charaData[currentCharacter].hatchTimer > currentLine[currentCharacter]->hatchTime) {
                interruptKey = EGG_HATCH_SCREEN;
                screenKey = TIMER_FINISHED_SCREEN;
            }
    
        }
        
        vpet_debugTimers(diffSec);

        runVpetTasks = false;
        vpetLastEvaluationTime = currentEvaluationTime;
    }
}

void vpet_debugTimers(uint8_t diffSec) {
    if (charaData[currentCharacter].hatched) {
        printf("[MAIN]: Hunger timer %d, hunger %d\n", charaData[currentCharacter].hungerCareMistakeTimer, charaData[currentCharacter].hunger);
        printf("[MAIN]: Strength timer %d, strength %d\n", charaData[currentCharacter].strengthCareMistakeTimer, charaData[currentCharacter].strength);
        printf("[MAIN]: Change timer %d\n", charaData[currentCharacter].changeTimerLeft);
        printf("[MAIN]: RTC time is %d\n", dayUnixTime);
        printf("[MAIN]: Sleep counter is %d\n", charaData[currentCharacter].sleepCareMistakeCounter);
        printf("[MAIN]: Care mistake count is %d\n", charaData[currentCharacter].careMistakes);
        printf("[MAIN]: Is sleep care mistake tripped? %d\n", charaData[currentCharacter].sleepCareMistakeObtained);
    } else if(!charaData[currentCharacter].hatched && charaData[currentCharacter].hatching) {
        printf("[DEBUG] hatchTimer=%i out of hatchTimer=%i\n", charaData[currentCharacter].hatchTimer, currentLine[currentCharacter]->hatchTime);

    }
}