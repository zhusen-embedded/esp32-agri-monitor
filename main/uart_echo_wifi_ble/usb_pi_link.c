#include "usb_pi_link.h"

#include <string.h>
#include <ctype.h>
#include <stdio.h>

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/usb_serial_jtag.h"

static const char *TAG = "USB_PI_LINK";

#define USB_PI_RX_TASK_STACK 3072
#define USB_PI_RX_TASK_PRIO   (tskIDLE_PRIORITY + 4)
#define USB_PI_LINE_MAX       256

static char s_latest_line[USB_PI_LINE_MAX];
static bool s_has_latest = false;
static bool s_task_started = false;

static void trim_line(char *line)
{
    if (line == NULL) {
        return;
    }

    char *start = line;
    while (*start != '\0' && isspace((unsigned char)*start)) {
        start++;
    }

    char *end = start + strlen(start);
    while (end > start && isspace((unsigned char)*(end - 1))) {
        end--;
    }
    *end = '\0';

    if (start != line) {
        memmove(line, start, (size_t)(end - start) + 1);
    }
}

static void store_result(const char *line)
{
    if (line == NULL || line[0] == '\0') {
        return;
    }

    strncpy(s_latest_line, line, sizeof(s_latest_line) - 1);
    s_latest_line[sizeof(s_latest_line) - 1] = '\0';
    s_has_latest = true;
    ESP_LOGI(TAG, "PI inference result: %s", s_latest_line);
}

static void usb_pi_rx_task(void *arg)
{
    (void)arg;

    if (!usb_serial_jtag_is_driver_installed()) {
        usb_serial_jtag_driver_config_t cfg = USB_SERIAL_JTAG_DRIVER_CONFIG_DEFAULT();
        esp_err_t err = usb_serial_jtag_driver_install(&cfg);
        if (err != ESP_OK) {
            ESP_LOGE(TAG, "usb_serial_jtag_driver_install failed: %s", esp_err_to_name(err));
            vTaskDelete(NULL);
            return;
        }
    }

    ESP_LOGI(TAG, "USB Serial/JTAG receiver started");

    char line[USB_PI_LINE_MAX];
    size_t line_len = 0;
    int json_depth = 0;
    bool in_json = false;
    uint8_t buf[64];

    while (true) {
        int read_len = usb_serial_jtag_read_bytes(buf, sizeof(buf), pdMS_TO_TICKS(100));
        if (read_len <= 0) {
            continue;
        }

        for (int i = 0; i < read_len; ++i) {
            char ch = (char)buf[i];

            // 兼容粘包/无换行场景：按 JSON 对象边界分包
            if (!in_json) {
                if (ch == '{') {
                    in_json = true;
                    json_depth = 1;
                    line_len = 0;
                    line[line_len++] = ch;
                }
                continue;
            }

            if (line_len < sizeof(line) - 1) {
                line[line_len++] = ch;
            } else {
                // 当前对象过长，丢弃本对象并重新等待下一个 '{'
                in_json = false;
                json_depth = 0;
                line_len = 0;
                continue;
            }

            if (ch == '{') {
                json_depth++;
            } else if (ch == '}') {
                json_depth--;
                if (json_depth <= 0) {
                    line[line_len] = '\0';
                    trim_line(line);
                    store_result(line);
                    in_json = false;
                    json_depth = 0;
                    line_len = 0;
                }
            }
        }
    }
}

esp_err_t usb_pi_link_start(void)
{
    if (s_task_started) {
        return ESP_OK;
    }

    BaseType_t ret = xTaskCreate(usb_pi_rx_task,
                                 "usb_pi_rx_task",
                                 USB_PI_RX_TASK_STACK,
                                 NULL,
                                 USB_PI_RX_TASK_PRIO,
                                 NULL);
    if (ret != pdPASS) {
        ESP_LOGE(TAG, "Failed to create USB PI RX task");
        return ESP_FAIL;
    }

    s_task_started = true;
    return ESP_OK;
}

bool usb_pi_link_get_latest(char *out, size_t out_len)
{
    if (out == NULL || out_len == 0) {
        return false;
    }

    if (!s_has_latest) {
        out[0] = '\0';
        return false;
    }

    strncpy(out, s_latest_line, out_len - 1);
    out[out_len - 1] = '\0';
    return true;
}
