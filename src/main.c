#include "esp_log.h"
#include "freertos/FreeRTOS.h"

void app_main() {
    while (1) {
        ESP_LOGI("task_app_main", "Hello world");
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}