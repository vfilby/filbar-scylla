/*
 * Copyright 2021 Quentin LEBASTARD <qlebastard@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */



 /*
  * Keymap not clearing after changes with VIA?
  * dynamic layer colour: https://github.com/Bastardkb/bastardkb-qmk/blob/bkb-master/keyboards/bastardkb/dilemma/4x6_4/4x6_4.c#L69-L120
  */

#include QMK_KEYBOARD_H
#include "quantum.h"

#ifndef LAYER_INDICATOR_BRIGHTNESS_INC
#    define LAYER_INDICATOR_BRIGHTNESS_INC 22
#endif

enum scylla_layers {
    _QWERTY = 0,
    _NAV = 1,
    _RAISE = 2,
    _COLEMAK = 3,
    _CONF = 4,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    /* QWERTY
     * ,-----------------------------------------.                    ,-----------------------------------------.
     * | ESC  |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  |  `   |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * | Tab  |   Q  |   W  |   E  |   R  |   T  |                    |   Y  |   U  |   I  |   O  |   P  |  -   |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * |LShift|   A  |   S  |   D  |   F  |   G  |                    |   H  |   J  |   K  |   L  |   ;  |  '   |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * |LCTRL |   Z  |   X  |   C  |   V  |   B  |                    |   N  |   M  |   ,  |   .  |   /  |RShift|
     * `------------------------------------------------\      /------------------------------------------------'
     *                             | CTRL | SOC  | MO(1)|      | MO(2)| ENT  | RGUI |
     *                             `--------------------|      |--------------------'
     *                                    | HOME | BKSP |      |  DEL | RALT |
     *                                    `-------------/      \-------------'
     */

    [0] = LAYOUT_split_4x6_5(
        QK_GESC, KC_1, KC_2, KC_3, KC_4, LT(_CONF, KC_5),          KC_6, KC_7, KC_8, KC_9, KC_0, KC_BSPC,
        KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, 			KC_Y, KC_U, KC_I, KC_O, KC_P, KC_MINS,
        KC_LSFT, KC_A, KC_S, KC_D, KC_F, KC_G, 			KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT,
        KC_LCTL, KC_Z, KC_X, KC_C, KC_V, KC_B, 			KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_BSLS,

                        KC_LCTL, KC_SPC, MO(1), 		MO(2), KC_ENT, KC_RGUI,
                                KC_HOME, KC_BSPC, 		KC_DEL, KC_RALT
    ),

    [1] = LAYOUT_split_4x6_5(KC_TILD, KC_EXLM, KC_AT, KC_HASH, KC_DLR, KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_DEL,
                             //---------------------------------------------------------//-----------------------------------------------------------//
                             QK_BOOT, _______, _______, _______, _______, KC_LBRC, KC_RBRC, KC_P7, KC_P8, KC_P9, _______, KC_PLUS,
                             //---------------------------------------------------------//-----------------------------------------------------------//
                             _______, KC_HOME, KC_PGUP, KC_PGDN, KC_END, KC_LPRN, KC_RPRN, KC_P4, KC_P5, KC_P6, KC_MINS, KC_PIPE,
                             //---------------------------------------------------------//-----------------------------------------------------------//
                             _______, _______, _______, _______, _______, _______, _______, KC_P1, KC_P2, KC_P3, KC_EQL, KC_UNDS,
                             //---------------------------------------------------------//-----------------------------------------------------------//
                             KC_LCTL, KC_HOME, KC_TRNS, KC_TRNS, KC_RALT, KC_RGUI, KC_SPC, KC_BSPC, KC_RCTL, KC_ENT),

