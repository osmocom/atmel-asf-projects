#pragma once

#include <stdint.h>

#include "idt82v2081.h"

int idt82_asf_init(struct idt82 *idt, Spi *spi, uint8_t cs);
