/* Taken from https://github.com/djpohly/dwl/issues/466 */
#define COLOR(hex)    { ((hex >> 24) & 0xFF) / 255.0f, \
                        ((hex >> 16) & 0xFF) / 255.0f, \
                        ((hex >> 8) & 0xFF) / 255.0f, \
                        (hex & 0xFF) / 255.0f }
/* appearance */
static const int sloppyfocus               = 1;  /* focus follows mouse */
static const int bypass_surface_visibility = 0;  /* 1 means idle inhibitors will disable idle tracking even if it's surface isn't visible  */
static const unsigned int borderpx         = 1;  /* border pixel of windows */
static const int showbar                   = 1;  /* 0 means no bar */
static const int topbar                    = 0;  /* 0 means bottom bar */
static const char *fonts[]                 = { "Source Code Pro:size=11" };
static const float rootcolor[]             = COLOR(0xf5f5dcff); // equivalent of x11 `beige`
/* This conforms to the xdg-protocol. Set the alpha to zero to restore the old behavior */
static const float fullscreen_bg[]         = {0.0f, 0.0f, 0.0f, 1.0f}; /* You can also use glsl colors */
/*                                            fg          bg          border      */
static uint32_t colors[][3]             = {
	[SchemeNorm] = { 0x586e75ff, 0xeee8d5ff, 0xeee8d5ff }, /* base01 / S_base2 / S_base2 */
	[SchemeSel]  = { 0xfdf6e3ff, 0x268bd2ff, 0x268bd2ff }, /* base3  / S_blue  / S_blue  */
	[SchemeUrg]  = { 0xfdf6e3ff, 0xdc322fff, 0xdc322fff }, /* base3  / S_red   / S_red   */
};

/* tagging */
static char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

/* logging */
static int log_level = WLR_ERROR;

static const Rule rules[] = {
	/* app_id             title       tags mask     isfloating   monitor */
	{ "Gimp_EXAMPLE",     NULL,       0,            1,           -1 }, /* Start on currently visible tags floating, not tiled */
	{ "firefox_EXAMPLE",  NULL,       1 << 8,       0,           -1 }, /* Start on ONLY tag "9" */
    /* default/example rule: can be changed but cannot be eliminated; at least one rule must exist */
};

/* layout(s) */
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* monitors */
/* (x=-1, y=-1) is reserved as an "autoconfigure" monitor position indicator
 * WARNING: negative values other than (-1, -1) cause problems with Xwayland clients due to
 * https://gitlab.freedesktop.org/xorg/xserver/-/issues/899 */
static const MonitorRule monrules[] = {
   /* name        mfact  nmaster scale layout       rotate/reflect                x    y
    * example of a HiDPI laptop monitor:
    { "eDP-1",    0.5f,  1,      2,    &layouts[0], WL_OUTPUT_TRANSFORM_NORMAL,   -1,  -1 }, */
	{ NULL,       0.55f, 1,      1,    &layouts[0], WL_OUTPUT_TRANSFORM_NORMAL,   -1,  -1 },
	/* default monitor rule: can be changed but cannot be eliminated; at least one monitor rule must exist */
};

/* keyboard */
static const struct xkb_rule_names xkb_rules = {
	/* custom_dvorak: ~/.config/xkb/symbols/custom_dvorak
	 * Symbols unshifted on number row; numbers on Super (level3).
	 * Super_L acts as ISO_Level3_Shift. */
	.layout  = "custom_dvorak",
	.options = NULL,
};

static const int repeat_rate = 25;
static const int repeat_delay = 600;

/* Trackpad */
static const int tap_to_click = 1;
static const int tap_and_drag = 1;
static const int drag_lock = 1;
static const int natural_scrolling = 0;
static const int disable_while_typing = 1;
static const int left_handed = 0;
static const int middle_button_emulation = 0;
/* You can choose between:
LIBINPUT_CONFIG_SCROLL_NO_SCROLL
LIBINPUT_CONFIG_SCROLL_2FG
LIBINPUT_CONFIG_SCROLL_EDGE
LIBINPUT_CONFIG_SCROLL_ON_BUTTON_DOWN
*/
static const enum libinput_config_scroll_method scroll_method = LIBINPUT_CONFIG_SCROLL_2FG;

