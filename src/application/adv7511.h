// Copyright 2021, Ryan Wendland, XboxHDMI by Ryzee119
// SPDX-License-Identifier: MIT

#ifndef __ADV7511_H__
#define __ADV7511_H__

/* Hardware defined default addresses for I2C register maps */
#define ADV7511_CEC_I2C_ADDR_DEFAULT    0x78 //0x78>>1
#define ADV7511_EDID_I2C_ADDR_DEFAULT   0x7E //0x7E>>1
#define ADV7511_PACKET_I2C_ADDR_DEFAULT 0x70 //0x70>>1

enum adv7511_sync_polarity
{
    ADV7511_SYNC_POLARITY_PASSTHROUGH,
    ADV7511_SYNC_POLARITY_LOW,
    ADV7511_SYNC_POLARITY_HIGH,
};

void adv7511_write_cec(const uint8_t address, uint8_t value);

#endif // __ADV7511_H__
