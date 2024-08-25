/*
 * Copyright 2024 Vincent Filby (v@filby.ca)
 *
 * Based off the original Scylla keymap and https://github.com/archydragon/lily-layout/blob/main/qmk_keymap/keymap.c
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
  * dynamic layer colour: https://github.com/Bastardkb/bastardkb-qmk/blob/bkb-master/keyboards/bastardkb/dilemma/4x6_4/4x6_4.c#L69-L120
  */

#include QMK_KEYBOARD_H
#include "quantum.h"

#ifdef CONSOLE_ENABLE
#   include "print.h"
#endif // CONSOLE_ENABLE

#ifndef LAYER_INDICATOR_BRIGHTNESS_INC
#    define LAYER_INDICATOR_BRIGHTNESS_INC 22
#endif

enum scylla_layers {
    _BASE = 0,
    _QWERTY,
    _COLEMAK,
    _RAISE,
    _NAV,
    _CONF,
};


/* Set the default layer.
 *
 * Only colemak and qwerty make sense as options.
 */
#define DEFAULT_LAYER _COLEMAK


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {


    /* BASE
     * ,-----------------------------------------.                    ,-----------------------------------------.
     * | ESC  |  __  |  __  |  __  |  __  |  __  |                    |  __  |  __  |  __  |  __  |  __  |  __  |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * | Tab  |  __  |  __  |  __  |  __  |  __  |                    |  __  |  __  |  __  |  __  |  __  |  __  |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * |LShift|  __  |  __  |  __  |  __  |  __  |                    |  __  |  __  |  __  |  __  |  __  |  __  |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * |LCTRL |  __  |  __  |  __  |  __  |  __  |                    |  __  |  __  |  __  |  __  |  __  |  __  |
     * `------------------------------------------------\      /------------------------------------------------'
     *                             | CMD  |  SPC | RAISE|      |  NAV | ENT  | CMD  |
     *                             `--------------------|      |--------------------'
     *                                    | OPT  | BKSP |      |  DEL | RALT |
     *                                    `-------------/      \-------------'
     */
    [_BASE] = LAYOUT_split_4x6_5(
        QK_GESC, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,       KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_TAB,  KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,       KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_LSFT, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,       KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_LCTL, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,       KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,

                       KC_LGUI, KC_SPC, MO(_RAISE),            MO(_NAV), KC_ENT, KC_RGUI,
                               KC_LOPT, KC_BSPC, 		        KC_LSFT, KC_RALT
    ),

    /* QWERTY
     * ,-----------------------------------------.                    ,-----------------------------------------.
     * | TRNS |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  |  `   |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * | TRNS |   Q  |   W  |   E  |   R  |   T  |                    |   Y  |   U  |   I  |   O  |   P  |  -   |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * | TRNS |   A  |   S  |   D  |   F  |   G  |                    |   H  |   J  |   K  |   L  |   ;  |  '   |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * | TRNS |   Z  |   X  |   C  |   V  |   B  |                    |   N  |   M  |   ,  |   .  |   /  |  \   |
     * `------------------------------------------------\      /------------------------------------------------'
     *                             | TRNS | TRNS | TRNS |      | TRNS | TRNS | TRNS |
     *                             `--------------------|      |--------------------'
     *                                    | TRNS | TRNS |      | TRNS | TRNS |
     *                                    `-------------/      \-------------'
     */

    [_QWERTY] = LAYOUT_split_4x6_5(
        KC_TRNS, KC_1, KC_2, KC_3, KC_4, LT(_CONF, KC_5),         KC_6, KC_7, KC_8, KC_9, KC_0, KC_BSPC,
        KC_TRNS, KC_Q, KC_W, KC_E, KC_R, KC_T, 			          KC_Y, KC_U, KC_I, KC_O, KC_P, KC_MINS,
        KC_TRNS, KC_A, KC_S, KC_D, KC_F, KC_G, 			          KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT,
        KC_TRNS, KC_Z, KC_X, KC_C, KC_V, KC_B, 			          KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_RGUI,

                        KC_TRNS, KC_TRNS, KC_TRNS,      KC_TRNS, KC_TRNS, KC_TRNS,
                                KC_TRNS, KC_TRNS,        KC_TRNS, KC_TRNS
    ),


    /* COLEMAK-DHm
     * ,-----------------------------------------.                    ,-----------------------------------------.
     * | TRNS |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  |  `   |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * | TRNS |   Q  |   W  |   F  |   P  |   B  |                    |   J  |   L  |   U  |   Y  |   ;  |  -   |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * | TRNS |   A  |   R  |   S  |   T  |   G  |                    |   M  |   N  |   E  |   I  |   O  |  '   |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * | TRNS |   Z  |   X  |   C  |   D  |   V  |                    |   K  |   H  |   ,  |   .  |   /  |   \  |
     * `------------------------------------------------\      /------------------------------------------------'
     *                             | TRNS | TRNS | TRNS |      | TRNS | TRNS | TRNS |
     *                             `--------------------|      |--------------------'
     *                                    | TRNS | TRNS |      | TRNS | TRNS |
     *                                    `-------------/      \-------------'
     */
    [_COLEMAK] = LAYOUT_split_4x6_5(
        KC_TRNS, KC_1, KC_2, KC_3, KC_4, LT(_CONF, KC_5),              KC_6, KC_7, KC_8, KC_9, KC_0, KC_BSPC,
        KC_TRNS, KC_Q, KC_W, KC_F, KC_P, KC_B, 						    KC_J, KC_L, KC_U, KC_Y, KC_SCLN, KC_MINS,
        KC_TRNS, KC_A, KC_R, KC_S, KC_T, KC_G,                         KC_M, KC_N, KC_E, KC_I, KC_O, KC_QUOT,
        KC_TRNS, KC_Z, KC_X, KC_C, KC_D, KC_V, 				        KC_K, KC_H, KC_COMM, KC_DOT, KC_SLSH, KC_BSLS,

                       KC_TRNS, KC_TRNS, KC_TRNS,      KC_TRNS, KC_TRNS, KC_TRNS,
                               KC_TRNS, KC_TRNS,        KC_TRNS, KC_TRNS
    ),


    /* RAISE
     * ,-----------------------------------------.                    ,-----------------------------------------.
     * |  ~   |  !   |  @   |  #   |  $   |  %   |                    |  ^   |  &   |  *   |  (   |  )   |  DEL |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * |  __  |  __  |   [  |   ]  |  __  |  __  |                    |  __  |   7  |   8  |   9  |  __  |  +   |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * |  __  |  <   |   (  |   )  |  >   |  __  |                    |  __  |   4  |   5  |   6  |  -   |  |   |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * |  __  |  __  |   {  |   }  |  __  |  __  |                    |  __  |   1  |   2  |   3  |  =   |  _   |
     * `------------------------------------------------\      /------------------------------------------------'
     *                             |  __  |  __  |  __  |      |  __  |  __  |  __  |
     *                             `--------------------|      |--------------------'
     *                                    |  __  |  __  |      |  __  |  __  |
     *                                    `-------------/      \-------------'
     */
    [_RAISE] = LAYOUT_split_4x6_5(
        KC_TILD, KC_EXLM, KC_AT, KC_HASH, KC_DLR, KC_PERC,          KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_DEL,
         _______, _______, KC_LBRC, KC_RBRC, _______, _______,      _______, KC_P7, KC_P8, KC_P9, _______, KC_PLUS,
         _______, KC_LT, KC_LPRN, KC_RPRN, KC_GT, _______,          _______, KC_P4, KC_P5, KC_P6, KC_MINS, KC_PIPE,
         _______, _______, KC_LCBR, KC_RCBR, _______, _______,      _______, KC_P1, KC_P2, KC_P3, KC_EQL, KC_UNDS,
                                    KC_TRNS, KC_TRNS, KC_TRNS,      KC_TRNS, KC_TRNS, KC_TRNS,
                                            KC_TRNS, KC_TRNS,        KC_TRNS, KC_TRNS
     ),

    /* NAV
     * ,-----------------------------------------.                    ,-----------------------------------------.
     * |  __  |  __  |  __  |  __  |  __  |  __  |                    |  __  |  <<  | PLAY |  >>  |  __  |  __  |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * |  __  |  __  |  __  |  __  |  __  |  __  |                    |  __  |CMD+<-|  UP  |CMD+->|  __  | PGUP |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * |  __  |  __  |  __  |  __  |  __  |  __  |                    |OPT+<-| LEFT | DOWN | RIGHT|OPT+->| PGDN |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * |  __  |  __  |  __  |  __  |  __  |  __  |                    |  __  |  __  |  __  |  __  |  __  |  __  |
     * `------------------------------------------------\      /------------------------------------------------'
     *                             |  __  |  __  |  __  |      |  __  |  __  |  __  |
     *                             `--------------------|      |--------------------'
     *                                    |  __  |  __  |      |  __  |  __  |
     *                                    `-------------/      \-------------'
     */
    [_NAV] = LAYOUT_split_4x6_5(
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,       KC_NO, KC_MPRV, KC_MPLY, KC_MNXT, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,       KC_NO, G(KC_LEFT), KC_UP, G(KC_RIGHT), KC_NO, KC_PGUP,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,       LOPT(KC_LEFT), KC_LEFT, KC_DOWN, KC_RIGHT, LOPT(KC_RIGHT), KC_PGDN,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,       KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,

                            KC_TRNS, KC_TRNS, KC_TRNS, 		KC_TRNS, KC_TRNS, KC_TRNS,
                                   KC_TRNS, KC_TRNS, 		KC_TRNS, KC_TRNS
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


#ifdef CONSOLE_ENABLE
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    uprintf("KL: kc: 0x%04X, col: %2u, row: %2u, pressed: %u, time: %5u, int: %u, count: %u\n",
         keycode,
         record->event.key.col,
         record->event.key.row,
         record->event.pressed,
         record->event.time,
         record->tap.interrupted,
         record->tap.count);
    return true;
}
#endif


#ifdef RGB_MATRIX_ENABLE
/* Colours to use per layer
 *
 * Note that _QWERTY and _COLEMAK act as the default layer so they
 * don't have defined layer colours here.  If you want to have
 * colours for those layers you will need to tweak rgb_matrix_indicators_advanced_user(...)
 */
static HSV _get_hsv_for_layer_index(uint8_t layer) {
    switch (layer) {
        case _QWERTY:
            return (HSV){HSV_RED};
        case _COLEMAK:
            return (HSV){HSV_GREEN};
        case _NAV:
            return (HSV){HSV_AZURE};
        case _CONF:
            return (HSV){HSV_RED};
        case _RAISE:
            return (HSV){HSV_PURPLE};
        default:
            return (HSV){HSV_OFF};
    };
}

/* Layer effects that dynamically control LEDS on different layers to indicate which keys are available
 *
 * NOTE: Any changes to this function must be flashed to both halves.
 */
bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {



    const uint8_t layer = get_highest_layer(layer_state);


    /* For typing layers light the whole keyboard */
    if( layer <= _COLEMAK ) {
        for( uint8_t layer = _BASE; layer < _CONF; layer++ ) {
            if( default_layer_state & (1 << layer) ) {
                HSV hsv = _get_hsv_for_layer_index(layer);
                rgblight_sethsv( hsv.h, hsv.s, hsv.v );

#               ifdef CONSOLE_ENABLE
                uprintf( "Default layer: layer: %d, default_layer: %d, hsv:(%d,%d,%d)\n", layer, default_layer_state, hsv.h, hsv.s, hsv.v );
#               endif // CONSOLE_ENABLE
            }
        }







        //rgblight_setrgb(rgb.r, rgb.g, rgb.b);
        // for( int i = led_min; i < led_max; i++ ) {
        //     if( HAS_FLAGS(g_led_config.flags[i], LED_FLAG_KEYLIGHT) || HAS_FLAGS(g_led_config.flags[i], LED_FLAG_UNDERGLOW) ) {
        //         rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
        //     }
        // }

    /* For special layers use lighting that reflects the keybindings. */
    } else {


        HSV hsv = _get_hsv_for_layer_index(layer);

        // Set brightness to the configured interval brighter than current brightness, clamped to 255
        // (ie. uint8_t max value). This compensates for the dimmer appearance of the underglow LEDs.
        hsv.v         = MIN(rgb_matrix_get_val() + LAYER_INDICATOR_BRIGHTNESS_INC, 255);
        const RGB rgb = hsv_to_rgb(hsv);
        const RGB off = hsv_to_rgb((HSV){HSV_OFF});

        uint8_t layer = get_highest_layer(layer_state);

        for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
            for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
                uint8_t index = g_led_config.matrix_co[row][col];

#               ifdef CONSOLE_ENABLE
                // uprintf( "Row: %d, Col: %d, Index: %d\n", row, col, index );
#               endif // CONSOLE_ENABLE


                if (index >= led_min && index < led_max && index != NO_LED) {
                    if( keymap_key_to_keycode(layer, (keypos_t){col,row}) > KC_TRNS ) {
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

/* Sample indicator callback that changes the colour of all keys on a given layer
 *
 * NOTE: Any changes to this function must be flashed to both halves.
 */
// bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
//
//     // Set underglow to a solid color for highest active layer apart from the base layer.
//     const uint8_t layer = get_highest_layer(layer_state);
//     if (layer > 0) {
//         HSV hsv = _get_hsv_for_layer_index(layer);
//
//         // Set brightness to the configured interval brighter than current brightness,
//         // clamped to 255 (ie. uint8_t max value). This compensates for the dimmer
//         // appearance of the underglow LEDs.
//         hsv.v         = MIN(rgb_matrix_get_val() + LAYER_INDICATOR_BRIGHTNESS_INC, 255);
//         const RGB rgb = hsv_to_rgb(hsv);
//
//         for( int i = led_min; i < led_max; i++ ) {
//             if( HAS_FLAGS(g_led_config.flags[i], LED_FLAG_KEYLIGHT) || HAS_FLAGS(g_led_config.flags[i], LED_FLAG_UNDERGLOW) ) {
//                 rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
//             }
//         }
//     }
//     return true;
// };

#endif // RGB_MATRIX_ENABLE



void keyboard_post_init_user(void) {

    default_layer_set(1 << DEFAULT_LAYER );


#   ifdef CONSOLE_ENABLE
    debug_enable=true;
    // debug_matrix=true;
    // debug_keyboard=true;
    // debug_mouse=true;
#   endif // CONSOLE_ENABLE
}



