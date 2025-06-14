// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (c) 2015 Terri Cain <terri@dolphincorp.co.uk>
 */

#include "razerchromacommon.h"

static unsigned char orochi2011_led[]  = { 0x01, 0x00, 0x00, 0x06, 0x48, 0x00, 0x00, 0x00, 0x01, 0xFF, 0x03, 0x05, 0x06, 0x06, 0x10, 0x10, 0x10, 0x10, 0x24, 0x24, 0x4c, 0x4c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x01, 0x01, 0x03, 0x03, 0x04, 0x01, 0x04, 0x04, 0x01, 0x01, 0x05, 0x05, 0x01, 0x01, 0x06, 0x31, 0x88, 0x00, 0x07, 0x31, 0x87, 0x00, 0x08, 0x08, 0x01, 0x01, 0x09, 0x09, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x01 };
static unsigned char orochi2011_dpi[] = { 0x01, 0x00, 0x00, 0x05, 0x05, 0x00, 0x00, 0x00, 0x01, 0x02, 0x03, 0x4c, 0x4c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 };

/*
 * Standard Device Functions
 */

/**
 * Set what mode the device will operate in.
 *
 * Currently known modes
 * 0x00, 0x00: Normal Mode
 * 0x02, 0x00: Unknown Mode
 * 0x03, 0x00: Driver Mode
 *
 * 0x02, 0x00 Will make M1-5 and FN emit normal keystrokes. Some sort of factory test mode. Not recommended to be used.
 */
struct razer_report razer_chroma_standard_set_device_mode(unsigned char mode, unsigned char param)
{
    struct razer_report report = get_razer_report(0x00, 0x04, 0x02);

    if(mode != 0x00 && mode != 0x03) { // Explicitly blocking the 0x02 mode
        mode = 0x00;
    }
    // Only allow 0x00 as param
    param = 0x00;

    report.arguments[0] = mode;
    report.arguments[1] = param;

    return report;
}

/**
 * Get what mode the device is operating in.
 *
 * Currently known modes
 * 0x00, 0x00: Normal Mode
 * 0x02, 0x00: Unknown Mode
 * 0x03, 0x00: Driver Mode
 *
 * 0x02, 0x00 Will make M1-5 and FN emit normal keystrokes. Some sort of factory test mode. Not recommended to be used.
 */
struct razer_report razer_chroma_standard_get_device_mode(void)
{
    return get_razer_report(0x00, 0x84, 0x02);
}

/**
 * Get serial from device
 */
struct razer_report razer_chroma_standard_get_serial(void)
{
    return get_razer_report(0x00, 0x82, 0x16);
}

/**
 * Get firmware version from device
 */
struct razer_report razer_chroma_standard_get_firmware_version(void)
{
    return get_razer_report(0x00, 0x81, 0x02);
}

/*
 * Standard Functions
 */

/**
 * Set the state of an LED on the device
 *
 * Status Trans Packet Proto DataSize Class CMD Args
 * 00     3f    0000   00    03       03    00  010801                     | SET LED STATE (VARSTR, GAMEMODE, ON)
 * 00     3f    0000   00    03       03    00  010800                     | SET LED STATE (VARSTR, GAMEMODE, OFF)
 */
struct razer_report razer_chroma_standard_set_led_state(unsigned char variable_storage, unsigned char led_id, unsigned char led_state)
{
    struct razer_report report = get_razer_report(0x03, 0x00, 0x03);
    report.arguments[0] = variable_storage;
    report.arguments[1] = led_id;
    report.arguments[2] = clamp_u8(led_state, 0x00, 0x01);

    return report;
}

struct razer_report razer_chroma_standard_set_led_blinking(unsigned char variable_storage, unsigned char led_id)
{
    struct razer_report report = get_razer_report(0x03, 0x04, 0x04);
    report.arguments[0] = variable_storage;
    report.arguments[1] = led_id;
    report.arguments[2] = 0x05;
    report.arguments[3] = 0x05;

    return report;
}

/**
 * Get the state of an LED on the device
 */
struct razer_report razer_chroma_standard_get_led_state(unsigned char variable_storage, unsigned char led_id)
{
    struct razer_report report = get_razer_report(0x03, 0x80, 0x03);
    report.arguments[0] = variable_storage;
    report.arguments[1] = led_id;

    return report;
}

/**
 * Set LED RGB parameters
 */
struct razer_report razer_chroma_standard_set_led_rgb(unsigned char variable_storage, unsigned char led_id, struct razer_rgb *rgb1)
{
    struct razer_report report = get_razer_report(0x03, 0x01, 0x05);
    report.arguments[0] = variable_storage;
    report.arguments[1] = led_id;
    report.arguments[2] = rgb1->r;
    report.arguments[3] = rgb1->g;
    report.arguments[4] = rgb1->b;

    return report;
}

/**
 * Get LED RGB parameters
 */
struct razer_report razer_chroma_standard_get_led_rgb(unsigned char variable_storage, unsigned char led_id)
{
    struct razer_report report = get_razer_report(0x03, 0x81, 0x05);
    report.arguments[0] = variable_storage;
    report.arguments[1] = led_id;
    return report;
}

/**
 * Set the effect of an LED on the device
 *
 * Status Trans Packet Proto DataSize Class CMD Args
 * ? TODO fill this
 */
struct razer_report razer_chroma_standard_set_led_effect(unsigned char variable_storage, unsigned char led_id, unsigned char led_effect)
{
    struct razer_report report = get_razer_report(0x03, 0x02, 0x03);
    report.arguments[0] = variable_storage;
    report.arguments[1] = led_id;
    report.arguments[2] = clamp_u8(led_effect, 0x00, 0x05);

    return report;
}

/**
 * Get the effect of an LED on the device
 *
 * Status Trans Packet Proto DataSize Class CMD Args
 * ? TODO fill this
 */
struct razer_report razer_chroma_standard_get_led_effect(unsigned char variable_storage, unsigned char led_id)
{
    struct razer_report report = get_razer_report(0x03, 0x82, 0x03);
    report.arguments[0] = variable_storage;
    report.arguments[1] = led_id;

    return report;
}

/**
 * Set the brightness of an LED on the device
 *
 * Status Trans Packet Proto DataSize Class CMD Args
 * ? TODO fill this
 */
struct razer_report razer_chroma_standard_set_led_brightness(unsigned char variable_storage, unsigned char led_id, unsigned char brightness)
{
    struct razer_report report = get_razer_report(0x03, 0x03, 0x03);
    report.arguments[0] = variable_storage;
    report.arguments[1] = led_id;
    report.arguments[2] = brightness;

    return report;
}

/**
 * Get the brightness of an LED on the device
 *
 * Status Trans Packet Proto DataSize Class CMD Args
 * ? TODO fill this
 */
struct razer_report razer_chroma_standard_get_led_brightness(unsigned char variable_storage, unsigned char led_id)
{
    struct razer_report report = get_razer_report(0x03, 0x83, 0x03);
    report.arguments[0] = variable_storage;
    report.arguments[1] = led_id;

    return report;
}

/*
 * Standard Matrix Effects Functions
 */

static struct razer_report razer_chroma_standard_matrix_effect_base(unsigned char arg_size, unsigned char effect_id)
{
    struct razer_report report = get_razer_report(0x03, 0x0A, arg_size);
    report.arguments[0] = effect_id;
    return report;
}

