/* See /LICENSE for usage info */

#include <X11/Xlib.h>

#define SCRW 1280
#define SCRH 1024

static int size         = 240;
static int pos_x        = SCRW - (40+240);
static int pos_y        = SCRH - (40+240);
static int padding      = 10;
static int border_width = 0;
static int line_width   = 3;

static Bool draw_bg                   = False;
static Bool draw_fill                 = True;
static unsigned long clock_color      = 0xf7cfc3;
static unsigned long clock_fill_color = 0xb34f4f;
static unsigned long bg_color         = 0xb34f4f;
static unsigned long border_color     = 0xf7cfc3;

