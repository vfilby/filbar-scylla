#include "swapper.h"

#ifdef CONSOLE_ENABLE
#   include "print.h"
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

void update_swapper(
    bool *active,
    uint16_t cmdish,
    uint16_t tabish,
    uint16_t trigger,
    uint16_t keycode,
    keyrecord_t *record
) {
    if (keycode == trigger) {
        uprintf("KL: kc: 0x%04X, col: %2u, row: %2u, pressed: %u, time: %5u, int: %u, count: %u\n",
            keycode,
            record->event.key.col,
            record->event.key.row,
            record->event.pressed,
            record->event.time,
            record->tap.interrupted,
            record->tap.count);

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
        unregister_code(cmdish);
        *active = false;
    }
}

