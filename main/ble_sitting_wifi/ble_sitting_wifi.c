#include "ble_sitting_wifi.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "esp_event.h"
#include "esp_log.h"
#include "esp_mac.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "freertos/event_groups.h"
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "host/ble_hs.h"
#include "host/ble_hs_id.h"
#include "host/ble_gap.h"
#include "host/ble_uuid.h"
#include "host/ble_hs_adv.h"
#include "host/ble_store.h"
#include "os/os_mbuf.h"
#include "services/gap/ble_svc_gap.h"
#include "services/gatt/ble_svc_gatt.h"

static const char *TAG = "BLE_CUSTOM_WIFI";

enum {
    OP_SCAN_START = 0x01,
    OP_SET_CONFIG = 0x02,
    OP_GET_INFO = 0x03,

    OP_SCAN_RESULT = 0x81,
    OP_SCAN_DONE = 0x82,
    OP_CONFIG_ACK = 0x83,
    OP_CONFIG_RESULT = 0x84,
    OP_INFO = 0x85,
    OP_ERROR = 0xFF,
};

enum {
    STATUS_OK = 0,
    STATUS_AUTH_FAIL = 1,
    STATUS_NO_AP = 2,
    STATUS_CONNECT_FAIL = 3,
    STATUS_INVALID_ARG = 4,
    STATUS_INTERNAL = 5,
};

static bool s_ble_inited = false;
static bool s_ble_synced = false;
static bool s_adv_active = false;
static bool s_notify_enabled = false;
static bool s_wifi_event_handlers_registered = false;
static bool s_config_in_progress = false;
static bool s_boot_connect_in_progress = false;
static uint8_t s_config_seq = 0;
static uint8_t s_scan_seq = 0;
static uint8_t s_mac[6] = {0};
static uint8_t s_own_addr_type = BLE_OWN_ADDR_PUBLIC;
static uint16_t s_conn_handle = BLE_HS_CONN_HANDLE_NONE;
static uint16_t s_tx_handle = 0;
static EventGroupHandle_t s_boot_connect_event_group = NULL;

#define HOTSPOT_SSID_PREFIX   "ESP32_PI_"
#define HOTSPOT_PASSWORD      "12345678"

#define BOOT_WIFI_CONNECTED_BIT BIT0
#define BOOT_WIFI_FAIL_BIT      BIT1

static ble_uuid128_t s_svc_uuid;
static ble_uuid128_t s_rx_uuid;
static ble_uuid128_t s_tx_uuid;
static ble_uuid128_t s_info_uuid;

static esp_err_t ensure_base_inited(void);
static void ble_host_task(void *param);
static void ble_start_advertising(void);
static void notify_packet(uint8_t op, uint8_t seq, const uint8_t *payload, uint16_t len);
static void handle_rx_command(const uint8_t *data, uint16_t len);
static void boot_wifi_event_init(void);
static esp_err_t ensure_hotspot_started(void);
void ble_store_config_init(void);

static void ble_on_reset(int reason)
{
    ESP_LOGW(TAG, "nimble reset, reason=%d", reason);
}

static void build_uuid_with_mac(ble_uuid128_t *out, const uint8_t base[16])
{
    out->u.type = BLE_UUID_TYPE_128;
    memcpy(out->value, base, 16);
    out->value[10] = s_mac[0];
    out->value[11] = s_mac[1];
    out->value[12] = s_mac[2];
    out->value[13] = s_mac[3];
    out->value[14] = s_mac[4];
    out->value[15] = s_mac[5];
}

static void log_uuid(const char *label, const ble_uuid_t *uuid)
{
    char buf[BLE_UUID_STR_LEN] = {0};
    ble_uuid_to_str(uuid, buf);
    ESP_LOGI(TAG, "%s: %s", label, buf);
}