    /* NAV
     * ,-----------------------------------------.                    ,-----------------------------------------.
     * |  __  |  __  |  __  |  __  |  __  |  __  |                    |  __  |  __  |  __  |  __  |  __  |  __  |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * |  __  |  __  |  __  |  __  |  __  |  __  |                    |  __  |CMD+<-|  UP  |CMD+->| PGUP |  __  |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * |  __  |  __  |  __  |  __  |  __  |  __  |                    |  __  | LEFT | DOWN | RIGHT| PGDN |  __  |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * |  __  |  __  |  __  |  __  |  __  |  __  |                    |  __  |OPT+<-|  __  |OPT+->|  __  |  __  |
     * `------------------------------------------------\      /------------------------------------------------'
     *                             |  __  |  __  |  __  |      |  __  |  __  |  __  |
     *                             `--------------------|      |--------------------'
     *                                    |  __  |  __  |      |  __  |  __  |
     *                                    `-------------/      \-------------'
     */
     [2] = LAYOUT_split_4x6_5(
         KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,       KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
         KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,       KC_NO, G(KC_LEFT), KC_UP, G(KC_RIGHT), KC_PGUP, KC_NO,
         KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,       KC_NO, KC_LEFT, KC_DOWN, KC_RIGHT, KC_PGDN, KC_NO,
         KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,       KC_NO, LOPT(KC_LEFT), KC_NO, LOPT(KC_RIGHT), KC_NO, KC_NO,

                             KC_NO, KC_NO, KC_NO, 		KC_NO, KC_NO, KC_NO,
                                   KC_NO, KC_NO, 		KC_NO, KC_NO
                                  ),

    /* COLEMAK-DH
     * ,-----------------------------------------.                    ,-----------------------------------------.
     * | ESC  |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  |  `   |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * | Tab  |   Q  |   W  |   F  |   P  |   B  |                    |   J  |   L  |   U  |   Y  |   ;  |  -   |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * |LShift|   A  |   R  |   S  |   T  |   G  |                    |   M  |   N  |   E  |   I  |   O  |  '   |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * |LCTRL |   Z  |   X  |   C  |   D  |   V  |                    |   K  |   H  |   ,  |   .  |   /  |   \  |
     * `------------------------------------------------\      /------------------------------------------------'
     *                             | CTRL | SPC  | _NAV |      |_RAISE| ENT  | RGUI |
     *                             `--------------------|      |--------------------'
     *                                    | HOME | BKSP |      |  DEL | RALT |
     *                                    `-------------/      \-------------'
     */

     [_COLEMAK] = LAYOUT_split_4x6_5(
         QK_GESC, KC_1, KC_2, KC_3, KC_4, LT(_CONF, KC_5),          	KC_6, KC_7, KC_8, KC_9, KC_0, KC_BSPC,
         KC_TAB, KC_Q, KC_W, KC_F, KC_P, KC_B, 						KC_J, KC_L, KC_U, KC_Y, KC_SCLN, KC_MINS,
         KC_LSFT, KC_A, KC_R, KC_S, KC_T, KC_G, 						KC_M, KC_N, KC_E, KC_I, KC_O, KC_QUOT,
         KC_LCTL, KC_Z, KC_X, KC_C, KC_D, KC_V, 						KC_K, KC_H, KC_COMM, KC_DOT, KC_SLSH, KC_BSLS,

                             KC_LCTL, KC_SPC, MO(_NAV), 			MO(_RAISE), KC_ENT, KC_RGUI,
                                     KC_HOME, KC_BSPC, 			KC_DEL, KC_RALT
     ),

     /* CONFIGURATION
      * ,-----------------------------------------.                    ,-----------------------------------------.
      * |  __  |  __  |  __  |  __  |  __  |  __  |                    |CLEAR |  __  |  __  |  __  |  __  |  __  |
      * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
      * |  __  |  __  |  __  |  __  |  __  |  __  |                    |  RGB | MOD U| HUE U|  __  |  __  |  __  |
      * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
      * |  __  |  __  |  __  |  __  |  __  |  __  |                    |  __  | MOD D| HUE D|  __  |  __  |  __  |
      * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
      * |  __  |  __  |  __  |  __  |  __  |  __  |                    |  __  |  __  |  __  |  __  |  __  |  __  |
      * `------------------------------------------------\      /------------------------------------------------'
      *                             |  __  |  __  |  __  |      |QWERTY|COLEMK|  __  |
      *                             `--------------------|      |--------------------'
      *                                    |  __  |  __  |      |  __  |  __  |
      *                                    `-------------/      \-------------'
      */
     [_CONF] = LAYOUT_split_4x6_5(
         KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,       QK_CLEAR_EEPROM, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
         KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,       RGB_TOG, RGB_MOD, RGB_HUI, KC_NO, KC_NO, KC_NO,
         KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,       KC_NO, RGB_RMOD, RGB_HUD, KC_NO, KC_NO, KC_NO,
         KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,       KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,

                             KC_NO, KC_NO, KC_NO, 		DF(_QWERTY), DF(_COLEMAK), KC_NO,
                                   KC_NO, KC_NO, 		KC_NO, KC_NO
     ),
};


