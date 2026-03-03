#include "net.h"
#include "mode.h"
#include "ntp.h"
#include "esp_log.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"

void app_main() {
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    wifi_init();

    xTaskCreate(sntp_sync_task, "ntp_monitor_task", 4096, NULL, 5, NULL);
    xTaskCreate(update_mode_task, "update_mode_task", 4096, NULL, 5, NULL);
}