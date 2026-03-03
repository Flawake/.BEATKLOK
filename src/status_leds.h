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

void init_status_leds(S_StatusLedConfig *config);

void set_net_led(S_StatusLedConfig *config, bool state);

void set_time_led(S_StatusLedConfig *config, bool state);

void set_mode_led(S_StatusLedConfig *config, bool state);

#endif
