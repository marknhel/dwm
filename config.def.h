/* See LICENSE file for copyright and license details. */
#define TERMINAL "st"
#define TERMCLASS "St"


/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int snap      = 5;       /* snap pixel */
static const unsigned int gappih    = 10;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 10;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int swallowfloating    = 1;        /* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int vertpad            = 5;       /* vertical padding of bar */
static const int sidepad            = 5;       /* horizontal padding of bar */
static const char *fonts[]          = { "MesloLGS NF:size=10" };
static const char dmenufont[]       = "MesloLGS NF:size=10";
static unsigned int baralpha        = 0xd0;
static unsigned int borderalpha     = OPAQUE;
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_gray1,  col_cyan  },
//	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd0[] = {TERMINAL, "-n", "spterm", "-g", "120x34", NULL };
const char *spcmd1[] = {TERMINAL, "-n", "spdic", "-g", "144x41", "-e", "dic", NULL };
const char *spcmd2[] = {TERMINAL, "-n", "spcalc", "-g", "144x41", "-e", "bc", "-q" , NULL };
const char *spcmd3[] = {TERMINAL, "-n", "splf", "-g", "144x41", "-e", "lf", NULL };
const char *spcmd4[] = {TERMINAL, "-n", "spwatch", "-g", "120x34", "-e", "watchv", NULL };
const char *spcmd5[] = {TERMINAL, "-n", "spmusic", "-g", "145x35", "-e", "ncmpcpp", NULL };
static Sp scratchpads[] = {
	/* name			cmd  */
	{"spterm",		spcmd0},
	{"spdic",		spcmd1},
	{"spcalc",		spcmd2},
	{"splf",		spcmd3},
	{"spwatch",		spcmd4},
	{"spmusic",		spcmd5},
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const unsigned int ulinepad	= 7;	/* horizontal padding between the underline and tag */
static const unsigned int ulinestroke	= 2;	/* thickness / height of the underline */
static const unsigned int ulinevoffset	= 0;	/* how far above the bottom of the bar the line should appear */
static const int ulineall 		= 0;	/* 1 to show underline on all tags, 0 for just the active ones */

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class	instance	title		tags mask	isfloating	isterminal	noswallow	monitor */
	{ "Gimp",	NULL,		NULL,		0,		1,		0,		0,		-1 },
	{ "Brave-browser",NULL,		NULL,		1 << 1,		0,		0,		-1,		-1 },
//	{ "mpv",	NULL,		NULL,		0,		0,		0,		0,		1 },
	{ TERMINAL,	NULL,		NULL,		0,		0,		1,		0,		-1 },
	{ TERMINAL,	NULL,		"Event Tester",	0,		0,		0,		1,		-1 }, /* xev */
//	Scratchpad
	{ TERMINAL,	"spterm",	NULL,		SPTAG(0),	1,		1,		0,		-1 },
	{ TERMINAL,	"spdic",	NULL,		SPTAG(1),	1,		1,		0,		-1 },
	{ TERMINAL,	"spcalc",	NULL,		SPTAG(2),	1,		1,		0,		-1 },
	{ TERMINAL,	"splf",		NULL,		SPTAG(3),	1,		1,		0,		-1 },
	{ TERMINAL,	"spwatch",	NULL,		SPTAG(4),	1,		1,		0,		1 },
	{ TERMINAL,	"spmusic",	NULL,		SPTAG(5),	1,		1,		0,		-1 }
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      bstack },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
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
static const char *termcmd[]  = { TERMINAL, NULL };

static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_r,		spawn,          {.v = dmenucmd } },
	{ MODKEY,			XK_x,		spawn,          {.v = termcmd } },
