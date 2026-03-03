#include "render.h"
#include "display.h"
#include "freertos/FreeRTOS.h"
#include <freertos/queue.h>
#include <freertos/projdefs.h>

#define RENDER_QUEUE_SIZE 10

typedef enum {
    RENDER_TEXT,
    RENDER_BITMAP
} e_CommandType;

typedef struct {
    e_CommandType commandType;
    Vector2 drawPosition;
    union {
        const Bitmap *bitmap;
        const char *text;
    } data;
    int text_len; // Used only for text commands
} Command;

static QueueHandle_t render_queue;

void render_bitmap(const Bitmap *bitmap, Vector2 position) {
    Command command = {
        .commandType = RENDER_BITMAP,
        .drawPosition = position,
        .data.bitmap = bitmap,
        .text_len = 0,
    };
    xQueueSendToBack(render_queue, &command, 0);
}

void render_text(const char *text, Vector2 position, int text_len) {
    Command command = {
        .commandType = RENDER_TEXT,
        .drawPosition = position,
        .data.text = text,
        .text_len = text_len,
    };
    xQueueSendToBack(render_queue, &command, 0);
}

#define DISPLAY_FPS 10

void render_task(void *pvParameters) {
    (void)pvParameters;

    display_handle_t display = display_create_default();
    display_init(&display);

    render_queue = xQueueCreate(RENDER_QUEUE_SIZE, sizeof(Command));

    while (1) {
        Command sample;
        if (xQueueReceive(render_queue, &sample, portMAX_DELAY) == pdTRUE) {
            switch (sample.commandType) {
                case RENDER_BITMAP: {
                    const Bitmap *bitmap = sample.data.bitmap;
                    if (bitmap != NULL) {
                        display_show_bitmap(&display,
                                            *bitmap,
                                            sample.drawPosition.x,
                                            sample.drawPosition.y);
                    }
                    break;
                }
                case RENDER_TEXT: {
                    if (sample.data.text != NULL && sample.text_len > 0) {
                        display_show_text(&display,
                                          sample.drawPosition.x,
                                          sample.drawPosition.y,
                                          sample.data.text,
                                          sample.text_len);
                    }
                    break;
                }
                default:
                    break;
            }
        }
        vTaskDelay(pdMS_TO_TICKS(1000 / DISPLAY_FPS));
    }
}
