#ifndef NTP_H
#define NTP_H

#include <time.h>

#define MILLISECONDS_IN_DAY (86400 * 1000)
#define CENTIBEAD_IN_DAY (1000 * 100)

void sntp_sync_task(void *arg);

uint32_t get_centibeads_clock(void);

#endif