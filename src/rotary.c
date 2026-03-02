#include <stdint.h>
#include "rotary.h"
#include "driver/gpio.h"
#include "debounce.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "esp_timer.h"

#define BUTTON_ISR_QUEUE_SIZE 50

typedef struct
{
    uint32_t time_ms;
    button_state_t state;
    gpio_num_t triggered_pin;
} button_sample_t;

static QueueHandle_t button_queue;

typedef struct {
    gpio_num_t key_pin;
    gpio_num_t s1_pin;
    gpio_num_t s2_pin;
} S_RotaryEncoderConfig;

typedef struct {
    S_RotaryEncoderConfig config;
    button_state_t last_s2;
    int16_t step_count;
} S_RotaryEncoder;

S_RotaryEncoder rotary_encoder = {
    .config = {
        .key_pin = GPIO_NUM_33,
        .s1_pin = GPIO_NUM_18,
        .s2_pin = GPIO_NUM_5,
    },
    .last_s2 = RELEASED,
    .step_count = 0,
};

button_debouncer_t key_button;
button_debouncer_t sw1_key;
button_debouncer_t sw2_key;

const char *rotary_step_to_string(e_ROTARY_STEP step)
{
    switch (step) {
        case ROTARY_NONE: return "NONE";
        case ROTARY_CW:   return "CW";
        case ROTARY_CCW:  return "CCW";
        default:          return "UNKNOWN";
    }
}

e_ROTARY_STEP rotary_detect_update(button_state_t current_s1, button_state_t current_s2, button_state_t last_s2) {
    if (current_s2 != last_s2 && current_s2 == RELEASED) {
        return (current_s1 == RELEASED) ? ROTARY_CW : ROTARY_CCW;
    }
    return ROTARY_NONE;
}

static void replay_encoder_interrupts() {
    if (button_queue == NULL) {
        return;
    }

    button_sample_t sample;
    while (xQueueReceive(button_queue, &sample, 0) == pdTRUE) {
        if (sample.triggered_pin == rotary_encoder.config.key_pin) {
            (void)button_debouncer_update(&key_button, sample.state, sample.time_ms);
            continue;
        }

        if (sample.triggered_pin == rotary_encoder.config.s1_pin) {
            (void)button_debouncer_update(&sw1_key, sample.state, sample.time_ms);
            continue;
        }

        if (sample.triggered_pin == rotary_encoder.config.s2_pin) {
            button_state_t current_s1 = sw1_key.stable_state;
            button_state_t current_s2 = button_debouncer_update(&sw2_key, sample.state, sample.time_ms);

            e_ROTARY_STEP step = rotary_detect_update(current_s1, current_s2, rotary_encoder.last_s2);
            rotary_encoder.last_s2 = current_s2;

            if (step == ROTARY_CW) {
                rotary_encoder.step_count++;
            } else if (step == ROTARY_CCW) {
                rotary_encoder.step_count--;
            }
        }
    }
}

static void IRAM_ATTR encoder_isr_handler(void *arg) {
    gpio_num_t triggered_pin = (gpio_num_t)(uintptr_t)arg;
    button_sample_t sample = {
        .time_ms = esp_timer_get_time() / 1000,
        .state = gpio_get_level(triggered_pin) ? RELEASED : PRESSED,
        .triggered_pin = triggered_pin
    };

    xQueueSendFromISR(button_queue, &sample, NULL);
}

void set_encoder_pin(gpio_num_t pin_num) {
    gpio_config_t io_conf = {
        .pin_bit_mask = 1ULL << pin_num,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .intr_type = GPIO_INTR_ANYEDGE,
    };
    gpio_config(&io_conf);

    gpio_isr_handler_add(pin_num, encoder_isr_handler, (void *)(uintptr_t)pin_num);
}

void init_rotary_encoder(void) {
    button_queue = xQueueCreate(BUTTON_ISR_QUEUE_SIZE, sizeof(button_sample_t));

    gpio_install_isr_service(ESP_INTR_FLAG_IRAM);
    set_encoder_pin(rotary_encoder.config.key_pin);
    set_encoder_pin(rotary_encoder.config.s1_pin);
    set_encoder_pin(rotary_encoder.config.s2_pin);

    button_state_t s1_state = gpio_get_level(rotary_encoder.config.s1_pin) ? RELEASED : PRESSED;
    button_state_t s2_state = gpio_get_level(rotary_encoder.config.s2_pin) ? RELEASED : PRESSED;

    button_debouncer_init(&key_button, RELEASED, DEFAULT_DEBOUNCE_TIME_MS);
    button_debouncer_init(&sw1_key, s1_state, DEFAULT_DEBOUNCE_TIME_MS);
    button_debouncer_init(&sw2_key, s2_state, DEFAULT_DEBOUNCE_TIME_MS);
}

button_state_t read_rotary_button(void) {
    replay_encoder_interrupts();

    uint32_t now_ms = esp_timer_get_time() / 1000;
    button_state_t raw = gpio_get_level(rotary_encoder.config.key_pin) ? RELEASED : PRESSED;
    return button_debouncer_update(&key_button, raw, now_ms);
}

int16_t read_rotary_step() {
    replay_encoder_interrupts();

    int16_t steps = rotary_encoder.step_count;
    rotary_encoder.step_count = 0;
    return steps;
}