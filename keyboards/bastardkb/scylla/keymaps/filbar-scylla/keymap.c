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
  * Layerlock: https://getreuer.info/posts/keyboards/layer-lock/index.html
  *
  * Ideas?
  * Ditch the numpad and use raise for key combos
  * Lock layer? https://getreuer.info/posts/keyboards/layer-lock/index.html
  */

#include QMK_KEYBOARD_H
#include "quantum.h"
#include "features/layer_lock.h"

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
    _SYM,
    _CONF,
};

enum custom_keycodes {
  LLOCK = SAFE_RANGE,
  // Other custom keys...
};


#define WEBTABL G(KC_LCBR)
#define WEBTABR G(KC_RCBR)
#define LOGOUT G(C(KC_Q))

// qwerty home row mods
#define QW_MH_A LGUI_T(KC_A)
#define QW_MH_S LALT_T(KC_S)
#define QW_MH_D LSFT_T(KC_D)
#define QW_MH_F LCTL_T(KC_F)
#define QW_MH_J RCTL_T(KC_J)
#define QW_MH_K RSFT_T(KC_K)
#define QW_MH_L LALT_T(KC_L)
#define QW_MH_SCLN RGUI_T(KC_SCLN)

// Left-hand home row mods
#define CM_MH_Z LGUI_T(KC_Z)
#define CM_MH_X LALT_T(KC_X)
#define CM_MH_C LSFT_T(KC_C)
#define CM_MH_D LCTL_T(KC_D)

// Right-hand home row mods
#define CM_MH_SLSH RCTL_T(KC_SLSH)
#define CM_MH_DOT RSFT_T(KC_DOT)
#define CM_MH_COMM LALT_T(KC_COMM)
#define CM_MH_H RGUI_T(KC_H)


/* Set the default layer.
 *
 * Only colemak and qwerty make sense as options.
 */
