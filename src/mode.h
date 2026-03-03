#ifndef MODE_H
#define MODE_H

typedef enum {
    MANUAL,
    AUTO,
} e_SystemMode;

e_SystemMode get_system_mode(void);

void update_mode_task(void *arg);

#endif