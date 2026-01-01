/*
Copyright 2019 @foostan
Copyright 2020 Drashna Jaelre <@drashna>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H

#include "layers.h"

// Left-hand home row mods for Colemak
#define CMH_Z LGUI_T(KC_Z)
#define CMH_X LALT_T(KC_X)
#define CMH_C LSFT_T(KC_C)
#define CMH_D LCTL_T(KC_D)

// Right-hand home row mods for Colemak
#define CMH_SLSH RGUI_T(KC_SLSH)
#define CMH_DOT LALT_T(KC_DOT)
#define CMH_COMM RSFT_T(KC_COMM)
#define CMH_H RCTL_T(KC_H)

// Left-hand home row mods for QWERTY
#define QMH_Z LGUI_T(KC_Z)
#define QMH_X LALT_T(KC_X)
#define QMH_C LSFT_T(KC_C)
#define QMH_V LCTL_T(KC_V)

// Right-hand home row mods for QWERTY
#define QMH_SLSH RGUI_T(KC_SLSH)
#define QMH_DOT LALT_T(KC_DOT)
#define QMH_COMM RSFT_T(KC_COMM)
#define QMH_M RCTL_T(KC_M)

#define B_CONF LT(_CONF, KC_B)
#define T_CONF LT(_CONF, KC_T)
#define SPC_NUM LT(_NUMBER,KC_SPC)
#define ENT_NAV LT(_NAV, KC_ENT)

#define QWERTY DF(_QWERTY)
#define COLEMK DF(_COLEMAK)

#define DEFAULT_LAYER _COLEMAK

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] =
{
    [_BASE] = LAYOUT_split_3x6_3_ex2(
        //,-----------------------------------------------------. --------  -------- ,-----------------------------------------------------.
            QK_GESC, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, QK_BOOT,   QK_BOOT,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_BSPC,
        //|--------+--------+--------+--------+--------+--------| --------  -------- |--------+--------+--------+--------+--------+--------|
            KC_LCTL, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,   KC_K,     KC_K,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_QUOT,
        //|--------+--------+--------+--------+--------+--------| --------  -------- |--------+--------+--------+--------+--------+--------|
            KC_LSFT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  KC_ESC,
        //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                                KC_LGUI, KC_BSPC, SPC_NUM,     KC_ENT,  MO(_RAISE), KC_RALT
        //                                    `--------------------------'  `--------------------------'
    ),

    [_QWERTY] = LAYOUT_split_3x6_3_ex2(
        //,-----------------------------------------------------. --------  -------- ,-----------------------------------------------------.
            _______,    KC_Q,    KC_W,    KC_E,    KC_R,  T_CONF, _______,   _______,     KC_Y,    KC_U,    KC_I,    KC_O,    KC_P, _______,
        //|--------+--------+--------+--------+--------+--------| --------  -------- |--------+--------+--------+--------+--------+--------|
            _______,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G, _______,   _______,     KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, _______,
        //|--------+--------+--------+--------+--------+--------| --------  -------- |--------+--------+--------+--------+--------+--------|
            _______,   QMH_Z,   QMH_X,   QMH_C,   QMH_V,    KC_B,                         KC_N,   QMH_M,QMH_COMM, QMH_DOT,QMH_SLSH, _______,
        //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                                _______, _______, _______,    _______, _______, _______
        //                                    `--------------------------'  `--------------------------'
    ),

    [_COLEMAK] = LAYOUT_split_3x6_3_ex2(
        //,-----------------------------------------------------. --------  -------- ,-----------------------------------------------------.
            _______,    KC_Q,    KC_W,    KC_F,    KC_P,  B_CONF, _______,   _______,     KC_J,    KC_L,    KC_U,    KC_Y, KC_SCLN, _______,
        //|--------+--------+--------+--------+--------+--------| --------  -------- |--------+--------+--------+--------+--------+--------|
            _______,    KC_A,    KC_R,    KC_S,    KC_T,    KC_G, _______,   _______,     KC_M,    KC_N,    KC_E,    KC_I,    KC_O, _______,
        //|--------+--------+--------+--------+--------+--------| --------  -------- |--------+--------+--------+--------+--------+--------|
            _______,   CMH_Z,   CMH_X,   CMH_C,   CMH_D,    KC_V,                         KC_K,   CMH_H,CMH_COMM, CMH_DOT,CMH_SLSH, _______,
        //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                                _______, _______, _______,    _______, _______, _______
        //                                    `--------------------------'  `--------------------------'
    ),

    [_NUMBER] = LAYOUT_split_3x6_3_ex2(
        //,-----------------------------------------------------. --------  -------- ,-----------------------------------------------------.
            XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,   KC_NO,     KC_NO,  KC_LBRC,    KC_7,    KC_8,    KC_9, KC_RBRC, KC_BSPC,
        //|--------+--------+--------+--------+--------+--------| --------  -------- |--------+--------+--------+--------+--------+--------|
            KC_LCTL, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,   KC_NO,     KC_NO,  KC_SCLN,    KC_4,    KC_5,    KC_6,  KC_EQL, XXXXXXX,
        //|--------+--------+--------+--------+--------+--------| --------  -------- |--------+--------+--------+--------+--------+--------|
            KC_LSFT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_TILD,    KC_1,    KC_2,    KC_3,    KC_0, XXXXXXX,
        //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                                KC_LGUI, _______,  KC_SPC,     KC_ENT,   MO(_CONF), KC_DOT
        //                                    `--------------------------'  `--------------------------'
    ),

    [_RAISE] = LAYOUT_split_3x6_3_ex2(
        //,-----------------------------------------------------. --------  -------- ,-----------------------------------------------------.
             KC_TAB, KC_EXLM,   KC_AT, KC_HASH,  KC_DLR, KC_PERC,   KC_NO,     KC_NO,  KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_BSPC,
        //|--------+--------+--------+--------+--------+--------| --------  -------- |--------+--------+--------+--------+--------+--------|
            KC_LCTL, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,   KC_NO,     KC_NO,  KC_MINS,  KC_EQL, KC_LBRC, KC_RBRC, KC_BSLS,  KC_GRV,
        //|--------+--------+--------+--------+--------+--------| --------  -------- |--------+--------+--------+--------+--------+--------|
            KC_LSFT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, KC_PIPE, KC_TILD,
        //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                                KC_LGUI,   MO(_CONF),  KC_SPC,     KC_ENT, _______, KC_RALT
        //                                    `--------------------------'  `--------------------------'
    ),

    [_CONF] = LAYOUT_split_3x6_3_ex2(
        //,-----------------------------------------------------. --------  -------- ,-----------------------------------------------------.
            QK_BOOT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,   KC_NO,     KC_NO,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        //|--------+--------+--------+--------+--------+--------| --------  -------- |--------+--------+--------+--------+--------+--------|
            RM_TOGG, RM_HUEU, RM_SATU, RM_VALU, XXXXXXX, XXXXXXX,   KC_NO,     KC_NO,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        //|--------+--------+--------+--------+--------+--------| --------  -------- |--------+--------+--------+--------+--------+--------|
            RM_NEXT, RM_HUED, RM_SATD, RM_VALD, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                                KC_LGUI, _______,  KC_SPC,     KC_ENT,  QWERTY,  COLEMK
        //                                    `--------------------------'  `--------------------------'
    )
};

void keyboard_post_init_user(void) {
    default_layer_set(1 << DEFAULT_LAYER);
}

#ifndef LAYER_INDICATOR_BRIGHTNESS_INC
#    define LAYER_INDICATOR_BRIGHTNESS_INC 22
#endif

/* RGB Layer Color Configuration
 *
 * This structure allows you to define:
 * - Default color for each layer
 * - Specific keycodes that should be highlighted with different colors
 *
 * To add a highlight:
 * 1. Add a keycode/color pair to the highlights array
 * 2. Set the highlight_count to the number of entries
 */