static int gatt_svr_chr_access(uint16_t conn_handle, uint16_t attr_handle,
                               struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    if (ctxt->op == BLE_GATT_ACCESS_OP_READ_CHR) {
        if (ble_uuid_cmp(ctxt->chr->uuid, &s_info_uuid.u) == 0) {
            uint8_t info[64] = {0};
            uint8_t name_len = 0;
            const char *name = ble_svc_gap_device_name();
            if (name) {
                name_len = (uint8_t)strnlen(name, 20);
            }
            info[0] = 1; /* version */
            info[1] = name_len;
            if (name_len > 0) {
                memcpy(&info[2], name, name_len);
            }
            memcpy(&info[2 + name_len], s_mac, sizeof(s_mac));
            memcpy(&info[2 + name_len + sizeof(s_mac)], s_svc_uuid.value, 16);
            uint16_t total = 2 + name_len + sizeof(s_mac) + 16;
            return os_mbuf_append(ctxt->om, info, total) == 0 ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;
        }
        return BLE_ATT_ERR_UNLIKELY;
    }

    if (ctxt->op == BLE_GATT_ACCESS_OP_WRITE_CHR) {
        if (ble_uuid_cmp(ctxt->chr->uuid, &s_rx_uuid.u) == 0) {
            uint8_t buf[256];
            uint16_t len = OS_MBUF_PKTLEN(ctxt->om);
            if (len > sizeof(buf)) {
                return BLE_ATT_ERR_INVALID_ATTR_VALUE_LEN;
            }
            int rc = os_mbuf_copydata(ctxt->om, 0, len, buf);
            if (rc != 0) {
                return BLE_ATT_ERR_UNLIKELY;
            }
            handle_rx_command(buf, len);
            return 0;
        }
        return BLE_ATT_ERR_UNLIKELY;
    }

    return BLE_ATT_ERR_UNLIKELY;
}

static const struct ble_gatt_svc_def gatt_svcs[] = {
    {
        .type = BLE_GATT_SVC_TYPE_PRIMARY,
        .uuid = &s_svc_uuid.u,
        .characteristics = (struct ble_gatt_chr_def[]){
            {
                .uuid = &s_rx_uuid.u,
                .access_cb = gatt_svr_chr_access,
                .flags = BLE_GATT_CHR_F_WRITE | BLE_GATT_CHR_F_WRITE_NO_RSP,
            },
            {
                .uuid = &s_tx_uuid.u,
                .access_cb = gatt_svr_chr_access,
                .flags = BLE_GATT_CHR_F_NOTIFY,
            },
            {
                .uuid = &s_info_uuid.u,
                .access_cb = gatt_svr_chr_access,
                .flags = BLE_GATT_CHR_F_READ,
            },
            {0},
        },
    },
    {0},
};

static void gatt_svr_register_cb(struct ble_gatt_register_ctxt *ctxt, void *arg)
{
    if (ctxt->op == BLE_GATT_REGISTER_OP_CHR) {
        if (ble_uuid_cmp(ctxt->chr.chr_def->uuid, &s_tx_uuid.u) == 0) {
            s_tx_handle = ctxt->chr.val_handle;
        }
    }
}

static int gap_event_cb(struct ble_gap_event *event, void *arg)
{
    switch (event->type) {
        case BLE_GAP_EVENT_CONNECT:
            if (event->connect.status == 0) {
                s_conn_handle = event->connect.conn_handle;
                ESP_LOGI(TAG, "BLE connected");
            } else {
                ESP_LOGW(TAG, "BLE connect failed: %d", event->connect.status);
                s_conn_handle = BLE_HS_CONN_HANDLE_NONE;
                if (s_adv_active) {
                    ble_start_advertising();
                }
            }
            return 0;
        case BLE_GAP_EVENT_DISCONNECT:
            ESP_LOGI(TAG, "BLE disconnected, reason=%d", event->disconnect.reason);
            s_conn_handle = BLE_HS_CONN_HANDLE_NONE;
            s_notify_enabled = false;
            if (s_adv_active) {
                ble_start_advertising();
            }
            return 0;
        case BLE_GAP_EVENT_SUBSCRIBE:
            s_notify_enabled = event->subscribe.cur_notify;
            ESP_LOGI(TAG, "Notify %s", s_notify_enabled ? "enabled" : "disabled");
            return 0;
        default:
            return 0;
    }
}