#ifdef RGB_MATRIX_ENABLE
static HSV _get_hsv_for_layer_index(uint8_t layer) {
    switch (layer) {
        case 1:
            return (HSV){HSV_BLUE};
        case 2:
            return (HSV){HSV_AZURE};
        case 3:
            return (HSV){HSV_GREEN};
        case _CONF:
            return (HSV){HSV_RED};
        case 5:
            return (HSV){HSV_TEAL};
        case 6:
            return (HSV){HSV_PURPLE};
        default:
            return (HSV){HSV_RED};
    };
}

/* Layer effects that dynamically control LEDS on different layers to indicate which keys are available
 *
 */
bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {

    const uint8_t layer = get_highest_layer(layer_state);
    if (layer > 0) {

        HSV hsv = _get_hsv_for_layer_index(layer);

        // Set brightness to the configured interval brighter than current brightness, clamped to 255
        // (ie. uint8_t max value). This compensates for the dimmer appearance of the underglow LEDs.
        hsv.v         = MIN(rgb_matrix_get_val() + LAYER_INDICATOR_BRIGHTNESS_INC, 255);
        const RGB rgb = hsv_to_rgb(hsv);
        const RGB off = hsv_to_rgb((HSV){HSV_OFF});

        uint8_t layer = get_highest_layer(layer_state);

        // for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
        //     for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
        //         uint8_t index = g_led_config.matrix_co[row][col];

        for (int i = led_min; i <= led_max; i++) {
                if (HAS_FLAGS(g_led_config.flags[i], LED_FLAG_UNDERGLOW)) {
                    if( keymap_key_to_keycode(layer, (keypos_t){col,row}) > KC_TRNS ) {
                        rgb_matrix_set_color(index, rgb.r, rgb.g, rgb.b);
                    } else {
                        rgb_matrix_set_color(index, off.r, off.g, off.b);
                    }
                }
            }
        }
                // if (index >= led_min
                //     && index < led_max
                //     && index != NO_LED
                // ) {
                //     if( keymap_key_to_keycode(layer, (keypos_t){col,row}) > KC_TRNS ) {
                //         rgb_matrix_set_color(index, rgb.r, rgb.g, rgb.b);
                //     } else {
                //         rgb_matrix_set_color(index, off.r, off.g, off.b);
                //     }
                // }
        //     }
        // }
    }
    return false;
}
// // Layer state indicator
// bool rgb_matrix_indicators_advanced_kb(uint8_t led_min, uint8_t led_max) {
//     if (!rgb_matrix_indicators_advanced_user(led_min, led_max)) {
//         return false;
//     }
//
//     // Set underglow to a solid color for highest active layer apart from the base layer.
//     const uint8_t layer = get_highest_layer(layer_state);
//     if (layer > 0) {
//         HSV hsv = _get_hsv_for_layer_index(layer);
//
//         // Set brightness to the configured interval brighter than current brightness, clamped to 255 (ie. uint8_t max value). This compensates for the dimmer appearance of the underglow LEDs.
//         hsv.v         = MIN(rgb_matrix_get_val() + LAYER_INDICATOR_BRIGHTNESS_INC, 255);
//         const RGB rgb = hsv_to_rgb(hsv);
//
//         for (int i = led_min; i < led_max; i++) {
//             if (HAS_FLAGS(g_led_config.flags[i], LED_FLAG_UNDERGLOW)) {
//                 rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
//             }
//         }
//     }
//     return true;
// };
#endif // RGB_MATRIX_ENABLE
