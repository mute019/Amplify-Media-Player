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

#ifndef HEADER_APPLICATION_STATE_H_
#define HEADER_APPLICATION_STATE_H_


/*
 * This structure have the overall state of our application which includes both our GUI and Gstreamer Pipeline
 * */

typedef struct application_state
{
	GstElement* pipeline;
	GstElement* filesrc;
	GstElement* volume = nullptr;

	gboolean repeat= FALSE;
	gboolean shuffle= FALSE;
	gint64 duration;
	gboolean Terminate = FALSE;

	GstState pipeline_state;
	std::vector<std::string> playlist_vector;
	std::vector<std::string> shuffled_playlist_vector;
	std::vector<std::string>fav_list_vector;
	std::vector<std::string>::iterator play_pointer;

	gboolean video_flag = FALSE;

	GtkWidget* video_window;
	GtkWidget* top_window;
	guintptr window_handle;
	GdkWindow* gd_window;
	GstVideoOverlay* overlay;
	GThread* thread;

	gdouble temp_vol;
	GtkWidget* temp_range;
	GtkWidget* temp_mute;
	GtkWidget* temp_fav;
	GtkWidget* temp_main_window;

	GtkWidget* ref_current;
	GtkWidget* ref_duration;

	GtkWidget* seek_slider;
	gulong slider_update_signal_id;//used block or unblock the seek slider value changed signal

	GtkWidget* meta_window = nullptr;

	GtkWidget* rewind;

	gint timing = 0;
	GtkWidget* repeatButton;
	GtkWidget* repeatMb;
	GtkWidget* shuffleButton;
	GtkWidget* shuffleMb;
	GtkWidget* play_pauseButton;
	GtkWidget* play_pauseMb;
	GtkWidget* entry;
	GtkWidget* seekwindow;
	gint seek;
	GtkWidget* darkmode;

} app_state;

#endif /* HEADER_APPLICATION_STATE_H_ */
