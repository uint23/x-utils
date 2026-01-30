#include <stdio.h>
#include <stdlib.h>

#include <X11/Xlib.h>

/* globals */
Display* dpy = NULL;
Window root = None;
Bool running = False;

void run(void);
void setup(void);

void run(void)
{
	XEvent e;
	while (running && !XNextEvent(dpy, &e)) {

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
