# dvorak keyboard setup notes

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

### `bubble` — move client up/down in tiling order
Ported from the dwm bubble patch. Moves the focused client one position
forward or backward in the `clients` list (tiling order), wrapping at
both ends.

Original dwm patch used singly-linked `->next` list, `selmon->sel`,
`selmon->clients`, `swap()`, `focus()`, and `ISVISIBLE()`. Port differences:

- `selmon->sel` → `focustop(selmon)`
- `selmon->clients` + `->next` → `wl_list` iteration
- `ISVISIBLE()` → `VISIBLEON(c, selmon)`
- `swap(t, s)` → `wl_list_remove` + `wl_list_insert`
- `focus(c)` → `focusclient(c, 1)`

Wrapping: `wl_list_for_each` / `wl_list_for_each_reverse` are circular; the
sentinel node (`&clients` / `&c->link`) is skipped with a `continue`. If after
a full wrap no other visible client is found (`o == c`), the function returns
without moving anything.

List manipulation:
- Non-wrap forward: find next visible `o`, insert `c` after `o` (swap adjacent)
- Non-wrap backward: find previous visible `o`, insert `c` before `o` (swap adjacent)
- Wrap forward (c was last): find first visible `o`; remove both, put `c` at head, `o` at tail (swap ends)
- Wrap backward (c was first): find last visible `o`; remove both, put `o` at head, `c` at tail (swap ends)

Wrapping is detected with a `wrapped` flag set when the sentinel is crossed during iteration.

Forward declaration added alongside other `static void` declarations.

### Functions not available in stock dwl
The following bindings from the dwm config are commented out and still need patches:
- `togglebar` — requires a bar patch
- `roll` — dwm-specific patch (rotate windows in stack)



#### end