/**
 * Set the effect of the LED matrix to None
 *
 * Status Trans Packet Proto DataSize Class CMD Args
 * ? TODO fill this
 */
struct razer_report razer_chroma_standard_matrix_effect_none(void)
{
    return razer_chroma_standard_matrix_effect_base(0x01, MATRIX_EFFECT_OFF);
}

/**
 * Set the effect of the LED matrix to Wave
 *
 * Status Trans Packet Proto DataSize Class CMD Args
 * ? TODO fill this
 */
struct razer_report razer_chroma_standard_matrix_effect_wave(unsigned char wave_direction)
{
    struct razer_report report = razer_chroma_standard_matrix_effect_base(0x02, MATRIX_EFFECT_WAVE);
    report.arguments[1] = clamp_u8(wave_direction, 0x01, 0x02);

    return report;
}

/**
 * Set the effect of the LED matrix to Spectrum
 *
 * Status Trans Packet Proto DataSize Class CMD Args
 * ? TODO fill this
 */
struct razer_report razer_chroma_standard_matrix_effect_spectrum(void)
{
    return razer_chroma_standard_matrix_effect_base(0x01, MATRIX_EFFECT_SPECTRUM);
}

/**
 * Set the effect of the LED matrix to Reactive
 *
 * Status Trans Packet Proto DataSize Class CMD Args
 * ? TODO fill this
 */
struct razer_report razer_chroma_standard_matrix_effect_reactive(unsigned char speed, struct razer_rgb *rgb1)
{
    struct razer_report report = razer_chroma_standard_matrix_effect_base(0x05, MATRIX_EFFECT_REACTIVE);
    report.arguments[1] = clamp_u8(speed, 0x01, 0x04); // Time
    report.arguments[2] = rgb1->r; /*rgb color definition*/
    report.arguments[3] = rgb1->g;
    report.arguments[4] = rgb1->b;

    return report;
}

/**
 * Set the effect of the LED matrix to Static
 *
 * Status Trans Packet Proto DataSize Class CMD Args
 * ? TODO fill this
 */
struct razer_report razer_chroma_standard_matrix_effect_static(struct razer_rgb *rgb1)
{
    struct razer_report report = razer_chroma_standard_matrix_effect_base(0x04, MATRIX_EFFECT_STATIC);
    report.arguments[1] = rgb1->r; /*rgb color definition*/
    report.arguments[2] = rgb1->g;
    report.arguments[3] = rgb1->b;

    return report;
}

/**
 * Set the effect of the LED matrix to Starlight
 *
 * Status Trans Packet Proto DataSize Class CMD Args
 * ? TODO fill this
 */
struct razer_report razer_chroma_standard_matrix_effect_starlight_single(unsigned char speed, struct razer_rgb *rgb1)
{
    struct razer_report report = razer_chroma_standard_matrix_effect_base(0x01, MATRIX_EFFECT_STARLIGHT);

    report.arguments[1] = 0x01; // Type one color
    report.arguments[2] = clamp_u8(speed, 0x01, 0x03); // Speed

    report.arguments[3] = rgb1->r; // Red 1
    report.arguments[4] = rgb1->g; // Green 1
    report.arguments[5] = rgb1->b; // Blue 1

    // For now haven't seen any chroma using this, seen the extended version
    report.arguments[6] = 0x00; // Red 2
    report.arguments[7] = 0x00; // Green 2
    report.arguments[8] = 0x00; // Blue 2

    return report;
}

/**
 * Set the effect of the LED matrix to Starlight
 *
 * Status Trans Packet Proto DataSize Class CMD Args
 * ? TODO fill this
 */
struct razer_report razer_chroma_standard_matrix_effect_starlight_dual(unsigned char speed, struct razer_rgb *rgb1, struct razer_rgb *rgb2)
{
    struct razer_report report = razer_chroma_standard_matrix_effect_base(0x01, MATRIX_EFFECT_STARLIGHT);

    report.arguments[1] = 0x02; // Type two color
    report.arguments[2] = clamp_u8(speed, 0x01, 0x03); // Speed

    report.arguments[3] = rgb1->r; // Red 1
    report.arguments[4] = rgb1->g; // Green 1
    report.arguments[5] = rgb1->b; // Blue 1

    report.arguments[6] = rgb2->r; // Red 2
    report.arguments[7] = rgb2->g; // Green 2
    report.arguments[8] = rgb2->b; // Blue 2

    return report;
}

struct razer_report razer_chroma_standard_matrix_effect_starlight_random(unsigned char speed)
{
    struct razer_report report = razer_chroma_standard_matrix_effect_base(0x01, MATRIX_EFFECT_STARLIGHT);

    report.arguments[1] = 0x03; // Type random color
    report.arguments[2] = clamp_u8(speed, 0x01, 0x03); // Speed

    return report;
}

/**
 * Set the device to "Breathing" effect
 *
 * Status Trans Packet Proto DataSize Class CMD Args
 * ??
 * ??
 * ??
 */
struct razer_report razer_chroma_standard_matrix_effect_breathing_random(void)
{
    struct razer_report report = razer_chroma_standard_matrix_effect_base(0x08, MATRIX_EFFECT_BREATHING);
    report.arguments[1] = 0x03; // Breathing type

    return report;
}
struct razer_report razer_chroma_standard_matrix_effect_breathing_single(struct razer_rgb *rgb1)
{
    struct razer_report report = razer_chroma_standard_matrix_effect_base(0x08, MATRIX_EFFECT_BREATHING);
    report.arguments[1] = 0x01; // Breathing type
    report.arguments[2] = rgb1->r;
    report.arguments[3] = rgb1->g;
    report.arguments[4] = rgb1->b;

    return report;
}
struct razer_report razer_chroma_standard_matrix_effect_breathing_dual(struct razer_rgb *rgb1, struct razer_rgb *rgb2)
{
    struct razer_report report = razer_chroma_standard_matrix_effect_base(0x08, MATRIX_EFFECT_BREATHING);
    report.arguments[1] = 0x02; // Breathing type
    report.arguments[2] = rgb1->r;
    report.arguments[3] = rgb1->g;
    report.arguments[4] = rgb1->b;
    report.arguments[5] = rgb2->r;
    report.arguments[6] = rgb2->g;
    report.arguments[7] = rgb2->b;

    return report;
}

/**
 * Set the device to "Custom" effect
 *
 * Status Trans Packet Proto DataSize Class CMD Args
 * ??
 *
 * Apparently Ultimate2016, Stealth and Stealth2016 need frame id to be 0x00, I don't think it's needed (depending on set_custom_frame)
 */
struct razer_report razer_chroma_standard_matrix_effect_custom_frame(unsigned char variable_storage)
{
    struct razer_report report = razer_chroma_standard_matrix_effect_base(0x02, MATRIX_EFFECT_CUSTOMFRAME);
    report.arguments[1] = variable_storage; // Data frame ID

    return report;
}

