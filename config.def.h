/* See LICENSE file for copyright and license details. */
// 功能键依赖
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows 窗口边框宽度*/
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 7;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 7;       /* vert inner gap between windows */
static const unsigned int gappoh    = 7;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 7;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar 是否显示状态栏*/
static const int topbar             = 0;        /* 0 means bottom bar 状态栏显示怎么顶部还是底部*/
static const Bool viewontag         = True;     /* Switch view on tag switch 窗口是否根据应用显示*/
static const char *fonts[]          = { 
	"JetBrains Mono:size=12:type=Regular:entialias=true:autohint=true", /* 英文字体 */
	"WenQuanYi Micro Hei Mono:type=Regular:size=12:entialias=true:autohint=true", /* 中文字体 */
	"Symbols Nerd Font Mono:pixelsize=22:type=2048-em:entialias=true:autohint=true", /* 符号字体 */
};
static const char dmenufont[]       = "monospace:size=14";
// 状态栏透明补丁
static unsigned int baralpha        = 0xd0; // 透明度16进制
static unsigned int borderalpha     = OPAQUE;

// static const char col_bg1[]       = "#101010";
static const char col_bg1[]       = "#1c1c1c";
static const char col_bg2[]       = "#272727";
static const char col_fg[]       = "#ffffff";
static const char col_fg1[]       = "#B7B7B7";
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
static const char *tags[] = { "󰎤", "󰎧", "󰎪", "󰎭", "󰎱", "󰎳", "󰎶", "󰎹", "󰎼" }; // 工作区图标

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Gpick",    NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "",      tile },    /* first entry is default */
	{ "",      monocle },
	{ "[@]",      spiral },
	{ "[\\]",     dwindle },
	{ "H[]",      deck },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
	{ "HHH",      grid },
	{ "###",      nrowgrid },
	{ "---",      horizgrid },
	{ ":::",      gaplessgrid },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
	{ "",      NULL },    /* no layout function means floating behavior */
	{ NULL,       NULL },
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

#define SCRIPTS(name) "/home/yf/space/scripts/sde/"name

// dwm重启后复原窗口位置临时文件
#define SESSION_FILE "/tmp/dwm-session"

#define SESSION_TAG_FILE "/tmp/dwm-tag-session"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_fg, "-nf", col_fg, "-sb", col_fg, "-sf", col_fg, NULL };

static const char *roficmd[] = { "rofi", "-show", "run", NULL };
static const char *statusshowallcmd[] = { "bash", SCRIPTS("blocks/toggle_show_all.sh"), NULL };
static const char *displaycmd[] = { "bash", SCRIPTS("display.sh"), NULL };
static const char *nemocmd[] = { "nemo", NULL };
static const char *terstmcmd[]  = { "st", NULL };
static const char *termcmd[]  = { "alacritty", NULL };

