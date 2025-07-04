#pragma once

#include QMK_KEYBOARD_H

#ifdef OLED_ENABLE

// Function declarations for minimal OLED display
bool oled_task_user(void);

#endif // OLED_ENABLE
