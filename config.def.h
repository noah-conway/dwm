/* See LICENSE file for copyright and license details. */

/*
    Patches applied:
    - Pertag
    - Swallow
    - Alpha
    - Attachbelow
    - Statuspadding
    - Fullgaps
    - Scratchpads
    - Switchtotags
*/

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int gappx     = 8;        /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int splitstatus        = 1;        /* 1 for split status items */
static const char *splitdelim        = ";";       /* Character used for separating status */
static const int horizpadbar        = 2;        /* horizontal padding for statusbar */
static const int vertpadbar         = 8;        /* vertical padding for statusbar */
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const unsigned int baralpha = 0xd0;
static const unsigned int borderalpha = OPAQUE;
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};
static const unsigned int alphas[][3]      = {
    /*               fg      bg        border*/
    [SchemeNorm] = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]  = { OPAQUE, baralpha, borderalpha },
};

typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd1[] = {"st", "-n", "spterm", "-g", "120x34", NULL };
const char *spcmd2[] = {"st", "-n", "spfm", "-g", "144x41", "-e", "ranger", NULL };
const char *spcmd3[] = {"keepassxc", NULL };
static Sp scratchpads[] = {
	/* name          cmd  */
	{"spterm",      spcmd1},
	{"spranger",    spcmd2},
	{"keepassxc",   spcmd3},
};


/* tagging */
static const char *tags[] = { "a", "s", "d", "f", "w", "e", "g"};
static const char *tagsalt[] = { "1", "2", "3", "4", "5", "6", "7"};

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance  title           tags mask  switchtotag isfloating  isterminal  noswallow  monitor */
	{ "Gimp",    NULL,     NULL,           0,         0,          1,          0,           0,        -1 },
	{ "REAPER",  NULL,     NULL,           1,         0,          0,          0,           0,        -1 },
	{ "QjackCtl",  NULL,     NULL,           0,       0,          1,          0,           0,        -1 },
	{ "firefox", NULL,     NULL,           1 << 3,    1,          0,          0,          -1,        -1 },
	{ "st",      NULL,     NULL,           0,         0,          0,          1,           0,        -1 },
	{ "steam",   NULL,     NULL,           1 << 6,    0,          0,          0,           0,        -1 },
	{ NULL,      NULL,     "Event Tester", 0,         0,          0,          0,           1,        -1 }, /* xev */
	{ NULL,		  "spterm",		NULL,		       SPTAG(0),	0,         	1,			    1,           0,        -1 },
  { NULL,		  "spfm",		NULL,	           SPTAG(1),	0,        	1,			    0,           0,        -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int attachbelow = 1;    /* 1 means attach after the currently active window */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
//static const char *dmenucmd[] = { "dmenu_run", "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *dmenucmd[] = { "rofi", "-show", "drun" };
static const char *termcmd[]  = { "st", NULL };
static const char *browsercmd[] = { "firefox", NULL};
static const char *pbrowsercmd[] = { "firefox", "-private-window", NULL};
static const char *btcmd[] = { "dmenu-bluetooth", NULL};
static const char *mailcmd[] = { "thunderbird", NULL};
static const char *scrotcmd[] = { "flameshot", "gui", NULL};
static const char *powercmd[] = { "powermenu.sh", NULL};

static const char *mpdtogglecmd[] = { "mpc", "toggle", NULL};
static const char *mpdnextcmd[] = { "mpc", "next", NULL};
static const char *mpdprevcmd[] = { "mpc", "prev", NULL};
static const char *volraisecmd[] = { "pamixer", "-i", "10", NULL};
static const char *vollowercmd[] = { "pamixer", "-d", "10", NULL};
static const char *volmutecmd[] = { "pamixer", "--toggle-mute", NULL};
static const char *audioswtchcmd[] = { "audioswitch.sh", NULL};

static const Key keys[] = {
	/* modifier                     key        function        argument */
// Launch Keys
	{ MODKEY,                       XK_u,      spawn,          {.v = dmenucmd } },
//{ MODKEY,                       XK_b,      spawn,          {.v = btcmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_i,      spawn,          {.v = browsercmd } },
	{ MODKEY|ShiftMask,             XK_i,      spawn,          {.v = pbrowsercmd } },
//	{ MODKEY,                       XK_m,      spawn,          {.v = mailcmd } },
// { MODKEY,                       XK_a,      spawn,          {.v = audioswtchcmd } },
	{ 0,                            XK_Print,  spawn,          {.v = scrotcmd } },
	{ 0,                            XF86XK_Tools,  spawn,          {.v = powercmd } },

// Media Keys
	{ 0,                            XF86XK_AudioPlay,             spawn,          {.v = mpdtogglecmd } },
	{ 0,                            XF86XK_AudioNext,             spawn,          {.v = mpdnextcmd } },
	{ 0,                            XF86XK_AudioPrev,             spawn,          {.v = mpdprevcmd } },
	{ 0,                            XF86XK_AudioRaiseVolume,      spawn,          {.v = volraisecmd } },
	{ 0,                            XF86XK_AudioLowerVolume,      spawn,          {.v = vollowercmd } },
	{ 0,                            XF86XK_AudioMute,             spawn,          {.v = volmutecmd } },
// WM Keys
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_equal,  incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_minus,  incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_Tab,    zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_o,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,            			XK_y,  	   togglescratch,  {.ui = 0 } },
	{ MODKEY,            			XK_u,	   togglescratch,  {.ui = 1 } },
	{ MODKEY,            			XK_x,	   togglescratch,  {.ui = 2 } },
	TAGKEYS(                        XK_a,                      0)
	TAGKEYS(                        XK_s,                      1)
	TAGKEYS(                        XK_d,                      2)
	TAGKEYS(                        XK_f,                      3)
	TAGKEYS(                        XK_w,                      4)
	TAGKEYS(                        XK_e,                      5)
	TAGKEYS(                        XK_g,                      6)
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

