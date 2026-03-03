#ifndef RENDER_H
#define RENDER_H

#include <stdint.h>

typedef struct {
    int x;
    int y;
} S_Vector2;

typedef struct {
    int width;
    int heigth;
    const uint8_t *data;
} S_Bitmap;

void render_bitmap(const S_Bitmap *bitmap, S_Vector2 position);

void render_text(const char *text, S_Vector2 position, int text_len);

void render_task(void *pvParameters);

#endif