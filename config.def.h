/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h> /* 功能键 */

/* 基础配置 */
static const unsigned int borderpx = 1; /* border pixel of windows 窗口边框宽度 */
static const unsigned int snap = 32; /* snap pixel */
static const int showbar = 1; /* 0 means no bar 是否显示状态栏 */
static const int topbar = 0; /* 0 means bottom bar 状态栏显示怎么顶部还是底部 */
static const Bool viewontag = True; /* Switch view on tag switch 窗口是否根据应用显示 */
static const char *fonts[] = { 
    "UbuntuSansMono Nerd Font:size=10:type=Regular:entialias=true:autohint=true", /* 英文字体 */
    "WenQuanYi Micro Hei:type=Regular:size=10:entialias=true:autohint=true", /* 中文字体 */
    "Symbols Nerd Font Mono:pixelsize=20:type=2048-em:entialias=true:autohint=true", /* 符号字体 */
};
static const char dmenufont[] = "UbuntuSansMono Nerd Font:size=10:type=Regular:entialias=true:autohint=true"; /* dmenu字体 */
/* 状态栏透明补丁 */
static unsigned int baralpha = 0xd0; /* 透明度，16进制 */
static unsigned int borderalpha = OPAQUE;

/* 颜色配置 */

#define PRIMARYCOLOR "#00b7c3"