//	GAPS
	{ MODKEY|Mod1Mask,              XK_h,		incrgaps,       {.i = +1 } },
	{ MODKEY|Mod1Mask,              XK_l,		incrgaps,       {.i = -1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_h,		incrogaps,      {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_l,		incrogaps,      {.i = -1 } },
	{ MODKEY|Mod1Mask|ControlMask,  XK_h,		incrigaps,      {.i = +1 } },
	{ MODKEY|Mod1Mask|ControlMask,  XK_l,		incrigaps,      {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_0,		togglegaps,     {0} },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_0,		defaultgaps,    {0} },
	{ MODKEY,                       XK_y,		incrihgaps,     {.i = +1 } },
	{ MODKEY,                       XK_o,		incrihgaps,     {.i = -1 } },
	{ MODKEY|ControlMask,           XK_y,		incrivgaps,     {.i = +1 } },
	{ MODKEY|ControlMask,           XK_o,		incrivgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_y,		incrohgaps,     {.i = +1 } },
	{ MODKEY|Mod1Mask,              XK_o,		incrohgaps,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_y,		incrovgaps,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_o,		incrovgaps,     {.i = -1 } },
//	GAPS
	{ MODKEY|ShiftMask,		XK_x,		togglescratch,	{.v = 0 } },
	{ MODKEY|ShiftMask,		XK_d,		togglescratch,	{.v = 1 } },
	{ MODKEY|ShiftMask,		XK_c,		togglescratch,	{.v = 2 } },
	{ MODKEY,			XK_e,		togglescratch,	{.v = 3 } },
//	{ MODKEY,			XK_z,		togglescratch,	{.v = 4 } },
	{ MODKEY,			XK_m,		togglescratch,	{.v = 5 } },

	{ MODKEY,                       XK_b,		togglebar,      {0} },
	{ MODKEY|ShiftMask,		XK_b,		spawn,		SHCMD("xdotool type $(grep -v '^#'  ~/.config/bookmarks | dmenu -i -l 10 | cut -d' ' -f1)") },
	{ MODKEY,                       XK_s,		togglesticky,	{0} },
	{ MODKEY|ShiftMask,		XK_s,		spawn,		SHCMD("togglesk") },
	{ MODKEY,                       XK_f,		togglefullscr,	{0} },
	{ MODKEY,                       XK_F4,		spawn,		SHCMD("sysact") },

	{ MODKEY,                       XK_w,		spawn,		SHCMD("brave")  },
	{ MODKEY|ShiftMask,		XK_w,		spawn,		SHCMD("watchyt")  },
//	{ MODKEY,			XK_e,		spawn,		SHCMD(TERMINAL " -e lf ~/")  },
	{ MODKEY|ShiftMask,		XK_e,		spawn,		SHCMD(TERMINAL " -e thunar")  },
	{ MODKEY,			XK_g,		spawn,		SHCMD("discord")  },
	{ MODKEY,			XK_n,		spawn,		SHCMD(TERMINAL " -e sudo nmtui")  },
	{ MODKEY|ShiftMask,		XK_n,		spawn,		SHCMD(TERMINAL " -e newsboat")  },
//	{ MODKEY,			XK_m,		spawn,		SHCMD(TERMINAL " -e ncmpcpp")  },
	{ MODKEY|ShiftMask,		XK_m,		spawn,		SHCMD(TERMINAL " -e neomutt")  },
	{ MODKEY,			XK_z,		spawn,		SHCMD(TERMINAL " -e watchv")  },
	{ MODKEY|ShiftMask,		XK_z,		spawn,		SHCMD("qmusic")  },
	{ MODKEY,			XK_space,	spawn,		SHCMD("mpc toggle")  },
	{ MODKEY|ShiftMask,		XK_space,	spawn,		SHCMD("qsongs")  },
	{ MODKEY,			XK_v,		spawn,		SHCMD("btcon")  },
	{ MODKEY|ShiftMask,		XK_v,		spawn,		SHCMD("btdcon")  },
	{ MODKEY,			XK_c,		spawn,		SHCMD(TERMINAL " -e camtoggle")  },
//	{ MODKEY|ShiftMask,		XK_c,		spawn,		SHCMD("btdcon")  },
	{ MODKEY,			XK_a,		spawn,		SHCMD(TERMINAL " -e android-file-transfer")  },
	{ MODKEY|ShiftMask,		XK_a,		spawn,		SHCMD("dla")  },
	{ MODKEY,			XK_Up,		spawn,		SHCMD("pamixer --allow-boost -i 5")  },
	{ MODKEY|ShiftMask,		XK_Up,		spawn,		SHCMD("xrandr -o normal")  },
	{ MODKEY,			XK_Down,	spawn,		SHCMD("pamixer --allow-boost -d 5")  },
	{ MODKEY|ShiftMask,		XK_Down,	spawn,		SHCMD("xrandr -o inverted")  },
	{ MODKEY,			XK_Left,	shiftview,	{.i = -1 }  },
	{ MODKEY|ShiftMask,		XK_Left,	spawn,		SHCMD("xrandr -o left")  },
	{ MODKEY,			XK_Right,	shiftview,	{.i = +1 }  },
	{ MODKEY|ShiftMask,		XK_Right,	spawn,		SHCMD("xrandr -o right")  },
	{ MODKEY,			XK_p,		spawn,		SHCMD("flameshot gui")  },
	{ MODKEY|ShiftMask,		XK_p,		spawn,		SHCMD("passes")  },
	{ MODKEY|ShiftMask,		XK_l,		spawn,		SHCMD("slock")  },
	{ 0,				XK_Print,	spawn,		SHCMD("flameshot full -p ~/pics/screenshits")  },
	{ MODKEY,			XK_Print,	spawn,		SHCMD("dmenurecord")  },
	{ MODKEY|ShiftMask,		XK_Print,	spawn,		SHCMD("dmenurecord kill")  },

	{ MODKEY,                       XK_j,		focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,		focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_bracketleft,	incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_bracketright,incnmaster,     {.i = -1 } },
	{ MODKEY|ShiftMask,		XK_bracketleft,	spawn,		SHCMD("mpc prev") },
	{ MODKEY|ShiftMask,		XK_bracketright,spawn,		SHCMD("mpc next") },
	{ MODKEY,                       XK_h,		setmfact,       {.f = -0.25} },
	{ MODKEY,                       XK_l,		setmfact,       {.f = +0.25} },
	{ MODKEY,                       XK_Return,	zoom,           {0} },
	{ MODKEY,                       XK_Tab,		view,           {0} },
	{ MODKEY|ShiftMask,             XK_j,		aspectresize,   {.i = +24} },
	{ MODKEY|ShiftMask,             XK_k,		aspectresize,   {.i = -24} },
	{ MODKEY,			XK_q,		killclient,     {0} },
	{ MODKEY,                       XK_t,		setlayout,      {.v = &layouts[0]} },
	{ MODKEY|ShiftMask,		XK_t,		setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_u,		setlayout,      {.v = &layouts[2]} },
//	{ MODKEY,                       XK_space,	setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_Return,	togglefloating, {0} },
	{ MODKEY,                       XK_0,		view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,		tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
