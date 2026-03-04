#include <math.h>
#include <stdint.h>
#include "ULN2003.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "mode.h"
#include "ntp.h"

typedef struct
{
    S_ULN2003 driver;
    float local_rotation;
    uint16_t steps_per_revolution;
} S_MotorDriver;

S_MotorDriver driver = {
    .driver = {
        .config = {
            .IN1 = 13,
            .IN2 = 14,
            .IN3 = 15,
            .IN4 = 16,
        },
        .step_phase = 0,
    },
    .local_rotation = 0,
};

void motor_reset_local_rotation(void) {
    driver.local_rotation = 0;
}

static float motor_normalize_deg(float angle)
{
    angle = fmodf(angle, 360.0f);
    if (angle < 0.0f)
        angle += 360.0f;
    return angle;
}

void motor_move_to(float position) {
    float delta = position - driver.local_rotation;
    E_MotorDir dir;

    if (delta > 0.0f) {
        dir = COUNTER_CLOCKWISE;
    } else {
        dir = CLOCKWISE;
    }

    uint32_t steps = (uint32_t)((fabsf(delta) / 360.0f) * driver.steps_per_revolution);

    uln2003_make_steps(&driver.driver, dir, steps);

    /* Store the actual moved angle, not the angle we wanted to move. That will accumalate errors */
    driver.local_rotation += ((float)steps / driver.steps_per_revolution) * 360.0f;
    driver.local_rotation = motor_normalize_deg(driver.local_rotation);
}

void motor_move_relative(float degrees) {
    motor_move_to(driver.local_rotation + degrees);
}

float map_to_degrees(uint32_t centibead) {
    return ((float)centibead * 359) / (float)CENTIBEAD_IN_DAY;
}

void motor_drive_task(void *arg) {
    uint16_t steps_per_revolution = (uint16_t)(uintptr_t)arg;
    driver.steps_per_revolution = steps_per_revolution;

    gpio_config_t io_conf = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask =
            (1ULL << driver.driver.config.IN1) |
            (1ULL << driver.driver.config.IN2) |
            (1ULL << driver.driver.config.IN3) |
            (1ULL << driver.driver.config.IN4),
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en   = GPIO_PULLUP_DISABLE,
        .intr_type    = GPIO_INTR_DISABLE
    };

    gpio_config(&io_conf);

    while (1)
    {
        if (get_system_mode() == AUTO) {
            uint32_t current_centibead = get_centibeads_clock();
            float angle = map_to_degrees(current_centibead);
            motor_move_to(angle);
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    
}
