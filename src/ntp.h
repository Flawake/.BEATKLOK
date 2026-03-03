#ifndef NTP_H
#define NTP_H

#include <time.h>

void sntp_sync_task(void *arg);

uint32_t time_to_centibeads(struct tm *instant);

uint32_t get_centibeads_clock(void);

#endif