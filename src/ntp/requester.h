#ifndef NTP_REQUESTER_H
#define NTP_REQUESTER_H

#include <stdint.h>
#include "esp_err.h"

void setservername_sntp(uint8_t index, const char *hostname);

esp_err_t sntp_request(void);

#endif