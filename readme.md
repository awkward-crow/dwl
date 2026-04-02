# dwl

## fork ...

This is a "manual" fork of https://codeberg.org/dwl/dwl. The readme of the original repo has been moved to `README_original.md`.

This version has been patched for a customised dvorak keyboard layout with some extra functionality thrown in e.g. `roll` and `bubble`.

## latest

 - add roll and keybindings
 - add bubble and keybindings
 - spawn new terminal after the current one
 - customised us dvorak keyboard layout and key mappings, see config.h

## next

 - foot, neovim and colors, see below
 - clipboard in nvim, try :h clipboard

 - introduce appearance.h and behaviour.h with defaults and customised colour scheme
 - a status bar
 - can nmaster be set dynamically

## build and install

minimal changes to `config.mk` to install locally

```sh
make dwl
make install
```

seem ok.

## run 

Try `dwl` at the prompt after logging in.


## colors: foot and neovim

foot color customizations in `foot.ini` (the 16 ANSI palette) are **not** picked
up by neovim by default.

neovim uses two separate color systems:

- `termguicolors` **on** (default for most configs): neovim sends 24-bit RGB
  escape sequences from the colorscheme directly — the terminal palette is
  bypassed entirely
- `termguicolors` **off**: neovim uses the terminal's 16 ANSI colors — foot's
  palette applies

foot sets `COLORTERM=truecolor` automatically, which enables `termguicolors` in
neovim. so foot palette changes have no effect on neovim appearance.

### options

1. **match the colorscheme to the foot palette** — keep `termguicolors` on, pick
   a neovim colorscheme whose RGB values match what is defined in `foot.ini`.
   base16/base24 generators (e.g. [flavours](https://github.com/Misterio77/flavours))
   can generate both configs from the same palette source.

2. **use the terminal palette in neovim** — disable `termguicolors` and use a
   colorscheme with good `cterm` definitions (e.g. `default`, `habamax`). then
   foot's palette controls neovim's colors.

   Note, `:set notgc` will do this!


#### end

