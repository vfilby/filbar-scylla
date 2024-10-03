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
  * swapper: https://github.com/qmk/qmk_firmware/tree/user-keymaps-still-present/users/callum
  *
  * Ideas?
  * Ditch the numpad and use raise for key combos
  * Lock layer? https://getreuer.info/posts/keyboards/layer-lock/index.html
  */

#include QMK_KEYBOARD_H
#include "quantum.h"
#include "features/layer_lock.h"
#include "features/swapper.h"

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
  SW_APP,  // Switch app windows (cmd-tab)
  SW_WIN   // Switch apps        (cmd-`)
};


#define WEBTAB_L G(KC_LCBR)
#define WEBTAB_R G(KC_RCBR)
#define LN_END G(KC_RIGHT)
#define LN_BEG G(KC_LEFT)
#define WORD_R A(KC_RIGHT)
#define WORD_L A(KC_LEFT)
#define LOGOUT G(C(KC_Q))

#define QWERTY DF(_QWERTY)
#define COLEMK DF(_COLEMAK)
#define CLEAR QK_CLEAR_EEPROM

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

// Left-hand home row mods for Qwerty
#define QMH_Z LGUI_T(KC_Z)
#define QMH_X LALT_T(KC_X)
#define QMH_C LSFT_T(KC_C)
#define QMH_V LCTL_T(KC_V)

// Right-hand home row mods for Qwerty
#define QMH_M RGUI_T(KC_M)
#define QMH_N RCTL_T(KC_N)
#define QMH_COMM RSFT_T(KC_COMM)
#define QMH_DOT LALT_T(KC_DOT)


/* Set the default layer.
 *
 * Only colemak and qwerty make sense as options.
 */
