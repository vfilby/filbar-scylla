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

#include "keycodes.h"
#include "keymap_us.h"
#include QMK_KEYBOARD_H
#include "transactions.h"
#ifdef CONSOLE_ENABLE
#include "print.h"
#endif

#include "layers.h"
#include "features/swapper.h"

enum crkbd_keycodes {
    SW_APP = SAFE_RANGE, // Switch app windows (cmd-tab)
    SW_WIN,              // Switch windows     (cmd-`)
    KC_SCREENSHOT,       // cmd+shift+ctrl+4 (macOS screenshot to clipboard)
    SMH_LPRN_KC,         // Custom keycode for Alt + ( mod-tap
    SMH_RPRN_KC,         // Custom keycode for Shift + ) mod-tap
};

// Left-hand home row mods for Colemak (ARST)
#define CMH_A LGUI_T(KC_A)
#define CMH_R LALT_T(KC_R)
#define CMH_S LSFT_T(KC_S)
#define CMH_T LCTL_T(KC_T)

// Right-hand home row mods for Colemak (NEIO)
#define CMH_O RGUI_T(KC_O)
#define CMH_I LALT_T(KC_I)
#define CMH_E RSFT_T(KC_E)
#define CMH_N RCTL_T(KC_N)

// Left-hand home row mods for QWERTY (ASDF)
#define QMH_A LGUI_T(KC_A)
#define QMH_S LALT_T(KC_S)
#define QMH_D LSFT_T(KC_D)
#define QMH_F LCTL_T(KC_F)

// Right-hand home row mods for QWERTY (JKL;)
#define QMH_SCLN RGUI_T(KC_SCLN)
#define QMH_L LALT_T(KC_L)
#define QMH_K RSFT_T(KC_K)
#define QMH_J RCTL_T(KC_J)

// Left-hand home row mods for SYM layer (brackets)
// Note: ( and ) are shifted keys, so we use custom keycodes handled in process_record_user
#define SMH_LBRC LGUI_T(KC_LBRC)
#define SMH_LPRN SMH_LPRN_KC
#define SMH_RPRN SMH_RPRN_KC
#define SMH_RBRC LCTL_T(KC_RBRC)

#define B_CONF LT(_CONF, KC_B)
#define T_CONF LT(_CONF, KC_T)

/*
 * Custom key definitions
 */
#define WEBTAB_L G(KC_LCBR)
#define WEBTAB_R G(KC_RCBR)
#define LN_END G(KC_RIGHT)
#define LN_BEG G(KC_LEFT)
#define WORD_R A(KC_RIGHT)
#define WORD_L A(KC_LEFT)
#define LOGOUT G(C(KC_Q))
#define UNDO G(KC_Z)
#define REDO G(S(KC_Z))

// Layer short names
#define SPC_NUM LT(_NUMBER,KC_SPC)
#define ENT_NAV LT(_NAV, KC_ENT)
#define L_NUM MO(_NUMBER)
#define L_FUNC MO(_FUNCTION)
#define L_SPECIAL MO(_SPECIAL)
#define BSPC_NAV LT(_NAV, KC_BSPC)
#define DEL_MOUS LT(_MOUSE, KC_DEL)
#define ENT_MED LT(_MEDIA, KC_ENT)

#define QWERTY DF(_QWERTY)
#define COLEMK DF(_COLEMAK)

