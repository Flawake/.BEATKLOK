#ifndef RENDER_H
#define RENDER_H

#include <stdint.h>

typedef struct {
    int x;
    int y;
} Vector2;

typedef struct {
    int width;
    int heigth;
    const uint8_t *data;
} Bitmap;

void render_bitmap(const Bitmap *bitmap, Vector2 position);

void render_text(const char *text, Vector2 position, int text_len);

void render_task(void *pvParameters);

#endif