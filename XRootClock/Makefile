CC ?= cc
OUT = xrootclock
SRC = *.c

# OpenBSD paths
OBSDL = -L/usr/X11R6/lib
OBSDI = -I/usr/X11R6/include

CPPFLAGS = $(OBSDI) $(FT2I)
CFLAGS = -std=c99 -Wall -Wextra -O2 -fdiagnostics-color=always
LDFLAGS = $(OBSDL) -Iinclude/
LDLIBS = -lX11 -lm
LINE = $(CC) $(CPPFLAGS) $(CFLAGS) $(SRC) -o $(OUT) $(LDFLAGS) $(LDLIBS)

all:
	$(LINE)

clean:
	rm -f $(OUT)

compile_flags:
	rm -f compile_flags.txt
	for f in $(LINE); do echo $$f >> compile_flags.txt; done

.PHONY: all clean compile_flags
