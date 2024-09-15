/*
 *
 * Inspiration taken from: https://github.com/archydragon/lily-layout/blob/main/qmk_keymap/keymap.c
 * led as a caps indicator: https://discord.com/channels/574598631399751680/574738351836626944/1133073274209841202
 */



#include QMK_KEYBOARD_H
#include "features/layer_lock.h"
#include "features/swapper.h"

extern keymap_config_t keymap_config;

enum lily_layers {
    _BASE = 0,
    _COLEMAK,
    _QWERTY,
    _SYM,
    _NAV,
    _RAISE,
    _CONF,
};

#define DEFAULT_LAYER _COLEMAK

enum lily_keycodes {
    LLOCK = SAFE_RANGE,
    SW_APP,  // Switch app windows (cmd-tab)
    SW_WIN   // Switch apps        (cmd-`)
};


/*
 * Custom key definitions
 */
#define WEBTAB_L G(KC_LCBR)
#define WEBTAB_R G(KC_RCBR)
#define LINE_END G(KC_RIGHT)
#define LINE_BEG G(KC_LEFT)
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



// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* BASE_MODIFIER
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | ESC  |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  | BcSp |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  |      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Shft |      |      |      |      |      |-------.    ,-------|      |      |      |      |      |      |
 * |------+------+------+------+------+------| BcSp  |    | Shft  |------+------+------+------+------+------|
 * | CMD  |      |      |      |      |      |-------|    |-------|      |      |      |      |      |      |
 * `-----------------------------------------/ (SYM) /     \      \-----------------------------------------'
 *                   |      |      |      | / Space /       \ Entr \  |      |      |      |
 *                   | LCtl | CMD  |  NAV |/       /         \      \ | RAISE| RAlt | LGUI |
 *                   `----------------------------'           '------''--------------------'
 */

[_BASE] = LAYOUT(
   QK_GESC, KC_1,    KC_2,    KC_3,    KC_4,    LT(_CONF, KC_5),            KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC,
   KC_TAB,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
   KC_LSFT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
   KC_LGUI, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_BSPC,  KC_LSFT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                     KC_LCTL, KC_LGUI, MO(_NAV),LT(_SYM,KC_SPC),            KC_ENT,  MO(_RAISE), KC_RALT, KC_LGUI
),

/* COLEMAK-dhm)
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | TRNS | TRNS | TRNS | TRNS | TRNS | TRNS |                    | TRNS | TRNS | TRNS | TRNS | TRNS | TRNS |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | TRNS |   Q  |   W  |   F  |   P  |   B  |                    |   J  |   L  |   U  |   Y  |   ;  |  -   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | TRNS |   A  |   R  |   S  |   T  |   G  |-------.    ,-------|   M  |   N  |   E  |   I  |   O  |  '   |
 * |------+------+------+------+------+------|  TRNS |    | TRNS  |------+------+------+------+------+------|
 * | TRNS |   Z  |   X  |   C  |   D  |   V  |-------|    |-------|   K  |   H  |   ,  |   .  |   /  |   \  |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   |      |      |      | /  TRNS /       \ TRNS \  |      |      |      |
 *                   | TRNS | TRNS | TRNS |/       /         \      \ | TRNS | TRNS | TRNS |
 *                   `----------------------------'           '------''--------------------'
 */

[_COLEMAK] = LAYOUT(
   _______, _______, _______, _______, _______, _______,                    _______, _______, _______, _______, _______, _______,
   _______, KC_Q,    KC_W,    KC_F,    KC_P,    KC_B,                       KC_J,    KC_L,    KC_U,    KC_Y,    KC_SCLN, KC_MINS,
   _______, KC_A,    KC_R,    KC_S,    KC_T,    KC_G,                       KC_M,    KC_N,    KC_E,    KC_I,    KC_O,    KC_QUOT,
   _______, CMH_Z,   CMH_X,   CMH_C,   CMH_D,   KC_V,  _______,   _______,  KC_K,    CMH_H,   CMH_COMM,CMH_DOT, CMH_SLSH,KC_BSLS,
                     _______, _______, _______, _______,           _______, _______, _______, _______
),

/* QWERTY
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | TRNS | TRNS | TRNS | TRNS | TRNS | TRNS |                    | TRNS | TRNS | TRNS | TRNS | TRNS | TRNS |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | TRNS |   Q  |   W  |   E  |   R  |   T  |                    |   Y  |   U  |   I  |   O  |   P  |  -   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | TRNS |   A  |   S  |   D  |   F  |   G  |-------.    ,-------|   H  |   J  |   K  |   L  |   ;  |  '   |
 * |------+------+------+------+------+------|  TRNS |    | TRNS  |------+------+------+------+------+------|
 * | TRNS |   Z  |   X  |   C  |   V  |   B  |-------|    |-------|   N  |   M  |   ,  |   .  |   /  |   \  |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   |      |      |      | /  TRNS /       \ TRNS \  |      |      |      |
 *                   | TRNS | TRNS | TRNS |/       /         \      \ | TRNS | TRNS | TRNS |
 *                   `----------------------------'           '------''--------------------'
 */

 [_QWERTY] = LAYOUT(
    _______, _______, _______, _______, _______, _______,                    _______, _______, _______, _______, _______, _______,
    _______, KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                       KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_MINS,
    _______, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                       KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
    _______, QMH_Z,   QMH_X,   QMH_C,   QMH_V,   KC_B,    _______,  _______, QMH_N,   QMH_M,   QMH_COMM,QMH_DOT, KC_SLSH, KC_BSLS,
                      _______, _______, _______, _______,                    _______, _______, _______, _______
),

