#include "net.h"
#include "ntp.h"
#include "esp_log.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"

void app_main() {
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    wifi_init();
    ntp_init();

    xTaskCreate(ntp_monitor_task, "ntp_monitor_task", 4096, NULL, 5, NULL);
}