#include "buzzer.h"

#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define BUZZER_GPIO             35
#define BUZZER_LEDC_MODE        LEDC_LOW_SPEED_MODE
#define BUZZER_LEDC_TIMER       LEDC_TIMER_0
#define BUZZER_LEDC_CHANNEL     LEDC_CHANNEL_0
#define BUZZER_LEDC_DUTY_RES    LEDC_TIMER_10_BIT

//  10-bit, max 1023
#define BUZZER_DUTY             16

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
        .gpio_num       = BUZZER_GPIO,
        .speed_mode     = BUZZER_LEDC_MODE,
        .channel        = BUZZER_LEDC_CHANNEL,
        .timer_sel      = BUZZER_LEDC_TIMER,
        .duty           = 0,
        .hpoint         = 0,
        .intr_type      = LEDC_INTR_DISABLE,
    };
    ledc_channel_config(&channel_conf);
}

void buzzer_start(uint32_t frequency_hz) {
    buzzer_init();

    ledc_set_freq(BUZZER_LEDC_MODE, BUZZER_LEDC_TIMER, frequency_hz);
    ledc_set_duty(BUZZER_LEDC_MODE, BUZZER_LEDC_CHANNEL, BUZZER_DUTY);
    ledc_update_duty(BUZZER_LEDC_MODE, BUZZER_LEDC_CHANNEL);
}

void buzzer_stop(void) {

    ledc_set_duty(BUZZER_LEDC_MODE, BUZZER_LEDC_CHANNEL, 0);
    ledc_update_duty(BUZZER_LEDC_MODE, BUZZER_LEDC_CHANNEL);
}

void buzzer_play_tone(uint32_t frequency_hz, uint32_t duration_ms) {
    buzzer_start(frequency_hz);
    vTaskDelay(pdMS_TO_TICKS(duration_ms));
    buzzer_stop();
}
