/*          XRootClock
   A minimal xclock replica widget
   for you X desktop.

   This program is licensed under the
   same license as all other programs
   under this repository.

   See /LICENSE for more info.
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <time.h>
#include <unistd.h>

#include <X11/Xlib.h>

#include "config.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif /* M_PI */

Display* dpy = NULL;
Window root = None;
Window win = None;
GC gc = None;
Bool running = True;

void run(void);
void setup(void);
void draw(void);

void draw(void)
{
	if (draw_bg)
		XClearWindow(dpy, win);

	time_t now = time(NULL);
	struct tm* t = localtime(&now);

	int cx = size / 2;
	int cy = size / 2;
	int r  = size / 2 - padding;

	double sec = t->tm_sec * M_PI / 30.0;
	double min = t->tm_min * M_PI / 30.0;
	double hr  = (t->tm_hour % 12 + t->tm_min / 60.0) * M_PI / 6.0;

	if (draw_fill) {
		XSetForeground(dpy, gc, clock_fill_color);
		XFillArc(
			dpy, win, gc,
			padding, padding,
			size - padding * 2, size - padding * 2,
			0, 360 * 64
		);
	}

	XSetForeground(dpy, gc, border_color);
	XSetLineAttributes(dpy, gc, line_width, LineSolid, CapButt, JoinMiter);
	XDrawArc(
		dpy, win, gc,
		padding, padding,
		size - padding * 2 - 1, size - padding * 2 - 1,
		0, 360 * 64
	);

	XSetForeground(dpy, gc, clock_color);

	XDrawLine( /* hour hand */
		dpy, win, gc,
		cx, cy,
		cx + sin(hr)  * r * 0.5,
		cy - cos(hr) * r * 0.5
	);

	XDrawLine( /* minutes hand */
		dpy, win, gc,
		cx, cy,
		cx + sin(min) * r * 0.8,
		cy - cos(min)* r * 0.8
	);

	XDrawLine( /* seconds hand */
		dpy, win, gc,
		cx, cy,
		cx + sin(sec) * r * 0.9,
		cy - cos(sec)* r * 0.9
	);
}

void run(void)
{
	XEvent e;
	fd_set fds;

	while (running) {
		FD_ZERO(&fds);
		FD_SET(ConnectionNumber(dpy), &fds);

		struct timeval tv = { 1, 0 };
		select(ConnectionNumber(dpy) + 1, &fds, NULL, NULL, &tv);

		while (XPending(dpy)) {
			XNextEvent(dpy, &e);
			if (e.type == Expose)
				draw();
		}

		draw();
	}
}

void setup(void)
{
	dpy = XOpenDisplay(NULL);
	if (!dpy) {
		fprintf(stderr, "Failed to open default X11 Display");
		exit(EXIT_FAILURE);
	}

	int scr = DefaultScreen(dpy);
	root = RootWindow(dpy, scr);

	XSetWindowAttributes wa;
	wa.override_redirect = True;
	if (draw_bg)
		wa.background_pixel = bg_color;
	else
		wa.background_pixmap = ParentRelative;
	wa.border_pixel = border_color;
	wa.event_mask = ExposureMask;

	unsigned long wamask = CWOverrideRedirect | CWBorderPixel | CWEventMask;
	if (draw_bg)
		wamask |= CWBackPixel;
	else
		wamask |= CWBackPixmap;

	win = XCreateWindow(
		dpy, root,
		pos_x, pos_y, size, size,
		border_width,
		DefaultDepth(dpy, scr),
		InputOutput,
		DefaultVisual(dpy, scr),
		wamask,
		&wa
	);

	XMapWindow(dpy, win);
	XLowerWindow(dpy, win);

	gc = XCreateGC(dpy, win, 0, NULL);
	XSetLineAttributes(dpy, gc, line_width, LineSolid, CapButt, JoinMiter);
}

int main(void)
{
	setup();
#ifdef __OpenBSD__
	if (pledge("stdio unix", NULL) == -1) {
		fprintf(stderr, "Pledge failed");
		return EXIT_FAILURE;
	}
#endif /* __OpenBSD__ */
	run();

	return EXIT_SUCCESS;
}

