CC=gcc

GST_LIBS=`pkg-config --libs gstreamer-0.10` -lgstinterfaces-0.10
GST_CFLAGS=`pkg-config --cflags gstreamer-0.10`
GTK_LIBS=`pkg-config --libs gtk+-2.0`
GTK_CFLAGS=`pkg-config --cflags gtk+-2.0`

CFLAGS=-ggdb

BINS=gst-player

all: $(BINS)

gst-player: ui.c gst-backend.c
	$(CC) $(CFLAGS) $(GTK_CFLAGS) $(GTK_LIBS) $(GST_CFLAGS) $(GST_LIBS) $+ -o $@

clean:
	rm -rf $(BINS)
