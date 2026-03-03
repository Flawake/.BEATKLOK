#ifndef DISPLAY_H
#define DISPLAY_H

#include "ssd1306.h"
#include "render.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    int sda_pin;
    int scl_pin;
    int reset_pin;
    int width;
    int height;
} display_config_t;

typedef struct {
    SSD1306_t device;
    display_config_t config;
} display_handle_t;

display_handle_t display_create_default(void);

bool display_init(display_handle_t *handle);

void display_clear(display_handle_t *handle);

void display_clear_rectangle(display_handle_t *handle, int mix_x, int max_x, int min_y, int max_y);

void display_show_text(display_handle_t *handle, int x_pos, int y_pos,
                       const char *text, int text_len);

void display_show_text_centered(display_handle_t *handle, int page,
                                const char *text, int text_len);

void display_show_bitmap(display_handle_t *handle, Bitmap bitmap, int x_pos, int y_pos);

#endif // DISPLAY_H