static void ble_start_advertising(void)
{
    int rc;
    struct ble_gap_adv_params adv_params;
    struct ble_hs_adv_fields fields;
    uint8_t adv_data[BLE_HS_ADV_MAX_SZ];
    uint8_t adv_len = 0;

    memset(&fields, 0, sizeof(fields));
    fields.flags = BLE_HS_ADV_F_DISC_GEN | BLE_HS_ADV_F_BREDR_UNSUP;
    fields.tx_pwr_lvl_is_present = 1;
    fields.tx_pwr_lvl = BLE_HS_ADV_TX_PWR_LVL_AUTO;
    fields.name = (uint8_t *)ble_svc_gap_device_name();
    fields.name_len = strlen(ble_svc_gap_device_name());
    fields.name_is_complete = 1;

    rc = ble_hs_adv_set_fields(&fields, adv_data, &adv_len, sizeof(adv_data));
    if (rc != 0) {
        ESP_LOGW(TAG, "adv_set_fields rc=%d", rc);
        return;
    }

    rc = ble_gap_adv_set_data(adv_data, adv_len);
    if (rc != 0) {
        ESP_LOGW(TAG, "adv_set_data rc=%d", rc);
        return;
    }

    memset(&adv_params, 0, sizeof(adv_params));
    adv_params.conn_mode = BLE_GAP_CONN_MODE_UND;
    adv_params.disc_mode = BLE_GAP_DISC_MODE_GEN;

    rc = ble_gap_adv_start(s_own_addr_type, NULL, BLE_HS_FOREVER, &adv_params, gap_event_cb, NULL);
    if (rc != 0) {
        ESP_LOGW(TAG, "adv_start rc=%d", rc);
    }
}

static void ble_on_sync(void)
{
    int rc;
    uint8_t addr_val[6] = {0};
    rc = ble_hs_id_infer_auto(0, &s_own_addr_type);
    if (rc == 0) {
        ble_hs_id_copy_addr(s_own_addr_type, addr_val, NULL);
        ESP_LOGI(TAG, "BLE addr type=%u addr=%02X:%02X:%02X:%02X:%02X:%02X",
                 (unsigned)s_own_addr_type,
                 addr_val[5], addr_val[4], addr_val[3], addr_val[2], addr_val[1], addr_val[0]);
    } else {
        ESP_LOGW(TAG, "ble_hs_id_infer_auto rc=%d", rc);
    }
    s_ble_synced = true;
    ble_start_advertising();
}

static void ble_host_task(void *param)
{
    nimble_port_run();
    nimble_port_freertos_deinit();
}

static void notify_packet(uint8_t op, uint8_t seq, const uint8_t *payload, uint16_t len)
{
    if (s_conn_handle == BLE_HS_CONN_HANDLE_NONE || !s_notify_enabled || s_tx_handle == 0) {
        return;
    }

    uint8_t header[4];
    header[0] = op;
    header[1] = seq;
    header[2] = (uint8_t)(len & 0xFF);
    header[3] = (uint8_t)((len >> 8) & 0xFF);

    struct os_mbuf *om = ble_hs_mbuf_from_flat(header, sizeof(header));
    if (!om) {
        return;
    }
    if (len > 0 && payload) {
        if (os_mbuf_append(om, payload, len) != 0) {
            os_mbuf_free_chain(om);
            return;
        }
    }
    ble_gatts_notify_custom(s_conn_handle, s_tx_handle, om);
}

static void send_error(uint8_t seq, uint8_t code)
{
    notify_packet(OP_ERROR, seq, &code, 1);
}

static void wifi_notify_config_result(uint8_t status)
{
    uint8_t payload[1] = {status};
    notify_packet(OP_CONFIG_RESULT, s_config_seq, payload, sizeof(payload));
}

static void boot_wifi_event_init(void)
{
    if (s_boot_connect_event_group == NULL) {
        s_boot_connect_event_group = ();
    }
}

