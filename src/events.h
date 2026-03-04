#ifndef EVENTS_H
#define EVENTS_H

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

#define EVENT_BEAT_TICK BIT0
#define EVENT_BOOT_PRESSED BIT1

void events_init(void);
EventGroupHandle_t events_get_group(void);
void events_set_beat_tick(void);
void events_set_boot_pressed(void);

#endif