/**
 * Set the RGB or a row
 *
 * Start and stop columns are inclusive
 *
 * This sets the colour of a row on the keyboard. Takes in an array of RGB bytes.
 * The mappings below are correct for the BlackWidow Chroma. The BlackWidow Ultimate 2016
 * contains LEDs under the spacebar and the FN key so there will be changes once I get the
 * hardware.
 *
 * Row 0:
 *  0      Unused
 *  1      ESC
 *  2      Unused
 *  3-14   F1-F12
 *  15-17  PrtScr, ScrLk, Pause
 *  18-19  Unused
 *  20     Razer Logo
 *  21     Unused
 *
 * Row 1:
 *  0-21   M1 -> NP Minus
 *
 * Row 2:
 *  0-13   M2 -> Right Square Bracket ]
 *  14 Unused
 *  15-21 Delete -> NP Plus
 *
 * Row 3:
 *  0-14   M3 -> Return
 *  15-17  Unused
 *  18-20  NP4 -> NP6
 *
 * Row 4:
 *  0-12   M4 -> Forward Slash /
 *  13     Unused
 *  14     Right Shift
 *  15     Unused
 *  16     Up Arrow Key
 *  17     Unused
 *  18-21  NP1 -> NP Enter
 *
 * Row 5:
 *  0-3    M5 -> Alt
 *  4-10   Unused
 *  11     Alt GR
 *  12     Unused
 *  13-17  Context Menu Key -> Right Arrow Key
 *  18     Unused
 *  19-20  NP0 -> NP.
 *  21     Unused
 */
struct razer_report razer_chroma_standard_matrix_set_custom_frame(unsigned char row_index, unsigned char start_col, unsigned char stop_col, unsigned char *rgb_data)
{
    const size_t start_arg_offset = 4;
    struct razer_report report = {0};
    size_t row_length = (size_t) (((stop_col + 1) - start_col) * 3);

    if (row_length > sizeof(report.arguments) - start_arg_offset) {
        printk(KERN_ALERT "razerchroma: RGB data too long\n");
        row_length = 0;
    }

    report = get_razer_report(0x03, 0x0B, 0x46); // In theory should be able to leave data size at max as we have start/stop

    // printk(KERN_ALERT "razerkbd: Row ID: %d, Start: %d, Stop: %d, row length: %d\n", row_index, start_col, stop_col, (unsigned char)row_length);

    report.arguments[0] = 0xFF; // Frame ID
    report.arguments[1] = row_index;
    report.arguments[2] = start_col;
    report.arguments[3] = stop_col;
    memcpy(&report.arguments[4], rgb_data, row_length);

    return report;
}

/*
 * Extended Matrix Effects
 */

/**
 * Gets the state from extended matrix effect payload
 */
static struct razer_report razer_chroma_extended_matrix_get_effect_base(unsigned char arg_size, unsigned char variable_storage, unsigned char led_id, unsigned char effect_id)
{
    struct razer_report report = get_razer_report(0x0F, 0x82, arg_size);

    report.arguments[0] = variable_storage;
    report.arguments[1] = led_id;
    report.arguments[2] = effect_id; // Effect ID

    return report;
}

/**
 * Get the device's "Static" effect state
 */
struct razer_report razer_chroma_extended_matrix_get_effect_static(unsigned char variable_storage, unsigned char led_id)
{
    struct razer_report report = razer_chroma_extended_matrix_get_effect_base(0x09, variable_storage, led_id, 0x01);

    report.arguments[5] = 0x01;
    return report;
}

/**
 * Sets up the extended matrix effect payload
 */
static struct razer_report razer_chroma_extended_matrix_effect_base(unsigned char arg_size, unsigned char variable_storage, unsigned char led_id, unsigned char effect_id)
{
    struct razer_report report = get_razer_report(0x0F, 0x02, arg_size);

    report.arguments[0] = variable_storage;
    report.arguments[1] = led_id;
    report.arguments[2] = effect_id; // Effect ID

    return report;
}

/**
 * Set the device to "None" effect
 *
 * Status Trans Packet Proto DataSize Class CMD Args
 * 00     3f    0000   00    06       0f    02  010500000000  | SET LED MATRIX Effect (VARSTR, Backlight, None 0x00, 0x000000)
 */
struct razer_report razer_chroma_extended_matrix_effect_none(unsigned char variable_storage, unsigned char led_id)
{
    return razer_chroma_extended_matrix_effect_base(0x06, variable_storage, led_id, 0x00);
}

/**
 * Set the device to "Static" effect
 *
 * Status Trans Packet Proto DataSize Class CMD Args
 * 00     3f    0000   00    09       0f    02  010501000001ff0000 | SET LED MATRIX Effect (VARSTR, Backlight, Static 0x01, ? 0x000001, RGB 0xFF0000)
 * 00     3f    0000   00    09       0f    02  01050100000100ff00 | SET LED MATRIX Effect (VARSTR, Backlight, Static 0x01, ? 0x000001, RGB 0x00FF00)
 * 00     3f    0000   00    09       0f    02  010501000001008000 | SET LED MATRIX Effect (VARSTR, Backlight, Static 0x01, ? 0x000001, RGB 0x008000)
 */
struct razer_report razer_chroma_extended_matrix_effect_static(unsigned char variable_storage, unsigned char led_id, struct razer_rgb *rgb)
{
    struct razer_report report = razer_chroma_extended_matrix_effect_base(0x09, variable_storage, led_id, 0x01);

    report.arguments[5] = 0x01;
    report.arguments[6] = rgb->r;
    report.arguments[7] = rgb->g;
    report.arguments[8] = rgb->b;
    return report;
}

/**
 * Set the device to "Wave" effect
 *
 * Seems like direction is now 0x00, 0x01 for Left/Right, used to be 0x01, 0x02
 *
 * Status Trans Packet Proto DataSize Class CMD Args
 * 00     3f    0000   00    06       0f    02  010504002800 | SET LED MATRIX Effect (VARSTR, Backlight, Wave 0x04, Dir 0x00, ? 0x2800)
 * 00     3f    0000   00    06       0f    02  010504012800 | SET LED MATRIX Effect (VARSTR, Backlight, Wave 0x04, Dir 0x01, ? 0x2800)
 */
struct razer_report razer_chroma_extended_matrix_effect_wave(unsigned char variable_storage, unsigned char led_id, unsigned char direction)
{
    struct razer_report report = razer_chroma_extended_matrix_effect_base(0x06, variable_storage, led_id, 0x04);

    // Some devices use values 0x00, 0x01
    // Others use values 0x01, 0x02
    direction = clamp_u8(direction, 0x00, 0x02);

    // Razer has also added a "Fast Wave" effect for at least one device
    // which uses the same effect command but a speed parameter of 0x10
    report.arguments[3] = direction;
    report.arguments[4] = 0x28; // Speed, lower values are faster
    return report;
}

/**
 * Set the device to "Starlight" effect
 *
 * Speed is 0x01 - 0x03
 *
 * Status Trans Packet Proto DataSize Class CMD Args
 * 00     3f    0000   00    06       0f    02  010507000100             | SET LED MATRIX Effect (VARSTR, Backlight, Starlight 0x07, ? 0x00, Speed 0x01, Colours 0x00)
 * 00     3f    0000   00    06       0f    02  010507000200             | SET LED MATRIX Effect (VARSTR, Backlight, Starlight 0x07, ? 0x00, Speed 0x02, Colours 0x00)
 * 00     3f    0000   00    06       0f    02  010507000300             | SET LED MATRIX Effect (VARSTR, Backlight, Starlight 0x07, ? 0x00, Speed 0x03, Colours 0x00)
 * 00     3f    0000   00    09       0f    02  010507000301ff0000       | SET LED MATRIX Effect (VARSTR, Backlight, Starlight 0x07, ? 0x00, Speed 0x03, Colours 0x01, RGB 0xFF0000)
 * 00     3f    0000   00    0c       0f    02  010507000302ff000000ff00 | SET LED MATRIX Effect (VARSTR, Backlight, Starlight 0x07, ? 0x00, Speed 0x03, Colours 0x02, RGB 0xFF0000, RGB 0x00FF00)
 */
