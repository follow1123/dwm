/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows 窗口边框宽度*/
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar 是否显示状态栏*/
static const int topbar             = 0;        /* 0 means bottom bar 状态栏显示怎么顶部还是底部*/
static const Bool viewontag         = True;     /* Switch view on tag switch 窗口是否根据应用显示*/
static const char *fonts[]          = { 
	"JetBrains Mono:size=10:type=Regular:entialias=true:autohint=true", /* 英文字体 */
	"WenQuanYi Micro Hei Mono:type=Regular:size=10:entialias=true:autohint=true", /* 中文字体 */
	"Symbols Nerd Font Mono:pixelsize=18:type=2048-em:entialias=true:autohint=true", /* 符号字体 */
};
static const char dmenufont[]       = "monospace:size=14";

// static const char col_bg1[]       = "#101010";
static const char col_bg1[]       = "#1c1c1c";
static const char col_bg2[]       = "#272727";
static const char col_fg[]       = "#ffffff";
static const char col_primary[]        = "#00b7c3";
// static const char col_primary[]        = "#308280";
static const char col_debian[]        = "#A80030";
static const char col_normal[]        = "#3FA63B";
static const char col_warn[]        = "#BE9B00";
static const char col_error[]        = "#DC143C";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_fg, col_bg1, col_bg1 },
	[SchemeSel]  = { col_fg, col_bg2,  col_bg2 },
	[SchemeHid]  = { col_bg2,  col_bg1, col_bg2  },
	[SchemeUnderline]  = { col_primary, col_bg1, col_bg2 },
	[SchemeDebian] = { col_debian, col_bg1, col_bg2 },
	[SchemeWarn] = { col_warn, col_bg1, col_bg2 },
	[SchemeError] = { col_error, col_bg1, col_bg2 },
};

/* tagging 󰎤 󰎧 󰎪 󰎭 󰎱 󰎳 󰎶 󰎹 󰎼 */
static const char *tags[] = { "󰎤", "󰎧", "󰎪", "󰎭", "󰎱", "󰎳", "󰎶", "󰎹", "󰎼" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "",      tile },    /* first entry is default */
	{ "",      NULL },    /* no layout function means floating behavior */
	{ "",      monocle },
};

/*自定义变量*/

static const int newclientathead    = 0;         /* 定义新窗口在栈顶还是栈底 */

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

#define STATUSBAR "dwmblocks"


/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_fg, "-nf", col_fg, "-sb", col_fg, "-sf", col_fg, NULL };

static const char *roficmd[] = { "rofi", "-show", "run", NULL };
static const char *statusshowallcmd[] = { "bash", "/home/yf/space/scripts/dwmblocks/toggle_show_all.sh", NULL };
static const char *nemocmd[] = { "nemo", NULL };
static const char *terstmcmd[]  = { "st", NULL };
static const char *termcmd[]  = { "alacritty", NULL };
static const char *volup[]  = { "/home/yf/space/scripts/volm.sh", "0" };
static const char *voldown[]  = { "/home/yf/space/scripts/volm.sh", "1" };
static const char *voltoogle[]  = { "/home/yf/space/scripts/volm.sh", "2" };

static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_r,      spawn,          {.v = roficmd } },
	{ MODKEY,                       XK_a,      spawn,          {.v = statusshowallcmd } },
	{ MODKEY,                       XK_e,      spawn,          {.v = nemocmd } },
	{ MODKEY,             			XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,       		XK_Return, spawn,          {.v = terstmcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	// { MODKEY|ShiftMask,             XK_j,      rotatestack,    {.i = +1 } },
	// { MODKEY|ShiftMask,             XK_k,      rotatestack,    {.i = -1 } },
	// { MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	// { MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_j,      focusstackvis,  {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstackvis,  {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_j,      focusstackhid,  {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      focusstackhid,  {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	// { MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,             			XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_s,      show,           {0} },
	{ MODKEY|ShiftMask,             XK_s,      showall,        {0} },
	//{ MODKEY,                       XK_h,      hide,           {0} },
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
	{ MODKEY|ControlMask|ShiftMask, XK_q,      quit,           {1} }, 

	/* 自定义快捷键 */
	{ MODKEY,       	      		XK_F3,      spawn,           {.v = volup } },
	{ MODKEY,             			XK_F2,      spawn,           {.v = voldown } },
	{ MODKEY,             			XK_F4,      spawn,           {.v = voltoogle } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button1,        togglewin,      {0} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigstatusbar,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigstatusbar,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigstatusbar,   {.i = 3} },
	{ ClkStatusText,        0,              Button4,        sigstatusbar,   {.i = 4} },
	{ ClkStatusText,        0,              Button5,        sigstatusbar,   {.i = 5} },
	{ ClkStatusText,        ControlMask,    Button1,        sigstatusbar,   {.i = 6} },
	{ ClkStatusText,        ControlMask,    Button3,        sigstatusbar,   {.i = 7} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

