#include "adv7511_xbox.h"

void init_adv(adv7511 *encoder, const xbox_encoder xb_encoder) {
    adv7511_i2c_init();

    // Initialise the encoder object
    adv7511_struct_init(encoder);

    // [7:6] HPD Control (forced to high)
    // [5] Fixed 0
    // [4] TMDS Clock soft turn on
    // [3:1] Fixed 000
    // [0] AV gating off
    adv7511_write_register(0xD6, 0b11010000);

    // Power up the encoder and set fixed registers
    adv7511_power_up(encoder);

    adv7511_disable_video();

    // [3:0] Set video input mode to RGB/YCbCr 4:4:4, 12bit databus DDR
    // [7:4] Audio to 48kHz
    adv7511_write_register(0x15, 0b00100101);

    // [7] Output Format 4:4:4
    // [5:4] 8 bit video
    // [3:2] video style 1 (Y[3:0] Cb[7:0] first edge, Cr[7:0] Y[7:4] second edge)
    // [1] Set DDR Input Rising Edge
    // [0] YCbCr
    adv7511_write_register(0x16, 0b00111011);

    // Setup xbox encoder specific stuff (Xcalibur uses different settings)
    init_adv_encoder_specific(xb_encoder);

    // [0] Enable DE generation. This is derived from HSYNC,VSYNC for video active framing
    adv7511_update_register(0x17, 0b00000001, 0b00000001);

    adv7511_disable_csc();

    // Set Output to HDMI Mode (Instead of DVI Mode)
    // [7] HDCP Disabled
    // [6:5] Must be set to default value 00
    // [4] Frame encryption
    // [3:2] Must be set to default value 01
    // [1] HDMI/DVI
    // [0] Must be set to default 0
    adv7511_write_register(0xAF, 0b00000110);

    // [7] Enable General Control Packet CHECK
    adv7511_update_register(0x40, 0b10000000, 0b10000000);

    init_adv_audio();

    adv7511_enable_video();
}

void init_adv_encoder_specific(const xbox_encoder xb_encoder) {
    if (xb_encoder == ENCODER_XCALIBUR) {
        // [6] Normal Bus Order, [5] DDR Alignment D[35:18] (left aligned)
        adv7511_update_register(0x48, 0b01100000, 0b00100000);
        // [7] Disable DDR Negative Edge CLK Delay, [6:4] with -400ps delay
        // [3:2] No sync pulse, [1] Data enable, then sync, [0] Fixed
        adv7511_write_register(0xD0, 0b00101110);
        // [7:5] -0.8ns clock delay
        adv7511_update_register(0xBA, 0b11100000, 0b00100000);
    } else if (xb_encoder == ENCODER_FOCUS) {
        // Focus (FS454) needs slightly adjusted DDR sampling vs Conexant
        // to avoid blue color artifacts from marginal data sampling
        // [6] LSB .... MSB Reverse Bus Order, [5] DDR Alignment D[17:0] (right aligned)
        adv7511_update_register(0x48, 0b01100000, 0b01000000);
        // [7] Enable DDR Negative Edge CLK Delay, [6:4] with 0ps delay
        // [3:2] No sync pulse, [1] Data enable, then sync, [0] Fixed
        adv7511_write_register(0xD0, 0b10111110);
        // [7:5] -0.4ns clock delay (one step from Conexant's 0ns)
        adv7511_update_register(0xBA, 0b11100000, 0b01000000);
    } else {
        // Conexant
        // [6] LSB .... MSB Reverse Bus Order, [5] DDR Alignment D[17:0] (right aligned)
        adv7511_update_register(0x48, 0b01100000, 0b01000000);
        // [7] Enable DDR Negative Edge CLK Delay, [6:4] with 0ps delay
        // [3:2] No sync pulse, [1] Data enable, then sync, [0] Fixed
        adv7511_write_register(0xD0, 0b10111110);
        // [7:5] No clock delay
        adv7511_update_register(0xBA, 0b11100000, 0b01100000);
    }
}

void init_adv_audio() {
    // [19:0] Set 48kHz Audio clock CHECK (N Value)
    adv7511_write_register(0x01, 0x00);
    adv7511_write_register(0x02, 0x18);
    adv7511_write_register(0x03, 0x00);

    // [6:4] Set SPDIF audio source
    adv7511_update_register(0x0A, 0b01110000, 0b00010000);

    // [7] SPDIF enable
    adv7511_update_register(0x0B, 0b10000000, 0b10000000);
}

void update_avi_infoframe(const bool widescreen, const bool rgb, const uint8_t vic) {
    // Determine if HD content for colorimetry selection
    const bool is_hd = (vic == VIC_04_720p_60_16_9 || vic == VIC_05_1080i_60_16_9);

    // [6] Start AVI Infoframe Update
    adv7511_update_register(0x4A, 0b01000000, 0b01000000);

    // DB1 (0x55):
    // [6:5] Infoframe output format: RGB (00) or YCbCr4:4:4 (10)
    // [4] Active format info present (A0 = 1)
    // [1:0] Scan info: underscan (10) - prevents TV from cropping edges
    adv7511_update_register(0x55, 0b01110011, rgb ? 0b00010010 : 0b01010010);

    // DB2 (0x56):
    // [7:6] Colorimetry: BT.601 (01) for SD, BT.709 (10) for HD
    // [5:4] Picture aspect ratio (M1:M0): 16:9 (10) or 4:3 (01)
    // [3:0] Active format aspect ratio: same as coded frame (1000)
    adv7511_write_register(0x56, (is_hd ? 0b10000000 : 0b01000000)
                                | (widescreen ? 0b00101000 : 0b00011000));

    // DB3 (0x57):
    // [7]   ITC = 1 (IT content - tells TV to minimize processing / enable game mode)
    // [6:4] EC = 000 (extended colorimetry not used)
    // [3:2] Q = 01 (Limited Range) when RGB, 00 (Default) when YCbCr
    // [1:0] SC = 00 (no non-uniform scaling)
    adv7511_write_register(0x57, rgb ? 0b10000100 : 0b10000000);

    // DB4 (0x58): VIC code
    adv7511_write_register(0x58, vic);

    // DB5 (0x59):
    // [7:6] YQ = 00 (Limited Range YCbCr quantization)
    // [5:4] CN = 11 (Game content type, used when ITC=1 for reduced input lag)
    // [3:0] PR = 0000 (no pixel repetition)
    adv7511_write_register(0x59, 0b00110000);

    // [6] End AVI Infoframe Update
    adv7511_update_register(0x4A, 0b01000000, 0b00000000);
}
