/*
 *
 * Inspiration taken from: https://github.com/archydragon/lily-layout/blob/main/qmk_keymap/keymap.c
 * led as a caps indicator: https://discord.com/channels/574598631399751680/574738351836626944/1133073274209841202
 * Getting retro shift to work with tap-hold mods: https://www.reddit.com/r/qmk/comments/10k1oya/autoshift_with_homerow_mods/
 */

#include QMK_KEYBOARD_H
#include "features/layer_lock.h"
#include "features/swapper.h"
#include "oled.h"
#include "layers.h"

#ifdef CONSOLE_ENABLE
#    include "print.h"
#endif // CONSOLE_ENABLE

#ifndef LAYER_INDICATOR_BRIGHTNESS_INC
#    define LAYER_INDICATOR_BRIGHTNESS_INC 22
#endif

extern keymap_config_t keymap_config;
extern bool            oled_sleeping;
extern uint32_t        anim_sleep;

/* Mod hold indicator LED flash
 * Tracks tap-hold mod keys and flashes LED once when held long enough
 */
#define MOD_HOLD_FLASH_THRESHOLD_MS (TAPPING_TERM) // Flash 50ms after tapping term
#define MOD_HOLD_FLASH_DURATION_MS 50              // How long to flash the LED

typedef struct {
    uint16_t keycode;     // The mod keycode being tracked
    uint32_t press_time;  // When it was pressed
    bool     has_flashed; // Whether we've already flashed for this press
} mod_hold_tracker_t;

static mod_hold_tracker_t mod_hold_trackers[8]    = {0}; // Track up to 8 mod keys
static uint8_t            mod_hold_count          = 0;
static bool               caps_word_active        = false; // Track caps word state for LED management
static uint32_t           mod_hold_flash_end_time = 0;     // When the mod hold flash should end

#define DEFAULT_LAYER _COLEMAK

enum lily_keycodes {
    LLOCK = SAFE_RANGE,
    SW_APP,        // Switch app windows (cmd-tab, with shift for reverse)
    SW_WIN,        // Switch apps        (cmd-`)
    KC_RD_ARROW,   // ->
    KC_LD_ARROW,   // <-
    KC_SCREENSHOT, // cmd+shift+ctrl+4
    KC_EMDASH      // em dash
};

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
#define K_UNDO G(KC_Z)
#define K_REDO G(S(KC_Z))

#define QWERTY DF(_QWERTY)
#define COLEMK DF(_COLEMAK)
#define CLEAR QK_CLEAR_EEPROM

#define T_ENTSH LSFT_T(KC_ENT)
#define T_ENTFUN LT(_FUNCTION, KC_ENT)

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
#define QMH_M LALT_T(KC_M)
#define QMH_N RSFT_T(KC_N)
#define QMH_COMM RCTL_T(KC_COMM)
#define QMH_DOT RGUI_T(KC_DOT)

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* BASE_MODIFIER
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | ESC  |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  | BcSp |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  |      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Shft |      |      |      |      |      |-------.    ,-------|      |      |      |      |      |      |
 * |------+------+------+------+------+------|  Entr |    | BcSp  |------+------+------+------+------+------|
 * | CMD  |      |      |      |      |      |-------|    |-------|      |      |      |      |      |      |
 * `-----------------------------------------/ (SYM) /     \ (NAV)\-----------------------------------------'
 *                   |      |      |      | / Space /       \ Entr \  |      |      |      |
 *                   | LCtl | CMD  | BcSp |/       /         \      \ | RAISE| RAlt | LGUI |
 *                   `----------------------------'           '------''--------------------'
 */

[_BASE] = LAYOUT(
   QK_GESC, KC_1,    KC_2,    KC_3,    KC_4,    LT(_CONF, KC_5),              KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC,
   KC_TAB,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
   T_ENTSH, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
   KC_LGUI, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, T_ENTFUN,   KC_BSPC, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                     KC_LCTL, KC_LGUI, KC_BSPC,LT(_NUMBER,KC_SPC),          LT(_NAV, KC_ENT),  MO(_RAISE), KC_RALT, KC_LGUI
),

