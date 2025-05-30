#include "vpet.h"
#include "defs/defs.h"
#include "defs/chara_data.h"
#include "vpet/evolution/evolution.h"

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
    charaData.careMistakeCallLight = (
        (charaData.hunger == 0 && !charaData.hungerCareMistakeObtained) || 
        (charaData.strength == 0 && !charaData.strengthCareMistakeObtained) || 
        (charaData.sleepy && !charaData.asleep && !charaData.sleepCareMistakeObtained)
    );
}

bool vpet_evalSleep(uint8_t diff_sec) {
    // Se devuelve true si quieres pausar los otros contadores
    // False ejecutara los contadores correspondientes
    if (
        dayUnixTime < charaData.sleepTime && 
        dayUnixTime > charaData.wakeupTime &&
        charaData.sleepy
        // Esto se ejecuta cuando ya es hora de despertarse
        // Resultado el personaje se despierta
    ) {
        charaData.sleepCareMistakeCounter = 0;
        charaData.sleepCareMistakeObtained = false;
        charaData.gotLifeYearAdded = false;

        charaData.sleepy = false;
        charaData.asleep = false;
        
        if (charaData.dynamicSleepDists > 0) {
            // Primero, el sleep time ahora está reventado, hay que restaurarlo
            // fácil, simplemente recalcula a la hora de despertar
            charaData.dynamicSleepDists--;

            charaData.sleepTime = (charaData.initialSleepTime + (charaData.dynamicSleepDists * 3600)) % SECONDS_IN_DAY;
            charaData.wakeupTime = (charaData.initialWakeupTime + (charaData.dynamicSleepDists * 3600)) % SECONDS_IN_DAY; 
        }

        return false;
        
    } else if (
        dayUnixTime < charaData.sleepTime && 
        dayUnixTime > charaData.wakeupTime &&
        charaData.asleep &&
        charaData.sleepCareMistakeCounter < SLEEP_COUNTER_MAX
        // Esto se ejecuta cuando mandamos a dormir al personaje
        // durante el dia. 
        // Resultado, el personaje deberia de dormir una siesta
    ) {
        charaData.sleepCareMistakeCounter += diff_sec;

        return true;

    } else if (
        dayUnixTime < charaData.sleepTime && 
        dayUnixTime > charaData.wakeupTime &&
        charaData.asleep &&
        charaData.sleepCareMistakeCounter >= SLEEP_COUNTER_MAX
        // Esto se ejecuta cuando la siesta del personaje acaba
        // Resultado, el personaje se despierta
    ) {
        charaData.sleepCareMistakeCounter = 0;
        charaData.asleep = false;

        return false;
        
    } else if (
        (
            dayUnixTime > charaData.sleepTime || 
            dayUnixTime < charaData.wakeupTime
        ) && 
        !charaData.sleepy
        // Esto se ejecuta cuando la hora actual del sistema
        // está en el intervalo temporal de las horas el las que el
        // personaje duerme
        // Resultado: el personaje se duerme, y se llama a la pantalla
        // de se acabo el temporizador, ademas activa la call light
    ) {
        charaData.sleepy = true;
        charaData.careMistakeCallLight = true;

        screenKey = TIMER_FINISHED_SCREEN;
        interruptKey = SLEEPY_SCREEN;

        return true; 

    } else if (
        charaData.sleepy && !charaData.asleep &&
        charaData.sleepCareMistakeCounter < SLEEP_COUNTER_MAX &&
        !charaData.sleepCareMistakeObtained
        // Esto se ejecuta cuando el personaje debería de estar durmiendo
        // pero no se le ha mandado a dormir, empieza a contar para pasar
        // un care mistake
    ) {
        charaData.sleepCareMistakeCounter += diff_sec;

        return true;

    } else if (
        charaData.sleepy && !charaData.asleep &&
        charaData.sleepCareMistakeCounter >= SLEEP_COUNTER_MAX &&
        !charaData.sleepCareMistakeObtained
        // Esto se ejecuta cuando el personaje deberia de estar durmiendo 
        // pero no se le ha mandado a dormir, y el contador ya ha llegado
        // al tiempo maximo
        // Resultado: se añade el care mistake y se activa la flag para
        // evitar otro care mistake
    ) {
        charaData.sleepCareMistakeObtained = true;
        charaData.careMistakes++;

        return true;

    } else if (
        !charaData.gotLifeYearAdded &&
        dayUnixTime < 60 // This stinks
        // Esto se ejecuta cuando es media noche.
        // Resultado: se incrementa la edad por 1
    ) {
        charaData.age++;
        charaData.gotLifeYearAdded = true;

        return true;

    } else if (
        charaData.sleepy
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
    if (charaData.hungerCareMistakeTimer > 0) {
        charaData.hungerCareMistakeTimer -= diff_sec;
    } 

    if (charaData.strengthCareMistakeTimer > 0) {
        charaData.strengthCareMistakeTimer -= diff_sec;
    } 

    if (charaData.changeTimerLeft > 0) {
        charaData.changeTimerLeft -= diff_sec;
    }
}

void vpet_evalHungerTimer() {
    if (
        charaData.hungerCareMistakeTimer <= 0 &&
        charaData.hunger > 0
    ) {
        charaData.hunger--;

        if (charaData.hunger > 0) {
            charaData.hungerCareMistakeTimer = charaData.initialStatsReductionTime;
            screenKey = TIMER_FINISHED_SCREEN;
            interruptKey = POOPING_SCREEN;
        } else {
            charaData.hungerCareMistakeTimer = CARE_MISTAKE_COUNTER_MAX;
            screenKey = TIMER_FINISHED_SCREEN;
            interruptKey = CARE_MISTAKE_SCREEN;
        }

        if (charaData.poopNumber < 8) {
            charaData.poopNumber++;
        } else {
            charaData.injured = true;
            charaData.injuries++;
        }

    } else if (
        charaData.hungerCareMistakeTimer <= 0 &&
        charaData.hunger == 0 &&
        !charaData.hungerCareMistakeObtained
    ) {
        charaData.hungerCareMistakeObtained = true;
        charaData.careMistakes++;

    }
}

void vpet_evalStrengthTimer() {
    if (
        charaData.strengthCareMistakeTimer <= 0 &&
        charaData.strength > 0
    ) {
        charaData.strength--;
        if (charaData.strength > 0) {
            charaData.strengthCareMistakeTimer = charaData.initialStatsReductionTime;
        } else {
            charaData.strengthCareMistakeTimer = CARE_MISTAKE_COUNTER_MAX;
            if (interruptKey != POOPING_SCREEN) {
                interruptKey = CARE_MISTAKE_SCREEN;
                screenKey = TIMER_FINISHED_SCREEN;
            }
        }

    } else if (
        charaData.strengthCareMistakeTimer <= 0 &&
        charaData.strength == 0 &&
        !charaData.strengthCareMistakeObtained
    ) {
        charaData.strengthCareMistakeObtained = true;
        charaData.careMistakes++;

    }
}

void vpet_evalChangeTimer() {
    if (charaData.changeTimerLeft <= 0) {
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

        if (charaData.hatched) {
            vpet_computeCallLight();

            if (!vpet_evalSleep(diffSec)) {
                vpet_reduceTimers(diffSec);
                vpet_evalTimers();
            }
            
            vpet_evalChangeTimer();
        
        } else if (!charaData.hatched && charaData.hatching) {
            charaData.hatchTimer += diffSec;
            
            if (charaData.hatchTimer > currentLine[currentCharacter]->hatchTime) {
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
    printf("[DEBUG] diffSec=%i\n", diffSec);

    if (charaData.hatched) {
        printf("[MAIN]: Hunger timer %d, hunger %d\n", charaData.hungerCareMistakeTimer, charaData.hunger);
        printf("[MAIN]: Strength timer %d, strength %d\n", charaData.strengthCareMistakeTimer, charaData.strength);
        printf("[MAIN]: Change timer %d\n", charaData.changeTimerLeft);
        printf("[MAIN]: RTC time is %d\n", dayUnixTime);
        printf("[MAIN]: Sleep counter is %d\n", charaData.sleepCareMistakeCounter);
        printf("[MAIN]: Care mistake count is %d\n", charaData.careMistakes);
        printf("[MAIN]: Is sleep care mistake tripped? %d\n", charaData.sleepCareMistakeObtained);
    } else if(!charaData.hatched && charaData.hatching) {
        printf("[DEBUG] hatchTimer=%i out of hatchTimer=%i\n", charaData.hatchTimer, currentLine[currentCharacter]->hatchTime);

    }
}