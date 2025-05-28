#include "vpet.h"
#include "defs/defs.h"
#include "defs/chara_data.h"

hw_timer_t *actionTimerDelta = NULL;
bool runVpetTasks = false;

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

bool vpet_evalSleep() {
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

        return false;
        
    } else if (
        dayUnixTime < charaData.sleepTime && 
        dayUnixTime > charaData.wakeupTime &&
        charaData.asleep &&
        charaData.sleepCareMistakeCounter < 60
        // Esto se ejecuta cuando mandamos a dormir al personaje
        // durante el dia. 
        // Resultado, el personaje deberia de dormir una siesta
    ) {
        charaData.sleepCareMistakeCounter++;

        return true;

    } else if (
        dayUnixTime < charaData.sleepTime && 
        dayUnixTime > charaData.wakeupTime &&
        charaData.asleep &&
        charaData.sleepCareMistakeCounter >= 60
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
        charaData.sleepCareMistakeCounter < 60 &&
        !charaData.sleepCareMistakeObtained
        // Esto se ejecuta cuando el personaje debería de estar durmiendo
        // pero no se le ha mandado a dormir, empieza a contar para pasar
        // un care mistake
    ) {
        charaData.sleepCareMistakeCounter++;

        return true;

    } else if (
        charaData.sleepy && !charaData.asleep &&
        charaData.sleepCareMistakeCounter >= 60 &&
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
        dayUnixTime < 1 // This stinks
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
    if (charaData.hungerCareMistakeTimer >= 0) {
        charaData.hungerCareMistakeTimer -= 1;
    }

    if (charaData.strengthCareMistakeTimer >= 0) {
        charaData.strengthCareMistakeTimer -= 1;
    }

    if (
        charaData.hungerCareMistakeTimer < 0 ||
        charaData.strengthCareMistakeTimer < 0
    ) {
        if (charaData.hunger > 0) {
            charaData.hunger--;
            screenKey = TIMER_FINISHED_SCREEN;
            interruptKey = POOPING_SCREEN;
            charaData.poopNumber++;
            if (charaData.hunger > 0) {
                charaData.hungerCareMistakeTimer = charaData.initialStatsReductionTime;
            } else {
                charaData.hungerCareMistakeTimer = CARE_MISTAKE_COUNTER_MAX;
            }
        } 

        if (!charaData.hungerCareMistakeObtained) {
            if (
                charaData.hunger == 0 && 
                charaData.hungerCareMistakeTimer < 0
            ) {
                charaData.careMistakes++;
                charaData.hungerCareMistakeObtained = true;
                
            } else if (charaData.hunger == 0) {
                interruptKey = POOPING_SCREEN;
                screenKey = TIMER_FINISHED_SCREEN;
                charaData.poopNumber++;
            }
        }

        if (charaData.strength > 0) {
            charaData.strength--;
            if (charaData.strength > 0) {
                charaData.strengthCareMistakeTimer = charaData.initialStatsReductionTime;
            } else {
                charaData.strengthCareMistakeTimer = CARE_MISTAKE_COUNTER_MAX;
            }
        } 

        if (!charaData.strengthCareMistakeObtained) {
            if (
                charaData.strength == 0 && 
                charaData.strengthCareMistakeTimer < 0
            ) {
                charaData.careMistakes++;
                charaData.strengthCareMistakeObtained = true;
    
            } else if(charaData.strength == 0) {
                if (interruptKey != POOPING_SCREEN) {
                    interruptKey = CARE_MISTAKE_SCREEN;
                    screenKey = TIMER_FINISHED_SCREEN;
                }
            }
        }
    }
}

void IRAM_ATTR onActionTimerDelta() {
    runVpetTasks = true;
}

void vpet_runVpetTasks() {
    if (runVpetTasks && charaData.hatched) {
        vpet_computeCallLight();
        if (!vpet_evalSleep()) {
            vpet_evalTimers();
        }
        printf("[MAIN]: Hunger timer %d, hunger %d\n", charaData.hungerCareMistakeTimer, charaData.hunger);
        printf("[MAIN]: Strength timer %d, strength %d\n", charaData.strengthCareMistakeTimer, charaData.strength);
        printf("[MAIN]: Evo timer %d\n", charaData.evoLeftTimer);
        printf("[MAIN]: RTC time is %d\n", dayUnixTime);
        printf("[MAIN]: Sleep counter is %d\n", charaData.sleepCareMistakeCounter);
        printf("[MAIN]: Care mistake count is %d\n", charaData.careMistakes);
        printf("[MAIN]: Is sleep care mistake tripped? %d\n", charaData.sleepCareMistakeObtained);

        runVpetTasks = false;
    } else if (runVpetTasks && !charaData.hatched && charaData.hatching) {
        charaData.hatchTimer++;
        printf("[DEBUG] hatchTimer=%i out of hatchTimer=%i\n", charaData.hatchTimer, currentLine[currentCharacter]->hatchTime);
        if (charaData.hatchTimer > currentLine[currentCharacter]->hatchTime) {
            interruptKey = EGG_HATCH_SCREEN;
            screenKey = TIMER_FINISHED_SCREEN;
        }

        runVpetTasks = false;
    }
}