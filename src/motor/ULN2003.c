#include "esp_rom_sys.h"
#include "ULN2003.h"

#define MASK_CLAMP_4 0x03

const uint8_t step_table[4] = {
    0b0001,
    0b0010,
    0b0100,
    0b1000
};

void set_motor_pins(S_ULN2003Config *config, uint8_t pin_mask) {
    gpio_set_level(config->IN1, (pin_mask >> 0) & 0x01);
    gpio_set_level(config->IN2, (pin_mask >> 1) & 0x01);
    gpio_set_level(config->IN3, (pin_mask >> 2) & 0x01);
    gpio_set_level(config->IN4, (pin_mask >> 3) & 0x01);
}

void uln2003_step(S_ULN2003 *driver, E_MotorDir dir) {
    set_motor_pins(&driver->config, step_table[driver->step_phase]);
    driver->step_phase = (driver->step_phase + dir) & MASK_CLAMP_4;
}

void uln2003_make_steps(S_ULN2003 *driver, E_MotorDir dir, uint16_t steps) {
    for (int i = 0; i < steps; i++) {
        uln2003_step(driver, dir);
        esp_rom_delay_us(2500);
    }
}
