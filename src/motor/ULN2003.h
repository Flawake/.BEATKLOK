#ifndef ULN2003_H
#define ULN2003_H

#include "driver/gpio.h"
#include "motor.h"

typedef struct {
    gpio_num_t IN1;
    gpio_num_t IN2;
    gpio_num_t IN3;
    gpio_num_t IN4;
} S_ULN2003Config;

typedef struct {
    S_ULN2003Config config;
    int8_t step_phase;
} S_ULN2003;

void uln2003_make_steps(S_ULN2003 *driver, E_MotorDir dir, uint16_t steps);

#endif
