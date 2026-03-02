#ifndef ROTARY_H
#define ROTARY_H

#include <stdbool.h>
#include <stddef.h>
#include "debounce.h"

#define ROTARY_MAX_STEPS 50

typedef enum {
    ROTARY_NONE = 0,
    ROTARY_CW,
    ROTARY_CCW
} e_ROTARY_STEP;

const char *rotary_step_to_string(e_ROTARY_STEP step);

void init_rotary_encoder();

button_state_t read_rotary_button(void);

int16_t read_rotary_step(void);
#endif