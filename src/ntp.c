#include "esp_sntp.h"
#include "esp_log.h"
#include "ntp/requester.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <time.h>

#define NTP_TASK_TAG "SNTP"

#define SYNC_OK_INTERVAL_MS     (60 * 60 * 1000)   // 1 hour
#define SYNC_FAIL_INTERVAL_MS   (1  * 60 * 1000)   // 1 minute

static time_t last_ntp_sync = 0;

void time_sync_cb(struct timeval *tv)
{
    last_ntp_sync = tv->tv_sec;
    ESP_LOGI(NTP_TASK_TAG, "Switching to 1-hour sync interval");
    esp_sntp_set_sync_interval(SYNC_OK_INTERVAL_MS);
}

void ntp_monitor_task(void *arg)
{
    (void)arg;
    time_t now;
    struct tm tm;
    for (;;) {
        time(&now);
        localtime_r(&now, &tm);
        if ((now - last_ntp_sync) * 1000 > SYNC_OK_INTERVAL_MS || tm.tm_year < (2026 - 1900)) {
            ESP_LOGW(NTP_TASK_TAG, "Time not synchronized, retrying every 1 minute");
            esp_sntp_set_sync_interval(SYNC_FAIL_INTERVAL_MS);
        }
        vTaskDelay(pdMS_TO_TICKS(45 * 1000)); // check every 45 seconds
    }
}

void ntp_init(void) {
    ESP_LOGI(NTP_TASK_TAG, "Initializing SNTP");
    // Initialize SNTP
    esp_sntp_setoperatingmode(SNTP_OPMODE_POLL);
    esp_sntp_setservername(0, "pool.ntp.org");
    esp_sntp_setservername(1, "time.nist.gov");
    esp_sntp_setservername(2, "time.google.com");

    esp_sntp_set_sync_interval(SYNC_FAIL_INTERVAL_MS);

    // Function called on successful time sync
    esp_sntp_set_time_sync_notification_cb(time_sync_cb);

    esp_sntp_init();
}