/* COLEMAK-dhm
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

/* Numbers
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |SW_WIN| Shft |SW_WIN|SW_APP| Shft |LOGOUT|                    |      |   (  |   :  |   )  |   _  | TRNS |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |SW_APP|  ⌘Q  |  ⌘W  |      |      |      |                    |   *  |   7  |   8  |   9  |   +  |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Shft | SelAl| Cut  | Copy | Paste| CapsW|-------.    ,-------|   /  |   4  |   5  |   6  |   -  |   =  |
 * |------+------+------+------+------+------|  UNDO |    | REDO  |------+------+------+------+------+------|
 * | TRNS |  ⌘   |  ⌥   |  ⇧   |   ^  |      |-------|    |-------|   .  |   1  |   2  |   3  |   0  |      |
 * `-----------------------------------------/       /     \       \----------------------------------------'
 *                   |      |      |      | /  TRNS /       \ TRNS \  |      |      |      |
 *                   | TRNS | TRNS | TRNS |/       /         \      \ | TRNS | TRNS | TRNS |
 *                   `----------------------------'           '------''--------------------'
 */

[_NUMBER] = LAYOUT(
    SW_WIN,  KC_LSFT, SW_WIN,  SW_APP,  XXXXXXX, LOGOUT,                     KC_LSFT, KC_LPRN, KC_COLN, KC_RPRN, KC_UNDS, _______,
    SW_APP,  G(KC_Q), G(KC_W), XXXXXXX, XXXXXXX, XXXXXXX,                    KC_PAST, KC_7,    KC_8,    KC_9,    KC_PPLS, XXXXXXX,
    KC_LSFT, G(KC_A), G(KC_X), G(KC_C), G(KC_V), CW_TOGG,                    KC_PSLS, KC_4,    KC_5,    KC_6,    KC_MINS, KC_EQL,
    _______, KC_LGUI, KC_LALT, KC_LSFT, KC_LCTL, XXXXXXX, K_UNDO,   K_REDO,  KC_DOT,  KC_1,    KC_2,    KC_3,    KC_0,    XXXXXXX,
                      _______, _______, _______, _______,                    _______, _______, _______, _______
),

/* NAVIGATION
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |      |      |  M1  |  M2  |  M3  |      |                    | PgUp | |<<  |  ||  |  >>| | VOLU | TRNS |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      | MWLt | MUp  | MWRt | MWUp |                    | PgDn | TabL |  Up  | TabR | VOLD |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |  M1  | MLft | MDn  | MRgt | MWDn |-------.    ,-------| LineB| Left | Down | Rght | LineE|      |
 * |------+------+------+------+------+------|       |    | LLOCK |------+------+------+------+------+------|
 * |      |  ⌘   |  ⌥   |  ⇧   |   ^  |      |-------|    |-------|      | WordL|      | WordR|  ^←  |  ^→  |
 * `-----------------------------------------/      /      \      \-----------------------------------------'
 *                   |     |      |      |  / TRNS /        \LLOCK \  |      |      |      |
 *                   |TRNS | TRNS | TRNS | /      /          \      \ | TRNS | TRNS | TRNS |
 *                   `-------------------''------'            '------''--------------------'
 */

[_NAV] = LAYOUT(
    _______, XXXXXXX, KC_BTN1, KC_BTN2, KC_BTN3, XXXXXXX,                    KC_PGUP, KC_MRWD, KC_MPLY, KC_MFFD,  KC_VOLU, _______,
    _______, XXXXXXX, KC_WH_L, KC_MS_U, KC_WH_R, KC_WH_U,                    KC_PGDN, WEBTAB_L,KC_UP,   WEBTAB_R, KC_VOLD, XXXXXXX,
    _______, KC_BTN1, KC_MS_L, KC_MS_D, KC_MS_R, KC_WH_D,                    LN_BEG,  KC_LEFT, KC_DOWN, KC_RGHT,  LN_END,  XXXXXXX,
    _______, KC_LGUI, KC_LALT, KC_LSFT, KC_LCTL, XXXXXXX,  _______,  LLOCK,  XXXXXXX, WORD_L,  XXXXXXX, WORD_R,   C(KC_LEFT), C(KC_RIGHT),
                      _______, _______, _______, _______,                    LLOCK, _______, _______, _______
),