static void wifi_event_handler(void *arg, esp_event_base_t event_base,
                               int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_SCAN_DONE) {
        uint16_t ap_num = 0;
        esp_wifi_scan_get_ap_num(&ap_num);
        if (ap_num == 0) {
            notify_packet(OP_SCAN_DONE, s_scan_seq, NULL, 0);
            return;
        }

        uint16_t max_records = ap_num > 20 ? 20 : ap_num;
        wifi_ap_record_t *ap_records = (wifi_ap_record_t *)calloc(max_records, sizeof(wifi_ap_record_t));
        if (!ap_records) {
            send_error(s_scan_seq, STATUS_INTERNAL);
            return;
        }

        if (esp_wifi_scan_get_ap_records(&max_records, ap_records) == ESP_OK) {
            for (uint16_t i = 0; i < max_records; ++i) {
                const wifi_ap_record_t *ap = &ap_records[i];
                uint8_t ssid_len = (uint8_t)strnlen((const char *)ap->ssid, sizeof(ap->ssid));
                uint8_t payload[64];
                if (ssid_len > 32) {
                    ssid_len = 32;
                }
                payload[0] = ssid_len;
                memcpy(&payload[1], ap->ssid, ssid_len);
                payload[1 + ssid_len] = (uint8_t)ap->rssi;
                payload[2 + ssid_len] = (uint8_t)ap->authmode;
                notify_packet(OP_SCAN_RESULT, s_scan_seq, payload, (uint16_t)(3 + ssid_len));
            }
        }
        free(ap_records);

        uint8_t done_payload[1] = {(uint8_t)max_records};
        notify_packet(OP_SCAN_DONE, s_scan_seq, done_payload, sizeof(done_payload));
        return;
    }

    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        if (!s_config_in_progress && !s_boot_connect_in_progress) {
            return;
        }
        wifi_event_sta_disconnected_t *disc = (wifi_event_sta_disconnected_t *)event_data;
        uint8_t status = STATUS_CONNECT_FAIL;
        if (disc) {
            if (disc->reason == WIFI_REASON_AUTH_FAIL) {
                status = STATUS_AUTH_FAIL;
            } else if (disc->reason == WIFI_REASON_NO_AP_FOUND) {
                status = STATUS_NO_AP;
            }
        }
        s_config_in_progress = false;
        if (s_boot_connect_in_progress && s_boot_connect_event_group != NULL) {
            xEventGroupSetBits(s_boot_connect_event_group, BOOT_WIFI_FAIL_BIT);
        }
        wifi_notify_config_result(status);
        return;
    }

    if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        if (!s_config_in_progress && !s_boot_connect_in_progress) {
            return;
        }
        s_config_in_progress = false;
        if (s_boot_connect_in_progress && s_boot_connect_event_group != NULL) {
            xEventGroupSetBits(s_boot_connect_event_group, BOOT_WIFI_CONNECTED_BIT);
        }
        wifi_notify_config_result(STATUS_OK);
        return;
    }
}