typedef struct {
    uint16_t keycode; // Keycode to highlight
    HSV      color;   // Color to use for this keycode
} keycode_highlight_t;

typedef struct {
    HSV                 default_color;   // Default color for this layer
    keycode_highlight_t highlights[8];   // Array of keycode/color pairs (adjust size as needed)
    uint8_t             highlight_count; // Number of highlights defined
} layer_color_config_t;

/* Get the color configuration for a specific layer */
static layer_color_config_t _get_layer_color_config(uint8_t layer) {
    layer_color_config_t config = {0};

    switch (layer) {
        case _QWERTY:
            config.default_color   = (HSV){HSV_RED};
            config.highlight_count = 0;
            break;
        case _COLEMAK:
            config.default_color   = (HSV){HSV_GREEN};
            config.highlight_count = 0;
            break;
        // case _NAV:
        //     config.default_color = (HSV){HSV_BLUE};
        //     // Arrow keys highlighted in white
        //     config.highlights[0]   = (keycode_highlight_t){KC_UP, (HSV){HSV_WHITE}};
        //     config.highlights[1]   = (keycode_highlight_t){KC_DOWN, (HSV){HSV_WHITE}};
        //     config.highlights[2]   = (keycode_highlight_t){KC_LEFT, (HSV){HSV_WHITE}};
        //     config.highlights[3]   = (keycode_highlight_t){KC_RGHT, (HSV){HSV_WHITE}};
        //     config.highlight_count = 4;
        //     break;
        case _NUMBER:
            config.default_color   = (HSV){HSV_WHITE};
            config.highlight_count = 0;
            break;
        // case _FUNCTION:
        //     config.default_color   = (HSV){HSV_YELLOW};
        //     config.highlight_count = 0;
        //     break;
        case _CONF:
            config.default_color   = (HSV){HSV_RED};
            config.highlight_count = 0;
            break;
        case _RAISE:
            config.default_color   = (HSV){HSV_PURPLE};
            config.highlight_count = 0;
            break;
        default:
            config.default_color   = (HSV){HSV_OFF};
            config.highlight_count = 0;
            break;
    }

    return config;
}

