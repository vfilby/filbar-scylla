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
