/*
 *
 * Inspiration taken from: https://github.com/archydragon/lily-layout/blob/main/qmk_keymap/keymap.c
 * led as a caps indicator: https://discord.com/channels/574598631399751680/574738351836626944/1133073274209841202
 * Getting retro shift to work with tap-hold mods: https://www.reddit.com/r/qmk/comments/10k1oya/autoshift_with_homerow_mods/
 */

#include "keycodes.h"
#include QMK_KEYBOARD_H
#include "transactions.h"
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
static uint16_t           smh_lprn_timer          = 0;     // Timer for SMH_LPRN custom mod-tap
static uint16_t           smh_rprn_timer          = 0;     // Timer for SMH_RPRN custom mod-tap

#define DEFAULT_LAYER _COLEMAK

enum lily_keycodes {
    SW_APP = SAFE_RANGE, // Switch app windows (cmd-tab, with shift for reverse)
    SW_WIN,              // Switch apps        (cmd-`)
    KC_RD_ARROW,         // ->
    KC_LD_ARROW,         // <-
    KC_SCREENSHOT,       // cmd+shift+ctrl+4
    KC_EMDASH,           // em dash
    SMH_LPRN_KC,         // Custom keycode for Alt + ( mod-tap
    SMH_RPRN_KC,         // Custom keycode for Shift + ) mod-tap
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

// Left-hand home row mods for Colemak (ARST) - matches Corne
#define CMH_A LGUI_T(KC_A)
#define CMH_R LALT_T(KC_R)
#define CMH_S LSFT_T(KC_S)
#define CMH_T LCTL_T(KC_T)

// Right-hand home row mods for Colemak (NEIO) - matches Corne
#define CMH_O RGUI_T(KC_O)
#define CMH_I LALT_T(KC_I)
#define CMH_E RSFT_T(KC_E)
#define CMH_N RCTL_T(KC_N)

// Left-hand home row mods for QWERTY (ASDF) - matches Corne
#define QMH_A LGUI_T(KC_A)
#define QMH_S LALT_T(KC_S)
#define QMH_D LSFT_T(KC_D)
#define QMH_F LCTL_T(KC_F)

// Right-hand home row mods for QWERTY (JKL;) - matches Corne
#define QMH_SCLN RGUI_T(KC_SCLN)
#define QMH_L LALT_T(KC_L)
#define QMH_K RSFT_T(KC_K)
#define QMH_J RCTL_T(KC_J)

// Left-hand home row mods for SYM layer (brackets) - matches Corne
#define SMH_LBRC LGUI_T(KC_LBRC)
#define SMH_LPRN SMH_LPRN_KC
#define SMH_RPRN SMH_RPRN_KC
#define SMH_RBRC LCTL_T(KC_RBRC)

// Config layer tap keys - matches Corne
#define B_CONF LT(_CONF, KC_B)
#define T_CONF LT(_CONF, KC_T)

// Layer short names - matches Corne
#define SPC_NUM LT(_NUMBER,KC_SPC)
#define BSPC_NAV LT(_NAV, KC_BSPC)
#define DEL_MOUS LT(_MOUSE, KC_DEL)
#define ENT_MED LT(_MEDIA, KC_ENT)

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* BASE_MODIFIER - Matches Corne layout (no number row, 3 outer pinky keys per side)
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |      |      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | ESC  |      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  |      |      |      |      |      |-------.    ,-------|      |      |      |      |      |      |
 * |------+------+------+------+------+------|  UNDO |    | REDO  |------+------+------+------+------+------|
 * | Shft |      |      |      |      |      |-------|    |-------|      |      |      |      |      | Shft |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   |      |      |      | / Space /       \ Entr \  |      |      |      |
 *                   | LGUI |BsNAV |SpcNUM|/       /         \      \ |EntMED| SYM  |DelMOU|
 *                   `----------------------------'           '------''--------------------'
 */

[_BASE] = LAYOUT(
   KC_F13,  KC_F14,  KC_F15,  KC_F16,  KC_F17,  KC_F18,                       KC_F19,  KC_F20,  KC_F21,  KC_F22,  KC_F23,  KC_F24,
   QK_GESC, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
   KC_TAB,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
   KC_LSFT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, K_UNDO,     K_REDO,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_RSFT,
                     KC_LCTL, KC_LGUI, BSPC_NAV, SPC_NUM,              ENT_MED,MO(_SYM), DEL_MOUS, KC_LGUI
),

/* COLEMAK-dhm
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | TRNS | TRNS | TRNS | TRNS | TRNS | TRNS |                    | TRNS | TRNS | TRNS | TRNS | TRNS | TRNS |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | TRNS |   Q  |   W  |   F  |   P  |   B  |                    |   J  |   L  |   U  |   Y  |   ;  |  -   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | TRNS |   A  |   R  |   S  |   T  |   G  |-------.    ,-------|   M  |   N  |   E  |   I  |   O  |  '   |
 * |------+------+------+------+------+------|  TRNS |    | TRNS  |------+------+------+------+------+------|
 * | TRNS |   Z  |   X  |   C  |   D  |   V  |-------|    |-------|   K  |   H  |   ,  |   .  |   /  | TRNS |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   |      |      |      | /  TRNS /       \ TRNS \  |      |      |      |
 *                   | TRNS | TRNS | TRNS |/       /         \      \ | TRNS | TRNS | TRNS |
 *                   `----------------------------'           '------''--------------------'
 */

[_COLEMAK] = LAYOUT(
   _______, _______, _______, _______, _______, _______,                    _______, _______, _______, _______, _______, _______,
   _______, KC_Q,    KC_W,    KC_F,    KC_P,    B_CONF,                     KC_J,    KC_L,    KC_U,    KC_Y,    KC_SCLN, KC_MINS,
   _______, CMH_A,   CMH_R,   CMH_S,   CMH_T,   KC_G,                       KC_M,    CMH_N,   CMH_E,   CMH_I,   CMH_O,   KC_QUOT,
   _______, KC_Z,    KC_X,    KC_C,    KC_D,    KC_V,  _______,   _______,  KC_K,    KC_H,    KC_COMM, KC_DOT,  KC_SLSH, _______,
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
 * | TRNS |   Z  |   X  |   C  |   V  |   B  |-------|    |-------|   N  |   M  |   ,  |   .  |   /  | TRNS |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   |      |      |      | /  TRNS /       \ TRNS \  |      |      |      |
 *                   | TRNS | TRNS | TRNS |/       /         \      \ | TRNS | TRNS | TRNS |
 *                   `----------------------------'           '------''--------------------'
 */

 [_QWERTY] = LAYOUT(
    _______, _______, _______, _______, _______, _______,                    _______, _______, _______, _______, _______, _______,
    _______, KC_Q,    KC_W,    KC_E,    KC_R,    T_CONF,                     KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_MINS,
    _______, QMH_A,   QMH_S,   QMH_D,   QMH_F,   KC_G,                       KC_H,    QMH_J,   QMH_K,   QMH_L,   QMH_SCLN,KC_QUOT,
    _______, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    _______,  _______, KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, _______,
                      _______, _______, _______, _______,                    _______, _______, _______, _______
),

/* Numbers
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |      |      |      |      |      |      |                    |   [  |   7  |   8  |   9  |   ]  | TRNS |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |SW_WIN|      |      |      |      |LOGOUT|                    |   ;  |   4  |   5  |   6  |   =  | TRNS |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |SW_APP|  ⌘   |  ⌥   |  ⇧   |   ^  |      |-------.    ,-------|   ~  |   1  |   2  |   3  |   \  | TRNS |
 * |------+------+------+------+------+------|  UNDO |    | REDO  |------+------+------+------+------+------|
 * | TRNS |      |      |      |      |      |-------|    |-------|   -  |   0  |   .  |      |      | TRNS |
 * `-----------------------------------------/       /     \       \----------------------------------------'
 *                   |      |      |      | /  TRNS /       \ TRNS \  |      |      |      |
 *                   | TRNS | TRNS | TRNS |/       /         \      \ | TRNS | TRNS | TRNS |
 *                   `----------------------------'           '------''--------------------'
 */

[_NUMBER] = LAYOUT(
    _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,
     SW_WIN, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  LOGOUT,                    KC_LBRC, KC_7,    KC_8,    KC_9,    KC_RBRC, _______,
     SW_APP, KC_LGUI, KC_LALT, KC_LSFT, KC_LCTL, XXXXXXX,                    KC_SCLN, KC_4,    KC_5,    KC_6,     KC_EQL, _______,
    _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, K_UNDO,   K_REDO,  KC_TILD, KC_1,    KC_2,    KC_3,    KC_BSLS, _______,
                      _______, _______, _______, _______,                    KC_0, KC_MINS,  KC_DOT, _______
),

/* NAVIGATION
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |      |      |      |      |      |      |                    |      |      |      |      |      | TRNS |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |      |      |      |      |                    | PgUp | TabL |  Up  | TabR |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |  ⌘   |  ⌥   |  ⇧   |   ^  |      |-------.    ,-------| LineB| Left | Down | Rght | LineE|      |
 * |------+------+------+------+------+------|  TRNS |    | TRNS  |------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------|    |-------| PgDn | WordL|      | WordR|  ^←  |  ^→  |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   |      |      |      | / TRNS /       \ TRNS \  |      |      |      |
 *                   | TRNS | TRNS | TRNS |/       /         \      \ | TRNS | TRNS | TRNS |
 *                   `----------------------------'           '------''--------------------'
 */

[_NAV] = LAYOUT(
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                    KC_PGUP,WEBTAB_L, KC_UP,  WEBTAB_R, XXXXXXX, XXXXXXX,
    XXXXXXX, KC_LGUI, KC_LALT, KC_LSFT, KC_LCTL, XXXXXXX,                     LN_BEG, KC_LEFT, KC_DOWN, KC_RGHT,  LN_END, XXXXXXX,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  _______,  _______,KC_PGDN,  WORD_L,  XXXXXXX, WORD_R, C(KC_LEFT), C(KC_RIGHT),
                      _______, _______, _______, _______,                    _______, _______, _______, _______
),

/* MOUSE - Dedicated mouse layer (matches Corne)
 */
[_MOUSE] = LAYOUT(
    _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    _______, XXXXXXX, XXXXXXX, MS_UP,   XXXXXXX, XXXXXXX,                    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    _______, XXXXXXX, MS_LEFT, MS_DOWN, MS_RGHT, XXXXXXX,                    XXXXXXX, KC_RCTL, KC_RSFT, KC_RALT, KC_RGUI, XXXXXXX,
    _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, MS_WHLU, MS_WHLD, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                      MS_BTN3, MS_BTN2, MS_BTN1, MS_BTN1,                    _______, _______, _______, _______
),

/* SYM - Symbol layer (matches Corne)
 */
[_SYM] = LAYOUT(
   KC_TILD, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    KC_GRV, KC_EXLM,   KC_AT, KC_HASH, KC_DLR,  KC_PERC,                    KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, _______,
   _______,SMH_LBRC,SMH_LPRN,SMH_RPRN,SMH_RBRC, XXXXXXX,                    KC_TILD, KC_DQUO, KC_QUOT, KC_MINS, KC_UNDS, KC_BSLS,
   _______, KC_LT,   KC_LCBR, KC_RCBR, KC_GT,   XXXXXXX, _______,  _______, KC_SLSH, KC_EXLM, KC_PIPE, KC_COLN, KC_QUES, KC_GRV,
                      _______, _______, KC_BSPC, _______,                    _______, _______, _______, _______
),

/* MEDIA - Dedicated media layer (matches Corne)
 */
[_MEDIA] = LAYOUT(
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                    XXXXXXX, KC_VOLD, KC_MUTE, KC_VOLU, XXXXXXX, XXXXXXX,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                    XXXXXXX, KC_MRWD, KC_MPLY, KC_MFFD, XXXXXXX, XXXXXXX,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,  _______, XXXXXXX, KC_BRID, XXXXXXX, KC_BRIU, XXXXXXX, XXXXXXX,
                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                    _______, _______, _______, _______
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
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                    KC_PSCR, KC_F7,   KC_F8,   KC_F9,   KC_F12,  XXXXXXX,
    XXXXXXX, KC_LGUI, KC_LALT, KC_LSFT, KC_LCTL, XXXXXXX,                    KC_SCRL, KC_F4,   KC_F5,   KC_F6,   KC_F11,  XXXXXXX,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______, _______,  KC_PAUS, KC_F1,   KC_F2,   KC_F3,   KC_F10,  XXXXXXX,
                      _______, _______, _______, _______,                    _______, _______, _______, _______
),

/* SPECIAL - Placeholder layer (matches Corne)
 */
[_SPECIAL] = LAYOUT(
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______, _______,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
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
      KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,                      KC_NO,  KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
    QK_BOOT, RM_NEXT, RM_HUEU,   KC_NO,   KC_NO,   KC_NO,                  QK_REBOOT,  KC_NO,   KC_NO,   KC_NO,   AS_UP,   DT_UP,
    RM_TOGG, RM_PREV, RM_HUED,   KC_NO,   KC_NO,   KC_NO,                    RM_TOGG,  KC_NO,   KC_NO,   KC_NO,   AS_DOWN, DT_DOWN,
      KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,  _______,  _______,  KC_NO,  KC_NO,   KC_NO,   KC_NO,   AS_RPT,  DT_PRNT,
                      _______, _______, _______, _______,                    _______, QWERTY,  COLEMK,  _______
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

/* Split keyboard Caps Word state sync
 * The master sends its Caps Word state to the slave so both halves
 * can show the breathing red LED indicator.
 */
static bool caps_word_state_synced = false;  // Slave-side copy of Caps Word state

void caps_word_sync_handler(uint8_t in_buflen, const void* in_data, uint8_t out_buflen, void* out_data) {
    const bool* master_state = (const bool*)in_data;
    caps_word_state_synced = *master_state;
}

/* Helper to check Caps Word state on either half */
static bool is_caps_word_active(void) {
    if (is_keyboard_master()) {
        return is_caps_word_on();
    } else {
        return caps_word_state_synced;
    }
}

/* Standard init with the default layer set here (see definition above)
 */
void keyboard_post_init_user(void) {
    default_layer_set(1 << DEFAULT_LAYER);

    // Register the split transaction handler for Caps Word sync
    transaction_register_rpc(USER_SYNC_CAPS_WORD, caps_word_sync_handler);

#ifdef CONSOLE_ENABLE
    // debug_enable=true;
    // debug_matrix=true;
    // debug_keyboard=true;
    // debug_mouse=true;
#endif // CONSOLE_ENABLE
}

/* Housekeeping task - runs every matrix scan cycle
 * Used to sync Caps Word state to slave half
 */
void housekeeping_task_user(void) {
    // Only run on master side
    if (is_keyboard_master()) {
        static bool last_caps_word_state = false;
        bool current_state = is_caps_word_on();

        // Only sync when state changes to reduce traffic
        if (current_state != last_caps_word_state) {
            if (transaction_rpc_send(USER_SYNC_CAPS_WORD, sizeof(current_state), &current_state)) {
                last_caps_word_state = current_state;
            }
        }
    }
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
    // Disable auto-shift during Caps Word to prevent conflicts
    if (is_caps_word_on()) return false;

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

/* Check if a keycode is a tap-hold mod key */
static bool _is_tap_hold_mod(uint16_t keycode) {
    // Check for all the home row mod keycodes (actual home row - matches Corne)
    // Colemak: ARST (left), NEIO (right)
    // QWERTY: ASDF (left), JKL; (right)
    return (keycode == CMH_A || keycode == CMH_R || keycode == CMH_S || keycode == CMH_T ||
            keycode == CMH_N || keycode == CMH_E || keycode == CMH_I || keycode == CMH_O ||
            keycode == QMH_A || keycode == QMH_S || keycode == QMH_D || keycode == QMH_F ||
            keycode == QMH_J || keycode == QMH_K || keycode == QMH_L || keycode == QMH_SCLN);
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
    // Simple swapper calls - enhanced swapper allows shift to be held
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
        case _SYM:
            config.default_color   = (HSV){HSV_PURPLE};
            config.highlight_count = 0;
            break;
        case _MOUSE:
            config.default_color   = (HSV){HSV_GREEN};
            config.highlight_count = 0;
            break;
        case _MEDIA:
            config.default_color   = (HSV){HSV_ORANGE};
            config.highlight_count = 0;
            break;
        case _SPECIAL:
            config.default_color   = (HSV){HSV_OFF};
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

/* Check if a matrix position is on the home row (where fingers rest)
 * Home row is row 2 (left: A R S T) and row 7 (right: N E I O), columns 1-4
 */
static bool _is_home_row(uint8_t row, uint8_t col) {
    return ((row == 2 || row == 7) && col >= 1 && col <= 4);
}

/* Check if a matrix position is on the top row (row 0 left, row 5 right) */
static bool _is_top_row(uint8_t row) {
    return (row == 0 || row == 5);
}

/* Get rainbow color for top row keys - each key gets a different hue */
static HSV _get_top_row_color(uint8_t row, uint8_t col) {
    // 12 keys total in top row (6 left + 6 right)
    // Spread hues across the spectrum (0-255)
    uint8_t key_index;
    if (row == 0) {
        key_index = col;  // Left side: 0-5
    } else {
        key_index = 6 + (5 - col);  // Right side: 6-11 (reversed so colors flow naturally)
    }
    // Spread 12 keys across 256 hue values
    uint8_t hue = (key_index * 255) / 11;
    return (HSV){hue, 255, 200};
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

    /* For typing layers: dim cyan for all keys, bright cyan for homing keys, rainbow for top row */
    if (layer <= _COLEMAK) {
        for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
            for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
                uint8_t index = g_led_config.matrix_co[row][col];

                if (index >= led_min && index < led_max && index != NO_LED) {
                    if (_is_home_row(row, col)) {
                        // Home row: bright cyan/teal
                        HSV hsv = {HSV_CYAN};
                        hsv.v = MIN(rgb_matrix_get_val() + LAYER_INDICATOR_BRIGHTNESS_INC, 255);
                        RGB rgb = hsv_to_rgb(hsv);
                        rgb_matrix_set_color(index, rgb.r, rgb.g, rgb.b);
                    } else if (_is_top_row(row)) {
                        // Top row: rainbow colors
                        HSV hsv = _get_top_row_color(row, col);
                        hsv.v = MIN(rgb_matrix_get_val() + LAYER_INDICATOR_BRIGHTNESS_INC, 255);
                        RGB rgb = hsv_to_rgb(hsv);
                        rgb_matrix_set_color(index, rgb.r, rgb.g, rgb.b);
                    } else {
                        // All other keys: dim cyan
                        HSV hsv = {HSV_CYAN};
                        hsv.v = rgb_matrix_get_val() / 4;  // 25% brightness
                        RGB rgb = hsv_to_rgb(hsv);
                        rgb_matrix_set_color(index, rgb.r, rgb.g, rgb.b);
                    }
                }
            }
        }

    /* For special layers use lighting that reflects the keybindings. */
    } else {
        const RGB off = hsv_to_rgb((HSV){HSV_OFF});

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
const uint16_t PROGMEM combo_copy[]       = {KC_Z, KC_X, KC_C, COMBO_END};
const uint16_t PROGMEM combo_paste[]      = {KC_X, KC_C, KC_D, COMBO_END};

combo_t key_combos[] = {
    COMBO(combo_rd_arrow, KC_RD_ARROW),
    COMBO(combo_ld_arrow, KC_LD_ARROW),
    COMBO(combo_screenshot, KC_SCREENSHOT),
    COMBO(combo_copy, G(KC_C)),
    COMBO(combo_paste, G(KC_V)),
};