/* Get the color for a specific keycode on a layer, checking highlights first */
static HSV _get_keycode_color(uint8_t layer, uint16_t keycode) {
    layer_color_config_t config = _get_layer_color_config(layer);

    // Check if this keycode has a highlight color
    for (uint8_t i = 0; i < config.highlight_count; i++) {
        if (config.highlights[i].keycode == keycode) {
            return config.highlights[i].color;
        }
    }

    // Return default color for this layer
    return config.default_color;
}

/* Layer effects that dynamically control LEDS on different layers to indicate which keys are available
 *
 * NOTE: Any changes to this function must be flashed to both halves.
 */
bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    const uint8_t layer = get_highest_layer(layer_state);

    /* For typing layers light the whole keyboard, just set the hue and keep the matrix effects */
    if (layer <= _COLEMAK) {
        for (uint8_t layer = _BASE; layer < _CONF; layer++) {
            if (default_layer_state & (1 << layer)) {
                layer_color_config_t config = _get_layer_color_config(layer);
                rgblight_sethsv(config.default_color.h, config.default_color.s, config.default_color.v);
            }
        }

        /* For special layers use lighting that reflects the keybindings. */
    } else {
        const RGB off   = hsv_to_rgb((HSV){HSV_OFF});
        uint8_t   layer = get_highest_layer(layer_state);

        for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
            for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
                uint8_t index = g_led_config.matrix_co[row][col];

                if (index >= led_min && index < led_max && index != NO_LED) {
                    uint16_t keycode = keymap_key_to_keycode(layer, (keypos_t){col, row});
                    if (keycode > KC_TRNS) {
                        // Get the color for this keycode (checks highlights first, then defaults)
                        HSV hsv = _get_keycode_color(layer, keycode);

                        // Set brightness to the configured interval brighter than current brightness, clamped to 255
                        // (ie. uint8_t max value). This compensates for the dimmer appearance of the underglow LEDs.
                        hsv.v         = MIN(rgb_matrix_get_val() + LAYER_INDICATOR_BRIGHTNESS_INC, 255);
                        const RGB rgb = hsv_to_rgb(hsv);

                        rgb_matrix_set_color(index, rgb.r, rgb.g, rgb.b);
                    } else {
                        rgb_matrix_set_color(index, off.r, off.g, off.b);
                    }
                }
            }
        }
    }
    return false;
}
