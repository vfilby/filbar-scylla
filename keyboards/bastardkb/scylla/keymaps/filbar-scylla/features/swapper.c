#include "swapper.h"

#ifdef CONSOLE_ENABLE
#    include "print.h"
#endif // CONSOLE_ENABLE

// update_swapper( &sw_cmd_active, KC_LGUI, KC_TAB, SW_APP, keycode, record );

// void mo_layer_as_mod(
//     bool *active,
//     std::uint16_t event_keycode,
//     std::uint16_t event_record,
//     std::uint16_t modifier,
//     std::uint16_t *triggers,
//     std::uint16_t
//
// )

// Helper function to check if a keycode is a modifier that should be allowed
static bool is_allowed_modifier(uint16_t keycode) {
    // Allow all shift keys to be used without exiting swapper mode
    return (keycode == KC_LSFT || keycode == KC_RSFT);
}

void update_swapper(bool *active, uint16_t cmdish, uint16_t tabish, uint16_t trigger, uint16_t keycode, keyrecord_t *record) {
    if (keycode == trigger) {
        uprintf("KL: kc: 0x%04X, col: %2u, row: %2u, pressed: %u, time: %5u, int: %u, count: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count);

        if (record->event.pressed) {
            if (!*active) {
                *active = true;
                register_code(cmdish);
            }
            register_code(tabish);
            uprintf("KL: kc: 0x%04X", keycode);
        } else {
            unregister_code(tabish);
            // Don't unregister cmdish until some other key is hit or released.
        }
    } else if (*active) {
        // Check if this is an allowed modifier key
        if (is_allowed_modifier(keycode)) {
            // Allow shift to be pressed/released without exiting swapper mode
            if (record->event.pressed) {
                register_code(keycode);
            } else {
                unregister_code(keycode);
            }
            // Don't exit swapper mode, keep cmdish held
        } else {
            // Any other key exits swapper mode
            unregister_code(cmdish);
            *active = false;
        }
    }
}