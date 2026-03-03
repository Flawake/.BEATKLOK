#include "status_leds.h"

S_StatusLedConfig default_led_config = {
    .net_led_pin = 36,
    .time_led_pin = 37,
    .mode_led_pin = 38,
};

S_StatusLedConfig *get_default_led_config(void) {
    return &default_led_config;
}

void init_status_leds(S_StatusLedConfig *config) {
    gpio_config_t io_conf = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask =
            (1ULL << config->net_led_pin) |
            (1ULL << config->time_led_pin) |
            (1ULL << config->mode_led_pin),
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en   = GPIO_PULLUP_DISABLE,
        .intr_type    = GPIO_INTR_DISABLE
    };

    gpio_config(&io_conf);
}

void set_net_led(S_StatusLedConfig *config, bool state) {
    gpio_set_level(config->net_led_pin, state);
}

void set_time_led(S_StatusLedConfig *config, bool state) {
    gpio_set_level(config->time_led_pin, state);
}

void set_mode_led(S_StatusLedConfig *config, bool state) {
    gpio_set_level(config->mode_led_pin, state);
}
