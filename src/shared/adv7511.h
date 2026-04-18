// Copyright 2021, Ryan Wendland, XboxHDMI by Ryzee119
// SPDX-License-Identifier: MIT

#ifndef __ADV7511_H__
#define __ADV7511_H__

#include "stm32.h"
#include "stdbool.h"

#define BIT(nr) (1UL << (nr))

#define ADV7511_INT0_HPD BIT(7)
#define ADV7511_INT0_MONITOR_SENSE BIT(6)

#define ADV7511_VIC_CHANGED         0x80
#define ADV7511_VIC_CHANGED_CLEAR   0x7F

/* Hardware defined default addresses for I2C register maps */
#define ADV7511_MAIN_I2C_ADDR           0x72 //0x72>>1

static const uint8_t identityMatrix[] = {
    0xA8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00};

static const uint8_t CscRgbToYuv709[] = {
    0x87, 0x06, 0x19, 0x9E, 0x1F, 0x5D, 0x08, 0x00,
    0x02, 0xED, 0x09, 0xD2, 0x00, 0xFD, 0x01, 0x00,
    0x1E, 0x63, 0x1A, 0x98, 0x07, 0x06, 0x08, 0x00};

static const uint8_t CscRgbToYuv601[] = {
    0x87, 0x06, 0x1A, 0x1E, 0x1E, 0xDE, 0x08, 0x00,
    0x04, 0x1C, 0x08, 0x10, 0x01, 0x91, 0x01, 0x00,
    0x1D, 0xA2, 0x1B, 0x59, 0x07, 0x06, 0x08, 0x00};

typedef struct
{
    uint8_t hot_plug_detect;
    uint8_t monitor_sense;
    uint8_t interrupt;
    uint8_t vic;
} adv7511;

void adv7511_power_up(adv7511 *encoder);
void adv7511_update_register(const uint8_t address, const uint8_t mask, uint8_t new_value);
uint8_t adv7511_read_register(const uint8_t address);
void adv7511_write_register(const uint8_t address, uint8_t value);
void adv7511_struct_init(adv7511 *encoder);

void adv_handle_interrupts(adv7511 *encoder);

void adv7511_enable_csc();
void adv7511_disable_csc();
void adv7511_apply_csc(const uint8_t * const coefficients);

// New stuff
void adv7511_disable_video();
void adv7511_enable_video();
void adv7511_power_down_tmds();
void adv7511_power_up_tmds();

#endif // __ADV7511_H__