#define DEFAULT_LAYER _COLEMAK

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] =
{
    [_BASE] = LAYOUT_split_3x6_3_ex2(
        //,-----------------------------------------------------. --------  -------- ,-----------------------------------------------------.
            QK_GESC, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  L_FUNC, L_SPECIAL,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_BSPC,
        //|--------+--------+--------+--------+--------+--------| --------  -------- |--------+--------+--------+--------+--------+--------|
             KC_TAB, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    UNDO,      REDO,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_QUOT,
        //|--------+--------+--------+--------+--------+--------| --------  -------- |--------+--------+--------+--------+--------+--------|
            KC_LSFT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_RSFT,
        //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                                KC_LGUI,BSPC_NAV, SPC_NUM,    ENT_MED,MO(_SYM), DEL_MOUS
        //                                    `--------------------------'  `--------------------------'
    ),

    [_QWERTY] = LAYOUT_split_3x6_3_ex2(
        //,-----------------------------------------------------. --------  -------- ,-----------------------------------------------------.
            _______,    KC_Q,    KC_W,    KC_E,    KC_R,  T_CONF, _______,   _______,     KC_Y,    KC_U,    KC_I,    KC_O,    KC_P, _______,
        //|--------+--------+--------+--------+--------+--------| --------  -------- |--------+--------+--------+--------+--------+--------|
            _______,   QMH_A,   QMH_S,   QMH_D,   QMH_F,    KC_G, _______,   _______,     KC_H,   QMH_J,   QMH_K,   QMH_L,QMH_SCLN, _______,
        //|--------+--------+--------+--------+--------+--------| --------  -------- |--------+--------+--------+--------+--------+--------|
            _______,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                         KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH, _______,
        //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                                _______, _______, _______,    _______, _______, _______
        //                                    `--------------------------'  `--------------------------'
    ),

    [_COLEMAK] = LAYOUT_split_3x6_3_ex2(
        //,-----------------------------------------------------. --------  -------- ,-----------------------------------------------------.
            _______,    KC_Q,    KC_W,    KC_F,    KC_P,  B_CONF, _______,   _______,     KC_J,    KC_L,    KC_U,    KC_Y, KC_SCLN, _______,
        //|--------+--------+--------+--------+--------+--------| --------  -------- |--------+--------+--------+--------+--------+--------|
            _______,   CMH_A,   CMH_R,   CMH_S,   CMH_T,    KC_G, _______,   _______,     KC_M,   CMH_N,   CMH_E,   CMH_I,   CMH_O, _______,
        //|--------+--------+--------+--------+--------+--------| --------  -------- |--------+--------+--------+--------+--------+--------|
            _______,    KC_Z,    KC_X,    KC_C,    KC_D,    KC_V,                         KC_K,    KC_H, KC_COMM,  KC_DOT, KC_SLSH, _______,
        //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                                _______, _______, _______,    _______, _______, _______
        //                                    `--------------------------'  `--------------------------'
    ),

    [_NUMBER] = LAYOUT_split_3x6_3_ex2(
        //,-----------------------------------------------------. --------  -------- ,-----------------------------------------------------.
             SW_APP, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  LOGOUT, XXXXXXX,   XXXXXXX,  KC_LBRC,    KC_7,    KC_8,    KC_9, KC_RBRC, _______,
        //|--------+--------+--------+--------+--------+--------| --------  -------- |--------+--------+--------+--------+--------+--------|
             SW_WIN, KC_LGUI, KC_LALT, KC_LSFT, KC_BSLS, XXXXXXX, _______,   _______,  KC_SCLN,    KC_4,    KC_5,    KC_6,  KC_EQL, _______,
        //|--------+--------+--------+--------+--------+--------| --------  -------- |--------+--------+--------+--------+--------+--------|
            _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_TILD,    KC_1,    KC_2,    KC_3, KC_BSLS, _______,
        //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                                _______, _______, _______,    KC_MINS,   KC_0, KC_DOT
        //                                    `--------------------------'  `--------------------------'
    ),


    [_NAV] = LAYOUT_split_3x6_3_ex2(
        //,-----------------------------------------------------. --------  -------- ,-----------------------------------------------------.
            XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX,  KC_PGUP,  XXXXXXX,WEBTAB_L,   KC_UP,WEBTAB_R, XXXXXXX, XXXXXXX,
        //|--------+--------+--------+--------+--------+--------| --------  -------- |--------+--------+--------+--------+--------+--------|
            XXXXXXX, KC_LGUI, KC_LALT, KC_LSFT, KC_LCTL, XXXXXXX,  XXXXXXX,  KC_PGDN,   LN_BEG, KC_LEFT, KC_DOWN, KC_RGHT,  LN_END, XXXXXXX,
        //|--------+--------+--------+--------+--------+--------| --------  -------- |--------+--------+--------+--------+--------+--------|
            XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX,  WORD_L, XXXXXXX,  WORD_R, XXXXXXX, XXXXXXX,
        //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                                _______, _______, _______,    _______, _______, _______
        //                                    `--------------------------'  `--------------------------'
    ),

    [_MOUSE] = LAYOUT_split_3x6_3_ex2(
        //,-----------------------------------------------------. --------  -------- ,-----------------------------------------------------.
            _______, XXXXXXX, XXXXXXX,   MS_UP, XXXXXXX, XXXXXXX,  MS_WHLU,  XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        //|--------+--------+--------+--------+--------+--------| --------  -------- |--------+--------+--------+--------+--------+--------|
            _______, XXXXXXX, MS_LEFT, MS_DOWN, MS_RGHT, XXXXXXX,  MS_WHLD,  XXXXXXX,  XXXXXXX, KC_RCTL, KC_RSFT, KC_RALT, KC_RGUI, XXXXXXX,
        //|--------+--------+--------+--------+--------+--------| --------  -------- |--------+--------+--------+--------+--------+--------|
            _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        //|--------+--------+--------+--------+--------+-------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                                MS_BTN3, MS_BTN2, MS_BTN1,    _______, _______, _______
        //                                    `--------------------------'  `--------------------------'
    ),

    [_SYM] = LAYOUT_split_3x6_3_ex2(
        //,-----------------------------------------------------. --------  -------- ,-----------------------------------------------------.
            KC_TILD, KC_EXLM,   KC_AT, KC_HASH,  KC_DLR, KC_PERC,  XXXXXXX,  XXXXXXX,  KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, _______,
        //|--------+--------+--------+--------+--------+--------| --------  -------- |--------+--------+--------+--------+--------+--------|
             KC_GRV,SMH_LBRC,SMH_LPRN,SMH_RPRN,SMH_RBRC, XXXXXXX,  _______,  _______,  KC_TILD, KC_DQUO, KC_QUOT, KC_MINS, KC_UNDS, KC_BSLS,
        //|--------+--------+--------+--------+--------+--------| --------  -------- |--------+--------+--------+--------+--------+--------|
            _______,   KC_LT, KC_LCBR, KC_RCBR,   KC_GT, XXXXXXX,                      KC_SLSH, KC_EXLM, KC_PIPE, KC_COLN, KC_QUES,  KC_GRV,
        //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                                _______, KC_BSPC,_______,    _______, _______, _______
        //                                    `--------------------------'  `--------------------------'
    ),

    [_MEDIA] = LAYOUT_split_3x6_3_ex2(
        //,-----------------------------------------------------. --------  -------- ,-----------------------------------------------------.
            XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX, KC_VOLD, KC_MUTE, KC_VOLU, XXXXXXX, XXXXXXX,
        //|--------+--------+--------+--------+--------+--------| --------  -------- |--------+--------+--------+--------+--------+--------|
            XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX, KC_MRWD, KC_MPLY, KC_MFFD, XXXXXXX, XXXXXXX,
        //|--------+--------+--------+--------+--------+--------| --------  -------- |--------+--------+--------+--------+--------+--------|
            XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, KC_BRID, XXXXXXX, KC_BRIU, XXXXXXX, XXXXXXX,
        //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                                XXXXXXX, XXXXXXX, XXXXXXX,    _______, _______, _______
        //                                    `--------------------------'  `--------------------------'
    ),

    [_FUNCTION] = LAYOUT_split_3x6_3_ex2(
        //,-----------------------------------------------------. --------  -------- ,-----------------------------------------------------.
            XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  QK_LLCK,  QK_LLCK,  KC_PSCR,   KC_F7,   KC_F8,   KC_F9,  KC_F12, XXXXXXX,
        //|--------+--------+--------+--------+--------+--------| --------  -------- |--------+--------+--------+--------+--------+--------|
            XXXXXXX, KC_LGUI, KC_LALT, KC_LSFT, KC_LCTL, XXXXXXX,  XXXXXXX,  XXXXXXX,  KC_SCRL,   KC_F4,   KC_F5,   KC_F6,  KC_F11, XXXXXXX,
        //|--------+--------+--------+--------+--------+--------| --------  -------- |--------+--------+--------+--------+--------+--------|
            XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_PAUS,   KC_F1,   KC_F2,   KC_F3,  KC_F10, XXXXXXX,
        //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                                _______, _______, _______,    _______, _______, _______
        //                                    `--------------------------'  `--------------------------'
    ),

        [_SPECIAL] = LAYOUT_split_3x6_3_ex2(
        //,-----------------------------------------------------. --------  -------- ,-------------------------------------------------------.
            XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  QK_LLCK,  QK_LLCK,  XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX, XXXXXXX,
        //|--------+--------+--------+--------+--------+--------| --------  -------- |--------+----------+--------+--------+--------+--------|
            XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX, XXXXXXX,
        //|--------+--------+--------+--------+--------+--------| --------  -------- |--------+----------+--------+--------+--------+--------|
            XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX, XXXXXXX,
        //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+----------+--------+--------+--------+--------|
                                                _______, _______, _______,    _______, _______, _______
        //                                    `--------------------------'  `--------------------------'
    ),

    [_CONF] = LAYOUT_split_3x6_3_ex2(
        //,-----------------------------------------------------. --------  -------- ,-----------------------------------------------------.
            QK_BOOT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX,  XXXXXXX,QK_REBOOT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        //|--------+--------+--------+--------+--------+--------| --------  -------- |--------+--------+--------+--------+--------+--------|
            RM_TOGG, RM_HUEU, RM_SATU, RM_VALU, XXXXXXX, XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        //|--------+--------+--------+--------+--------+--------| --------  -------- |--------+--------+--------+--------+--------+--------|
            RM_NEXT, RM_HUED, RM_SATD, RM_VALD, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                                _______, _______, _______,    _______,  QWERTY,  COLEMK
        //                                    `--------------------------'  `--------------------------'
    )
};