/* You can choose between:
LIBINPUT_CONFIG_CLICK_METHOD_NONE
LIBINPUT_CONFIG_CLICK_METHOD_BUTTON_AREAS
LIBINPUT_CONFIG_CLICK_METHOD_CLICKFINGER
*/
static const enum libinput_config_click_method click_method = LIBINPUT_CONFIG_CLICK_METHOD_BUTTON_AREAS;

/* You can choose between:
LIBINPUT_CONFIG_SEND_EVENTS_ENABLED
LIBINPUT_CONFIG_SEND_EVENTS_DISABLED
LIBINPUT_CONFIG_SEND_EVENTS_DISABLED_ON_EXTERNAL_MOUSE
*/
static const uint32_t send_events_mode = LIBINPUT_CONFIG_SEND_EVENTS_ENABLED;

/* You can choose between:
LIBINPUT_CONFIG_ACCEL_PROFILE_FLAT
LIBINPUT_CONFIG_ACCEL_PROFILE_ADAPTIVE
*/
static const enum libinput_config_accel_profile accel_profile = LIBINPUT_CONFIG_ACCEL_PROFILE_ADAPTIVE;
static const double accel_speed = 0.0;

/* You can choose between:
LIBINPUT_CONFIG_TAP_MAP_LRM -- 1/2/3 finger tap maps to left/right/middle
LIBINPUT_CONFIG_TAP_MAP_LMR -- 1/2/3 finger tap maps to left/middle/right
*/
static const enum libinput_config_tap_button_map button_map = LIBINPUT_CONFIG_TAP_MAP_LRM;

/* If you want to use the windows key for MODKEY, use WLR_MODIFIER_LOGO */
#define MODKEY WLR_MODIFIER_ALT

#define TAGKEYS(KEY,SKEY,TAG) \
	{ MODKEY,                    KEY,            view,            {.ui = 1 << TAG} }, \
	{ MODKEY|WLR_MODIFIER_CTRL,  KEY,            toggleview,      {.ui = 1 << TAG} }, \
	{ MODKEY|WLR_MODIFIER_SHIFT, SKEY,           tag,             {.ui = 1 << TAG} }, \
	{ MODKEY|WLR_MODIFIER_CTRL|WLR_MODIFIER_SHIFT,SKEY,toggletag, {.ui = 1 << TAG} }

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *termcmd[] = { "kitty", NULL };
static const char *menucmd[] = { "wmenu-run", NULL };

