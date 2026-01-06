#pragma once

#include QMK_KEYBOARD_H

// Layer definitions - matches Corne layout
enum lily_layers {
    _BASE = 0,
    _COLEMAK,
    _QWERTY,
    _NUMBER,
    _NAV,
    _MOUSE,
    _SYM,
    _MEDIA,
    _FUNCTION,
    _SPECIAL,
    _CONF,
};

// Layer names for display
static const char PROGMEM layer_names[][6] = {
    "BASE ",
    "COLEM",
    "QWERT",
    "NUMB ",
    "NAV  ",
    "MOUSE",
    "SYM  ",
    "MEDIA",
    "FUNCT",
    "SPECL",
    "CONF ",
};
