#include "device_config.h"

display_config_t display_config = {
    .sda_pin   = DISPLAY_SDA_PIN,
    .scl_pin   = DISPLAY_SCL_PIN,
    .reset_pin = DISPLAY_RESET_PIN,
    .width     = DISPLAY_WIDTH,
    .height    = DISPLAY_HEIGHT,
};

S_StatusLedConfig status_led_config = {
    .net_led_pin  = (gpio_num_t)STATUS_LED_NET_PIN,
    .time_led_pin = (gpio_num_t)STATUS_LED_TIME_PIN,
    .mode_led_pin = (gpio_num_t)STATUS_LED_MODE_PIN,
};

S_MotorDriver motor_config = {
    .driver = {
        .config = {
            .IN1 = (gpio_num_t)MOTOR_IN1_PIN,
            .IN2 = (gpio_num_t)MOTOR_IN2_PIN,
            .IN3 = (gpio_num_t)MOTOR_IN3_PIN,
            .IN4 = (gpio_num_t)MOTOR_IN4_PIN,
        },
        .step_phase = 0,
    },
    .local_rotation = 0.0f,
    .steps_per_revolution = (uint16_t)(MOTOR_GEAR_RATIO * MOTOR_STEPS_REVOLUTION),
};

S_RotaryEncoderConfig g_rotary_encoder_config = {
    .key_pin = (gpio_num_t)ROTARY_KEY_PIN_NUM,
    .s1_pin  = (gpio_num_t)ROTARY_S1_PIN_NUM,
    .s2_pin  = (gpio_num_t)ROTARY_S2_PIN_NUM,
};

display_config_t *get_display_config(void) {
    return &display_config;
}

S_StatusLedConfig *get_status_led_config(void) {
    return &status_led_config;
}

S_MotorDriver *get_motor_config(void) {
    return &motor_config;
}

S_RotaryEncoderConfig *get_rotary_config(void) {
    return &g_rotary_encoder_config;
}