/* Split keyboard Caps Word state sync
 * The master sends its Caps Word state to the slave so both halves
 * can show the breathing red LED indicator.
 */
static bool caps_word_state_synced = false;  // Slave-side copy of Caps Word state

void caps_word_sync_handler(uint8_t in_buflen, const void* in_data, uint8_t out_buflen, void* out_data) {
#ifdef CONSOLE_ENABLE
    uprintf("caps_word_sync_handler: in_buflen=%d\n", in_buflen);
#endif
    const bool* master_state = (const bool*)in_data;
    caps_word_state_synced = *master_state;
}

void keyboard_post_init_user(void) {
#ifdef CONSOLE_ENABLE
    uprintf("keyboard_post_init_user: start, master=%d\n", is_keyboard_master());
#endif
    default_layer_set(1 << DEFAULT_LAYER);
    // Register the split transaction handler for Caps Word sync
    transaction_register_rpc(USER_SYNC_CAPS_WORD, caps_word_sync_handler);
#ifdef CONSOLE_ENABLE
    uprintf("keyboard_post_init_user: done\n");
#endif
}

void housekeeping_task_user(void) {
#ifdef CONSOLE_ENABLE
    static uint32_t last_debug_time = 0;
    static uint32_t housekeeping_count = 0;
    housekeeping_count++;
#endif

    // Only run on master side
    if (is_keyboard_master()) {
        static bool last_caps_word_state = false;
        bool current_state = is_caps_word_on();

        // Only sync when state changes to reduce traffic
        if (current_state != last_caps_word_state) {
#ifdef CONSOLE_ENABLE
            uprintf("housekeeping: caps_word changed to %d\n", current_state);
#endif
            if (transaction_rpc_send(USER_SYNC_CAPS_WORD, sizeof(current_state), &current_state)) {
                last_caps_word_state = current_state;
            }
        }
    }

#ifdef CONSOLE_ENABLE
    // Periodic heartbeat debug (every 5 seconds)
    if (timer_elapsed32(last_debug_time) > 5000) {
        uprintf("heartbeat: master=%d, layer=%d, hk_calls=%lu, uptime=%lus\n",
                is_keyboard_master(), get_highest_layer(layer_state),
                housekeeping_count, timer_read32() / 1000);
        last_debug_time = timer_read32();
    }
#endif
}

