#ifndef BUZZER_H
#define BUZZER_H

#include <stdint.h>
#include <stdbool.h>

void buzzer_init(void);
void buzzer_play_tone(uint32_t frequency_hz, uint32_t duration_ms);

#endif
