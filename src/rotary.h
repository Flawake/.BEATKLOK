#ifndef ROTARY_H
#define ROTARY_H

#include <stdbool.h>

typedef enum {
    ROTARY_NONE = 0,
    ROTARY_CW,
    ROTARY_CCW
} e_ROTARY_STEP;

const char *rotary_step_to_string(e_ROTARY_STEP step);

void init_rotary_encoder();

bool read_rotary_button();

e_ROTARY_STEP read_rotary_step();
#endif