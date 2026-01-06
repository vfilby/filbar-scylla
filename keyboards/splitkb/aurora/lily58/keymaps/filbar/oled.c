#include QMK_KEYBOARD_H
#include "oled.h"
#include "layers.h"

#ifdef OLED_ENABLE

// Animation state variables
static uint32_t anim_timer = 0;
uint32_t anim_sleep = 0;
static uint8_t current_idle_frame = 0;
static uint8_t current_tap_frame = 0;
// static uint32_t oled_timeout = OLED_TIMEOUT_MS;  // Unused variable - removed
static bool anim_initialized = false;
// static bool is_idle = true;  // Unused variable - removed
bool oled_sleeping = false;

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
	if (!is_keyboard_master()) {
        return OLED_ROTATION_180;
    } else {
        return rotation;
    }
}

void fb_render_space(void) {
    oled_write_P(PSTR("     "), false);
}

void fb_render_mod_status_gui_alt(uint8_t modifiers) {
    static const char PROGMEM gui_off_1[] = {0x85, 0x86, 0};
    static const char PROGMEM gui_off_2[] = {0xa5, 0xa6, 0};
    static const char PROGMEM gui_on_1[] = {0x8d, 0x8e, 0};
    static const char PROGMEM gui_on_2[] = {0xad, 0xae, 0};

    static const char PROGMEM alt_off_1[] = {0x87, 0x88, 0};
    static const char PROGMEM alt_off_2[] = {0xa7, 0xa8, 0};
    static const char PROGMEM alt_on_1[] = {0x8f, 0x90, 0};
    static const char PROGMEM alt_on_2[] = {0xaf, 0xb0, 0};

    // fillers between the modifier icons bleed into the icon frames
    static const char PROGMEM off_off_1[] = {0xc5, 0};
    static const char PROGMEM off_off_2[] = {0xc6, 0};
    static const char PROGMEM on_off_1[] = {0xc7, 0};
    static const char PROGMEM on_off_2[] = {0xc8, 0};
    static const char PROGMEM off_on_1[] = {0xc9, 0};
    static const char PROGMEM off_on_2[] = {0xca, 0};
    static const char PROGMEM on_on_1[] = {0xcb, 0};
    static const char PROGMEM on_on_2[] = {0xcc, 0};

    if(modifiers & MOD_MASK_GUI) {
        oled_write_P(gui_on_1, false);
    } else {
        oled_write_P(gui_off_1, false);
    }

    if ((modifiers & MOD_MASK_GUI) && (modifiers & MOD_MASK_ALT)) {
        oled_write_P(on_on_1, false);
    } else if(modifiers & MOD_MASK_GUI) {
        oled_write_P(on_off_1, false);
    } else if(modifiers & MOD_MASK_ALT) {
        oled_write_P(off_on_1, false);
    } else {
        oled_write_P(off_off_1, false);
    }

    if(modifiers & MOD_MASK_ALT) {
        oled_write_P(alt_on_1, false);
    } else {
        oled_write_P(alt_off_1, false);
    }

    if(modifiers & MOD_MASK_GUI) {
        oled_write_P(gui_on_2, false);
    } else {
        oled_write_P(gui_off_2, false);
    }

    if ((modifiers & MOD_MASK_GUI) && (modifiers & MOD_MASK_ALT)) {
        oled_write_P(on_on_2, false);
    } else if(modifiers & MOD_MASK_GUI) {
        oled_write_P(on_off_2, false);
    } else if(modifiers & MOD_MASK_ALT) {
        oled_write_P(off_on_2, false);
    } else {
        oled_write_P(off_off_2, false);
    }

    if(modifiers & MOD_MASK_ALT) {
        oled_write_P(alt_on_2, false);
    } else {
        oled_write_P(alt_off_2, false);
    }
}