/* Symbol
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |SW_WIN|      |      |      |      |LOGOUT|                    |      |   7  |   8  |   9  |   /  | TRNS |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |SW_APP|  <   |   [  |   ]  |  >   |      |                    |      |   4  |   5  |   6  |   *  |   -  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |  CW  |  {   |   (  |   )  |  }   |      |-------.    ,-------|      |   1  |   2  |   3  |   +  |   =  |
 * |------+------+------+------+------+------|  TRNS |    | LLOCK |------+------+------+------+------+------|
 * | TRNS |      |      |      |      |      |-------|    |-------|      |  __  |   0  |   .  |  ENT |   _  |
 * `-----------------------------------------/       /     \       \-----------------------------------------'
 *                   |      |      |      | /  TRNS /       \ TRNS \  |      |      |      |
 *                   | TRNS | TRNS | TRNS |/       /         \      \ | TRNS | TRNS | TRNS |
 *                   `----------------------------'           '------''--------------------'
 */

[_SYM] = LAYOUT(
    SW_WIN,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, LOGOUT,                     XXXXXXX, KC_7,    KC_8,    KC_9,    KC_SLSH, _______,
    SW_APP,  KC_LT,   KC_LBRC, KC_RBRC, KC_GT,   XXXXXXX,                    KC_NO,   KC_4,    KC_5,    KC_6,    KC_ASTR, KC_MINUS,
    CW_TOGG, KC_LCBR, KC_LPRN, KC_RPRN, KC_RCBR, XXXXXXX,                    KC_NO,   KC_1,    KC_2,    KC_3,    KC_PLUS, KC_EQL,
    _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,  LLOCK,   KC_NO,   KC_NO,   KC_0,    KC_DOT,  KC_ENT,  KC_UNDS,
                      _______, _______, _______, _______,                    _______, _______, _______, _______
),

/* NAVIGATION
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |      |      |      |      |      |      |                    | PgUp | |<<  |  ||  |  >>| |      | TRNS |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      | MWLt | MUp  | MWRt |      |                    | PgDn | WordL|  Up  | WordR|      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |  M1  | MLft | MDn  | MRgt | MWUp |-------.    ,-------| TabL | Left | Down | Rght | TabR |      |
 * |------+------+------+------+------+------| MAIN  |    | LLOCK |------+------+------+------+------+------|
 * |      |      |  M1  |  M2  |  M3  | MWDn |-------|    |-------|      | LineB|      | LineE|      |      |
 * `-----------------------------------------/      /      \      \-----------------------------------------'
 *                   |     |      |      | /  TRNS /        \ TRNS \  |      |      |      |
 *                   |TRNS | TRNS | TRNS |/       /          \      \ | TRNS | TRNS | TRNS |
 *                   `---------------------------'            '------''--------------------'
 */

[_NAV] = LAYOUT(
    _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                    KC_PGUP, KC_MRWD, KC_MPLY, KC_MFFD, XXXXXXX, _______,
    _______, XXXXXXX, KC_WH_L, KC_MS_U, KC_WH_R, XXXXXXX,                    KC_PGDN, WORD_L,  KC_UP,   WORD_R,  KC_MPRV, XXXXXXX,
    _______, XXXXXXX, KC_MS_L, KC_MS_D, KC_MS_R, KC_WH_U,                    WEBTAB_L,KC_LEFT, KC_DOWN, KC_RGHT, WEBTAB_R,XXXXXXX,
    _______, XXXXXXX, KC_BTN1, KC_BTN2, KC_BTN3, KC_WH_D,  KC_TRNS,  LLOCK,  XXXXXXX, LINE_BEG,XXXXXXX, LINE_END,XXXXXXX, XXXXXXX,
                      _______, _______, _______, _______,                    _______, _______, _______, _______
),

/* ADJUST (never used actually, saved from original config)
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |      |      |      |      |      |      |                    |CLEAR |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |      |      |      |      |                    |  RGB | MOD U| HUE U|      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------.    ,-------|      | MOD D| HUE D|      |      |      |
 * |------+------+------+------+------+------|       |    |       |------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------|    |-------|      |      |      |      |      |      |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   |      |      |      | /       /       \      \  |      |      |      |
 *                   |      |      |      |/       /         \      \ |QWERTY|COLEMK|      |
 *                   `----------------------------'           '------''--------------------'
 */

  [_CONF] = LAYOUT(
    KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,                      CLEAR,   KC_NO,    KC_NO,  KC_NO,   KC_NO,   KC_NO,
    KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,                      RGB_TOG, RGB_MOD,  RGB_HUI,KC_NO,   KC_NO,   KC_NO,
    KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,                      KC_NO,   RGB_RMOD, RGB_HUD,KC_NO,   KC_NO,   KC_NO,
    KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,  _______,  _______,  KC_NO,   KC_NO,    KC_NO,  KC_NO,   KC_NO,   KC_NO,
                      _______, _______, _______, _______,                    _______, QWERTY,   COLEMK, _______
  )
};
// clang-format on


void keyboard_post_init_user(void) {
    default_layer_set(1 << DEFAULT_LAYER );
}

void keyboard_pre_init_user(void) {
    // Set our LED pin as output
    setPinOutput(24);
    // Turn the LED off
    // (Due to technical reasons, high is off and low is on)
    writePinHigh(24);
}

void caps_word_set_user(bool active) {
    if (active) {
        writePinLow(24);
    } else {
        writePinHigh(24);
    }
}

bool sw_win_active = false;
bool sw_app_active = false;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    if (!process_layer_lock(keycode, record, LLOCK)) {
        return false;
    }


    /* there are some glitches...  shift exits, and you need to release SYM between different swaps that use the same mod */
    update_swapper( &sw_app_active, KC_LGUI, KC_TAB, SW_APP, keycode, record );
    update_swapper( &sw_win_active, KC_LGUI, KC_GRV, SW_WIN, keycode, record );


    return true;
}