#define DEFAULT_LAYER _COLEMAK


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {


    /* BASE
     * ,-----------------------------------------.                    ,-----------------------------------------.
     * | ESC  |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  |   =  |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * | Tab  |  __  |  __  |  __  |  __  |  __  |                    |  __  |  __  |  __  |  __  |  __  |  __  |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * |LShift|  __  |  __  |  __  |  __  |  __  |                    |  __  |  __  |  __  |  __  |  __  |  __  |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * |LCTRL |  __  |  __  |  __  |  __  |  __  |                    |  __  |  __  |  __  |  __  |  __  |  __  |
     * `------------------------------------------------\      /------------------------------------------------'
     *                             | BKSP |SPC/SY|  NAV |      | RAISE| ENT  | BKSP |
     *                             `--------------------|      |--------------------'
     *                                    | CMD  | BKSP |      | S+CMD| RALT |
     *                                    `-------------/      \-------------'
     */
    [_BASE] = LAYOUT_split_4x6_5(
        QK_GESC, KC_1,    KC_2,    KC_3,    KC_4,    LT(_CONF, KC_5),            KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_EQL,
        KC_TAB,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        KC_LSFT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        KC_LCTL, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,

                                   KC_BSPC, LT(_SYM,KC_SPC),  MO(_NAV),          MO(_RAISE),KC_ENT, KC_BSPC,
                                            KC_LGUI,  KC_BSPC, 		             KC_RGUI, KC_ROPT
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
        _______, _______, _______, _______, _______, _______,                    _______, _______, _______, _______, _______, _______,
        _______, KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                       KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_MINS,
        _______, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                       KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
        _______, QMH_Z,   QMH_X,   QMH_C,   QMH_V,   KC_B,                       QMH_N,   QMH_M,   QMH_COMM,QMH_DOT, KC_SLSH, KC_BSLS,
                                   KC_TRNS, KC_TRNS, KC_TRNS,                    KC_TRNS, KC_TRNS, KC_TRNS,
                                            KC_TRNS, KC_TRNS,                    KC_TRNS, KC_TRNS
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
        _______, _______, _______, _______, _______, _______,                    _______, _______, _______, _______, _______, _______,
        _______, KC_Q,    KC_W,    KC_F,    KC_P,    KC_B,                       KC_J,    KC_L,    KC_U,    KC_Y,    KC_SCLN, KC_MINS,
        _______, KC_A,    KC_R,    KC_S,    KC_T,    KC_G,                       KC_M,    KC_N,    KC_E,    KC_I,    KC_O,    KC_QUOT,
        _______, CMH_Z,   CMH_X,   CMH_C,   CMH_D,   KC_V,                       KC_K,    CMH_H,   CMH_COMM,CMH_DOT, CMH_SLSH,KC_BSLS,
                                   KC_TRNS, KC_TRNS, KC_TRNS,                    KC_TRNS, KC_TRNS, KC_TRNS,
                                            KC_TRNS, KC_TRNS,                    KC_TRNS, KC_TRNS
    ),


    /* RAISE
     * ,-----------------------------------------.                    ,-----------------------------------------.
     * |  ~   |  F1  |  F2  |  F3  |  F4  |  F5  |                    |  F6  |  F7  |  F8  |  F9  | F10  |LOGOUT|
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * |  `   |   !  |   @  |   #  |   $  |   %  |                    |   ^  |   &  |   *  |   (  |   )  |      |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * |      |      |      |      |      |      |                    |      |      |      |      |      |      |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * |      |      |      |      |      |      |                    |      |      |      |      |      |      |
     * `------------------------------------------------\      /------------------------------------------------'
     *                             | TRNS | TRNS | TRNS |      |  LL  | TRNS | TRNS |
     *                             `--------------------|      |--------------------'
     *                                    | TRNS | TRNS |      | TRNS | TRNS |
     *                                    `-------------/      \-------------'
     */
    [_RAISE] = LAYOUT_split_4x6_5(
        KC_TILD, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                      KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  LOGOUT,
        KC_GRV,  KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,                    KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, XXXXXXX,
        _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,

                                   _______, _______, _______,                    LLOCK, _______, _______,
                                            _______, _______,                    _______, _______
     ),

    /* NAV
     * ,-----------------------------------------.                    ,-----------------------------------------.
     * |      |      |      |      |      |      |                    | PgUp | |<<  |  ||  |  >>| | VOLU | TRNS |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * |      |      | MWLt | MUp  | MWRt |      |                    | PgDn | TabL |  Up  | TabR | VOLD |      |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * |      |  M1  | MLft | MDn  | MRgt | MWUp |                    |      | Left | Down | Rght |      |      |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * |      |      |  M1  |  M2  |  M3  | MWDn |                    | LineB| LineB|      | WordR| LineE|      |
     * `------------------------------------------------\      /------------------------------------------------'
     *                             | TRNS | TRNS |  LL  |      | TRNS | TRNS | TRNS |
     *                             `--------------------|      |--------------------'
     *                                    | TRNS | TRNS |      | TRNS | TRNS |
     *                                    `-------------/      \-------------'
     */
    [_NAV] = LAYOUT_split_4x6_5(
        _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                    KC_PGUP, KC_MRWD, KC_MPLY, KC_MFFD, KC_VOLU, XXXXXXX,
        _______, XXXXXXX, KC_WH_L, KC_MS_U, KC_WH_R, XXXXXXX,                    KC_PGDN, WEBTAB_L,KC_UP,   WEBTAB_R,KC_VOLD, XXXXXXX,
        _______, XXXXXXX, KC_MS_L, KC_MS_D, KC_MS_R, KC_WH_U,                    XXXXXXX, KC_LEFT, KC_DOWN, KC_RGHT, XXXXXXX, XXXXXXX,
        _______, XXXXXXX, KC_BTN1, KC_BTN2, KC_BTN3, KC_WH_D,                    LN_BEG,  WORD_L,  XXXXXXX, WORD_R,  LN_END,  XXXXXXX,

                                   _______, _______, LLOCK, 		             _______, _______, _______,
                                            _______, _______, 		             _______, _______
    ),



    /* SYM
     * ,-----------------------------------------.                    ,-----------------------------------------.
     * |SW_WIN|      |      |      |      |LOGOUT|                    |      |      |      |      |   /  | TRNS |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * |SW_APP|  <   |   [  |   ]  |  >   |      |                    |      |   7  |   8  |   9  |   *  |   -  |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * |  CW  |  {   |   (  |   )  |  }   |      |                    |   .  |   4  |   5  |   6  |   +  |   =  |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * | TRNS |      |      |      |      |      |                    |      |   1  |   2  |   3  |   0  |   _  |
     * `------------------------------------------------\      /------------------------------------------------'
     *                             | TRNS | TRNS | TRNS |      |  LL  | TRNS | TRNS |
     *                             `--------------------|      |--------------------'
     *                                    | TRNS | TRNS |      | TRNS | TRNS |
     *                                    `-------------/      \-------------'
     */
    [_SYM] = LAYOUT_split_4x6_5(
        SW_WIN,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, LOGOUT,                     XXXXXXX, KC_NO,   KC_NO,   KC_NO,   KC_SLSH, _______,
        SW_APP,  KC_LT,   KC_LBRC, KC_RBRC, KC_GT,   XXXXXXX,                    KC_NO,   KC_7,    KC_8,    KC_9,    KC_ASTR, KC_MINUS,
        CW_TOGG, KC_LCBR, KC_LPRN, KC_RPRN, KC_RCBR, XXXXXXX,                    KC_DOT,  KC_4,    KC_5,    KC_6,    KC_PLUS, KC_EQL,
        _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                    KC_NO,   KC_1,    KC_2,    KC_3,    KC_0,    KC_UNDS,

                                   _______, _______, _______,                    LLOCK, _______, _______,
                                            _______, _______,                    _______, _______
    ),

    /* CONFIG
     * ,-----------------------------------------.                    ,-----------------------------------------.
     * |      |      |      |      |      |      |                    |      |      |      |      |      |      |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * |      |      |      |      |      |      |                    |  RGB | MOD U| HUE U|      |      |      |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * |      |      |      |      |      |      |                    |      | MOD D| HUE D|      |      |      |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * |      |      |      |      |      |      |                    |      |      |      |      |      |      |
     * `------------------------------------------------\      /------------------------------------------------'
     *                             |  __  |  __  |  __  |      |  __  |  __  |  __  |
     *                             `--------------------|      |--------------------'
     *                                    |  __  |  __  |      |  __  |  __  |
     *                                    `-------------/      \-------------'
     */
    [_CONF] = LAYOUT_split_4x6_5(
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, AS_UP,   DT_UP,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                    RGB_TOG, RGB_MOD, RGB_HUI, XXXXXXX, AS_DOWN, DT_DOWN,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                    XXXXXXX, RGB_RMOD,RGB_HUD, XXXXXXX, AS_RPT,  DT_PRNT,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,

                                   _______, _______, _______,                    _______, _______, _______,
                                            _______, _______,                    _______, _______
    ),
};


