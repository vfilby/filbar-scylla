# Agent Rules for QMK Keymap Development

## Visual Keymap Comments

**CRITICAL**: When updating any keymap layer, you MUST also update the corresponding visual keymap comment block that appears above the layer definition.

### Why This Is Important
- Visual keymap comments provide an ASCII art representation of the keyboard layout
- They serve as documentation for understanding the keymap at a glance
- Keeping them in sync with the actual keymap code prevents confusion and errors
- Other developers rely on these comments to understand the layout without reading the code

### Rules
1. **Always update the visual comment** when you modify any key in a layer
2. The visual comment should accurately reflect the keycode or function of each key
3. Use appropriate symbols or abbreviations:
   - Use `:` for `KC_COLN`
   - Use `(` for `KC_LPRN` and `)` for `KC_RPRN`
   - Use descriptive labels like `SW_WIN`, `LOGOUT`, etc. for custom keys
   - Use `TRNS` for transparent keys (`_______`)
   - Use `      ` (spaces) for unused keys (`XXXXXXX`)
4. Maintain the ASCII art formatting and alignment
5. The visual comment should be directly above the layer definition it represents

### Example
If you change a key from `XXXXXXX` to `KC_COLN` in the keymap array, you must also update the corresponding position in the visual comment from `|      |` to `|   :  |`.

