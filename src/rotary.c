#include <stdbool.h>
#include "rotary.h"
#include "driver/gpio.h"

typedef struct {
    gpio_num_t key_pin;
    gpio_num_t s1_pin;
    gpio_num_t s2_pin;
} S_RotaryEncoderConfig;

typedef struct {
    S_RotaryEncoderConfig rotary_encoder_config;
    bool last_s1;
} S_RotaryEncoder;

S_RotaryEncoder rotary_encoder = {
    .rotary_encoder_config = {
        .key_pin = GPIO_NUM_5,
        .s1_pin = GPIO_NUM_18,
        .s2_pin = GPIO_NUM_33,
    },
    .last_s1 = false,
};

void set_encoder_pin(gpio_num_t pin_num) {
    gpio_config_t io_conf = {
        .pin_bit_mask = 1ULL << pin_num,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE
    };
    gpio_config(&io_conf);
}

void init_rotary_encoder(void) {
    set_encoder_pin(rotary_encoder.key_pin);
    set_encoder_pin(rotary_encoder.s1_pin);
    set_encoder_pin(rotary_encoder.s2_pin);
}

bool read_rotary_button(void) {
    // Invert button since pull up is ative on the pin
    return gpio_get_level(rotary_encoder.key_pin) == 0;
}

e_ROTARY_STEP rotary_detect_update(bool last_s1, bool current_s1, bool current_s2) {
    if (current_s1 != last_s1) {
        if (current_s1 == 1) {
            return (current_s2 == 0) ? ROTARY_CW : ROTARY_CCW;
        }
    }
    return ROTARY_NONE;
}

e_ROTARY_STEP read_rotary_step(void) {
    bool current_s1 = (bool)gpio_get_level(rotary_encoder.s1_pin);
    bool current_s2 = (bool)gpio_get_level(rotary_encoder.s2_pin);

    e_ROTARY_STEP step = rotary_detect_update(rotary_encoder.last_s1, current_s1, current_s2);

    rotary_encoder.last_s1 = current_s1;

    return step;
}