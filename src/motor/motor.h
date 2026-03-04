#ifndef MOTOR_H
#define MOTOR_H

#include "ULN2003.h"

typedef struct {
    S_ULN2003 driver;
    float local_rotation;
    uint16_t steps_per_revolution;
} S_MotorDriver;

void motor_reset_local_rotation(void);

void motor_drive_task(void *arg);

void motor_move(float position);

void motor_move_relative(float degrees);

#endif
