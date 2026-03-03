#include "mode.h"
#include "rotary.h"
#include "sound/buzzer.h"
#include "render/render.h"
#include "motor/motor.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

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

void update_mode_task(void *arg) {
    (void)arg;

    init_rotary_encoder();

    while(1) {
        currentSystemMode = update_system_mode(currentSystemMode);

        if (currentSystemMode == MANUAL) {
            int16_t step_count = read_rotary_step();
            motor_move_relative(step_count * 4);
            motor_reset_local_rotation();
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}