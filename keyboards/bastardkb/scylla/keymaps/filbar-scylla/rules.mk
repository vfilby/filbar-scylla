# Scylla Defaults
AUDIO_SUPPORTED = no        # Audio is not supported
RGB_MATRIX_SUPPORTED = yes  # RGB matrix is supported and enabled by default
SERIAL_DRIVER = vendor

# To enable debug messaging via qmk console set to 'yes'
CONSOLE_ENABLE = no

# Disable while making changes to the keymap
# or clear the EEPROM using the `clear` on the
# config layer
VIA_ENABLE = no

SRC += features/layer_lock.c