/* This is needed to handle retro shift for the tap-hold mods on the home (or lower) row
 * Without this they will not be shifted.
 *
 * This is used in conjuntion with auto shift (rules.mk) and retro_shift must be configured
 * to a value in config.h.  When tapping and holding a home-row mod, if you hold it and
 * release longer than the auto shift timeout, but less than the retro shift timeout then you
 * will get the shifted tap key.  If you hold it longer than retro-shift timeout you will get
 * not shifted keystroke and the mod will be held until you release (great for ctrl clicks or such)
 *
 * Note the default function calls this one and can be found here: https://docs.qmk.fm/features/auto_shift#auto-shift-per-key
 */
bool get_custom_auto_shifted_key(uint16_t keycode, keyrecord_t *record) {

    // Is this a tap and hold mod that wasn't used?
    if (IS_RETRO(keycode))
        return true;

    return false;
}



bool sw_app_active = false;
bool sw_win_active = false;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    if (!process_layer_lock(keycode, record, LLOCK)) {
        return false;
    }

    /* there are some glitches...  shift exits, and you need to release SYM between different swaps that use the same mod */
    update_swapper( &sw_app_active, KC_LGUI, KC_TAB, SW_APP, keycode, record );
    update_swapper( &sw_win_active, KC_LGUI, KC_GRV, SW_WIN, keycode, record );

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



