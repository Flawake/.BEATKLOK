#ifndef MOTOR_H
#define MOTOR_H

typedef enum {
    CLOCKWISE = -1,
    COUNTER_CLOCKWISE = 1,
} E_MotorDir;

void motor_reset_local_rotation(void);

void motor_drive_task(void *arg);

void motor_move(float position);

void motor_move_relative(float degrees);

#endif
