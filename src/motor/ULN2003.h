#ifndef ULN2003_H
#define ULN2003_H

#include "driver/gpio.h"

typedef enum
{
    CLOCKWISE = -1,
    COUNTER_CLOCKWISE = 1,
} E_MotorDir;

typedef struct {
    gpio_num_t IN1;
    gpio_num_t IN2;
    gpio_num_t IN3;
    gpio_num_t IN4;
} S_ULN2003_Config;

typedef struct {
    S_ULN2003_Config config;
    int8_t step_phase;
} S_ULN2003;

void uln2003_init(S_ULN2003 *driver);
void uln2003_make_steps(S_ULN2003 *driver, E_MotorDir dir, uint16_t steps);
void uln2003_release(S_ULN2003 *driver);

#endif