static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_r,      spawn,          {.v = roficmd } }, // Super+r rofi应用启动器
	{ MODKEY,                       XK_a,      spawn,          {.v = statusshowallcmd } }, // Super+a 显示状态栏信息部分的所有信息
	{ MODKEY,                       XK_p,      spawn,          {.v = displaycmd } }, // Super+b 使用rofi切换显示器
	{ MODKEY,                       XK_e,      spawn,          {.v = nemocmd } }, // Spuer+e nemo文件管理器
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } }, // Super+enter alacrity终端
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = terstmcmd } }, // Super+Shift+enter st终端
	{ MODKEY,                       XK_b,      togglebar,      {0} }, // Spuer+b 隐藏/显示状态栏
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
	// 窗口宽度调整
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	// { MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
	{ Mod1Mask,                     XK_Tab,    view,           {0} }, // Alt+tab 切换工作区
	{ MODKEY,                       XK_q,      killclient,     {0} },
	{ MODKEY|Mod1Mask,              XK_1,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY|Mod1Mask,              XK_2,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY|Mod1Mask,              XK_3,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY|Mod1Mask,              XK_4,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY|Mod1Mask,              XK_5,      setlayout,      {.v = &layouts[8]} },
	{ MODKEY|Mod1Mask,              XK_6,      setlayout,      {.v = &layouts[9]} },
	{ MODKEY|Mod1Mask,              XK_7,      setlayout,      {.v = &layouts[10]} },
	{ MODKEY|Mod1Mask,              XK_8,      setlayout,      {.v = &layouts[11]} },
	{ MODKEY|Mod1Mask,              XK_9,      setlayout,      {.v = &layouts[12]} },
	// { MODKEY|Mod1Mask,              XK_10,      setlayout,      {.v = &layouts[9]} },
	// { MODKEY|Mod1Mask,              XK_11,      setlayout,      {.v = &layouts[10]} },
	// { MODKEY|Mod1Mask,              XK_12,      setlayout,      {.v = &layouts[11]} },
	// { MODKEY|Mod1Mask,              XK_13,      setlayout,      {.v = &layouts[12]} },
	// { MODKEY|Mod1Mask,              XK_14,      setlayout,      {.v = &layouts[13]} },

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

	// 间距相关快捷键
	// { MODKEY|ShiftMask,             XK_h,      setcfact,       {.f = +0.25} },
	// { MODKEY|ShiftMask,             XK_l,      setcfact,       {.f = -0.25} },
	// { MODKEY|ShiftMask,             XK_o,      setcfact,       {.f =  0.00} },
	// { MODKEY|Mod1Mask,              XK_u,      incrgaps,       {.i = +1 } },
	// { MODKEY|Mod1Mask|ShiftMask,    XK_u,      incrgaps,       {.i = -1 } },
	// { MODKEY|Mod1Mask,              XK_i,      incrigaps,      {.i = +1 } },
	// { MODKEY|Mod1Mask|ShiftMask,    XK_i,      incrigaps,      {.i = -1 } },
	// { MODKEY|Mod1Mask,              XK_o,      incrogaps,      {.i = +1 } },
	// { MODKEY|Mod1Mask|ShiftMask,    XK_o,      incrogaps,      {.i = -1 } },
	// { MODKEY|Mod1Mask,              XK_6,      incrihgaps,     {.i = +1 } },
	// { MODKEY|Mod1Mask|ShiftMask,    XK_6,      incrihgaps,     {.i = -1 } },
	// { MODKEY|Mod1Mask,              XK_7,      incrivgaps,     {.i = +1 } },
	// { MODKEY|Mod1Mask|ShiftMask,    XK_7,      incrivgaps,     {.i = -1 } },
	// { MODKEY|Mod1Mask,              XK_8,      incrohgaps,     {.i = +1 } },
	// { MODKEY|Mod1Mask|ShiftMask,    XK_8,      incrohgaps,     {.i = -1 } },
	// { MODKEY|Mod1Mask,              XK_9,      incrovgaps,     {.i = +1 } },
	// { MODKEY|Mod1Mask|ShiftMask,    XK_9,      incrovgaps,     {.i = -1 } },
	// { MODKEY|Mod1Mask,              XK_0,      togglegaps,     {0} },
	// { MODKEY|Mod1Mask|ShiftMask,    XK_0,      defaultgaps,    {0} },
	// 功能键配置

	{ 0,                            XF86XK_AudioMute,         spawn, SHCMD(SCRIPTS("volume.sh mute")) },
	{ 0,                            XF86XK_AudioRaiseVolume,  spawn, SHCMD(SCRIPTS("volume.sh inc")) },
	{ 0,                            XF86XK_AudioLowerVolume,  spawn, SHCMD(SCRIPTS("volume.sh dec")) },

	{ 0,                            XF86XK_AudioPause,        spawn, SHCMD("playerctl pause") }, // 暂停
	{ 0,                            XF86XK_AudioPrev,         spawn, SHCMD("playerctl previous") }, // 上一个
	{ 0,                            XF86XK_AudioNext,         spawn, SHCMD("playerctl next") }, // 下一个
	{ 0,                            XF86XK_AudioPlay,         spawn, SHCMD("playerctl play-pause") }, // 播放/暂停
	{ 0,                            XF86XK_AudioStop,         spawn, SHCMD("playerctl stop") }, // 停止

	// { 0,                            XF86XK_MonBrightnessUp,   spawn, SHCMD("light -A 5; notify-send -r 9123 -h int:value:`light` -h string:hlcolor:#dddddd 'Backlight' " ) },
	// { 0,                            XF86XK_MonBrightnessDown, spawn, SHCMD("light -U 5; notify-send -r 9123 -h int:value:`light` -h string:hlcolor:#dddddd 'Backlight' " ) },
};

/*------------------------------------------------------------------------------状态栏点击事件----------------------------------------------------------------------------------------------*/
/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	// 布局按钮点击事件
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} }, // 鼠标左键 切换布局
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} }, // 鼠标右键 切换到第三个布局
	// 点击窗口标题事件
	{ ClkWinTitle,          0,              Button1,        togglewin,      {0} }, // 隐藏/显示窗口
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} }, //
	// 状态栏鼠标事件
	{ ClkStatusText,        0,              Button1,        sigstatusbar,   {.i = 1} },	// 鼠标左键
	{ ClkStatusText,        0,              Button2,        sigstatusbar,   {.i = 2} }, // 鼠标中键
	{ ClkStatusText,        0,              Button3,        sigstatusbar,   {.i = 3} }, // 鼠标右键
	{ ClkStatusText,        0,              Button4,        sigstatusbar,   {.i = 4} }, // 鼠标滚轮向上
	{ ClkStatusText,        0,              Button5,        sigstatusbar,   {.i = 5} }, // 鼠标滚轮向下
	{ ClkStatusText,        ControlMask,    Button1,        sigstatusbar,   {.i = 6} }, // Ctrl+鼠标右键
	{ ClkStatusText,        ControlMask,    Button3,        sigstatusbar,   {.i = 7} },	// Ctrl+鼠标左键
	// 窗口鼠标事件
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },	// Super+鼠标左键 移动窗口
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },	// Super+鼠标中键 接换浮动窗口
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },	// Super+鼠标右键 设置窗口大小
	// 工作区按钮点击事件
	{ ClkTagBar,            0,              Button1,        view,           {0} }, // 鼠标左键 切换到对应的工作区
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} }, // 鼠标右键 同时显示点击的工作区和当前工作区
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} }, // Spuer+鼠标左键 将当前窗口移动到点击的工作区内
	// { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} }, // Super+鼠标右键 将当前窗口复制到点击的工作区内（没什么用）
};

