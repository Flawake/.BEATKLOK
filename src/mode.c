#include "mode.h"
#include "rotary.h"
#include "sound/buzzer.h"
#include "render/render.h"
#include "motor/motor.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "debounce.h"
#include "events.h"

#define BOOT_BUTTON_PIN 0

e_SystemMode currentSystemMode = MANUAL;

e_SystemMode update_system_mode(e_SystemMode currentMode) {
    if (currentMode == AUTO) {
        return currentMode;
    }
    
    int level = read_rotary_button();

    if (level == 0) {
        return AUTO;
    }
    return MANUAL;
}

e_SystemMode get_system_mode(void) {
    return currentSystemMode;
}

void set_boot_pin(void) {
    gpio_config_t io_conf = {
        .pin_bit_mask = 1ULL << BOOT_BUTTON_PIN,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .intr_type = GPIO_INTR_ANYEDGE,
    };
    gpio_config(&io_conf);
}

void update_mode_task(void *arg) {
    (void)arg;

    init_rotary_encoder();

    set_boot_pin();

    button_debouncer_t boot_debouncer;
    button_debouncer_init(&boot_debouncer, RELEASED, DEFAULT_DEBOUNCE_TIME_MS);

    while(1) {
        currentSystemMode = update_system_mode(currentSystemMode);

        if (currentSystemMode == MANUAL) {
            int16_t step_count = read_rotary_step();
            motor_move_relative(step_count * 4);
            motor_reset_local_rotation();
        }

        uint32_t time_now = esp_timer_get_time() / 1000;
        if (button_debouncer_update(&boot_debouncer, gpio_get_level(BOOT_BUTTON_PIN), time_now) == RELEASED && button_edge_detected(&boot_debouncer)) {
            events_set_boot_pressed();
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}