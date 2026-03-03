#include "net.h"
#include "motor/motor.h"
#include "mode.h"
#include "render/render.h"
#include "ntp.h"
#include "esp_log.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"

#define MOTOR_GEAR_RATIO 64
#define MOTOR_STEPS_REVOLUTION 32

void app_main() {
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    wifi_init();

    motor_init_driver(MOTOR_STEPS_REVOLUTION * MOTOR_GEAR_RATIO);

    xTaskCreate(sntp_sync_task, "ntp_monitor_task", 4096, NULL, 5, NULL);
    xTaskCreate(update_mode_task, "update_mode_task", 4096, NULL, 5, NULL);
    xTaskCreate(render_task, "render_task", 10000, NULL, 5, NULL);
}