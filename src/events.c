#include "events.h"

static EventGroupHandle_t s_events_group;

void events_init(void) {
    if (s_events_group == NULL) {
        s_events_group = xEventGroupCreate();
    }
}

EventGroupHandle_t events_get_group(void) {
    return s_events_group;
}

void events_set_beat_tick(void) {
    if (s_events_group != NULL) {
        xEventGroupSetBits(s_events_group, EVENT_BEAT_TICK);
    }
}

void events_set_boot_pressed(void) {
    if (s_events_group != NULL) {
        xEventGroupSetBits(s_events_group, EVENT_BOOT_PRESSED);
    }
}
