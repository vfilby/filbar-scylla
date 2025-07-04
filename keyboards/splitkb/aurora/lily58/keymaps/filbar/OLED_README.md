# Lily58 OLED Display Customization

This directory contains the OLED display code for the Lily58 keyboard, using the original Lily58 OLED implementation from the QMK firmware.

## Files

- `oled.c` - Original Lily58 OLED implementation
- `oled.h` - Header file with function declarations
- `glcdfont.c` - Custom font data (from original Lily58)

## Current Features

The OLED display shows:

### Left Half (Master):
1. **Aurora Logo** - Beautiful Aurora logo at the top
2. **"lily" Text** - Keyboard name
3. **Layer State** - Visual layer indicator with custom graphics
4. **Modifier Status** - Visual indicators for:
   - GUI (Cmd/Windows) - Shows when pressed
   - ALT (Option) - Shows when pressed  
   - CTRL - Shows when pressed
   - SHIFT - Shows when pressed
5. **Lock Keys** - Shows NUM, CAPS, SCR lock states

### Right Half (Slave):
- **Aurora Art** - Beautiful Aurora artwork display

## Customization Options

### Layer Names
The layer display uses custom graphics for different layers:
- Default layer (BASE)
- Lower layer (NAV)
- Raise layer (NUMBER) 
- Adjust layer (CONF)

### Modifier Icons
The modifier status uses custom icon graphics that show:
- Individual modifier states (GUI, ALT, CTRL, SHIFT)
- Combined states (when multiple modifiers are pressed)
- Visual feedback with different icon styles

### Logo and Art
- Aurora logo on the left half
- Aurora artwork on the right half
- Both are high-quality custom graphics

## How to Customize

### 1. Change Layer Graphics
Edit the `render_layer_state()` function in `oled.c`:
```c
static const char PROGMEM your_layer[] = {
    0x20, 0x94, 0x95, 0x96, 0x20,
    0x20, 0xb4, 0xb5, 0xb6, 0x20,
    0x20, 0xd4, 0xd5, 0xd6, 0x20, 0};
```

### 2. Change Logo
Replace the `aurora_logo` array in `render_logo()` function.

### 3. Change Artwork
Replace the `aurora_art` array in the `oled_task_kb()` function.

### 4. Add New Information
You can add new functions and call them in `oled_task_kb()`:
```c
void render_your_info(void) {
    oled_write_P(PSTR("Your Info"), false);
}
```

## Font Customization

The `glcdfont.c` file contains the custom font data. You can:
- Modify existing characters
- Add new characters
- Change the font style

## Compilation

The OLED code is automatically included when you compile the firmware:
```bash
qmk flash -kb splitkb/aurora/lily58 -km filbar
```

## Notes

- The OLED rotation is set to 270 degrees for proper orientation
- The display is 128x32 pixels
- The code is optimized for the Lily58's specific OLED hardware
- All graphics are stored in PROGMEM for memory efficiency

## Credits

This OLED implementation is based on the original Lily58 OLED code, which was adapted from Soundmonster's Corne keyboard OLED code.