struct razer_report razer_chroma_extended_matrix_effect_starlight_random(unsigned char variable_storage, unsigned char led_id, unsigned char speed)
{
    struct razer_report report = razer_chroma_extended_matrix_effect_base(0x06, variable_storage, led_id, 0x07);

    speed = clamp_u8(speed, 0x01, 0x03);

    report.arguments[4] = speed;
    return report;
}
struct razer_report razer_chroma_extended_matrix_effect_starlight_single(unsigned char variable_storage, unsigned char led_id, unsigned char speed, struct razer_rgb *rgb1)
{
    struct razer_report report = razer_chroma_extended_matrix_effect_base(0x09, variable_storage, led_id, 0x07);

    speed = clamp_u8(speed, 0x01, 0x03);

    report.arguments[4] = speed;
    report.arguments[5] = 0x01;
    report.arguments[6] = rgb1->r;
    report.arguments[7] = rgb1->g;
    report.arguments[8] = rgb1->b;

    return report;
}
struct razer_report razer_chroma_extended_matrix_effect_starlight_dual(unsigned char variable_storage, unsigned char led_id, unsigned char speed, struct razer_rgb *rgb1, struct razer_rgb *rgb2)
{
    struct razer_report report = razer_chroma_extended_matrix_effect_base(0x0C, variable_storage, led_id, 0x07);

    speed = clamp_u8(speed, 0x01, 0x03);

    report.arguments[4] = speed;
    report.arguments[5] = 0x02;
    report.arguments[6] = rgb1->r;
    report.arguments[7] = rgb1->g;
    report.arguments[8] = rgb1->b;
    report.arguments[9] = rgb2->r;
    report.arguments[10] = rgb2->g;
    report.arguments[11] = rgb2->b;

    return report;
}

/**
 * Set the device to "Spectrum" effect
 *
 * Status Trans Packet Proto DataSize Class CMD Args
 * 00     3f    0000   00    06       0f    02  010503000000 | SET LED MATRIX Effect (VARSTR, Backlight, Spectrum 0x03, 0x000000)
 */
struct razer_report razer_chroma_extended_matrix_effect_spectrum(unsigned char variable_storage, unsigned char led_id)
{
    return razer_chroma_extended_matrix_effect_base(0x06, variable_storage, led_id, 0x03);
}

/**
 * Set the device to "Wheel" effect
 *
 * Status Trans Packet Proto DataSize Class CMD Args
 * 00     1f    0000   00    06       0f    02  01050a022800 | SET LED MATRIX Effect (VARSTR, Backlight, Wheel 0x0A, Dir 0x02, Speed 0x28, ? 0x00)
 */
struct razer_report razer_chroma_extended_matrix_effect_wheel(unsigned char variable_storage, unsigned char led_id, unsigned char direction)
{
    struct razer_report report = razer_chroma_extended_matrix_effect_base(0x06, variable_storage, led_id, 0x0a);

    // BlackWidow V4 Pro uses 0x01 and 0x02 for directions
    // Commands with direction 0x00 seem to be ignored
    direction = clamp_u8(direction, 0x01, 0x02);

    report.arguments[3] = direction;
    report.arguments[4] = 0x28; // Speed, lower values are faster

    return report;
}

/**
 * Set the device to "Reactive" effect
 *
 * Status Trans Packet Proto DataSize Class CMD Args
 * 00     3f    0000   00    09       0f    02  010505000101ffff00 | SET LED MATRIX Effect (VARSTR, Backlight, Reactive 0x05, ? 0x00, Speed 0x01, Colours 0x01, RGB 0xFFFF00)
 * 00     3f    0000   00    09       0f    02  010505000101ff0000 | SET LED MATRIX Effect (VARSTR, Backlight, Reactive 0x05, ? 0x00, Speed 0x02, Colours 0x01, RGB 0xFF0000)
 * 00     3f    0000   00    09       0f    02  010505000301ff0000 | SET LED MATRIX Effect (VARSTR, Backlight, Reactive 0x05, ? 0x00, Speed 0x03, Colours 0x01, RGB 0xFF0000)
 * 00     3f    0000   00    09       0f    02  010505000401ff0000 | SET LED MATRIX Effect (VARSTR, Backlight, Reactive 0x05, ? 0x00, Speed 0x04, Colours 0x01, RGB 0xFF0000)
 */
struct razer_report razer_chroma_extended_matrix_effect_reactive(unsigned char variable_storage, unsigned char led_id, unsigned char speed, struct razer_rgb *rgb)
{
    struct razer_report report = razer_chroma_extended_matrix_effect_base(0x09, variable_storage, led_id, 0x05);

    speed = clamp_u8(speed, 0x01, 0x04);

    report.arguments[4] = speed;
    report.arguments[5] = 0x01;
    report.arguments[6] = rgb->r;
    report.arguments[7] = rgb->g;
    report.arguments[8] = rgb->b;

    return report;
}

/**
 * Set the device to "Breathing" effect
 *
 * Status Trans Packet Proto DataSize Class CMD Args
 * 00     3f    0000   00    09       0f    02  01050201000100ff00       | SET LED MATRIX Effect (VARSTR, Backlight, Breathing 0x02, Colours 0x01, ? 0x00, Colours 0x01, RGB 0x00FF00)
 * 00     3f    0000   00    0c       0f    02  01050202000200ff00ff0000 | SET LED MATRIX Effect (VARSTR, Backlight, Breathing 0x02, Colours 0x02, ? 0x00, Colours 0x02, RGB 0x00FF00, RGB 0xFF0000)
 * 00     3f    0000   00    06       0f    02  010502000000             | SET LED MATRIX Effect (VARSTR, Backlight, Breathing 0x02, Colours 0x00, ? 0x0000)
 */
struct razer_report razer_chroma_extended_matrix_effect_breathing_random(unsigned char variable_storage, unsigned char led_id)
{
    struct razer_report report = razer_chroma_extended_matrix_effect_base(0x06, variable_storage, led_id, 0x02);
    return report;
}
struct razer_report razer_chroma_extended_matrix_effect_breathing_single(unsigned char variable_storage, unsigned char led_id, struct razer_rgb *rgb1)
{
    struct razer_report report = razer_chroma_extended_matrix_effect_base(0x09, variable_storage, led_id, 0x02);

    report.arguments[3] = 0x01;
    report.arguments[5] = 0x01;

    report.arguments[6] = rgb1->r;
    report.arguments[7] = rgb1->g;
    report.arguments[8] = rgb1->b;

    return report;
}
struct razer_report razer_chroma_extended_matrix_effect_breathing_dual(unsigned char variable_storage, unsigned char led_id, struct razer_rgb *rgb1, struct razer_rgb *rgb2)
{
    struct razer_report report = razer_chroma_extended_matrix_effect_base(0x0C, variable_storage, led_id, 0x02);

    report.arguments[3] = 0x02;
    report.arguments[5] = 0x02;

    report.arguments[6] = rgb1->r;
    report.arguments[7] = rgb1->g;
    report.arguments[8] = rgb1->b;
    report.arguments[9] = rgb2->r;
    report.arguments[10] = rgb2->g;
    report.arguments[11] = rgb2->b;

    return report;
}

