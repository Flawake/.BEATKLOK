#include "debounce.h"
#include "esp_timer.h"

void button_debouncer_init(button_debouncer_t *btn, button_state_t initial_state, uint32_t debounce_time_ms) {
    btn->stable_state = initial_state;
    btn->last_sample = initial_state;
    btn->debounce_time_ms = debounce_time_ms;
    btn->last_transition_ms = 0;
    btn->edge_detected = false;
}

button_state_t button_debouncer_update(button_debouncer_t *btn,
                                       button_state_t read_state,
                                       uint32_t now_ms)
{
    /* Check if long enough stable */
    if ((now_ms - btn->last_transition_ms) >= btn->debounce_time_ms)
    {
        if (btn->stable_state != read_state)
        {
            btn->prev_stable_state = btn->stable_state;
            btn->stable_state = read_state;
            btn->last_transition_ms = now_ms;

            btn->edge_detected = true;
        }
    }

    return btn->stable_state;
}

bool button_edge_detected(button_debouncer_t *btn)
{
    bool detected = btn->edge_detected;
    btn->edge_detected = false;
    return detected;
}
