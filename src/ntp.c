#include "ntp.h"
#include "esp_sntp.h"
#include "esp_log.h"
#include "ntp/requester.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "render/render.h"
#include <time.h>

#define NTP_TASK_TAG "SNTP"

#define SYNC_OK_INTERVAL_MS     (60 * 60 * 1000)   // 1 hour
#define SYNC_FAIL_INTERVAL_MS   (1  * 60 * 1000)   // 1 minute

uint32_t time_to_centibeads(struct tm *instant) {
    uint64_t seconds = (instant->tm_hour * 3600) + (instant->tm_min  * 60) + instant->tm_sec;

    return (uint32_t)((seconds * CENTIBEAD_IN_DAY) / SECONDS_IN_DAY);
}

uint32_t get_centibeads_clock(void) {
    time_t now;
    
    time(&now);

    /* BMT time is UTC+1 */
    now += 3600;

    struct tm bmt;
    gmtime_r(&now, &bmt);

    return time_to_centibeads(&bmt);
}

void sntp_sync_task(void *arg)
{
    setservername_sntp(0, "pool.ntp.org");
    setservername_sntp(1, "time.nist.gov");
    setservername_sntp(2, "time.google.com");

    time_t last_sync_time;
    
    while (1) {
        time_t now;
        time(&now);
        char text[20];
        int text_len = snprintf(text, sizeof(text), "Time: %li\n", get_centibeads_clock());

        render_text(text, (S_Vector2){10, 40}, text_len);
        if (now - last_sync_time < 3600) {
            vTaskDelay(pdMS_TO_TICKS(864)); // 864ms = 1 centibead;
        }

        if(sntp_request() != ESP_OK) {
            ESP_LOGW(NTP_TASK_TAG, "Time sync failed");
        }
        else {
            time(&last_sync_time);
        }
        vTaskDelay(pdMS_TO_TICKS(864)); // 864ms = 1 centibead
    }
}
