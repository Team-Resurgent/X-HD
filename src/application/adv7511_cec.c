// Copyright 2021, Ryan Wendland, XboxHDMI by Ryzee119
// SPDX-License-Identifier: MIT

#include "../shared/adv7511.h"
#include "../shared/adv7511_i2c.h"
#include "adv7511_cec.h"

void adv7511_write_cec(const uint8_t address, uint8_t value) {
    I2C_HandleTypeDef* hi2c = adv7511_i2c_instance();
    HAL_I2C_Mem_Write(hi2c, ADV7511_CEC_I2C_ADDR_DEFAULT, address, I2C_MEMADD_SIZE_8BIT, &value, 1, HAL_MAX_DELAY);
}