/* RAISE
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |  ~   |  F1  |  F2  |  F3  |  F4  |  F5  |                    |  F6  |  F7  |  F8  |  F9  | F10  | LOUT |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |  `   |   !  |   @  |   #  |   $  |   %  |                    |   ^  |   &  |   *  |   (  |   )  |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |  {   |   (  |   )  |  }   |      |-------.    ,-------|      |   "  |   '  |   -  |   _  |      |
 * |------+------+------+------+------+------|  TRNS |    | LOGOUT|------+------+------+------+------+------|
 * |      |  <   |   [  |   ]  |  >   |      |-------|    |-------|      |   !  |      |      |   ?  |      |
 * `-----------------------------------------/      /      \      \-----------------------------------------'
 *                   |     |      |      |  / TRNS /        \ TRNS \  |      |      |      |
 *                   |TRNS | TRNS | TRNS | /      /          \      \ | TRNS | TRNS | TRNS |
 *                   `-------------------''------'            '------''--------------------'
 */

[_RAISE] = LAYOUT(
   KC_TILD, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                      KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  LOGOUT,
   KC_GRV,  KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,                    KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, XXXXXXX,
   _______, KC_LCBR, KC_LPRN, KC_RPRN, KC_RCBR, XXXXXXX,                    XXXXXXX, KC_DQUO, KC_QUOT, KC_MINS, KC_UNDS, XXXXXXX,
   _______, KC_LT,   KC_LBRC, KC_RBRC, KC_GT, XXXXXXX, _______,  LOGOUT,    XXXXXXX, KC_EXLM, XXXXXXX, XXXXXXX, KC_QUES, XXXXXXX,
                      _______, _______, _______, _______,                    _______, _______, _______, _______
),

/* FUNCTION
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |      |      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |      |      |      |      |                    |PrtScr|  F7  |  F8  |  F9  | F12  |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------.    ,-------|ScrLck|  F4  |  F5  |  F6  | F11  |      |
 * |------+------+------+------+------+------|       |    |       |------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------|    |-------|Pause |  F1  |  F2  |  F3  | F10  |      |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   |      |      |      | /       /       \      \  |      |      |      |
 *                   |      |      |      |/       /         \      \ |      |      |      |
 *                   `----------------------------'           '------''--------------------'
 */

[_FUNCTION] = LAYOUT(
    _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,
    _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                    KC_PSCR, KC_F7,   KC_F8,   KC_F9,   KC_F12,  _______,
    _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                    KC_SCRL, KC_F4,   KC_F5,   KC_F6,   KC_F11,  _______,
    _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______, _______,  KC_PAUS, KC_F1,   KC_F2,   KC_F3,   KC_F10,  _______,
                      _______, _______, _______, _______,                    _______, _______, _______, _______
),

/* ADJUST
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
 *                   `--------------------''------'           '------''--------------------'
 */

  [_CONF] = LAYOUT(
    KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,                      CLEAR,   KC_NO,    KC_NO,  KC_NO,   AS_UP,   DT_UP,
    KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,                      RGB_TOG, RGB_MOD,  RGB_HUI,KC_NO,   AS_DOWN,   DT_DOWN,
    KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,                      KC_NO,   RGB_RMOD, RGB_HUD,KC_NO,   AS_RPT,   DT_PRNT,
    KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,  _______,  _______,  KC_NO,   KC_NO,    KC_NO,  KC_NO,   KC_NO,   KC_NO,
                      _______, _______, _______, _______,                    _______, QWERTY,   COLEMK, _______
  )
};
// clang-format on

/* The Liatris LED is hella bright, turn that off for dark rooms.
 */
void keyboard_pre_init_user(void) {
    // Set our LED pin as output
    setPinOutput(24);
    // Turn the LED off
    // (Due to technical reasons, high is off and low is on)
    writePinHigh(24);
}

/* Standard init with the default layer set here (see definition above)
 */