/* Helper to check Caps Word state on either half */
static bool is_caps_word_active(void) {
    if (is_keyboard_master()) {
        return is_caps_word_on();
    } else {
        return caps_word_state_synced;
    }
}

/* Caps Word customization
 * Space -> underscore is handled in process_record_user
 * This handles letters, numbers, and mod-tap keys (for home row mods)
 */
bool caps_word_press_user(uint16_t keycode) {
    // Extract the base keycode for mod-tap and layer-tap keys
    switch (keycode) {
        case QK_MOD_TAP ... QK_MOD_TAP_MAX:
        case QK_LAYER_TAP ... QK_LAYER_TAP_MAX:
            // Get the tap keycode (lower 8 bits)
            keycode = keycode & 0xFF;
            break;
    }

    switch (keycode) {
        case KC_A ... KC_Z:
            add_weak_mods(MOD_BIT(KC_LSFT));
            return true;
        case KC_1 ... KC_0:
        case KC_BSPC:
        case KC_DEL:
        case KC_MINS:
        case KC_UNDS:
            return true;
        default:
            return false;
    }
}

/* This is needed to handle retro shift for the tap-hold mods on the home row.
 * Without this they will not be shifted.
 *
 * When tapping and holding a home-row mod, if you hold it and release longer
 * than the auto shift timeout, but less than the retro shift timeout then you
 * will get the shifted tap key. If you hold it longer than retro-shift timeout
 * you will get the unshifted keystroke and the mod will be held until release.
 *
 * See: https://docs.qmk.fm/features/auto_shift#auto-shift-per-key
 */