static void handle_rx_command(const uint8_t *data, uint16_t len)
{
    if (len < 4) {
        return;
    }
    uint8_t op = data[0];
    uint8_t seq = data[1];
    uint16_t payload_len = (uint16_t)data[2] | ((uint16_t)data[3] << 8);
    if (payload_len + 4 != len) {
        send_error(seq, STATUS_INVALID_ARG);
        return;
    }
    const uint8_t *payload = &data[4];

    if (op == OP_GET_INFO) {
        uint8_t info[24] = {0};
        info[0] = 1;
        memcpy(&info[1], s_mac, sizeof(s_mac));
        memcpy(&info[1 + sizeof(s_mac)], s_svc_uuid.value, 16);
        notify_packet(OP_INFO, seq, info, (uint16_t)(1 + sizeof(s_mac) + 16));
        return;
    }

    if (op == OP_SCAN_START) {
        s_scan_seq = seq;
        wifi_scan_config_t cfg = {
            .ssid = NULL,
            .bssid = NULL,
            .channel = 0,
            .show_hidden = true,
            .scan_type = WIFI_SCAN_TYPE_ACTIVE,
        };
        esp_wifi_scan_stop();
        esp_wifi_set_mode(WIFI_MODE_STA);
        esp_err_t err = esp_wifi_scan_start(&cfg, false);
        if (err == ESP_ERR_WIFI_STATE) {
            ESP_LOGW(TAG, "scan already in progress");
            return;
        }
        if (err == ESP_ERR_WIFI_NOT_STARTED) {
            ESP_LOGW(TAG, "wifi not started, restarting for scan");
            esp_wifi_start();
            esp_wifi_set_mode(WIFI_MODE_STA);
            err = esp_wifi_scan_start(&cfg, false);
        }
        if (err != ESP_OK) {
            ESP_LOGW(TAG, "scan_start err=%s", esp_err_to_name(err));
            send_error(seq, STATUS_INTERNAL);
        }
        return;
    }

    if (op == OP_SET_CONFIG) {
        if (payload_len < 2) {
            send_error(seq, STATUS_INVALID_ARG);
            return;
        }
        uint8_t ssid_len = payload[0];
        if (ssid_len == 0 || ssid_len > 32 || payload_len < (uint16_t)(2 + ssid_len)) {
            send_error(seq, STATUS_INVALID_ARG);
            return;
        }
        uint8_t pass_len = payload[1 + ssid_len];
        if (pass_len > 64 || payload_len != (uint16_t)(2 + ssid_len + pass_len)) {
            send_error(seq, STATUS_INVALID_ARG);
            return;
        }

        wifi_config_t wifi_cfg;
        memset(&wifi_cfg, 0, sizeof(wifi_cfg));
        memcpy(wifi_cfg.sta.ssid, &payload[1], ssid_len);
        memcpy(wifi_cfg.sta.password, &payload[2 + ssid_len], pass_len);
        wifi_cfg.sta.threshold.authmode = WIFI_AUTH_OPEN;
        wifi_cfg.sta.pmf_cfg.capable = true;
        wifi_cfg.sta.pmf_cfg.required = false;

        esp_err_t err = esp_wifi_set_config(WIFI_IF_STA, &wifi_cfg);
        if (err != ESP_OK) {
            send_error(seq, STATUS_INTERNAL);
            return;
        }

        s_config_seq = seq;
        s_config_in_progress = true;
        notify_packet(OP_CONFIG_ACK, seq, NULL, 0);
        err = esp_wifi_connect();
        if (err != ESP_OK) {
            s_config_in_progress = false;
            send_error(seq, STATUS_INTERNAL);
        }
        return;
    }

    send_error(seq, STATUS_INVALID_ARG);
}

