#include "steps.h"
#include "defs/defs.h"

float gravity     = 0.0;
const float alpha = 0.99;
const float thresh = 0.80; 
uint64_t lastStepTime = esp_timer_get_time();


void steps_countSteps() {
    int16_t ax, ay, az;
    mpu.getAcceleration(&ax, &ay, &az);
  
    float axg = ax / 16384.0;
    float ayg = ay / 16384.0;
    float azg = az / 16384.0;
  
    float mag = sqrt(axg*axg + ayg*ayg + azg*azg);
  
    gravity = alpha * gravity + (1 - alpha) * mag;
  
    float dyn = mag - gravity;
  
    unsigned long now = esp_timer_get_time();
    if (dyn > thresh && (now - lastStepTime) > 500000) {
        stepCounter++;
        lastStepTime = now;
    }
}
