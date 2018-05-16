#pragma once

#include <asf.h>
#include "conf_usb.h"

bool usb_vendor_e1_enable(void);
void usb_vendor_e1_disable(void);

bool main_cdc_enable(uint8_t port);
void main_cdc_disable(uint8_t port);
void main_cdc_rx_notify(uint8_t port);
void main_cdc_set_dtr(uint8_t port, bool b_enable);