static const Key keys[] = {
	/* Keysyms are after layout processing (custom_dvorak).
	 * Note: Shift changes keysyms in Wayland unlike X11 — Shift+apostrophe = quotedbl. */
	/* modifier                  key                    function          argument */
	{ MODKEY,                    XKB_KEY_i,             spawn,            {.v = menucmd} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_Return,        spawn,            {.v = termcmd} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_i,             togglebar,        {0} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_t,             bubble,           {.i = +1} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_c,             bubble,           {.i = -1} },
	{ MODKEY,                    XKB_KEY_t,             focusstack,       {.i = +1} },
	{ MODKEY,                    XKB_KEY_c,             focusstack,       {.i = -1} },
	{ MODKEY|WLR_MODIFIER_CTRL,  XKB_KEY_t,             roll,             {.i = -1} },
	{ MODKEY|WLR_MODIFIER_CTRL,  XKB_KEY_c,             roll,             {.i = +1} },
	{ MODKEY,                    XKB_KEY_g,             setmfact,         {.f = -0.05f} },
	{ MODKEY,                    XKB_KEY_r,             setmfact,         {.f = +0.05f} },
	{ MODKEY|WLR_MODIFIER_CTRL,  XKB_KEY_Return,        zoom,             {0} },
	{ MODKEY,                    XKB_KEY_Tab,           view,             {0} },
	/* Shift+apostrophe: in Wayland the keysym becomes quotedbl when Shift is held */
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_quotedbl,      killclient,       {0} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_l,             incnmaster,       {.i = +1} },
	{ MODKEY|WLR_MODIFIER_CTRL,  XKB_KEY_l,             incnmaster,       {.i = -1} },
	{ MODKEY|WLR_MODIFIER_CTRL,  XKB_KEY_l,             setlayout,        {.v = &layouts[0]} },
	{ MODKEY|WLR_MODIFIER_CTRL,  XKB_KEY_f,             setlayout,        {.v = &layouts[1]} },
	{ MODKEY|WLR_MODIFIER_CTRL,  XKB_KEY_m,             setlayout,        {.v = &layouts[2]} },
	/* { MODKEY,                    XKB_KEY_space,       setlayout,        {0} }, */
	/* { MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_space,       togglefloating,   {0} }, */
	/* { MODKEY,                    XKB_KEY_bracketleft, view,             {.ui = ~0} }, */
	/* { MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_braceleft,   tag,              {.ui = ~0} }, */
	/* { MODKEY|WLR_MODIFIER_CTRL,  XKB_KEY_w,           focusmon,         {.i = WLR_DIRECTION_LEFT} }, */
	/* { MODKEY|WLR_MODIFIER_CTRL,  XKB_KEY_v,           focusmon,         {.i = WLR_DIRECTION_RIGHT} }, */
	/* { MODKEY|WLR_MODIFIER_CTRL|WLR_MODIFIER_SHIFT, XKB_KEY_w, tagmon,  {.i = WLR_DIRECTION_LEFT} }, */
	/* { MODKEY|WLR_MODIFIER_CTRL|WLR_MODIFIER_SHIFT, XKB_KEY_v, tagmon,  {.i = WLR_DIRECTION_RIGHT} }, */
	/* Tag keys: number row level1/level2 (symbols); Shift gives level2 keysym */
	TAGKEYS(          XKB_KEY_exclam,       XKB_KEY_percent,      0),
	TAGKEYS(          XKB_KEY_parenleft,    XKB_KEY_asciicircum,  1),
	TAGKEYS(          XKB_KEY_parenright,   XKB_KEY_numbersign,   2),
	TAGKEYS(          XKB_KEY_less,         XKB_KEY_dollar,       3),
	TAGKEYS(          XKB_KEY_minus,        XKB_KEY_bar,          4),
	TAGKEYS(          XKB_KEY_greater,      XKB_KEY_at,           5),
	TAGKEYS(          XKB_KEY_plus,         XKB_KEY_ampersand,    6),
	TAGKEYS(          XKB_KEY_equal,        XKB_KEY_asterisk,     7),
	TAGKEYS(          XKB_KEY_bracketright, XKB_KEY_braceright,   8),
	{ MODKEY,                    XKB_KEY_z,             quit,             {0} },

	/* Ctrl-Alt-Backspace and Ctrl-Alt-Fx used to be handled by X server */
	{ WLR_MODIFIER_CTRL|WLR_MODIFIER_ALT,XKB_KEY_Terminate_Server, quit, {0} },
	/* Ctrl-Alt-Fx is used to switch to another VT, if you don't know what a VT is
	 * do not remove them.
	 */
#define CHVT(n) { WLR_MODIFIER_CTRL|WLR_MODIFIER_ALT,XKB_KEY_XF86Switch_VT_##n, chvt, {.ui = (n)} }
	CHVT(1), CHVT(2), CHVT(3), CHVT(4), CHVT(5), CHVT(6),
	CHVT(7), CHVT(8), CHVT(9), CHVT(10), CHVT(11), CHVT(12),
};

static const Button buttons[] = {
	{ ClkLtSymbol, 0,      BTN_LEFT,   setlayout,      {.v = &layouts[0]} },
	{ ClkLtSymbol, 0,      BTN_RIGHT,  setlayout,      {.v = &layouts[2]} },
	{ ClkTitle,    0,      BTN_MIDDLE, zoom,           {0} },
	{ ClkStatus,   0,      BTN_MIDDLE, spawn,          {.v = termcmd} },
	{ ClkClient,   MODKEY, BTN_LEFT,   moveresize,     {.ui = CurMove} },
	{ ClkClient,   MODKEY, BTN_MIDDLE, togglefloating, {0} },
	{ ClkClient,   MODKEY, BTN_RIGHT,  moveresize,     {.ui = CurResize} },
	{ ClkTagBar,   0,      BTN_LEFT,   view,           {0} },
	{ ClkTagBar,   0,      BTN_RIGHT,  toggleview,     {0} },
	{ ClkTagBar,   MODKEY, BTN_LEFT,   tag,            {0} },
	{ ClkTagBar,   MODKEY, BTN_RIGHT,  toggletag,      {0} },
};
