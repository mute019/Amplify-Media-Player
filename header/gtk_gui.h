#include <iostream>
#include <string>
#include <vector>

#include <gst/gst.h>
#include <gtk/gtk.h>
#include <gst/video/videooverlay.h>
#include <gdk/gdk.h>
#include <glib.h>

#if defined (GDK_WINDOWING_X11)
#include <gdk/gdkx.h>
#elif defined (GDK_WINDOWING_WIN32)
#include <gdk/gdkwin32.h>
#elif defined (GDK_WINDOWING_QUARTZ)
#include <gdk//gdkquartz.h>
#endif

#include "./gui_elements.h"
#include "./playbin.h"
#include "./application_state.h"

#ifndef HEADER_GTK_GUI_H_
#define HEADER_GTK_GUI_H_

/*
 * fun: This one initializes all our widgets, initial state of our application, and gstreamer pipeline
 * */

class gtkgui
{
	gui_elem *widget_configs;
	app_state *status;
	void create_gui();
	static void realize_window(GtkWidget* , app_state* );
	static gboolean draw_window(GtkWidget* , cairo_t* , app_state* );
public:
	gtkgui();
	static void separate_thread(GstChildProxy *, GObject *, gchar *, app_state **);
	static void pipeline_init(gtkgui *);
	app_state* app_state_getter();
	gui_elem* widgets_getter();
	void main_loop(std::vector<std::string>&);
};

#endif /* HEADER_GTK_GUI_H_ */
