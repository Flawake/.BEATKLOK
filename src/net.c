#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "render/bitmaps.h"
#include "render/render.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_eap_client.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_netif.h"
#include "esp_smartconfig.h"
#include "esp_mac.h"

#define STRINGIFY_X(x) #x
#define STRINGIFY(x) STRINGIFY_X(x)

#define WIFI_CONNECTED_BIT BIT0

typedef enum {
    NET_STATUS_CONNECTING = 0,
    NET_STATUS_CONNECTED,
    NET_STATUS_NOT_CONNECTED,
    NET_STATUS_UNAVAILABLE,
    NET_STATUS_ERROR
} net_status_t;

static EventGroupHandle_t s_wifi_event_group;
static volatile net_status_t s_net_status = NET_STATUS_CONNECTING;

static const char *TAG = "net";

static const char *net_status_to_text(net_status_t status)
{
    switch (status) {
        case NET_STATUS_CONNECTED:
            return "verbonden";
        case NET_STATUS_NOT_CONNECTED:
            return "niet verbonden";
        case NET_STATUS_UNAVAILABLE:
            return "onbeschikbaar";
        case NET_STATUS_CONNECTING:
            return "verbinden";
        case NET_STATUS_ERROR:
        default:
            return "error";
    }
}

static void event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        s_net_status = NET_STATUS_CONNECTING;
        esp_wifi_connect();
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        wifi_event_sta_disconnected_t *disconnected =
            (wifi_event_sta_disconnected_t *)event_data;
        ESP_LOGI(TAG, "ID: %i", disconnected->reason);

        switch (disconnected->reason) {
            case WIFI_REASON_NO_AP_FOUND:
                s_net_status = NET_STATUS_UNAVAILABLE;
                break;
            case WIFI_REASON_CONNECTION_FAIL:
            case WIFI_REASON_4WAY_HANDSHAKE_TIMEOUT:
                s_net_status = NET_STATUS_NOT_CONNECTED;
                break;
            default:
                s_net_status = NET_STATUS_ERROR;
                break;
        }

        render_bitmap(&disconnected_bitmap, (S_Vector2){10, 10});
        esp_wifi_connect();
        xEventGroupClearBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
        ESP_LOGI(TAG, "Retrying WiFi connection...");
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *) event_data;
        ESP_LOGI(TAG, "Got IP: " IPSTR, IP2STR(&event->ip_info.ip));
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
        s_net_status = NET_STATUS_CONNECTED;
    }
}

static int8_t wifi_get_rssi(void) {
    wifi_ap_record_t ap_info;
    esp_err_t result = esp_wifi_sta_get_ap_info(&ap_info);

    if (result == ESP_OK) {
        return ap_info.rssi;
    }

    return 0;
}

bool wifi_is_connected(void) {
    return (xEventGroupGetBits(s_wifi_event_group) & WIFI_CONNECTED_BIT) != 0;
}

void wifi_signal_monitor_task(void *pvParameters) {
    (void)pvParameters;

    while (1) {
        if (!wifi_is_connected()) {
            render_bitmap(&disconnected_bitmap, (S_Vector2){10, 10});
        } else {
            int8_t rssi = wifi_get_rssi();

            const S_Bitmap *icon = &wifi_bad_bitmap;

            if (rssi >= -55) {
                icon = &wifi_excellent_bitmap;
            } else if (rssi >= -60) {
                icon = &wifi_good_bitmap;
            } else if (rssi >= -68) {
                icon = &wifi_ok_bitmap;
            } else if (rssi >= -75) {
                icon = &wifi_bad_bitmap;
            }

            render_bitmap(icon, (S_Vector2){10, 10});
        }

        const char *status_str = net_status_to_text(s_net_status);
        char text[24];
        int text_len = snprintf(text, sizeof(text), "%s", status_str);
        if (text_len > 0) {
            render_text(text, (S_Vector2){10, 48}, text_len);
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void wifi_init(void) {
    ESP_ERROR_CHECK(esp_netif_init());
    s_wifi_event_group = xEventGroupCreate();
    esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
    assert(sta_netif);

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );

    ESP_ERROR_CHECK( esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL) );
    ESP_ERROR_CHECK( esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL) );

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = STRINGIFY(WIFI_SSID),
            .password = STRINGIFY(WIFI_PASS),
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
        },
    };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
}
