#ifndef NTP_H
#define NTP_H

void sntp_sync_task(void *arg);

uint32_t time_to_centibeads(void);

#endif