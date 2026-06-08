#pragma once

#include <stdbool.h>
#include <stdint.h>

uint32_t crc32_calc(const uint32_t start_addr, const uint32_t length);
uint32_t crc32_copy(const uint32_t start_addr, uint8_t* data, const uint16_t data_size);
