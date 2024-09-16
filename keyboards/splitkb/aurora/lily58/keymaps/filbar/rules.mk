# See default rules.mk for Lily58 board at https://github.com/qmk/qmk_firmware/blob/master/keyboards/lily58/rules.mk for more options.

EXTRAKEY_ENABLE = yes # Audio control and System control
OLED_ENABLE = yes     # OLED display
MOUSEKEY_ENABLE = yes
CONVERT_TO = liatris
CAPS_WORD_ENABLE = yes

# To enable debug messaging via qmk console set to 'yes'
CONSOLE_ENABLE = yes

SRC += features/layer_lock.c
SRC += features/swapper.c