bool get_custom_auto_shifted_key(uint16_t keycode, keyrecord_t *record) {
    // Disable auto-shift during Caps Word to prevent conflicts
    if (is_caps_word_on()) return false;
    if (IS_RETRO(keycode)) return true;
    return false;
}

/* Swapper state tracking */
bool sw_app_active = false;
bool sw_win_active = false;

/* Custom mod-tap state for shifted keys (parentheses)
 * These track the press time to determine tap vs hold
 */
static uint16_t smh_lprn_timer = 0;
static uint16_t smh_rprn_timer = 0;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
#ifdef CONSOLE_ENABLE
    uprintf("key: 0x%04X %s r=%d c=%d\n", keycode, record->event.pressed ? "DN" : "UP",
            record->event.key.row, record->event.key.col);
#endif

    update_swapper(&sw_app_active, KC_LGUI, KC_TAB, SW_APP, keycode, record);
    update_swapper(&sw_win_active, KC_LGUI, KC_GRV, SW_WIN, keycode, record);

    // Handle custom mod-tap for parentheses (shifted keys can't use normal mod-tap)
    switch (keycode) {
        case SMH_LPRN_KC:
            if (record->event.pressed) {
                smh_lprn_timer = timer_read();
                register_code(KC_LALT);
            } else {
                unregister_code(KC_LALT);
                if (timer_elapsed(smh_lprn_timer) < TAPPING_TERM) {
                    tap_code16(KC_LPRN);  // Tap: send (
                }
            }
            return false;
        case SMH_RPRN_KC:
            if (record->event.pressed) {
                smh_rprn_timer = timer_read();
                register_code(KC_LSFT);
            } else {
                unregister_code(KC_LSFT);
                if (timer_elapsed(smh_rprn_timer) < TAPPING_TERM) {
                    tap_code16(KC_RPRN);  // Tap: send )
                }
            }
            return false;
    }

    // Handle Caps Word special cases
    // Must intercept here before the layer-tap/mod-tap resolves
    if (is_caps_word_on() && record->event.pressed) {
        // Check for layer-tap keys with space as the tap action
        if ((keycode >= QK_LAYER_TAP && keycode <= QK_LAYER_TAP_MAX) &&
            (keycode & 0xFF) == KC_SPC) {
            tap_code16(KC_UNDS);
            return false;  // Don't process the layer-tap
        }

        // Disable home row mods during Caps Word - send the letter directly
        if (keycode >= QK_MOD_TAP && keycode <= QK_MOD_TAP_MAX) {
            uint8_t base_keycode = keycode & 0xFF;
            if (base_keycode >= KC_A && base_keycode <= KC_Z) {
                tap_code16(S(base_keycode));  // Send shifted letter
                return false;  // Don't process the mod-tap
            }
        }
    }

    if (record->event.pressed) {
        switch (keycode) {
            case KC_SCREENSHOT:
                tap_code16(LGUI(LSFT(LCTL(KC_4))));
                return false;
        }
    }

    return true;
}

