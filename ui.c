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

#include <gtk/gtk.h>
#include <gdk/gdkx.h>

#include <string.h>

static char *uri_to_play;
static GtkWidget *video_output;

static void
play_cb (GtkWidget *widget,
         gpointer data)
{
    /* backend_play (); */
}

static void
stop_cb (GtkWidget *widget,
         gpointer data)
{
    backend_stop ();
}

static gboolean
delete_event (GtkWidget *widget,
              GdkEvent *event,
              gpointer data)
{
    return FALSE;
}

static void
destroy (GtkWidget *widget,
         gpointer data)
{
    gtk_main_quit ();
}

static void
start (void)
{
    GtkWidget *window;
    GtkWidget *button;
    GtkWidget *hbox;
    GtkWidget *vbox;

    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

    g_signal_connect (G_OBJECT (window), "delete_event",
                      G_CALLBACK (delete_event), NULL);

    g_signal_connect (G_OBJECT (window), "destroy",
                      G_CALLBACK (destroy), NULL);

    gtk_container_set_border_width (GTK_CONTAINER (window), 2);

    vbox = gtk_vbox_new (FALSE, 0);

    gtk_container_add (GTK_CONTAINER (window), vbox);

    gtk_widget_show (vbox);

    hbox = gtk_hbox_new (FALSE, 0);

    gtk_box_pack_end (GTK_BOX (vbox), hbox, FALSE, FALSE, 2);

    gtk_widget_show (hbox);

    {
        video_output = gtk_drawing_area_new ();

        gtk_box_pack_start (GTK_BOX (vbox), video_output, TRUE, TRUE, 2);

        gtk_widget_set_size_request (video_output, 128, 128);

        gtk_widget_show (video_output);
    }

    {
        button = gtk_button_new_with_label ("Play");

        g_signal_connect (G_OBJECT (button), "clicked",
                          G_CALLBACK (play_cb), NULL);

        gtk_box_pack_start (GTK_BOX (hbox), button, FALSE, FALSE, 2);

        gtk_widget_show (button);
    }

    {
        button = gtk_button_new_with_label ("Stop");

        g_signal_connect (G_OBJECT (button), "clicked",
                          G_CALLBACK (stop_cb), NULL);

        gtk_box_pack_start (GTK_BOX (hbox), button, FALSE, FALSE, 2);

        gtk_widget_show (button);
    }

    gtk_widget_show (window);
}

static gboolean
init (gpointer data)
{
    backend_set_window (GDK_WINDOW_XWINDOW (video_output->window));

    if (uri_to_play)
        backend_play (uri_to_play);

    return FALSE;
}

int
main (int argc,
      char *argv[])
{
    gtk_init (&argc, &argv);
    backend_init (&argc, &argv);

    start ();

    if (argc > 1)
    {
        if (strchr (argv[1], ':'))
            uri_to_play = g_strdup (argv[1]);
        else
            uri_to_play = g_strdup_printf ("file://%s", argv[1]);
    }

    g_idle_add (init, NULL);

    gtk_main ();

    g_free (uri_to_play);

    backend_deinit ();

    return 0;
}
