#include "mode.h"
#include "rotary.h"
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
    ESP_LOGI(TAG, "Step: %i", read_rotary_button());
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

    while(1) {
        currentSystemMode = update_system_mode(currentSystemMode);
        e_ROTARY_STEP step = read_rotary_step();
        if (step != ROTARY_NONE) {
            ESP_LOGI(TAG, "Rotary step: %s", rotary_step_to_string(step));
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}