esp_err_t ble_wifi_provisioning_start(void)
{
    esp_err_t err = ensure_base_inited();
    if (err != ESP_OK && err != ESP_ERR_INVALID_STATE) {
        return err;
    }

    if (!s_ble_inited) {
        esp_read_mac(s_mac, ESP_MAC_WIFI_STA);

            const uint8_t svc_base[16] = {0x19, 0x7d, 0x2a, 0x6b, 0x4c, 0x10, 0x4f, 0x81,
                                          0x9a, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
            const uint8_t rx_base[16]  = {0x19, 0x7d, 0x2a, 0x6b, 0x4c, 0x10, 0x4f, 0x81,
                                          0x9a, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
            const uint8_t tx_base[16]  = {0x19, 0x7d, 0x2a, 0x6b, 0x4c, 0x10, 0x4f, 0x81,
                                          0x9a, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
            const uint8_t info_base[16]= {0x19, 0x7d, 0x2a, 0x6b, 0x4c, 0x10, 0x4f, 0x81,
                                          0x9a, 0x13, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

        build_uuid_with_mac(&s_svc_uuid, svc_base);
        build_uuid_with_mac(&s_rx_uuid, rx_base);
        build_uuid_with_mac(&s_tx_uuid, tx_base);
        build_uuid_with_mac(&s_info_uuid, info_base);

        char dev_name[20];
        snprintf(dev_name, sizeof(dev_name), "TEMP_%02X%02X%02X", s_mac[3], s_mac[4], s_mac[5]);

        esp_err_t init_ret = nimble_port_init();
        if (init_ret != ESP_OK) {
            ESP_LOGE(TAG, "nimble_port_init failed: %s", esp_err_to_name(init_ret));
            return init_ret;
        }

        ble_svc_gap_init();
        ble_svc_gatt_init();
        ble_svc_gap_device_name_set(dev_name);

        ble_hs_cfg.reset_cb = ble_on_reset;
        ble_hs_cfg.sync_cb = ble_on_sync;
        ble_hs_cfg.gatts_register_cb = gatt_svr_register_cb;
        ble_hs_cfg.store_status_cb = ble_store_util_status_rr;
        ble_store_config_init();

        int rc = ble_gatts_count_cfg(gatt_svcs);
        if (rc != 0) {
            ESP_LOGE(TAG, "gatt count rc=%d", rc);
            return ESP_FAIL;
        }
        rc = ble_gatts_add_svcs(gatt_svcs);
        if (rc != 0) {
            ESP_LOGE(TAG, "gatt add rc=%d", rc);
            return ESP_FAIL;
        }

        nimble_port_freertos_init(ble_host_task);

        log_uuid("Service UUID", &s_svc_uuid.u);
        log_uuid("RX UUID", &s_rx_uuid.u);
        log_uuid("TX UUID", &s_tx_uuid.u);
        log_uuid("INFO UUID", &s_info_uuid.u);
    }

    s_adv_active = true;
    if (s_ble_synced) {
        ble_start_advertising();
    }
    return ESP_OK;
}

esp_err_t ble_wifi_provisioning_stop(void)
{
    s_adv_active = false;
    if (s_ble_synced) {
        ble_gap_adv_stop();
    }
    return ESP_OK;
}

esp_err_t ble_wifi_force_reprovision(void)
{
    ESP_LOGW(TAG, "Force reprovision: clear wifi config and restart BLE advertising");

    esp_err_t err = ensure_base_inited();
    if (err != ESP_OK && err != ESP_ERR_INVALID_STATE) {
        ESP_LOGW(TAG, "base init err: %s", esp_err_to_name(err));
        return err;
    }

    esp_wifi_disconnect();
    esp_wifi_restore();
    s_config_in_progress = false;

    s_adv_active = true;
    if (s_ble_synced) {
        ble_start_advertising();
    }
    return ESP_OK;
}

esp_err_t ble_wifi_try_connect_saved(uint32_t timeout_ms)
{
    esp_err_t err = ensure_base_inited();
    if (err != ESP_OK && err != ESP_ERR_INVALID_STATE) {
        return err;
    }

    boot_wifi_event_init();
    if (s_boot_connect_event_group == NULL) {
        return ESP_ERR_NO_MEM;
    }

    wifi_config_t wifi_cfg;
    memset(&wifi_cfg, 0, sizeof(wifi_cfg));
    err = esp_wifi_get_config(WIFI_IF_STA, &wifi_cfg);
    if (err != ESP_OK) {
        ESP_LOGW(TAG, "Read saved WiFi config failed: %s", esp_err_to_name(err));
        return err;
    }

    if (wifi_cfg.sta.ssid[0] == '\0') {
        ESP_LOGW(TAG, "No saved WiFi config found");
        return ESP_ERR_NOT_FOUND;
    }

    ESP_LOGI(TAG, "Boot WiFi try connect, ssid=%s", (char *)wifi_cfg.sta.ssid);

    xEventGroupClearBits(s_boot_connect_event_group, BOOT_WIFI_CONNECTED_BIT | BOOT_WIFI_FAIL_BIT);
    s_boot_connect_in_progress = true;

    err = esp_wifi_set_config(WIFI_IF_STA, &wifi_cfg);
    if (err != ESP_OK) {
        s_boot_connect_in_progress = false;
        ESP_LOGW(TAG, "Apply saved WiFi config failed: %s", esp_err_to_name(err));
        return err;
    }

    esp_wifi_disconnect();
    err = esp_wifi_connect();
    if (err != ESP_OK) {
        s_boot_connect_in_progress = false;
        ESP_LOGW(TAG, "Boot WiFi connect start failed: %s", esp_err_to_name(err));
        return err;
    }

    EventBits_t bits = xEventGroupWaitBits(
        s_boot_connect_event_group,
        BOOT_WIFI_CONNECTED_BIT | BOOT_WIFI_FAIL_BIT,
        pdTRUE,
        pdFALSE,
        pdMS_TO_TICKS(timeout_ms)
    );

    s_boot_connect_in_progress = false;

    if ((bits & BOOT_WIFI_CONNECTED_BIT) != 0) {
        ESP_LOGI(TAG, "Boot WiFi connected");
        return ESP_OK;
    }

    esp_wifi_disconnect();
    if ((bits & BOOT_WIFI_FAIL_BIT) != 0) {
        ESP_LOGW(TAG, "Boot WiFi connect failed, stop retry until next boot");
        return ESP_FAIL;
    }

    ESP_LOGW(TAG, "Boot WiFi connect timeout after %lu ms", (unsigned long)timeout_ms);
    return ESP_ERR_TIMEOUT;
}

bool ble_wifi_is_client_connected(void)
{
    return s_conn_handle != BLE_HS_CONN_HANDLE_NONE;
}

static esp_err_t ensure_base_inited(void)
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    err = esp_netif_init();
    if (err != ESP_OK && err != ESP_ERR_INVALID_STATE) {
        return err;
    }

    err = esp_event_loop_create_default();
    if (err != ESP_OK && err != ESP_ERR_INVALID_STATE) {
        return err;
    }

    if (!esp_netif_get_handle_from_ifkey("WIFI_STA_DEF")) {
        esp_netif_create_default_wifi_sta();
    }

    if (!esp_netif_get_handle_from_ifkey("WIFI_AP_DEF")) {
        esp_netif_create_default_wifi_ap();
    }

    wifi_mode_t current_mode;
    err = esp_wifi_get_mode(&current_mode);
    if (err == ESP_ERR_WIFI_NOT_INIT) {
        wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
        ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    } else if (err != ESP_OK) {
        return err;
    }

    err = ensure_hotspot_started();
    if (err != ESP_OK) {
        ESP_LOGW(TAG, "Hotspot init failed, continue without abort: %s", esp_err_to_name(err));
    }

    err = esp_wifi_set_mode(WIFI_MODE_APSTA);
    if (err != ESP_OK && err != ESP_ERR_WIFI_STATE) {
        ESP_LOGW(TAG, "Set WIFI_MODE_APSTA failed: %s", esp_err_to_name(err));
        return err;
    }

    err = esp_wifi_start();
    if (err != ESP_OK && err != ESP_ERR_WIFI_STATE) {
        ESP_LOGW(TAG, "esp_wifi_start failed: %s", esp_err_to_name(err));
        return err;
    }

    if (!s_wifi_event_handlers_registered) {
        ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL));
        ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL));
        s_wifi_event_handlers_registered = true;
    }
    return ESP_OK;
}

