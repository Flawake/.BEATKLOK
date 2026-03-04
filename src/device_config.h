#ifndef DEVICE_CONFIG_H
#define DEVICE_CONFIG_H

#include "render/display.h"
#include "status_leds.h"
#include "motor/ULN2003.h"
#include "motor/motor.h"
#include "rotary.h"

// Display configuration
#define DISPLAY_SDA_PIN     8
#define DISPLAY_SCL_PIN     9
#define DISPLAY_RESET_PIN   -1
#define DISPLAY_WIDTH       128
#define DISPLAY_HEIGHT      64

// Status LEDs configuration
#define STATUS_LED_NET_PIN   36
#define STATUS_LED_TIME_PIN  37
#define STATUS_LED_MODE_PIN  38

// Stepper motor configuration
#define MOTOR_IN1_PIN  13
#define MOTOR_IN2_PIN  14
#define MOTOR_IN3_PIN  15
#define MOTOR_IN4_PIN  16

#define MOTOR_GEAR_RATIO        64
#define MOTOR_STEPS_REVOLUTION  32

// Rotary encoder configuration
#define ROTARY_KEY_PIN_NUM  33
#define ROTARY_S1_PIN_NUM   18
#define ROTARY_S2_PIN_NUM   5

// Boot button configuration
#define BOOT_BUTTON_PIN 0

// Buzzer configuration
#define BUZZER_GPIO_NUM          35
#define BUZZER_LEDC_MODE      LEDC_LOW_SPEED_MODE
#define BUZZER_LEDC_TIMER     LEDC_TIMER_0
#define BUZZER_LEDC_CHANNEL   LEDC_CHANNEL_0
#define BUZZER_LEDC_DUTY_RES  LEDC_TIMER_10_BIT

// 10-bit, max 1023
#define BUZZER_DUTY              16

S_MotorDriver *get_motor_config(void);

S_StatusLedConfig *get_status_led_config(void);

display_config_t *get_display_config(void);

S_RotaryEncoderConfig *get_rotary_config(void);

#endif