/**
 * Set the device to "Custom" effect
 *
 * Status Trans Packet Proto DataSize Class CMD Args
 * 00     3f    0000   00    0c       0f    02  000008000000000000000000   | DRAW LED MATRIX Frame
 */
struct razer_report razer_chroma_extended_matrix_effect_custom_frame(void)
{
    return razer_chroma_extended_matrix_effect_base(0x0C, 0x00, 0x00, 0x08);
}

/**
 * Set the device brightness
 *
 * Status Trans Packet Proto DataSize Class CMD Args
 * 00     3f    0000   00    03       0f    04  0104b7
 */
struct razer_report razer_chroma_extended_matrix_brightness(unsigned char variable_storage, unsigned char led_id, unsigned char brightness)
{
    struct razer_report report = get_razer_report(0x0F, 0x04, 0x03);

    report.arguments[0] = variable_storage;
    report.arguments[1] = led_id;
    report.arguments[2] = brightness;

    return report;
}

/**
 * Get the device brightness
 *
 * Status Trans Packet Proto DataSize Class CMD Args
 * 00     3f    0000   00    03       0f    84  0104
 */
struct razer_report razer_chroma_extended_matrix_get_brightness(unsigned char variable_storage, unsigned char led_id)
{
    struct razer_report report = get_razer_report(0x0F, 0x84, 0x03);

    report.arguments[0] = variable_storage;
    report.arguments[1] = led_id;

    return report;
}

/**
 * Set the RGB or a row
 *
 * Start and stop columns are inclusive
 */
struct razer_report razer_chroma_extended_matrix_set_custom_frame(unsigned char row_index, unsigned char start_col, unsigned char stop_col, unsigned char *rgb_data)
{
    return razer_chroma_extended_matrix_set_custom_frame2(row_index, start_col, stop_col, rgb_data, 0x47);
}

struct razer_report razer_chroma_extended_matrix_set_custom_frame2(unsigned char row_index, unsigned char start_col, unsigned char stop_col, unsigned char *rgb_data, size_t packetLength)
{
    const size_t start_arg_offset = 5;
    size_t data_length = 0;
    struct razer_report report = {0};
    size_t row_length = (size_t) (((stop_col + 1) - start_col) * 3);

    if (row_length > sizeof(report.arguments) - start_arg_offset) {
        printk(KERN_ALERT "razerchroma: RGB data too long\n");
        row_length = 0;
    }

    // Some devices need a specific packet length, most devices are happy with 0x47
    // e.g. the Mamba Elite needs a "row_length + 5" packet length
    data_length = (packetLength != 0) ? packetLength : row_length + 5;
    report = get_razer_report(0x0F, 0x03, data_length);

    // printk(KERN_ALERT "razerkbd: Row ID: %d, Start: %d, Stop: %d, row length: %d\n", row_index, start_col, stop_col, (unsigned char)row_length);

    report.arguments[2] = row_index;
    report.arguments[3] = start_col;
    report.arguments[4] = stop_col;
    memcpy(&report.arguments[5], rgb_data, row_length);

    return report;
}

/*
 * Extended Matrix Effects (Mouse)
 */
/**
 * Sets up the extended matrix effect payload for mouse devices
 */
static struct razer_report razer_chroma_mouse_extended_matrix_effect_base(unsigned char arg_size, unsigned char variable_storage, unsigned char led_id, unsigned char effect_id)
{
    struct razer_report report = get_razer_report(0x03, 0x0D, arg_size);

    report.arguments[0] = variable_storage;
    report.arguments[1] = led_id;
    report.arguments[2] = effect_id; // Effect ID

    return report;
}

/**
 * Set the device to "None" effect
 *
 * Status Trans Packet Proto DataSize Class CMD Args
 *  * 00     3f    0000   00    03       03    0d  010100 | SET Extended Matrix Effect (VARSTORE, LOGO_LED, OFF)
 */
struct razer_report razer_chroma_mouse_extended_matrix_effect_none(unsigned char variable_storage, unsigned char led_id)
{
    return razer_chroma_mouse_extended_matrix_effect_base(0x03, variable_storage, led_id, 0x00);
}

/**
 * Set the device to "Static" effect
 *
 * Status Trans Packet Proto DataSize Class CMD Args
 * 00     3f    0000   00    06       03    0d  010106 00ff00 | SET Extended Matrix Effect (VARSTORE, SCROLL_WHEEL, STATIC, RGB)
 */
struct razer_report razer_chroma_mouse_extended_matrix_effect_static(unsigned char variable_storage, unsigned char led_id, struct razer_rgb *rgb)
{
    struct razer_report report = razer_chroma_mouse_extended_matrix_effect_base(0x06, variable_storage, led_id, 0x06);

    report.arguments[3] = rgb->r;
    report.arguments[4] = rgb->g;
    report.arguments[5] = rgb->b;
    return report;
}

/**
 * Set the device to "Spectrum" effect
 *
 * Status Trans Packet Proto DataSize Class CMD Args
 * 00     3f    0000   00    03       03    0d  010104 | SET Extended Matrix Effect (VARSTORE, LOGO_LED, SPECTRUM)
 */
struct razer_report razer_chroma_mouse_extended_matrix_effect_spectrum(unsigned char variable_storage, unsigned char led_id)
{
    return razer_chroma_mouse_extended_matrix_effect_base(0x03, variable_storage, led_id, 0x04);
}

/**
 * Set the device to "Reactive" effect
 *
 * Status Trans Packet Proto DataSize Class CMD Args
 * 00     3f    0000   00    07       03    0d  010102 0300ff00            | SET Extended Matrix Effect (VARSTORE, SCROLL_WHEEL,  REACTIVE, TIME, RGB)
 * 00     3f    0000   00    07       03    0d  010102 0200ff00            | SET Extended Matrix Effect (VARSTORE, SCROLL_WHEEL,  REACTIVE, TIME, RGB)
 * 00     3f    0000   00    07       03    0d  010102 0100ff00            | SET Extended Matrix Effect (VARSTORE, SCROLL_WHEEL,  REACTIVE, TIME, RGB)
 */
struct razer_report razer_chroma_mouse_extended_matrix_effect_reactive(unsigned char variable_storage, unsigned char led_id, unsigned char speed, struct razer_rgb *rgb)
{
    struct razer_report report = razer_chroma_mouse_extended_matrix_effect_base(0x07, variable_storage, led_id, 0x02);

    speed = clamp_u8(speed, 0x01, 0x04);

    report.arguments[3] = speed;
    report.arguments[4] = rgb->r;
    report.arguments[5] = rgb->g;
    report.arguments[6] = rgb->b;

    return report;
}

/**
 * Set the device to "Breathing" effect
 *
 * Status Trans Packet Proto DataSize Class CMD Args
 * 00     3f    0000   00    0a       03    0d  010103 0100ff00000000      | SET Extended Matrix Effect (VARSTORE, SCROLL_WHEEL, BREATHING, single, RGB, RGB-none)
 * 00     3f    0000   00    0a       03    0d  010103 0200ff00ff0000      | SET Extended Matrix Effect (VARSTORE, SCROLL_WHEEL, BREATHING, dual, RGB, RGB)
 * 00     3f    0000   00    0a       03    0d  010103 03000000000000      | SET Extended Matrix Effect (VARSTORE, SCROLL_WHEEL, BREATHING, random, RGB-none, RGB-none)
 */
