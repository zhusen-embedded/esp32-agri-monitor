#ifndef USB_PI_LINK_H
#define USB_PI_LINK_H

#include "esp_err.h"
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t usb_pi_link_start(void);
bool usb_pi_link_get_latest(char *out, size_t out_len);
uint32_t usb_pi_link_get_seq(void);

#ifdef __cplusplus
}
#endif

#endif // USB_PI_LINK_H

