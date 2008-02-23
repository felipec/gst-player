/*
 * Copyright (C) 2008 Felipe Contreras.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <gst/gst.h>
#include <gst/interfaces/xoverlay.h>

#include "gst-backend.h"

static GstElement *pipeline;
static GstElement *bin;
static GstElement *videosink;
static gpointer window;

static gboolean
bus_cb (GstBus *bus,
        GstMessage *msg,
        gpointer data)
{
    switch (GST_MESSAGE_TYPE (msg))
    {
        case GST_MESSAGE_EOS:
            {
                g_debug ("end-of-stream");
                break;
            }
        case GST_MESSAGE_ERROR:
            {
                gchar *debug;
                GError *err;

                gst_message_parse_error (msg, &err, &debug);
                g_free (debug);

                g_warning ("Error: %s", err->message);
                g_error_free (err);
                break;
            }
        default:
            break;
    }

    return TRUE;
}

void
backend_init (int *argc,
              char **argv[])
{
    gst_init (argc, argv);
}

void
backend_set_window (gpointer window_)
{
    window = window_;
}

void
backend_play (const char *uri)
{
    backend_stop ();

    pipeline = gst_pipeline_new ("gst-player");

    bin = gst_element_factory_make ("playbin", "bin");
    videosink = gst_element_factory_make ("ximagesink", "videosink");

    g_object_set (G_OBJECT (bin), "video-sink", videosink, NULL);

    gst_bin_add (GST_BIN (pipeline), bin);

    {
        GstBus *bus;
        bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
        gst_bus_add_watch (bus, bus_cb, NULL);
        gst_object_unref (bus);
    }

    g_object_set (G_OBJECT (bin), "uri", uri, NULL);

    if (GST_IS_X_OVERLAY (videosink))
    {
        gst_x_overlay_set_xwindow_id (GST_X_OVERLAY (videosink), GPOINTER_TO_INT (window));
    }

    gst_element_set_state (pipeline, GST_STATE_PLAYING);
}

void
backend_stop (void)
{
    if (pipeline)
    {
        gst_element_set_state (pipeline, GST_STATE_NULL);
        gst_object_unref (GST_OBJECT (pipeline));
        pipeline = NULL;
    }
}

void
backend_pause (void)
{
    gst_element_set_state (pipeline, GST_STATE_PAUSED);
}

void
backend_resume (void)
{
    gst_element_set_state (pipeline, GST_STATE_PLAYING);
}

void
backend_reset (void)
{
    gst_element_seek (pipeline, 1.0,
                      GST_FORMAT_TIME,
                      GST_SEEK_FLAG_FLUSH,
                      GST_SEEK_TYPE_SET, 0,
                      GST_SEEK_TYPE_NONE, GST_CLOCK_TIME_NONE);
}

void
backend_deinit (void)
{
}
