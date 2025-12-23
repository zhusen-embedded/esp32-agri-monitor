file(GLOB_RECURSE SOURCES "*.c")
file(GLOB_RECURSE UI_MAIN_SOURCES "ui/*.c")
file(GLOB_RECURSE UI_GENERATED_SOURCES "ui/generated/*.c")
file(GLOB_RECURSE UI_FONTS_SOURCES "ui/generated/guider_fonts/*.c")
file(GLOB_RECURSE UI_CUSTOM_SOURCES "ui/custom/*.c")
file(GLOB_RECURSE UI_CUSTOM_IMAGES_SOURCES "ui/generated/images/*.c")
file(GLOB_RECURSE EVENTS_SOURCES "ui/events/*.c")
file(GLOB_RECURSE BLE_SITTING_WIFI_SOURCES "ble_sitting_wifi/*.c")
idf_component_register(
    SRCS 
        "main.c"
        "uart_echo_wifi_ble/uart_echo_wifi_ble.c"
        "esp_lcd_touch_ft6x36/esp_lcd_touch_ft6x36.c"
        ${UI_MAIN_SOURCES}
        ${UI_GENERATED_SOURCES}
        ${UI_FONTS_SOURCES}
        ${UI_CUSTOM_SOURCES}
        ${UI_CUSTOM_IMAGES_SOURCES}
        ${EVENTS_SOURCES}
        ${BLE_SITTING_WIFI_SOURCES}
    INCLUDE_DIRS 
        "."
        "ui"
        "ui/generated"
        "ui/custom"
        "ui/generated/guider_fonts"
        "ui/generated/images"
        "uart_echo_wifi_ble"
        "ui/events"
        "ble_sitting_wifi"
        "esp_lcd_touch_ft6x36/include"
    REQUIRES 
        esp_driver_uart 
        esp_driver_gpio 
        esp_http_client 
        esp_wifi 
        nvs_flash
        json
        wifi_provisioning
        bt
        protocomm
)
