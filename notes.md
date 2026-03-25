# Dvorak keyboard setup notes

## Files changed

### `~/.config/xkb/symbols/custom_dvorak` (new)
Translates the X11 xmodmap customisation into XKB format for use by
libxkbcommon (Wayland). This file is invisible to X11 — it does not affect
the dwm/X11 session.

Key changes from base `us(dvorak)`:
- Number row: symbols unshifted (`! ( ) < - > + = ] [`), more symbols
  shifted, actual numbers on level3 (Super_L)
- Comma key: `,`/`;` (standard Dvorak has `,`/`<`)
- Period key: `.`/`:` (standard Dvorak has `.`/`>`)
- `/` key: `/`/`~` (replaces `/`/`?`)
- `=` key: `\`/`+` (replaces `=`/`+`)
- Right home row h/t/n and m: letters normal, numbers 1/2/3/0 on level3
- g/c/r (at u/i/o positions): letters normal, 4/5/6 on level3
- v key (QWERTY `.`): v/V/KP_Decimal/Clear
- Super_L: acts as ISO_Level3_Shift (level3 modifier, replaces xmodmap Mode_switch)

### `config.h`
- `xkb_rules.layout` set to `"custom_dvorak"`
- `keys[]` rebuilt from dwm bindings

## Keybinding translation notes

### Wayland vs X11 keysym behaviour
In X11/dwm, `XLookupKeysym(&ev, 0)` always returns the base (unshifted)
keysym regardless of modifier state. In Wayland/dwl, `xkb_state_key_get_syms`
returns the keysym for the current modifier state — Shift changes it.

Consequence: `Shift+apostrophe` in dwm (XK_apostrophe + ShiftMask) must be
written as `XKB_KEY_quotedbl` with `WLR_MODIFIER_SHIFT` in dwl, because
holding Shift on the apostrophe key produces the `quotedbl` keysym.

Similarly, the TAGKEYS Shift variants use the level2 symbols from the remapped
number row (`%`, `^`, `#`, `$`, `|`, `@`, `&`, `*`, `}`) rather than the
base keysyms.

## Patches applied

### Insert new client after focused client (`mapnotify`)
In `mapnotify()` (dwl.c), new clients are inserted after the currently focused
client in tiling order rather than at the head of the `clients` list:

```c
if ((w = focustop(selmon)))
    wl_list_insert(&w->link, &c->link);
else
    wl_list_insert(&clients, &c->link);
```

This keeps new windows from displacing the master and places them next to the
window that spawned them in the stack.

### Functions not available in stock dwl
The following bindings from the dwm config are commented out and need patches:
- `togglebar` — requires a bar patch
- `bubble` — dwm-specific patch (move window up/down in stack)
- `roll` — dwm-specific patch (rotate windows in stack)