static esp_err_t ensure_hotspot_started(void)
{
    uint8_t sta_mac[6] = {0};
    esp_read_mac(sta_mac, ESP_MAC_WIFI_STA);

    char ssid[32] = {0};
    snprintf(ssid, sizeof(ssid), HOTSPOT_SSID_PREFIX "%02X%02X%02X", sta_mac[3], sta_mac[4], sta_mac[5]);

    wifi_config_t ap_cfg;
    memset(&ap_cfg, 0, sizeof(ap_cfg));
    strncpy((char *)ap_cfg.ap.ssid, ssid, sizeof(ap_cfg.ap.ssid));
    ap_cfg.ap.ssid_len = strlen(ssid);
    strncpy((char *)ap_cfg.ap.password, HOTSPOT_PASSWORD, sizeof(ap_cfg.ap.password));
    ap_cfg.ap.channel = 1;
    ap_cfg.ap.authmode = WIFI_AUTH_WPA2_PSK;
    ap_cfg.ap.ssid_hidden = 0;
    ap_cfg.ap.max_connection = 4;
    ap_cfg.ap.beacon_interval = 100;

    if (HOTSPOT_PASSWORD[0] == '\0') {
        ap_cfg.ap.authmode = WIFI_AUTH_OPEN;
    }

    esp_err_t err = esp_wifi_set_config(WIFI_IF_AP, &ap_cfg);
    if (err != ESP_OK && err != ESP_ERR_WIFI_STATE) {
        ESP_LOGW(TAG, "Set AP config failed: %s", esp_err_to_name(err));
        return err;
    }

    ESP_LOGI(TAG, "Hotspot ready: ssid=%s password=%s", ssid, HOTSPOT_PASSWORD);
    return ESP_OK;
}