void keyboard_post_init_user(void) {
    default_layer_set(1 << DEFAULT_LAYER);

#ifdef CONSOLE_ENABLE
    // debug_enable=true;
    // debug_matrix=true;
    // debug_keyboard=true;
    // debug_mouse=true;
#endif // CONSOLE_ENABLE
}

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
    if (IS_RETRO(keycode)) return true;

    return false;
}

/* Update LED state based on caps word and mod hold flash */
static void _update_led_state(void) {
    // If mod hold flash is active, LED is controlled by flash
    if (mod_hold_flash_end_time > 0) {
        // Flash is active, LED should be on (low = on)
        writePinLow(24);
    } else {
        // No flash active, set LED based on caps word state
        // LED pin 24 is inverted: low = on, high = off
        if (caps_word_active) {
            writePinLow(24); // Turn LED on when caps word is active
        } else {
            writePinHigh(24); // Turn LED off when caps word is inactive
        }
    }
}

/* This is used to like up the liatris LED as an indicator that we are in caps word mode
 *
 * In the future this could also be represented on the LED screens.
 * Note: LED pin 24 is inverted (low = on, high = off)
 */
void caps_word_set_user(bool active) {
    caps_word_active = active;
    // Update LED state immediately
    _update_led_state();
}

/* Check if a keycode is a tap-hold mod key */
static bool _is_tap_hold_mod(uint16_t keycode) {
    // Check for all the homerow mod keycodes
    return (keycode == CMH_Z || keycode == CMH_X || keycode == CMH_C || keycode == CMH_D || keycode == CMH_SLSH || keycode == CMH_DOT || keycode == CMH_COMM || keycode == CMH_H || keycode == QMH_Z || keycode == QMH_X || keycode == QMH_C || keycode == QMH_V || keycode == QMH_M || keycode == QMH_N || keycode == QMH_COMM || keycode == QMH_DOT);
}

/* Find or add a mod tracker */
static mod_hold_tracker_t *_find_mod_tracker(uint16_t keycode) {
    // First, try to find existing tracker
    for (uint8_t i = 0; i < mod_hold_count; i++) {
        if (mod_hold_trackers[i].keycode == keycode) {
            return &mod_hold_trackers[i];
        }
    }

    // If not found and we have space, add a new one
    if (mod_hold_count < 8) {
        mod_hold_trackers[mod_hold_count].keycode     = keycode;
        mod_hold_trackers[mod_hold_count].has_flashed = false;
        mod_hold_trackers[mod_hold_count].press_time  = 0;
        return &mod_hold_trackers[mod_hold_count++];
    }

    return NULL;
}

/* Remove a mod tracker */
static void _remove_mod_tracker(uint16_t keycode) {
    for (uint8_t i = 0; i < mod_hold_count; i++) {
        if (mod_hold_trackers[i].keycode == keycode) {
            // Shift remaining trackers down
            for (uint8_t j = i; j < mod_hold_count - 1; j++) {
                mod_hold_trackers[j] = mod_hold_trackers[j + 1];
            }
            mod_hold_count--;
            break;
        }
    }
}

/* Flash the LED once */
static void _flash_mod_hold_led(void) {
    // Turn LED on (low is on) - this will be managed by _update_led_state
    // Just set the flash timer, _update_led_state will handle the LED
}

/* This handles treating a layer-top as a modifier in some situations.  For example, if you
 * want the SYM layer switch to respond like cmd-tab you will need to register and hold cmd
 * if tab is detected.
 *
 * This works mostly well but has some glitches that I need to work with.  For instance if
 * if you press shift it will unregister cmd since it is seeing a key other than tab which
 * breaks the convenient cmd-tab / cmd-shft-tab functionality.  It also has some difficulty
 * tracking states when you are using cmd for different functions like cmd-` and cmd-tab.
 * the fix is likely to have a single function that takes state and a map of input to actions
 * as well as keys that will work as normal without deregistering the cmd (like shift)
 *
 * Original: https://github.com/qmk/qmk_firmware/tree/user-keymaps-still-present/users/callum
 */