#define DEFAULT_LAYER _COLEMAK


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {


    /* BASE
     * ,-----------------------------------------.                    ,-----------------------------------------.
     * | ESC  |  __  |  __  |  __  |  __  |  __  |                    |  __  |  __  |  __  |  __  |  __  |  <-  |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * | Tab  |  __  |  __  |  __  |  __  |  __  |                    |  __  |  __  |  __  |  __  |  __  |  __  |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * |LShift|  __  |  __  |  __  |  __  |  __  |                    |  __  |  __  |  __  |  __  |  __  |  __  |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * |LCTRL |  __  |  __  |  __  |  __  |  __  |                    |  __  |  __  |  __  |  __  |  __  |  __  |
     * `------------------------------------------------\      /------------------------------------------------'
     *                             | BKSP |  SPC | SYM  |      |  NAV | ENT  | SPC  |
     *                             `--------------------|      |--------------------'
     *                                    | CMD  | RAISE|      | S+CMD| RALT |
     *                                    `-------------/      \-------------'
     */
    [_BASE] = LAYOUT_split_4x6_5(
        QK_GESC, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,       KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_BSPC,
        KC_TAB,  KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,       KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_LSFT, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,       KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_LCTL, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,       KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,

                        KC_BSPC, KC_SPC, MO(_SYM),            MO(_NAV), KC_ENT, KC_SPC,
                                KC_LGUI, MO(_RAISE), 		        KC_RGUI, KC_ROPT
    ),

    /* QWERTY
     * ,-----------------------------------------.                    ,-----------------------------------------.
     * | TRNS |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  | TRNS |
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
        KC_TRNS, KC_1, KC_2, KC_3, KC_4, LT(_CONF, KC_5),         KC_6, KC_7, KC_8, KC_9, KC_0, KC_TRNS,
        KC_TRNS, KC_Q, KC_W, KC_E, KC_R, KC_T, 			          KC_Y, KC_U, KC_I, KC_O, KC_P, KC_MINS,
        KC_TRNS, KC_A, KC_S, KC_D, KC_F, KC_G, 			          KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT,
        KC_TRNS, KC_Z, KC_X, KC_C, KC_V, KC_B, 			          KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_RGUI,

                        KC_TRNS, KC_TRNS, KC_TRNS,      KC_TRNS, KC_TRNS, KC_TRNS,
                                KC_TRNS, KC_TRNS,        KC_TRNS, KC_TRNS
    ),


    /* COLEMAK-DHm
     * ,-----------------------------------------.                    ,-----------------------------------------.
     * | TRNS |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  | TRNS |
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
        KC_TRNS, KC_1, KC_2, KC_3, KC_4, LT(_CONF, KC_5),             KC_6, KC_7, KC_8, KC_9, KC_0, KC_TRNS,
        KC_TRNS, KC_Q, KC_W, KC_F, KC_P, KC_B, 			              KC_J, KC_L, KC_U, KC_Y, KC_SCLN, KC_MINS,
        KC_TRNS, KC_A, KC_R, KC_S, KC_T, KC_G,                        KC_M, KC_N, KC_E, KC_I, KC_O, KC_QUOT,
        KC_TRNS, CM_MH_Z, CM_MH_X, CM_MH_C, CM_MH_D, KC_V, 			  KC_K, CM_MH_H, CM_MH_COMM, CM_MH_DOT, CM_MH_SLSH, KC_BSLS,

                               KC_TRNS, KC_TRNS, KC_TRNS,        KC_TRNS, KC_TRNS, KC_TRNS,
                                        KC_TRNS, KC_TRNS,        KC_TRNS, KC_TRNS
    ),


    /* RAISE
     * ,-----------------------------------------.                    ,-----------------------------------------.
     * |  ~   |  !   |  @   |  #   |  $   |  %   |                    |  ^   |  &   |  *   |  (   |  )   | TRNS |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * |G(TAB)| G(Q) | G(W) | G(F) |  __  |  __  |                    |   `  |  <   |   [  |   ]  |  >   |  =   |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * | TRNS | G(A) | G(R) | G(S) | G(T) |  __  |                    |  __  |  {   |   (  |   )  |  }   |  +   |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * | TRNS | G(Z) | G(X) | G(C) | G(V) |  __  |                    |  __  |  __  |  __  |  __  |  |   |  _   |
     * `------------------------------------------------\      /------------------------------------------------'
     *                             | TRNS | TRNS | TRNS |      |  LL  | TRNS | TRNS |
     *                             `--------------------|      |--------------------'
     *                                    | TRNS | TRNS |      | TRNS | TRNS |
     *                                    `-------------/      \-------------'
     */
    [_RAISE] = LAYOUT_split_4x6_5(
        KC_TILD,   KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,       KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_TRNS,
        G(KC_TAB), G(KC_Q), G(KC_W), G(KC_F), KC_NO,   KC_NO,         KC_GRV,  KC_LT,   KC_LBRC, KC_RBRC, KC_GT,   KC_EQL,
        KC_TRNS,   G(KC_A), G(KC_R), G(KC_S), G(KC_T), KC_NO,         KC_NO,   KC_LCBR, KC_LPRN, KC_RPRN, KC_RCBR, KC_PLUS,
        KC_TRNS,   G(KC_Z), G(KC_X), G(KC_C), G(KC_V), KC_NO,         KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_PIPE, KC_UNDS,

                                    KC_TRNS, KC_TRNS, KC_TRNS,       LLOCK, KC_TRNS, KC_TRNS,
                                            KC_TRNS, KC_TRNS,        KC_TRNS, KC_TRNS
     ),

    /* NAV
     * ,-----------------------------------------.                    ,-----------------------------------------.
     * |LOGOUT|  __  |  __  |  __  |  __  |  __  |                    | MUTE |  <<  | PLAY |  >>  | VOLU | TRNS |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * | TRNS |  __  |  __  |  __  |  __  |  __  |                    |  __  |  __  |  UP  |  __  | VOLD |  __  |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * | TRNS |  __  |  __  |  __  |  __  |B TABL|                    |B TABR| LEFT | DOWN | RIGHT| PGUP |  __  |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * | TRNS |  __  |  __  |  __  |OPT+<-|CMD+<-|                    |CMD+->|OPT+->|  __  |  __  | PGDN |  __  |
     * `------------------------------------------------\      /------------------------------------------------'
     *                             | TRNS | TRNS |  LL  |      | TRNS | TRNS | TRNS |
     *                             `--------------------|      |--------------------'
     *                                    | TRNS | TRNS |      | TRNS | TRNS |
     *                                    `-------------/      \-------------'
     */
    [_NAV] = LAYOUT_split_4x6_5(
        LOGOUT, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,              KC_MUTE,     KC_MRWD,     KC_MPLY, KC_MFFD,  KC_VOLU, KC_TRNS,
        KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,             KC_NO,       KC_NO,       KC_UP,   KC_NO,    KC_VOLD, KC_NO,
        KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO, WEBTABL,           WEBTABR,  KC_LEFT,     KC_DOWN, KC_RIGHT, KC_PGUP, KC_NO,
        KC_TRNS, KC_NO, KC_NO, KC_NO, A(KC_LEFT), G(KC_LEFT),   G(KC_RIGHT), A(KC_RIGHT), KC_NO,   KC_NO,    KC_PGDN, KC_NO,

                            KC_TRNS, KC_TRNS, LLOCK, 		    KC_TRNS, KC_TRNS, KC_TRNS,
                                   KC_TRNS, KC_TRNS, 		    KC_TRNS, KC_TRNS
    ),



    /* SYM
     * ,-----------------------------------------.                    ,-----------------------------------------.
     * |  ~   |  !   |  @   |  #   |  $   |  %   |                    |  ^   |   7  |   8  |   9  |  )   | TRNS |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * |  __  |  <   |   [  |   ]  |  >   |  __  |                    |  __  |   4  |   5  |   6  |   *  |   -  |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * |  __  |  {   |   (  |   )  |  }   |  __  |                    |  __  |   1  |   2  |   3  |   +  |   =  |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * |  __  |  __  |  __  |  __  |  __  |  __  |                    |  __  |  __  |   0  |   .  |  ENT |   _  |
     * `------------------------------------------------\      /------------------------------------------------'
     *                             | TRNS | TRNS | TRNS |      |  LL  | TRNS | TRNS |
     *                             `--------------------|      |--------------------'
     *                                    | TRNS | TRNS |      | TRNS | TRNS |
     *                                    `-------------/      \-------------'
     */
    [_SYM] = LAYOUT_split_4x6_5(
        KC_TILD,   KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,     KC_CIRC, KC_7, KC_8, KC_9,   KC_RPRN, KC_TRNS,
        KC_NO, KC_LT,   KC_LBRC, KC_RBRC, KC_GT, KC_NO,             KC_NO,   KC_4, KC_5, KC_6,   KC_ASTR, KC_MINUS,
        KC_NO, KC_LCBR, KC_LPRN, KC_RPRN, KC_RCBR, KC_NO,           KC_NO,   KC_1, KC_2, KC_3,   KC_PLUS, KC_EQL,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,                   KC_NO,   KC_NO, KC_0, KC_DOT, KC_ENT, KC_UNDS,

                        KC_TRNS, KC_TRNS, KC_TRNS,        LLOCK, KC_TRNS, KC_TRNS,
                                KC_TRNS, KC_TRNS,        KC_TRNS, KC_TRNS
    ),
};



bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    if (!process_layer_lock(keycode, record, LLOCK)) {
        return false;
    }

#   ifdef CONSOLE_ENABLE
    uprintf("KL: kc: 0x%04X, col: %2u, row: %2u, pressed: %u, time: %5u, int: %u, count: %u\n",
         keycode,
         record->event.key.col,
         record->event.key.row,
         record->event.pressed,
         record->event.time,
         record->tap.interrupted,
         record->tap.count);
#   endif

    return true;
}



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
        case _SYM:
            return (HSV){HSV_WHITE};
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

    /* For typing layers light the whole keyboard, just set the hue and keep the matrix effects */
    if( layer <= _COLEMAK ) {
        for( uint8_t layer = _BASE; layer < _CONF; layer++ ) {
            if( default_layer_state & (1 << layer) ) {
                HSV hsv = _get_hsv_for_layer_index(layer);
                rgblight_sethsv( hsv.h, hsv.s, hsv.v );
            }
        }

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



