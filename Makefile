CC := gcc

EXTRA_WARNINGS := -Wextra -ansi -std=c99 -Wno-unused-parameter

GST_LIBS := $(shell pkg-config --libs gstreamer-0.10 gstreamer-interfaces-0.10)
GST_CFLAGS := $(shell pkg-config --cflags gstreamer-0.10 gstreamer-interfaces-0.10)
GTK_LIBS := $(shell pkg-config --libs gtk+-2.0)
GTK_CFLAGS := $(shell pkg-config --cflags gtk+-2.0)

CFLAGS := -ggdb -Wall $(EXTRA_WARNINGS)

gst-player: ui.o gst-backend.o
gst-player: CFLAGS := $(GTK_CFLAGS) $(GST_CFLAGS)
gst-player: LIBS := $(GTK_LIBS) $(GST_LIBS)
binaries += gst-player

all: $(binaries)

$(binaries):
	$(CC) $(LDFLAGS) $(LIBS) -o $@ $^

%.o:: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -rf $(binaries)
	find . -name "*.o" | xargs rm -rf