bool sw_app_active = false;
bool sw_win_active = false;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!process_layer_lock(keycode, record, LLOCK)) {
        return false;
    }

    // Simple swapper calls - enhanced swapper allows shift to be held
    update_swapper(&sw_app_active, KC_LGUI, KC_TAB, SW_APP, keycode, record);
    update_swapper(&sw_win_active, KC_LGUI, KC_GRV, SW_WIN, keycode, record);

    // Track tap-hold mod keys for LED flashing
    if (_is_tap_hold_mod(keycode)) {
        if (record->event.pressed) {
            // Key pressed - start tracking
            mod_hold_tracker_t *tracker = _find_mod_tracker(keycode);
            if (tracker) {
                tracker->press_time  = timer_read32();
                tracker->has_flashed = false;
            }
        } else {
            // Key released - check if it was held (tap.count == 0 means held, not tapped)
            if (record->tap.count == 0) {
                // It was held, remove tracker
                _remove_mod_tracker(keycode);
            } else {
                // It was tapped, remove tracker
                _remove_mod_tracker(keycode);
            }
        }
    }

    // Call OLED-specific handler (safe when OLED is disabled)
#ifdef OLED_ENABLE
    if (!oled_process_record_user(keycode, record)) {
        return false;
    }
#endif

    // Handle special characters
    if (record->event.pressed) {
        switch (keycode) {
            case KC_RD_ARROW:
                send_string("->");
                return false;
            case KC_LD_ARROW:
                send_string("<-");
                return false;
            case KC_SCREENSHOT:
                tap_code16(LGUI(LSFT(LCTL(KC_4))));
                return false;
        }
    }
    return true;
}

/* Check mod hold timers and flash LED when threshold is reached */
void matrix_scan_user(void) {
    uint32_t now = timer_read32();

    // Check if mod hold flash has ended
    if (mod_hold_flash_end_time > 0 && now >= mod_hold_flash_end_time) {
        mod_hold_flash_end_time = 0;
        // Update LED state to restore caps word state
        _update_led_state();
    }

    // Check all tracked mod keys
    for (uint8_t i = 0; i < mod_hold_count; i++) {
        mod_hold_tracker_t *tracker = &mod_hold_trackers[i];

        if (!tracker->has_flashed && tracker->press_time > 0) {
            uint32_t hold_duration = timer_elapsed32(tracker->press_time);

            // If held long enough, flash the LED once
            if (hold_duration >= MOD_HOLD_FLASH_THRESHOLD_MS) {
                _flash_mod_hold_led();
                tracker->has_flashed    = true;
                mod_hold_flash_end_time = now + MOD_HOLD_FLASH_DURATION_MS;
                // Update LED state to show flash
                _update_led_state();
            }
        }
    }
}

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
        case _NAV:
            config.default_color = (HSV){HSV_BLUE};
            // Arrow keys highlighted in white
            config.highlights[0]   = (keycode_highlight_t){KC_UP, (HSV){HSV_WHITE}};
            config.highlights[1]   = (keycode_highlight_t){KC_DOWN, (HSV){HSV_WHITE}};
            config.highlights[2]   = (keycode_highlight_t){KC_LEFT, (HSV){HSV_WHITE}};
            config.highlights[3]   = (keycode_highlight_t){KC_RGHT, (HSV){HSV_WHITE}};
            config.highlight_count = 4;
            break;
        case _NUMBER:
            config.default_color = (HSV){HSV_WHITE};
            // Logout button highlighted in red
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

// Add combo definitions before the keymaps array
const uint16_t PROGMEM combo_rd_arrow[]   = {KC_RIGHT, KC_DOWN, COMBO_END};
const uint16_t PROGMEM combo_ld_arrow[]   = {KC_LEFT, KC_DOWN, COMBO_END};
const uint16_t PROGMEM combo_screenshot[] = {KC_Q, KC_W, KC_F, KC_P, COMBO_END};

combo_t key_combos[] = {COMBO(combo_rd_arrow, KC_RD_ARROW), COMBO(combo_ld_arrow, KC_LD_ARROW), COMBO(combo_screenshot, KC_SCREENSHOT)};