struct razer_report razer_chroma_mouse_extended_matrix_effect_breathing_random(unsigned char variable_storage, unsigned char led_id)
{
    struct razer_report report = razer_chroma_mouse_extended_matrix_effect_base(0x0A, variable_storage, led_id, 0x03);

    report.arguments[3] = 0x03;

    return report;
}
struct razer_report razer_chroma_mouse_extended_matrix_effect_breathing_single(unsigned char variable_storage, unsigned char led_id, struct razer_rgb *rgb1)
{
    struct razer_report report = razer_chroma_mouse_extended_matrix_effect_base(0x0A, variable_storage, led_id, 0x03);

    report.arguments[3] = 0x01;

    report.arguments[4] = rgb1->r;
    report.arguments[5] = rgb1->g;
    report.arguments[6] = rgb1->b;

    return report;
}
struct razer_report razer_chroma_mouse_extended_matrix_effect_breathing_dual(unsigned char variable_storage, unsigned char led_id, struct razer_rgb *rgb1, struct razer_rgb *rgb2)
{
    struct razer_report report = razer_chroma_mouse_extended_matrix_effect_base(0x0A, variable_storage, led_id, 0x03);

    report.arguments[3] = 0x02;

    report.arguments[4] = rgb1->r;
    report.arguments[5] = rgb1->g;
    report.arguments[6] = rgb1->b;

    report.arguments[7] = rgb2->r;
    report.arguments[8] = rgb2->g;
    report.arguments[9] = rgb2->b;

    return report;
}

/*
 * Misc Functions
 */
/**
 * Toggled whether F1-12 act as F1-12 or if they act as the function options (without Fn pressed)
 *
 * If 0 should mean that the F-keys work as normal F-keys
 * If 1 should mean that the F-keys act as if the FN key is held
 */
struct razer_report razer_chroma_misc_fn_key_toggle(unsigned char state)
{
    struct razer_report report = get_razer_report(0x02, 0x06, 0x02);
    report.arguments[0] = 0x00; // ?? Variable storage maybe
    report.arguments[1] = clamp_u8(state, 0x00, 0x01); // State

    return report;
}

/**
 * Set the keyswitch optimization on the device (first of two commands)
 *
 * Status Trans Packet Proto DataSize Class CMD Args
 * 00     1f    0000   00    04       02    02  001400280000               | SET KEY OPTIMIZATION STATE (TYPING SET)
 * 00     1f    0000   00    04       02    02  000000000000               | SET KEY OPTIMIZATION STATE (GAMING SET)
 */
struct razer_report razer_chroma_misc_set_keyswitch_optimization_command1(unsigned char optimization_mode)
{
    struct razer_report report = get_razer_report(0x02, 0x02, 0x04); // class, id, data size

    // 0x00 -> Typing (Set)
    // 0x01 -> Gaming (Set) - Same report, doesn't include any arguments
    if(optimization_mode == 0x00) {
        report.arguments[0] = 0x00;
        report.arguments[1] = 0x14;
        report.arguments[2] = 0x00;
        report.arguments[3] = 0x28;
        report.arguments[4] = 0x00;
    }

    return report;
}

/**
* Set the keyswitch optimization on the device (second of two commands)
 *
 * Status Trans Packet Proto DataSize Class CMD Args
 * 00     1f    0000   00    05       02    15  010014002800               | SET KEY OPTIMIZATION STATE (TYPING VARSTORE) ????
 * 00     1f    0000   00    05       02    15  010000000000               | SET KEY OPTIMIZATION STATE (GAMING VARSTORE) ????
 */
struct razer_report razer_chroma_misc_set_keyswitch_optimization_command2(unsigned char optimization_mode)
{
    struct razer_report report = get_razer_report(0x02, 0x15, 0x05); // class, id, data size

    // 0x00 -> Typing (Store)
    // 0x01 -> Gaming (Store)
    switch(optimization_mode) {
    case 0x00:
        report.arguments[0] = 0x01;
        report.arguments[1] = 0x00;
        report.arguments[2] = 0x14;
        report.arguments[3] = 0x00;
        report.arguments[4] = 0x28;
        report.arguments[5] = 0x00;
        break;
    case 0x01:
        report.arguments[0] = 0x01;
        break;
    }

    return report;
}

/**
 * Get the keyswitch optimization on the device
 *
 * Identifiers in arg[1] and arg[3]
 *
 * 0x00<->0x14 is in arg[1]
 * 0x00<->0x28 is in arg[3]
 */
struct razer_report razer_chroma_misc_get_keyswitch_optimization(void)
{
    struct razer_report report = get_razer_report(0x02, 0x82, 0x04); // class, id, data size

    return report;
}

/**
 * Set the brightness of an LED on the device
 *
 * Status Trans Packet Proto DataSize Class CMD Args
 * ? TODO fill this
 */
struct razer_report razer_chroma_misc_set_blade_brightness(unsigned char brightness)
{
    struct razer_report report = get_razer_report(0x0E, 0x04, 0x02);
    report.arguments[0] = 0x01;
    report.arguments[1] = brightness;

    return report;
}

/**
 * Get the brightness of an LED on the device
 *
 * Status Trans Packet Proto DataSize Class CMD Args
 * ? TODO fill this
 */
struct razer_report razer_chroma_misc_get_blade_brightness(void)
{
    struct razer_report report = get_razer_report(0x0E, 0x84, 0x02);
    report.arguments[0] = 0x01;

    return report;
}

/**
 * Sets custom frame for the firefly
 */
struct razer_report razer_chroma_misc_one_row_set_custom_frame(unsigned char start_col, unsigned char stop_col, unsigned char *rgb_data) // TODO recheck custom frame hex
{
    const size_t start_arg_offset = 2;
    struct razer_report report = get_razer_report(0x03, 0x0C, 0x32);
    size_t row_length = (size_t) (((stop_col + 1) - start_col) * 3);

    if (row_length > sizeof(report.arguments) - start_arg_offset) {
        printk(KERN_ALERT "razerchroma: RGB data too long\n");
        row_length = 0;
    }

    report.arguments[0] = start_col;
    report.arguments[1] = stop_col;

    memcpy(&report.arguments[2], rgb_data, row_length);

    return report;
}

/**
 * Trigger reactive on Firefly
 */
struct razer_report razer_chroma_misc_matrix_reactive_trigger(void)
{
    struct razer_report report = razer_chroma_standard_matrix_effect_base(0x05, MATRIX_EFFECT_REACTIVE);
    report.arguments[1] = 0; // this speed triggers reactive
    report.arguments[2] = 0;
    report.arguments[3] = 0;
    report.arguments[4] = 0;

    return report;
}

/**
 * Gets battery level
 *
 * 0->255 is in arg[1]
 */
struct razer_report razer_chroma_misc_get_battery_level(void)
{
    return get_razer_report(0x07, 0x80, 0x02);
}

/**
 * Gets charging status
 *
 * 0->1 is in arg[1]
 */
struct razer_report razer_chroma_misc_get_charging_status(void)
{
    return get_razer_report(0x07, 0x84, 0x02);
}

/**
 * Set the charging effect, think if I remember correctly, it's either static colour, or "whatever the mouse was last on"
 */
struct razer_report razer_chroma_misc_set_dock_charge_type(unsigned char charge_type)
{
    struct razer_report report = get_razer_report(0x03, 0x10, 0x01);
    report.arguments[0] = clamp_u8(charge_type, 0x00, 0x01);

    return report;
}