#ifndef LAYER_INDICATOR_BRIGHTNESS_INC
#    define LAYER_INDICATOR_BRIGHTNESS_INC 22
#endif

/* RGB Layer Color ConfigurationA
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
        case _NAV:
            config.default_color = (HSV){HSV_BLUE};
            // Arrow keys highlighted in white
            config.highlights[0]   = (keycode_highlight_t){KC_UP, (HSV){HSV_WHITE}};
            config.highlights[1]   = (keycode_highlight_t){KC_DOWN, (HSV){HSV_WHITE}};
            config.highlights[2]   = (keycode_highlight_t){KC_LEFT, (HSV){HSV_WHITE}};
            config.highlights[3]   = (keycode_highlight_t){KC_RGHT, (HSV){HSV_WHITE}};
            config.highlight_count = 4;
            break;
        case _MOUSE:
            config.default_color   = (HSV){HSV_GREEN};
            config.highlight_count = 0;
            break;
        case _NUMBER:
            config.default_color   = (HSV){HSV_WHITE};
            config.highlights[0]   = (keycode_highlight_t){LOGOUT, (HSV){HSV_RED}};
            config.highlight_count = 1;
            break;
        case _FUNCTION:
            config.default_color   = (HSV){HSV_YELLOW};
            config.highlight_count = 0;
            break;
        case _CONF:
            config.default_color   = (HSV){HSV_RED};
            config.highlight_count = 0;
            break;
        case _SYM:
            config.default_color   = (HSV){HSV_PURPLE};
            config.highlight_count = 0;
            break;
        case _MEDIA:
            config.default_color   = (HSV){HSV_ORANGE};
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
    /* Caps Word indicator: breathing red effect on all LEDs */
    if (is_caps_word_active()) {
        // Create a breathing effect using a triangle wave
        // timer_read() returns ms, we want a ~2 second cycle
        uint16_t time = timer_read();
        uint8_t phase = (time / 8) % 256;  // 8ms per step, ~2 sec full cycle

        // Triangle wave: ramp up 0-127, ramp down 128-255
        uint8_t triangle;
        if (phase < 128) {
            triangle = phase * 2;  // 0 -> 254
        } else {
            triangle = (255 - phase) * 2;  // 254 -> 0
        }

        // Scale to range 80-255 (avoid going completely dark)
        uint8_t val = 80 + (triangle * 175 / 255);

        HSV hsv = {HSV_RED};
        hsv.v = val;
        RGB rgb = hsv_to_rgb(hsv);

        for (uint8_t i = led_min; i < led_max; i++) {
            rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
        }
        return false;  // Skip normal layer indicators during Caps Word
    }

    const uint8_t layer = get_highest_layer(layer_state);

    /* For typing layers light the whole keyboard, just set the hue and keep the matrix effects */
    if (layer <= _COLEMAK) {
        // Only update RGB when default layer changes, not every frame
        static layer_state_t last_default_layer = 0;
        if (default_layer_state != last_default_layer) {
            last_default_layer = default_layer_state;
            uint8_t default_layer = get_highest_layer(default_layer_state);
            layer_color_config_t config = _get_layer_color_config(default_layer);
            rgblight_sethsv(config.default_color.h, config.default_color.s, config.default_color.v);
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

// Combo definitions
const uint16_t PROGMEM combo_screenshot[] = {KC_Q, KC_W, KC_F, KC_P, COMBO_END};
const uint16_t PROGMEM combo_copy[] = {KC_Z, KC_X, KC_C, COMBO_END};
const uint16_t PROGMEM combo_paste[] = {KC_X, KC_C, KC_D, COMBO_END};

combo_t key_combos[] = {
    COMBO(combo_screenshot, KC_SCREENSHOT),
    COMBO(combo_copy, G(KC_C)),   // Cmd+C
    COMBO(combo_paste, G(KC_V)),  // Cmd+V
};
