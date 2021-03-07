/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

#define Button6 6
#define Button7 7
#define Button8 8
#define Button9 9

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappx     = 20;        /* gaps between windows */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "Monaco:size=12" };
static const char dmenufont[]       = "Monaco:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#AD9944";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	//{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
	/* class     instance     title       tags mask     isfloating   isterminal		noswallow  monitor */
	{ "st",      NULL,     NULL,           0,         0,          1,           0,        -1 },
	{ "URxvt",   NULL,     NULL,           0,         0,          1,           0,        -1 },
	{ "xev",   NULL,     NULL,           0,         0,          0,           1,        -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
//static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */

static const char *termcmd[] = {"st", NULL};

static Key keys[] = {
	/* modifier                     key                         function        argument */
	{ 0,                            XF86XK_AudioRaiseVolume,    spawn,          SHCMD("amixer set Master 3%+") },
	{ 0,                            XF86XK_AudioLowerVolume,    spawn,          SHCMD("amixer set Master 3%-") },
	{ 0,                            XF86XK_AudioMute,           spawn,          SHCMD("amixer set Master 0")   },
	{ MODKEY,                       XK_d,                       spawn,          SHCMD("rofi -show run") },
	{ MODKEY,                       XK_Return,                  spawn,          SHCMD("st") },
	{ MODKEY,                       XK_i,                       spawn,          SHCMD("brave") },
	{ MODKEY,						XK_o,                       spawn,          SHCMD("st -e nvim $HOME/Documents/md/todo.md") },
	{ MODKEY,						XK_m,                       spawn,          SHCMD("st -e mutt") },
	{ 0,   				         	XK_Prior,                   spawn,          SHCMD("$HOME/.local/bin/latestassignment") },
	{ 0,   				         	XK_End,                     spawn,          SHCMD("$HOME/.local/bin/textbooks") },
	{ 0,   				         	XK_Print,                   spawn,          SHCMD("$HOME/.local/bin/shot") },
	{ MODKEY,				        XK_semicolon,               spawn,          SHCMD("$HOME/.local/bin/actionmenu") },
	{ MODKEY,                       XK_b,                       togglebar,      {0} },
	{ MODKEY,                       XK_w,                       killclient,     {0} },
	{ MODKEY,                       XK_j,                       focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,                       focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_h,                       setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,                       setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_e,                       zoom,           {0} },
	{ MODKEY,                       XK_Tab,                     view,           {0} },
	{ MODKEY,                       XK_f,                       setlayout,      {0} },
	{ MODKEY,                       XK_f,                       settiled,       {0} },
	{ MODKEY,                       XK_t,                       setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_t,                       setbar,         {1} },
	{ MODKEY,                       XK_t,                       settiled,       {1} },
	{ MODKEY,                       XK_0,                       view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,                       tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,                   focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period,                  focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,                   tagmon,         {.i = -1 } },
	{ MODKEY,                       Button4,                    spawn,      SHCMD("amixer set Master 3%+") },
	{ MODKEY,                       Button5,                    spawn,      SHCMD("amixer set Master 3%-") },
	TAGKEYS(                        XK_1,                       0)
	TAGKEYS(                        XK_2,                       1)
	TAGKEYS(                        XK_3,                       2)
	TAGKEYS(                        XK_4,                       3)
	TAGKEYS(                        XK_5,                       4)
	TAGKEYS(                        XK_6,                       5)
	TAGKEYS(                        XK_7,                       6)
	TAGKEYS(                        XK_8,                       7)
	TAGKEYS(                        XK_9,                       8)
	{ MODKEY|ShiftMask,             XK_q,                       quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button2,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button1,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
	{ ClkTagBar,            0,              Button4,        view_adjacent,  { .i = -1 } },
	{ ClkTagBar,            0,              Button5,        view_adjacent,  { .i = +1 } },

	{ ClkClientWin,         MODKEY,         Button4,        spawn,          SHCMD("amixer set Master 1%+") },
	{ ClkRootWin,           MODKEY,         Button4,        spawn,          SHCMD("amixer set Master 1%+") },
	{ ClkWinTitle,          MODKEY,         Button4,        spawn,          SHCMD("amixer set Master 1%+") },
	{ ClkStatusText,        0,              Button4,        spawn,          SHCMD("amixer set Master 1%+") },

	{ ClkClientWin,         MODKEY,         Button5,        spawn,          SHCMD("amixer set Master 1%-") },
	{ ClkRootWin,           MODKEY,         Button5,        spawn,          SHCMD("amixer set Master 1%-") },
	{ ClkWinTitle,          MODKEY,         Button5,        spawn,          SHCMD("amixer set Master 1%-") },
	{ ClkStatusText,        0,              Button5,        spawn,          SHCMD("amixer set Master 1%-") },

	{ ClkClientWin,         MODKEY,         Button9,        spawn,          SHCMD("playerctl next") },
	{ ClkRootWin,           MODKEY,         Button9,        spawn,          SHCMD("playerctl next") },
	{ ClkWinTitle,          MODKEY,         Button9,        spawn,          SHCMD("playerctl next") },
	{ ClkStatusText,        MODKEY,         Button9,        spawn,          SHCMD("playerctl next") },

	{ ClkClientWin,         MODKEY,         Button8,        spawn,          SHCMD("playerctl previous") },
	{ ClkRootWin,           MODKEY,         Button8,        spawn,          SHCMD("playerctl previous") },
	{ ClkWinTitle,          MODKEY,         Button8,        spawn,          SHCMD("playerctl previous") },
	{ ClkStatusText,        MODKEY,         Button8,        spawn,          SHCMD("playerctl previous") },
};
