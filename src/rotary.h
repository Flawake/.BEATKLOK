#ifndef ROTARY_H
#define ROTARY_H

typedef enum {
    ROTARY_NONE = 0,
    ROTARY_CW,
    ROTARY_CCW
} e_ROTARY_STEP;

void init_rotary_encoder();

bool read_rotary_button();

e_ROTARY_STEP read_rotary_step();
#endif