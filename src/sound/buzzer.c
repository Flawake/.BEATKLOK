#include "buzzer.h"

#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "events.h"
#include "device_config.h"

void buzzer_init(void) {

    ledc_timer_config_t timer_conf = {
        .speed_mode       = BUZZER_LEDC_MODE,
        .duty_resolution  = BUZZER_LEDC_DUTY_RES,
        .timer_num        = BUZZER_LEDC_TIMER,
        .freq_hz          = 2000,
        .clk_cfg          = LEDC_AUTO_CLK,
    };
    ledc_timer_config(&timer_conf);

    ledc_channel_config_t channel_conf = {
        .gpio_num       = BUZZER_GPIO_NUM,
        .speed_mode     = BUZZER_LEDC_MODE,
        .channel        = BUZZER_LEDC_CHANNEL,
        .timer_sel      = BUZZER_LEDC_TIMER,
        .duty           = 0,
        .hpoint         = 0,
        .intr_type      = LEDC_INTR_DISABLE,
    };
    ledc_channel_config(&channel_conf);
}

static void buzzer_start(uint32_t frequency_hz) {
    ledc_set_freq(BUZZER_LEDC_MODE, BUZZER_LEDC_TIMER, frequency_hz);
    ledc_set_duty(BUZZER_LEDC_MODE, BUZZER_LEDC_CHANNEL, BUZZER_DUTY);
    ledc_update_duty(BUZZER_LEDC_MODE, BUZZER_LEDC_CHANNEL);
}

static void buzzer_stop(void) {

    ledc_set_duty(BUZZER_LEDC_MODE, BUZZER_LEDC_CHANNEL, 0);
    ledc_update_duty(BUZZER_LEDC_MODE, BUZZER_LEDC_CHANNEL);
}

static void buzzer_play_tone_blocking(uint32_t frequency_hz, uint32_t duration_ms) {
    buzzer_start(frequency_hz);
    vTaskDelay(pdMS_TO_TICKS(duration_ms));
    buzzer_stop();
}

void buzzer_task(void *arg) {
    (void)arg; // Cast to void to supress unused variable warning

    buzzer_init();

    EventGroupHandle_t events_group = events_get_group();
    
    while (1) {
        xEventGroupWaitBits(
            events_group,
            EVENT_BEAT_TICK,
            pdTRUE,
            pdFALSE,
            portMAX_DELAY
        );

        buzzer_play_tone_blocking(1000, 50);
    }
}