/**
 * Get the polling rate from the device
 *
 * Identifier is in arg[0]
 *
 * 0x01 = 1000Hz
 * 0x02 =  500Hz
 * 0x08 =  125Hz
 */
struct razer_report razer_chroma_misc_get_polling_rate(void)
{
    return get_razer_report(0x00, 0x85, 0x01);
}

/**
 * Set the polling rate of the device
 *
 * 0x01 = 1000Hz
 * 0x02 =  500Hz
 * 0x08 =  125Hz
 */
struct razer_report razer_chroma_misc_set_polling_rate(unsigned short polling_rate)
{
    struct razer_report report = get_razer_report(0x00, 0x05, 0x01);

    switch(polling_rate) {
    case 1000:
        report.arguments[0] = 0x01;
        break;
    case  500:
        report.arguments[0] = 0x02;
        break;
    case  125:
        report.arguments[0] = 0x08;
        break;
    default: // 500Hz
        report.arguments[0] = 0x02;
        break;
    }

    return report;
}

/**
 * Get the polling rate from the device
 *
 * Identifier is in arg[1]
 *
 * 0x01 = 8000Hz
 * 0x02 = 4000Hz
 * 0x04 = 2000Hz
 * 0x08 = 1000Hz
 * 0x10 =  500Hz
 * 0x40 =  125Hz
 */
struct razer_report razer_chroma_misc_get_polling_rate2(void)
{
    return get_razer_report(0x00, 0xC0, 0x01);
}

/**
 * Set the polling rate of the device
 *
 * 0x40 =  125 Hz
 * 0x10 =  500 Hz
 * 0x08 = 1000 Hz
 * 0x04 = 2000 Hz
 * 0x02 = 4000 Hz
 * 0x01 = 8000 Hz
 */
struct razer_report razer_chroma_misc_set_polling_rate2(unsigned short polling_rate, unsigned short argument)
{
    struct razer_report report = get_razer_report(0x00, 0x40, 0x02);

    report.arguments[0] = argument; // For some devices Razer sends each request once with 0x00 and once with 0x01 - maybe varstore?
    switch(polling_rate) {
    case 8000:
        report.arguments[1] = 0x01;
        break;
    case 4000:
        report.arguments[1] = 0x02;
        break;
    case 2000:
        report.arguments[1] = 0x04;
        break;
    case 1000:
        report.arguments[1] = 0x08;
        break;
    case  500:
        report.arguments[1] = 0x10;
        break;
    case  250:
        report.arguments[1] = 0x20;
        break;
    case  125:
        report.arguments[1] = 0x40;
        break;
    default: // 500Hz
        report.arguments[1] = 0x10;
        break;
    }

    return report;
}

/**
 * Get brightness of charging dock
 */
struct razer_report razer_chroma_misc_get_dock_brightness(void)
{
    return get_razer_report(0x07, 0x82, 0x01);

}

/**
 * Set brightness of charging dock
 */
struct razer_report razer_chroma_misc_set_dock_brightness(unsigned char brightness)
{
    struct razer_report report = get_razer_report(0x07, 0x02, 0x01);
    report.arguments[0] = brightness;

    return report;
}

/**
 * Set the DPI of the device
 */
struct razer_report razer_chroma_misc_set_dpi_xy(unsigned char variable_storage, unsigned short dpi_x,unsigned short dpi_y)
{
    struct razer_report report = get_razer_report(0x04, 0x05, 0x07);

    // Keep the DPI within bounds
    dpi_x = clamp_u16(dpi_x, 100, 35000);
    dpi_y = clamp_u16(dpi_y, 100, 35000);

    report.arguments[0] = VARSTORE;

    report.arguments[1] = (dpi_x >> 8) & 0x00FF;
    report.arguments[2] = dpi_x & 0x00FF;
    report.arguments[3] = (dpi_y >> 8) & 0x00FF;
    report.arguments[4] = dpi_y & 0x00FF;
    report.arguments[5] = 0x00;
    report.arguments[6] = 0x00;

    return report;
}

/**
 * Get the DPI of the device
 */
struct razer_report razer_chroma_misc_get_dpi_xy(unsigned char variable_storage)
{
    struct razer_report report = get_razer_report(0x04, 0x85, 0x07);

    report.arguments[0] = variable_storage;

    return report;
}

/**
 * Set the DPI of the device (Some stupid turd scaled 5600 dpi into a single byte)
 */
struct razer_report razer_chroma_misc_set_dpi_xy_byte(unsigned char dpi_x,unsigned char dpi_y)
{
    struct razer_report report = get_razer_report(0x04, 0x01, 0x03);

    report.arguments[0] = dpi_x;
    report.arguments[1] = dpi_y;
    report.arguments[2] = 0x00;

    return report;
}

/**
 * Get the DPI of the device (Some stupid turd scaled 5600 dpi into a single byte)
 */
struct razer_report razer_chroma_misc_get_dpi_xy_byte(void)
{
    struct razer_report report = get_razer_report(0x04, 0x81, 0x03);

    return report;
}

/**
 * Set DPI stages of the device.
 *
 * count is the number of stages to set.
 * active_stage selected stage number.
 * dpi is an array of size 2 * count containing pairs of dpi x and dpi y
 * values, one pair for each stage.
 *
 * E.g.:
 *   count = 3
 *   active_stage = 1
 *   dpi = [ 800, 800, 1800, 1800, 3200, 3200]
 *         | stage 1*|  stage 2  |  stage 3  |
 */
struct razer_report razer_chroma_misc_set_dpi_stages(unsigned char variable_storage, unsigned char count, unsigned char active_stage, const unsigned short *dpi)
{
    struct razer_report report = get_razer_report(0x04, 0x06, 0x26);
    unsigned int offset;
    unsigned int i;

    report.arguments[0] = variable_storage;
    report.arguments[1] = active_stage;
    report.arguments[2] = count;

    offset = 3;
    for (i = 0; i < count; i++) {
        // Stage number
        report.arguments[offset++] = i;

        // DPI X
        report.arguments[offset++] = (dpi[0] >> 8) & 0x00FF;
        report.arguments[offset++] = dpi[0] & 0x00FF;

        // DPI Y
        report.arguments[offset++] = (dpi[1] >> 8) & 0x00FF;
        report.arguments[offset++] = dpi[1] & 0x00FF;

        // Reserved
        report.arguments[offset++] = 0;
        report.arguments[offset++] = 0;

        dpi += 2;
    }

    return report;
}

/**
 * Get the DPI stages of the device
 */
struct razer_report razer_chroma_misc_get_dpi_stages(unsigned char variable_storage)
{
    struct razer_report report = get_razer_report(0x04, 0x86, 0x26);

    report.arguments[0] = variable_storage;

    return report;
}

/**
 * Get device idle time
 */
struct razer_report razer_chroma_misc_get_idle_time(void)
{
    return get_razer_report(0x07, 0x83, 0x02);
}

/**
 * Set device idle time
 *
 * Device will go into powersave after this time.
 *
 * Idle time is in seconds, must be between 60sec-900sec
 */
struct razer_report razer_chroma_misc_set_idle_time(unsigned short idle_time)
{
    struct razer_report report = get_razer_report(0x07, 0x03, 0x02);

    // Keep the idle time within bounds
    idle_time = clamp_u16(idle_time, 60, 900);

    report.arguments[0] = (idle_time >> 8) & 0x00FF;
    report.arguments[1] = idle_time & 0x00FF;

    return report;
}

/**
 * Get low battery threshold
 */
