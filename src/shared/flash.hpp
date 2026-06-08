#pragma once

#include <stdbool.h>
#include <stdint.h>

bool flash_erase_page(const uint16_t page);
bool flash_write_page(const uint16_t page, const uint8_t* data, const uint16_t data_size);
uint32_t flash_copy_page(const uint16_t page, uint8_t* data, const uint16_t data_size);
void flash_remove_flag();
void flash_set_flag();