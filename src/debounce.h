#ifndef DEBOUNCE_H
#define DEBOUNCE_H

#include <stdint.h>
#include <stdbool.h>
#include "driver/gpio.h"

#define DEFAULT_DEBOUNCE_TIME_MS 30

typedef enum {
    BUTTON_STATE_IDLE = 0,
    BUTTON_STATE_PRESSED,
    BUTTON_STATE_RELEASED
} button_state_t;

typedef struct
{
    button_state_t stable_state;
    button_state_t last_sample;

    uint32_t debounce_time_ms;
    uint32_t last_transition_ms;
} button_debouncer_t;

void button_debouncer_init(button_debouncer_t *btn, button_state_t initial_state, uint32_t debounce_time_ms);

button_state_t button_debouncer_update(button_debouncer_t *btn, button_state_t read_state, uint32_t now_ms);

#endif