struct razer_report razer_chroma_misc_get_low_battery_threshold(void)
{
    return get_razer_report(0x07, 0x81, 0x01);
}

/**
 * Set low battery threshold
 *
 * 0x3F = 25%
 * 0x26 = 15%
 * 0x0C =  5%
 */
struct razer_report razer_chroma_misc_set_low_battery_threshold(unsigned char battery_threshold)
{
    struct razer_report report = get_razer_report(0x07, 0x01, 0x01);

    // Keep the battery threshold within bounds
    battery_threshold = clamp_u8(battery_threshold, 0x0C, 0x3F);

    report.arguments[0] = battery_threshold;

    return report;
}

struct razer_report razer_chroma_misc_set_orochi2011_led(unsigned char led_bitfield)
{
    struct razer_report report = {0};
    memcpy(&report, &orochi2011_led, sizeof(orochi2011_led));

    report.arguments[1] = led_bitfield;

    return report;
}

struct razer_report razer_chroma_misc_set_orochi2011_poll_dpi(unsigned short poll_rate, unsigned char dpi_x, unsigned char dpi_y)
{
    struct razer_report report = {0};
    memcpy(&report, &orochi2011_dpi, sizeof(orochi2011_dpi));

    switch(poll_rate) {
    case 1000:
        poll_rate = 0x01;
        break;
    case  500:
        poll_rate = 0x02;
        break;
    case  125:
        poll_rate = 0x08;
        break;
    default: // 500Hz
        poll_rate = 0x02;
        break;
    }

    report.arguments[1] = poll_rate;

    report.arguments[3] = clamp_u8(dpi_x, 0x15, 0x9C);
    report.arguments[4] = clamp_u8(dpi_y, 0x15, 0x9C);

    return report;
}

/**
 * Set the Naga Trinity to "Static" effect
 */
struct razer_report razer_naga_trinity_effect_static(struct razer_rgb *rgb)
{
    struct razer_report report = get_razer_report(0x0f, 0x03, 0x0e);

    report.arguments[0] = 0x00; // Variable storage
    report.arguments[1] = 0x00; // LED ID
    report.arguments[2] = 0x00; // Unknown
    report.arguments[3] = 0x00; // Unknown
    report.arguments[4] = 0x02; // Effect ID
    report.arguments[5] = rgb->r; // RGB 3x
    report.arguments[6] = rgb->g;
    report.arguments[7] = rgb->b;
    report.arguments[8] = rgb->r;
    report.arguments[9] = rgb->g;
    report.arguments[10] = rgb->b;
    report.arguments[11] = rgb->r;
    report.arguments[12] = rgb->g;
    report.arguments[13] = rgb->b;

    return report;
}

/**
 * Set scroll wheel mode on the device
 *
 * Status Trans Packet Proto DataSize Class CMD Args
 * 00     1f    0000   00    02       02    14  0100    | SET SCROLL WHEEL MODE (VARSTR, TACTILE)
 * 00     1f    0000   00    02       02    14  0101    | SET SCROLL WHEEL MODE (VARSTR, FREESPIN)
 */
struct razer_report razer_chroma_misc_set_scroll_mode(unsigned int scroll_mode)
{
    struct razer_report report = get_razer_report(0x02, 0x14, 0x02);

    report.arguments[0] = VARSTORE;
    report.arguments[1] = scroll_mode;

    return report;
}

/**
 * Get scroll wheel mode from the device
 */
struct razer_report razer_chroma_misc_get_scroll_mode(void)
{
    struct razer_report report = get_razer_report(0x02, 0x94, 0x02);

    report.arguments[0] = VARSTORE;

    return report;
}

/**
 * Set scroll wheel acceleration on/off on the device
 *
 * Status Trans Packet Proto DataSize Class CMD Args
 * 00     1f    0000   00    02       02    16  0101    | SET SCROLL WHEEL ACCELERATION (VARSTR, ON)
 * 00     1f    0000   00    02       02    16  0100    | SET SCROLL WHEEL ACCELERATION (VARSTR, OFF)
 */
struct razer_report razer_chroma_misc_set_scroll_acceleration(bool acceleration)
{
    struct razer_report report = get_razer_report(0x02, 0x16, 0x02);

    report.arguments[0] = VARSTORE;
    report.arguments[1] = acceleration;

    return report;
}

/**
 * Get scroll wheel acceleration state from the device
 */
struct razer_report razer_chroma_misc_get_scroll_acceleration(void)
{
    struct razer_report report = get_razer_report(0x02, 0x96, 0x02);

    report.arguments[0] = VARSTORE;

    return report;
}

/**
 * Set scroll wheel "smart reel" on/off on the device.
 * Smart reel automatically changes scroll wheel mode from tactile to free spin and back depending on scroll speed.
 *
 * Status Trans Packet Proto DataSize Class CMD Args
 * 00     1f    0000   00    02       02    17  0101    | SET SCROLL WHEEL SMART REEL (VARSTR, ON)
 * 00     1f    0000   00    02       02    17  0100    | SET SCROLL WHEEL SMART REEL (VARSTR, OFF)
 */
struct razer_report razer_chroma_misc_set_scroll_smart_reel(bool smart_reel)
{
    struct razer_report report = get_razer_report(0x02, 0x17, 0x02);

    report.arguments[0] = VARSTORE;
    report.arguments[1] = smart_reel;

    return report;
}

/**
 * Get scroll wheel "smart reel" state from the device
 */
struct razer_report razer_chroma_misc_get_scroll_smart_reel(void)
{
    struct razer_report report = get_razer_report(0x02, 0x97, 0x02);

    report.arguments[0] = VARSTORE;

    return report;
}

/**
 * Set LED mode for HyperPolling Wireless Dongle
 * 1 = Connection Status
 * 2 = Battery Status
 * 3 = Battery Warning Only
 */
struct razer_report razer_chroma_misc_set_hyperpolling_wireless_dongle_indicator_led_mode(unsigned char mode)
{
    struct razer_report report = get_razer_report(0x07, 0x10, 0x01);

    if(mode < 0x01 || mode > 0x03) {
        mode = 0x01;
    }

    report.arguments[0] = mode;

    return report;
}

/**
 * Set pairing mode for HyperPolling Wireless Dongle (step 1 of pairing)
 */
struct razer_report razer_chroma_misc_set_hyperpolling_wireless_dongle_pair_step1(unsigned short pid)
{
    struct razer_report report;

    report = get_razer_report(0x00, 0x46, 0x01);
    report.arguments[0] = 0x01;

    return report;
}

/**
 * Pair HyperPolling Wireless Dongle with PID (step 2 of pairing)
 */
struct razer_report razer_chroma_misc_set_hyperpolling_wireless_dongle_pair_step2(unsigned short pid)
{
    struct razer_report report;

    report = get_razer_report(0x00, 0x41, 0x03);
    report.arguments[0] = 0x01;
    report.arguments[1] = (pid >> 8) & 0xFF;
    report.arguments[2] = pid & 0xFF;

    return report;
}

/**
 * Set unpairing mode for HyperPolling Wireless Dongle
 */
struct razer_report razer_chroma_misc_set_hyperpolling_wireless_dongle_unpair(unsigned short pid)
{
    struct razer_report report = get_razer_report(0x00, 0x42, 0x02);

    report.arguments[0] = (pid >> 8) & 0xFF;
    report.arguments[1] = pid & 0xFF;

    return report;
}
