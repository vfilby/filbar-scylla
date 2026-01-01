#include "swapper.h"

// Helper function to check if a keycode is a modifier that should be allowed
static bool is_allowed_modifier(uint16_t keycode) {
    // Allow all shift keys to be used without exiting swapper mode
    return (keycode == KC_LSFT || keycode == KC_RSFT);
}

void update_swapper(bool *active, uint16_t cmdish, uint16_t tabish, uint16_t trigger, uint16_t keycode, keyrecord_t *record) {
    if (keycode == trigger) {
        if (record->event.pressed) {
            if (!*active) {
                *active = true;
                register_code(cmdish);
            }
            register_code(tabish);
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
