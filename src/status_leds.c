#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "status_leds.h"
#include "mode.h"
#include "net.h"

S_StatusLedConfig default_led_config = {
    .net_led_pin = 36,
    .time_led_pin = 37,
    .mode_led_pin = 38,
};

S_StatusLedConfig *get_default_led_config(void) {
    return &default_led_config;
}

static void set_net_led(S_StatusLedConfig *config, bool state) {
    gpio_set_level(config->net_led_pin, state);
}

static void set_time_led(S_StatusLedConfig *config, bool state) {
    gpio_set_level(config->time_led_pin, state);
}

static void set_mode_led(S_StatusLedConfig *config, bool state) {
    gpio_set_level(config->mode_led_pin, state);
}

void status_leds_task(void *arg) {
    S_StatusLedConfig config = *(S_StatusLedConfig *)arg;

    gpio_config_t io_conf = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask =
            (1ULL << config.net_led_pin) |
            (1ULL << config.time_led_pin) |
            (1ULL << config.mode_led_pin),
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en   = GPIO_PULLUP_DISABLE,
        .intr_type    = GPIO_INTR_DISABLE
    };

    gpio_config(&io_conf);

    bool net_led_state = false;
    bool time_led_state = false;

    while (1)
    {
        if (wifi_is_connected()) {
            set_net_led(&config, true);
        }
        else {
            net_led_state = !net_led_state;
            set_net_led(&config, net_led_state);
        }

        time_led_state = !time_led_state;
        set_time_led(&config, time_led_state);

        set_mode_led(&config, (bool)get_system_mode());

        vTaskDelay(pdMS_TO_TICKS(864)); // 1 centibead = 864ms
    }
    
}
