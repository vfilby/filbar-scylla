#pragma once

#include QMK_KEYBOARD_H

// Implements cmd-tab like behaviour on a single key. On first tap of trigger
// cmdish is held and tabish is tapped -- cmdish then remains held until some
// other key is hit or released. For example:
//
//     trigger, trigger, a -> cmd down, tab, tab, cmd up, a
//     nav down, trigger, nav up -> nav down, cmd down, tab, cmd up, nav up
//
// Enhanced behavior: Shift keys can be pressed and released without exiting
// swapper mode. This allows for cmd-shift-tab to go backwards:
//
//     trigger, trigger, shift down, trigger, shift up, trigger ->
//         cmd down, tab, tab, shift down, tab, shift up, tab, cmd stays down
//
// This behaviour is useful for more than just cmd-tab, hence: cmdish, tabish.
void update_swapper(bool *active, uint16_t cmdish, uint16_t tabish, uint16_t trigger, uint16_t keycode, keyrecord_t *record);
