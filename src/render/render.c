#include "render.h"
#include "display.h"
#include "esp_log.h"
#include "events.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <freertos/queue.h>
#include <freertos/projdefs.h>

#define RENDERER_TASK_TAG "RENDERER"
#define RENDER_QUEUE_SIZE 10

typedef enum {
    RENDER_TEXT,
    RENDER_BITMAP
} e_CommandType;

typedef struct {
    e_CommandType commandType;
    S_Vector2 drawPosition;
    union {
        const S_Bitmap *bitmap;
        const char *text;
    } data;
    int text_len; // Used only for text commands
} S_Command;

static QueueHandle_t render_queue;
static bool render_enabled = true;

bool render_is_enabled(void) {
    return render_enabled;
}

void render_set_enabled(bool enabled) {
    render_enabled = enabled;
}

void render_bitmap(const S_Bitmap *bitmap, S_Vector2 position) {
    S_Command command = {
        .commandType = RENDER_BITMAP,
        .drawPosition = position,
        .data.bitmap = bitmap,
        .text_len = 0,
    };
    xQueueSendToBack(render_queue, &command, 0);
}

void render_text(const char *text, S_Vector2 position, int text_len) {
    S_Command command = {
        .commandType = RENDER_TEXT,
        .drawPosition = position,
        .data.text = text,
        .text_len = text_len,
    };
    xQueueSendToBack(render_queue, &command, 0);
}

#define DISPLAY_TARGET_FPS 10

void render_task(void *pvParameters) {
    (void)pvParameters; // Cast to void to supress unused variable warning

    render_queue = xQueueCreate(RENDER_QUEUE_SIZE, sizeof(S_Command));
    if (render_queue == NULL) {
        ESP_LOGW(RENDERER_TASK_TAG, "Could not create render_queue, stopping system execution");
        abort();
    }

    display_handle_t display = display_create_default();
    display_init(&display);

    bool enabled = true;

    while (1) {
        bool boot_button_pressed = (xEventGroupGetBits(events_get_group()) & EVENT_BOOT_PRESSED) != 0;

        if (boot_button_pressed) {
            xEventGroupClearBits(events_get_group(), EVENT_BOOT_PRESSED);
            enabled = !enabled;
            display_clear(&display);
        }

        if (!enabled) {
            vTaskDelay(pdMS_TO_TICKS(1000));
            continue;
        }
        S_Command sample;
        if (xQueueReceive(render_queue, &sample, portMAX_DELAY) == pdTRUE) {
            switch (sample.commandType) {
                case RENDER_BITMAP: {
                    const S_Bitmap *bitmap = sample.data.bitmap;
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
            }
        }
        vTaskDelay(pdMS_TO_TICKS(1000 / DISPLAY_TARGET_FPS));
    }
}
