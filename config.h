/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 100;       /* snap pixel */
static const int rmaster            = 0;        /* 1 means master-area is initially on the right */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "Terminus:style=Regular:pixelsize=14:antialias=true:autohint=true" };
static const char dmenufont[]       = "Terminus:style=Regular:pixelsize=18:antialias=true:autohint=true";
static const char col_gray1[]       = "#3b4252";
static const char col_gray2[]       = "#4c566a";
static const char col_gray3[]       = "#d8dee9";
static const char col_gray4[]       = "#2e3440";
static const char col_cyan[]        = "#88c0d8";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_cyan, col_gray2,  col_cyan  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   isfakefullscreen monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           0,               -1 },
	{ "firefox",  NULL,       NULL,       1 << 8,       0,           1,               -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static int previouslayout    = -1;   /* -1 means no previous layout */

#include "layouts.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "TTT",      bstack },
	{ "|D|",      deck },
	{ "HHH",      grid },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *dmenucmd2[] = { "via", "-r", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };

#include "actions.c"
static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_d,      spawn,          {.v = dmenucmd2 } },
	// { MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd2 } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_i,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_o,      winview,        {0} },
	{ MODKEY,                       XK_h,      hlwrapper,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      hlwrapper,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_space,  zoom,           {0} },
	{ MODKEY,                       XK_space,  switchcol,      {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[2]} },
//	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_u,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_g,      setlayout,      {.v = &layouts[4]} },
	//{ MODKEY|ShiftMask,           XK_g,      setlayout,      {.v = &layouts[5]} },
	{ MODKEY|ControlMask,           XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_y,      togglermaster,  {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0, } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },
    { MODKEY,                       XK_slash,  moveplace,      {.ui = WIN_SE  }},
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_r,      quit,           {0} },
	{ MODKEY|ShiftMask,		XK_Escape,	spawn,	SHCMD("[ \"$(printf \"No\\nYes\" | dmenu -i -nb darkred -sb red -sf white -nf gray -p \"Close Xorg?\")\" = Yes ] && killall Xorg") },
	{ MODKEY|ShiftMask,		XK_BackSpace,	spawn,		SHCMD("[ \"$(printf \"No\\nYes\" | dmenu -i -nb darkred -sb red -sf white -nf gray -p \"Reboot computer?\")\" = Yes ] && doas shutdown -r now") },
	{ MODKEY,			XK_w,		spawn,		SHCMD("$BROWSER") },
	{ MODKEY|ShiftMask,		XK_w,		spawn,		SHCMD("st -e sudo nmtui") },
	{ MODKEY,			XK_e,		spawn,		SHCMD("st -e neomutt") },
	{ MODKEY,			XK_r,		spawn,		SHCMD("st -e $FILE") },
	{ MODKEY,			XK_a,		spawn,		SHCMD("st -e alsamixer") },
	{ MODKEY,			XK_x,		spawn,		SHCMD("slock & xset dpms force off; mpc pause ; pauseallmpv") },
	{ MODKEY|ShiftMask,		XK_x,		spawn,		SHCMD("[ \"$(printf \"No\\nYes\" | dmenu -i -nb darkred -sb red -sf white -nf gray -p \"Shutdown computer?\")\" = Yes ] && doas shutdown -h now") },
	{ MODKEY|ShiftMask,		XK_c,		spawn,		SHCMD("mpv --no-cache --no-osc --no-input-default-bindings --input-conf=/dev/null --title=mpvfloat $(ls /dev/video[0,2,4,6,8] | tail -n 1)") },
	{ MODKEY,			XK_v,		spawn,		SHCMD("st -e $EDITOR -c \"VimwikiIndex\"") },
	{ MODKEY,			XK_n,		spawn,		SHCMD("st -e newsboat") },
	{ MODKEY,			XK_m,		spawn,		SHCMD("st -e ncmpcpp") },
	{ MODKEY|ShiftMask,			XK_m,		spawn,		SHCMD("mpd-toggle") },
	{ MODKEY,			XK_F6,		spawn,		SHCMD("torwrap") },
	{ MODKEY,			XK_F7,		spawn,		SHCMD("td-toggle") },
	{ MODKEY,			XK_F8,		spawn,		SHCMD("mailsync") },
	{ MODKEY,			XK_F9,		spawn,		SHCMD("dmenumount") },
	{ MODKEY,			XK_F10,		spawn,		SHCMD("dmenuumount") },
	{ 0,				XK_Print,	spawn,		SHCMD("maim pic-full-$(date '+%y%m%d-%H%M-%S').png") },
	{ ShiftMask,			XK_Print,	spawn,		SHCMD("maimpick") },
	{ MODKEY,			XK_Print,	spawn,		SHCMD("dmenurecord") },
	{ MODKEY|ShiftMask,		XK_Print,	spawn,		SHCMD("dmenurecord kill") },
	{ MODKEY,			XK_Delete,	spawn,		SHCMD("dmenurecord kill") },
	{ MODKEY,			XK_Scroll_Lock,	spawn,		SHCMD("killall screenkey || screenkey &") },

	{ 0,            0x1008ff12,	    spawn,		SHCMD("setvol toggle") },
	{ ControlMask,  0x1008ff13,	    spawn,		SHCMD("setvol 1%+") },
	{ 0,            0x1008ff13,	    spawn,		SHCMD("setvol 5%+") },
	{ ShiftMask,    0x1008ff13,	    spawn,		SHCMD("setvol 10%+") },
	{ ControlMask,  0x1008ff11,	    spawn,		SHCMD("setvol 1%-") },
	{ 0,            0x1008ff11,	    spawn,		SHCMD("setvol 5%-") },
	{ ShiftMask,    0x1008ff11,	    spawn,		SHCMD("setvol 10%-") },
	{ ControlMask,  0x1008FF02,	    spawn,		SHCMD("doas light 15") },
	{ 0,            0x1008FF02,	    spawn,		SHCMD("doas light 75") },
	{ ShiftMask,    0x1008FF02,	    spawn,		SHCMD("doas light 150") },
	{ ControlMask,  0x1008FF03,	    spawn,		SHCMD("doas light -15") },
	{ 0,            0x1008FF03,	    spawn,		SHCMD("doas light -75") },
	{ ShiftMask,    0x1008FF03,	    spawn,		SHCMD("doas light -150") },
	{ 0,            0x1008FF14,	    spawn,		SHCMD("mpc toggle") },
	{ 0,            0x1008FF16,	    spawn,		SHCMD("mpc prev") },
	{ 0,            0x1008FF17,	    spawn,		SHCMD("mpc next") },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
//	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
//	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
//	{ ClkTagBar,            0,              Button1,        view,           {0} },
//	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
//	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
//	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
