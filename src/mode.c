#include "mode.h"
#include "rotary.h"
#include "sound/buzzer.h"
#include "render/render.h"
#include "motor/motor.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "MODE";

typedef enum {
    AUTO,
    MANUAL,
} e_SystemMode;

e_SystemMode currentSystemMode = AUTO;

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

void update_mode_task(void *arg) {
    (void)arg;

    init_rotary_encoder();

    static char rotary_text[5];

    while(1) {
        currentSystemMode = update_system_mode(currentSystemMode);

        int16_t step_count = read_rotary_step();
        if (step_count != 0) {
            ESP_LOGI(TAG, "Rotary step: %i", step_count);

            uint8_t len = snprintf(rotary_text, sizeof(rotary_text), "%i", step_count);

            uint16_t text_width = len * 8;
            S_Vector2 pos = {
                .x = (128 - text_width) / 2,
                .y = (64 - 8) / 2,
            };

            render_text(rotary_text, pos, len);
            
            buzzer_play_tone(1000, 100);
            motor_move_relative(90);
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}