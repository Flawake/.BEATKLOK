#ifndef STATUS_LEDS_H
#define STATUS_LEDS_H

#include <stdbool.h>
#include "driver/gpio.h"

typedef struct {
    gpio_num_t net_led_pin;
    gpio_num_t time_led_pin;
    gpio_num_t mode_led_pin;
} S_StatusLedConfig;

S_StatusLedConfig *get_default_led_config(void);

void status_leds_task(void *arg);

#endif
