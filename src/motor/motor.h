#ifndef MOTOR_H
#define MOTOR_H

typedef enum {
    CLOCKWISE = -1,
    COUNTER_CLOCKWISE = 1,
} E_MotorDir;

void motor_reset_local_rotation(void);

void motor_init_driver(int steps_per_revolution);

void motor_move(float position);

void motor_move_relative(float degrees);

#endif
