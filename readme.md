# dwl

## fork ...

This is a "manual" fork of https://codeberg.org/dwl/dwl. The readme of the original repo has been moved to `README_original.md`.

This version has been patched for a customised dvorak keyboard layout with some extra functionality thrown in e.g. `roll` and `bubble`.

## latest

 - add Mod+Shift+[r|g] for incnmaster [+|-]
 - add status bar
 - tidy up ~/etc/wayland, .gitignore
 - vim-slime and kitty
 - kitty and my number pad fixed, beep silenced

## next

 - vim-slime and kitty
  - something more slick for "kitty listen on"
  - does it always send `\n`

 - kitty, neovim and colors, see below
 - clipboard in nvim, try :h clipboard

 - introduce appearance.h and behaviour.h with defaults and customised colour scheme

## build and install

minimal changes to `config.mk` to install locally

```sh
make dwl
make install
```

seem ok.

## run 

### `w`

    #!/bin/sh
    (while true; do
        echo "$(acpi -b | awk '{print $3" "$4}' | sed 's/,//g') $(date +%-H.%M)"
        sleep 30
    done) | dbus-run-session dwl -s "$HOME/a/images/wight.sh 300s $HOME/a/images/hokusai &" "$@"

### 

Try `dwl` at the prompt after logging in.

## background

using `swaybg` to set a solid color or wallpaper:

```sh
swaybg -c '#F5F5DC' &   # solid beige
SWAYBG_PID=$!
dwl
kill $SWAYBG_PID
```

or with an image:

```sh
swaybg -i /path/to/image.png -m fill &
```


## clipboard

have installed `wl-clipboard`. according to clod, with `clipboard = "unnamed,unnamedplus"` in `init.vim`, all should be well. in fact, simply `clipboard=unnamed` seeks ok.

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

