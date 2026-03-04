#include <time.h>
#include "ntp.h"
#include "esp_sntp.h"
#include "esp_log.h"
#include "ntp/requester.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "render/render.h"

#define NTP_TASK_TAG "SNTP"

#define SYNC_OK_INTERVAL_S     (60 * 60)   // 1 hour
#define SYNC_FAIL_INTERVAL_S   (1  * 60)   // 1 minute

uint32_t time_to_centibeads(uint32_t ms_since_midnight) {
    return (uint32_t)(
        ((uint64_t)ms_since_midnight * CENTIBEAD_IN_DAY) / MILLISECONDS_IN_DAY
    );
}

uint32_t get_centibeads_clock(void) {
    struct timeval now;
    struct tm bmt;

    gettimeofday(&now, NULL);

    /* BMT time is UTC+1 */
    now.tv_sec += 3600;

    gmtime_r(&now.tv_sec, &bmt);

    uint64_t ms_since_midnight =
        ((uint64_t)bmt.tm_hour * 3600000ULL) +
        ((uint64_t)bmt.tm_min  *   60000ULL) +
        ((uint64_t)bmt.tm_sec  *    1000ULL) +
        ((uint64_t)now.tv_usec / 1000ULL);

    return time_to_centibeads(ms_since_midnight);
}

void sntp_sync_task(void *arg)
{
    setservername_sntp(0, "pool.ntp.org");
    setservername_sntp(1, "time.nist.gov");
    setservername_sntp(2, "time.google.com");

    time_t last_sync_time;
    uint32_t time_till_new_sync = 0;
    
    while (1) {
        time_t now;
        time(&now);
        char text[20];

        long cur_time = get_centibeads_clock();
        int text_len = snprintf(text, sizeof(text), "Time: %li.%02li\n", cur_time / 100, labs(cur_time % 100));

        render_text(text, (S_Vector2){10, 40}, text_len);
        if (now - last_sync_time > time_till_new_sync) {
            if(sntp_request() != ESP_OK) {
                ESP_LOGW(NTP_TASK_TAG, "Time sync failed");
                time_till_new_sync = SYNC_FAIL_INTERVAL_S + now;
            }
            else {
                time(&last_sync_time);
                time_till_new_sync = SYNC_OK_INTERVAL_S + now;
            }
        }
        vTaskDelay(pdMS_TO_TICKS(864)); // 864ms = 1 centibead
    }
}