static const char col_bg1[] = "#1c1c1c";
static const char col_bg2[] = "#272727";
static const char col_fg[] = "#ffffff";
static const char col_fg1[] = "#B7B7B7";
static const char col_primary[] = PRIMARYCOLOR;
static const char col_debian[] = "#A80030";
static const char col_normal[] = "#3FA63B";
static const char col_warn[] = "#BE9B00";
static const char col_error[] = "#DC143C";
static const char *colors[][3] = {
    /* [name]         = { fg,          bg,      border } */
    [SchemeNorm]      = { col_fg,      col_bg1, col_bg1 },
    [SchemeSel]       = { col_fg,      col_bg2, col_bg2 },
    [SchemeHid]       = { col_bg2,     col_bg1, col_bg2 },
    [SchemeUnderline] = { col_primary, col_bg1, col_bg2 },
    [SchemeDebian]    = { col_debian,  col_bg1, col_bg2 },
    [SchemeWarn]      = { col_warn,    col_bg1, col_bg2 },
    [SchemeError]     = { col_error,   col_bg1, col_bg2 },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" }; // 工作区图标

static const Rule rules[] = {
    /* xprop(1):
     *	WM_CLASS(STRING) = instance, class
     *	WM_NAME(STRING) = title
     */
    /* { class, instance,  title, tags-mask, isfloating, monitor } */
    { "Gimp",              NULL, NULL, 0, 1, -1 },
    { "Gpick",             NULL, NULL, 0, 1, -1 },
    { "Nemo",              NULL, NULL, 0, 1, -1 },
    { "Sxiv",              NULL, NULL, 0, 1, -1 },
    { "Clash for Windows", NULL, NULL, 0, 1, -1 },
};

/* layout(s) */
/* 布局 */
static const float mfact = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster = 1; /* number of clients in master area */
static const int resizehints = 1; /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

/* 布局方式图标 */
static const Layout layouts[] = {
    /* { symbol, arrange-function } */
    { "[S]",    tile }, /* first entry is default */
    { "[F]",    NULL }, /* no layout function means floating behavior */
    { "[M]",    monocle },
};

static const int newclientathead = 0; /* 定义新窗口在栈顶还是栈底 */
static const char scratchpadname[] = "scratchpad"; /* 便笺名称 */

/* 脚本存放的目录 $XDG_DATA_HOME/dwm */
static const char dwmdir[] = "dwm";

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
{ MODKEY,                       KEY, view,       {.ui = 1 << TAG} }, \
{ MODKEY|ControlMask,           KEY, toggleview, {.ui = 1 << TAG} }, \
{ MODKEY|ShiftMask,             KEY, tag,        {.ui = 1 << TAG} }, \
{ MODKEY|ControlMask|ShiftMask, KEY, toggletag,  {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

#define STATUSBAR "dwmblocks"

/* dwm重启后复原窗口位置临时文件 */
#define SESSION_FILE "/tmp/dwm-session"
#define SESSION_TAG_FILE "/tmp/dwm-tag-session"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-b", "-m", dmenumon, "-fn", dmenufont, "-nb", col_bg1, "-nf", col_fg1, "-sb", col_bg2, "-sf", col_primary, NULL };

static const char *nemocmd[] = { "nemo", NULL };
static const char *stcmd[]  = { "st", NULL };
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "120x34", NULL };

static const Key keys[] = {
    /* { modifier, key, function, argument } */
    { MODKEY,                       XK_r,      spawn,          {.v = dmenucmd } }, /* 应用启动器 */
    { MODKEY,                       XK_a,      scriptspawn,    {.v = "toggle_show_all_blocks.sh "STATUSBAR } }, /* 显示或隐藏dwmblocks上的所有信息 */
    { MODKEY,                       XK_p,      scriptspawn,    {.v = "display.sh select" } }, /* 切换显示器 */
    { MODKEY,                       XK_e,      spawn,          {.v = nemocmd } }, /* 文件管理器 */
    { MODKEY|ShiftMask,             XK_Return, spawn,          {.v = stcmd } }, /* st终端 */

    { Mod1Mask,                     XK_Tab,    view,           {0} }, /* 切换工作区 */
    { Mod1Mask,                     XK_F4,     killclient,     {0} }, /* 关闭窗口 */

    { MODKEY,                       XK_s,      togglescratch,  {.v = scratchpadcmd } }, /* 显示或隐藏便笺 */

    { MODKEY,                       XK_b,      togglebar,      {0} }, /* 显示或隐藏状态栏 */
    /* 窗口移动 */
    { MODKEY|ShiftMask,             XK_j,      rotatestack,    {.i = +1 } }, /* 窗口向栈底移动 */
    { MODKEY|ShiftMask,             XK_k,      rotatestack,    {.i = -1 } }, /* 窗口向栈顶移动 */
    { MODKEY,                       XK_j,      focusstackvis,  {.i = +1 } }, /* 下一个窗口 */
    { MODKEY,                       XK_k,      focusstackvis,  {.i = -1 } }, /* 上一个窗口 */
    { MODKEY|Mod1Mask,              XK_j,      focusstackhid,  {.i = +1 } }, /* 下一个窗口包括隐藏的窗口 */
    { MODKEY|Mod1Mask,              XK_k,      focusstackhid,  {.i = -1 } }, /* 上一个窗口包括隐藏的窗口 */
    /* 窗口宽度调整 */
    { MODKEY,                       XK_Left,   setmfact,       {.f = -0.05} }, /* 减少窗口显示宽度 */
    { MODKEY,                       XK_Right,  setmfact,       {.f = +0.05} }, /* 增加窗口显示宽度 */
    { MODKEY,                       XK_0,      view,           {.ui = ~0 } }, /* 显示所有工作区内容 */
    /* 工作区布局 */
    { MODKEY|Mod1Mask,              XK_1,      setlayout,      {.v = &layouts[0]} },
    { MODKEY|Mod1Mask,              XK_2,      setlayout,      {.v = &layouts[1]} },
    { MODKEY|Mod1Mask,              XK_3,      setlayout,      {.v = &layouts[2]} },
    { MODKEY,                       XK_space,  setlayout,      {0} }, /* 切换工作局内的两个布局 */
    { MODKEY|ShiftMask,             XK_space,  togglefloating, {0} }, /* 切换窗口浮动显示 */
    /* 多显示器 */
    { MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
    { MODKEY,                       XK_period, focusmon,       {.i = +1 } },
    { MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
    { MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },

    { MODKEY|ShiftMask,             XK_s,      showall,        {0} }, /* 显示所有隐藏的窗口 */
    { MODKEY|ShiftMask,             XK_h,      hide,           {0} }, /* 隐藏当前窗口 */
    /* 工作区栏快捷键 */
    TAGKEYS(XK_1, 0)
    TAGKEYS(XK_2, 1)
    TAGKEYS(XK_3, 2)
    TAGKEYS(XK_4, 3)
    TAGKEYS(XK_5, 4)
    TAGKEYS(XK_6, 5)
    TAGKEYS(XK_7, 6)
    TAGKEYS(XK_8, 7)
    TAGKEYS(XK_9, 8)

    { MODKEY|ShiftMask,             XK_q,      quit,           {0} }, /* 退出dwm */
    { MODKEY|ControlMask|ShiftMask, XK_q,      quit,           {1} }, /* 重启dwm */

    /* 音频 */
    { 0, XF86XK_AudioMute,                     scriptspawn,    {.v = "volume.sh mute "STATUSBAR} }, /* 静音 */
    { 0, XF86XK_AudioRaiseVolume,              scriptspawn,    {.v = "volume.sh inc "STATUSBAR} }, /* 增加音量 */ 
    { 0, XF86XK_AudioLowerVolume,              scriptspawn,    {.v = "volume.sh dec "STATUSBAR} }, /* 减小音量 */ 
    { 0, XF86XK_AudioPause,                    scriptspawn,    {.v = "player.sh pause"} }, /* 暂停 */
    { 0, XF86XK_AudioPrev,                     scriptspawn,    {.v = "player.sh previous"} }, /* 上一个 */
    { 0, XF86XK_AudioNext,                     scriptspawn,    {.v = "player.sh next"} }, /* 下一个 */
    { 0, XF86XK_AudioPlay,                     scriptspawn,    {.v = "player.sh play-pause"} }, /* 播放或暂停 */
    { 0, XF86XK_AudioStop,                     scriptspawn,    {.v = "player.sh stop"} }, /* 停止 */
    /* 屏幕亮度 */
    { 0, XF86XK_MonBrightnessUp,               scriptspawn,    {.v = "light.sh inc \\"PRIMARYCOLOR} }, /* 提高亮度 */ 
    { 0, XF86XK_MonBrightnessDown,             scriptspawn,    {.v = "light.sh dec \\"PRIMARYCOLOR} }, /* 减少亮度 */ 
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
    /*
     * Button1 鼠标左键
     * Button2 鼠标中键
     * Button3 鼠标右键
     * Button4 鼠标滚轮向上
     * Button5 鼠标滚轮向下
     */
    /* { click,      event-mask,  button,  function,       argument } */
    /* 布局按钮点击事件 */
    { ClkLtSymbol,   0,           Button1, setlayout,      {0} }, /* 切换布局 */
    { ClkLtSymbol,   0,           Button3, setlayout,      {.v = &layouts[2]} }, /* 切换布局 */
    /* 点击窗口标题事件 */
    { ClkWinTitle,   0,           Button1, togglewin,      {0} }, /* 隐藏或显示窗口 */
    { ClkWinTitle,   0,           Button2, killclient,     {.i = 0} }, /* 关闭窗口 */
    /* 状态栏鼠标事件 */          
    { ClkStatusText, 0,           Button1, sigstatusbar,   {.i = 1} }, /* 向dwmblocks发送鼠标左键信号 */
    { ClkStatusText, 0,           Button2, sigstatusbar,   {.i = 2} }, /* 向dwmblocks发送鼠标中键信号 */
    { ClkStatusText, 0,           Button3, sigstatusbar,   {.i = 3} }, /* 向dwmblocks发送鼠标右键信号 */
    { ClkStatusText, 0,           Button4, sigstatusbar,   {.i = 4} }, /* 向dwmblocks发送鼠标滚轮向上键信号 */
    { ClkStatusText, 0,           Button5, sigstatusbar,   {.i = 5} }, /* 向dwmblocks发送鼠标滚轮向下键信号 */
    { ClkStatusText, ControlMask, Button1, sigstatusbar,   {.i = 6} }, /* 向dwmblocks发送ctrl+鼠标左键信号 */
    { ClkStatusText, ControlMask, Button3, sigstatusbar,   {.i = 7} }, /* 向dwmblocks发送ctrl+鼠标右键信号 */
    /* 窗口鼠标事件 */
    { ClkClientWin,  MODKEY,      Button1, movemouse,      {0} }, /* 移动窗口 */
    { ClkClientWin,  MODKEY,      Button2, togglefloating, {0} }, /* 接换浮动窗口 */
    { ClkClientWin,  MODKEY,      Button3, resizemouse,    {0} }, /* 调整窗口大小 */
    /* 工作区按钮点击事件 */
    { ClkTagBar,     0,           Button1, view,           {0} }, /* 切换到对应的工作区 */
    { ClkTagBar,     0,           Button3, toggleview,     {0} }, /* 临时将点击的工作区内的窗口显示在当前窗口 */
    { ClkTagBar,     MODKEY,      Button1, tag,            {0} }, /* 将当前窗口移动到点击的工作区内 */
};