void fb_render_mod_status_ctrl_shift(uint8_t modifiers) {
    static const char PROGMEM ctrl_off_1[] = {0x89, 0x8a, 0};
    static const char PROGMEM ctrl_off_2[] = {0xa9, 0xaa, 0};
    static const char PROGMEM ctrl_on_1[] = {0x91, 0x92, 0};
    static const char PROGMEM ctrl_on_2[] = {0xb1, 0xb2, 0};

    static const char PROGMEM shift_off_1[] = {0x8b, 0x8c, 0};
    static const char PROGMEM shift_off_2[] = {0xab, 0xac, 0};
    static const char PROGMEM shift_on_1[] = {0xcd, 0xce, 0};
    static const char PROGMEM shift_on_2[] = {0xcf, 0xd0, 0};

    // fillers between the modifier icons bleed into the icon frames
    static const char PROGMEM off_off_1[] = {0xc5, 0};
    static const char PROGMEM off_off_2[] = {0xc6, 0};
    static const char PROGMEM on_off_1[] = {0xc7, 0};
    static const char PROGMEM on_off_2[] = {0xc8, 0};
    static const char PROGMEM off_on_1[] = {0xc9, 0};
    static const char PROGMEM off_on_2[] = {0xca, 0};
    static const char PROGMEM on_on_1[] = {0xcb, 0};
    static const char PROGMEM on_on_2[] = {0xcc, 0};

    if(modifiers & MOD_MASK_CTRL) {
        oled_write_P(ctrl_on_1, false);
    } else {
        oled_write_P(ctrl_off_1, false);
    }

    if ((modifiers & MOD_MASK_CTRL) && (modifiers & MOD_MASK_SHIFT)) {
        oled_write_P(on_on_1, false);
    } else if(modifiers & MOD_MASK_CTRL) {
        oled_write_P(on_off_1, false);
    } else if(modifiers & MOD_MASK_SHIFT) {
        oled_write_P(off_on_1, false);
    } else {
        oled_write_P(off_off_1, false);
    }

    if(modifiers & MOD_MASK_SHIFT) {
        oled_write_P(shift_on_1, false);
    } else {
        oled_write_P(shift_off_1, false);
    }

    if(modifiers & MOD_MASK_CTRL) {
        oled_write_P(ctrl_on_2, false);
    } else {
        oled_write_P(ctrl_off_2, false);
    }

    if ((modifiers & MOD_MASK_CTRL) && (modifiers & MOD_MASK_SHIFT)) {
        oled_write_P(on_on_2, false);
    } else if(modifiers & MOD_MASK_CTRL) {
        oled_write_P(on_off_2, false);
    } else if(modifiers & MOD_MASK_SHIFT) {
        oled_write_P(off_on_2, false);
    } else {
        oled_write_P(off_off_2, false);
    }

    if(modifiers & MOD_MASK_SHIFT) {
        oled_write_P(shift_on_2, false);
    } else {
        oled_write_P(shift_off_2, false);
    }
}

//
// Render right OLED display animation
//
static void render_anim(void) {

    void animation_phase(void) {

        if (get_current_wpm() <= IDLE_SPEED) {
            current_idle_frame = (current_idle_frame + 1) % IDLE_FRAMES;
            oled_write_raw_P(idle[current_idle_frame], ANIM_SIZE);
        }

        if (get_current_wpm() > IDLE_SPEED && get_current_wpm() < TAP_SPEED) {
            oled_write_raw_P(prep[0], ANIM_SIZE);
        }

        if (get_current_wpm() >= TAP_SPEED) {
            current_tap_frame = (current_tap_frame + 1) % TAP_FRAMES;
            oled_write_raw_P(tap[current_tap_frame], ANIM_SIZE);
        }
    }

    // Initialize animation on first call
    if (!anim_initialized) {
        anim_timer = timer_read32();
        anim_sleep = timer_read32();
        oled_sleeping = false;
        anim_initialized = true;
        oled_clear();
        oled_write_raw_P(idle[0], ANIM_SIZE);
        return;
    }

    // Check if we should animate
    uint8_t wpm = get_current_wpm();
    if (wpm != 0) {
        oled_on();

        if (timer_elapsed32(anim_timer) > ANIM_FRAME_DURATION_MS) {
            anim_timer = timer_read32();
            animation_phase();
        }
        anim_sleep = timer_read32();
    } else {

        if (timer_elapsed32(anim_sleep) > OLED_TIMEOUT_MS) {
            //oled_off();
        } else {
            if (timer_elapsed32(anim_timer) > ANIM_FRAME_DURATION_MS) {
                anim_timer = timer_read32();
                animation_phase();
            }
        }

    }
}

// Custom OLED task function that overrides the default
bool oled_task_user(void) {

    // No typing - check if we should sleep
    // if (timer_elapsed32(anim_sleep) > oled_timeout) {
    //     oled_off();
    //     oled_sleeping = true;
    //     return true;
    // }

    // if (oled_sleeping) {
    //     oled_on();
    //     oled_sleeping = false;
    // }



    if (is_keyboard_master()) {
        // Use cursor positioning instead of oled_clear() to reduce I2C traffic
        oled_set_cursor(0, 0);

        // Render modifier status
        fb_render_mod_status_gui_alt(get_mods()|get_oneshot_mods());
        fb_render_mod_status_ctrl_shift(get_mods()|get_oneshot_mods());

        // Show current layer (pad with spaces to overwrite old content)
        fb_render_space();
        uint8_t layer = get_highest_layer(layer_state);
        oled_write_P(layer_names[layer], false);
        oled_write_P(PSTR("     \n"), false);  // Pad to clear previous text

        // Show default layer (pad with spaces to overwrite old content)
        fb_render_space();
        uint8_t default_layer = get_highest_layer(default_layer_state);
        oled_write_P(layer_names[default_layer], false);
        oled_write_P(PSTR("     \n"), false);  // Pad to clear previous text

    } else {
        render_anim();
    }

    return false;
}

// OLED-specific process record handler
bool oled_process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        anim_sleep = timer_read32();
    }
    return true; // Continue processing
}

#endif // OLED_ENABLE

