#ifndef EVENTS_H
#define EVENTS_H

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

#define EVENT_BEAT_TICK BIT0

void events_init(void);
EventGroupHandle_t events_get_group(void);
void events_set_beat_tick(void);

#endif

