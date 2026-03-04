#include <string.h>
#include <stdio.h>
#include "display.h"
#include "net.h"
#include "esp_log.h"
#include "device_config.h"

#define TAG "Display"

#define DISPLAY_LINE_TEXT_LEN 24

display_handle_t create_display_handle(display_config_t config) {
    return (display_handle_t){
        .config = config
    };
}

void display_clear(display_handle_t *handle) {
    ssd1306_clear_screen(&handle->device, false);
}

bool display_init(display_handle_t *handle) {
    i2c_master_init(&handle->device, 
                     handle->config.sda_pin, 
                     handle->config.scl_pin, 
                     handle->config.reset_pin);

    ssd1306_init(&handle->device, 
                 handle->config.width, 
                 handle->config.height);

    display_clear(handle);

    ESP_LOGI(TAG, "Display initialized");
    return true;
}

void display_clear_rectangle(display_handle_t *handle, uint8_t mix_x, uint8_t max_x, uint8_t min_y, uint8_t max_y) {
    if (max_x > handle->config.width) max_x = handle->config.width;
    if (max_y > handle->config.height) max_y = handle->config.height;

    for (uint8_t y = min_y; y < max_y; y++) {
        for (uint8_t x = mix_x; x < max_x; x++) {
            _ssd1306_pixel(&handle->device, x, y, true);
        }
    }
    ssd1306_show_buffer(&handle->device);
}

void display_show_text(display_handle_t *handle, int x_pos, int y_pos,
                       const char *text, int text_len) {

    int text_height = 8;

    display_clear_rectangle(handle,
                            x_pos,
                            handle->config.width,
                            y_pos,
                            y_pos + text_height);

    // Convert to page/segment for the SSD1306 API
    int page = y_pos / 8;
    int seg = x_pos;

    ssd1306_display_text_box1(&handle->device, page, seg,
                              text, text_len, text_len, false, 0);
}

void display_show_text_centered(display_handle_t *handle, int page,
                                const char *text, int text_len) {
    int cursor_pos_horizontal = (handle->config.width - text_len * 8) / 2;
    ssd1306_display_text_box1(&handle->device, page, cursor_pos_horizontal, text, text_len, text_len, false, 0);
}

void display_show_bitmap(display_handle_t *handle, S_Bitmap bitmap, int x_pos, int y_pos) {
    ssd1306_bitmaps(&handle->device, x_pos, y_pos, bitmap.data, bitmap.width, bitmap.heigth, false);
}
