#ifndef ULN2003_H
#define ULN2003_H

#include "driver/gpio.h"

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

typedef enum {
    FORWARD = -1,
    BACKWARD = 1,
} E_MotorDir;

#endif
