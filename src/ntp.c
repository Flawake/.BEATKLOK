#include "esp_sntp.h"
#include "esp_log.h"
#include "ntp/requester.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <time.h>

#define NTP_TASK_TAG "SNTP"

#define SYNC_OK_INTERVAL_MS     (60 * 60 * 1000)   // 1 hour
#define SYNC_FAIL_INTERVAL_MS   (1  * 60 * 1000)   // 1 minute

void sntp_sync_task(void *arg)
{
    setservername_sntp(0, "pool.ntp.org");
    setservername_sntp(1, "time.nist.gov");
    setservername_sntp(2, "time.google.com");

    while (1) {
        if(sntp_request() != ESP_OK) {
            ESP_LOGW(NTP_TASK_TAG, "Time sync failed");
            vTaskDelay(pdMS_TO_TICKS(SYNC_FAIL_INTERVAL_MS));
        }
        else {
            time_t now;
            struct tm tm;

            time(&now);
            localtime_r(&now, &tm);

            ESP_LOGI(NTP_TASK_TAG,
                     "Time synced: %lld (%04d-%02d-%02d %02d:%02d:%02d)",
                     (long long)now,
                     tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
                     tm.tm_hour, tm.tm_min, tm.tm_sec);

            vTaskDelay(pdMS_TO_TICKS(SYNC_OK_INTERVAL_MS));
        }
    }
}