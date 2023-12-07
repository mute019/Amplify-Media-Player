#include <iostream>
#include <string>
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

#ifndef HEADER_GUI_ELEMENTS_H_
#define HEADER_GUI_ELEMENTS_H_

/*
 * This keeps list of all the GUI widgets
 *
 * */

typedef struct gui_elements
{
	GtkWidget *main_window;
	GtkWidget *video_window;

	// buttons and sliders
	GtkWidget *play;
	GtkWidget *stop;
	GtkWidget *pause;
	GtkWidget *repeat;
	GtkWidget *shuffle;
	GtkWidget *seek_back;
	GtkWidget *seek_for;
	GtkWidget *next;
	GtkWidget *prev;
	GtkWidget *volume_slider;
	GtkWidget *seek_slider;
	GtkWidget *open_dir;
	GtkWidget *view_playlist;
	GtkWidget *meta_data_display;
	GtkWidget *current_time;
	GtkWidget *total_time;
	GtkWidget *help_button;
	GtkWidget *about;
	GtkWidget *mute;
	GtkWidget *favorite;
	GtkWidget *rewind;

	// containers for widgets
	GtkWidget *main_box;      /* VBox to hold main_hbox and the controls */
	GtkWidget *main_hbox;    /* HBox to hold the video_window and the stream info text widget */
	GtkWidget *control_box_1;
	GtkWidget *control_box_2;

	// Menubar
	GtkWidget *menubar;
	GtkWidget *playbackMenu;	/* Playback Menu */
	GtkWidget *playbackMb;
	GtkWidget *play_pauseMb;
	GtkWidget *stopMb;
	GtkWidget *sepMb1;
	GtkWidget *quitMb;
	GtkWidget *seek_backMb;
	GtkWidget *seek_forMb;
	GtkWidget *seek_toMb;
	GtkWidget *nextMb;
	GtkWidget *prevMb;
	GtkWidget *sepMb2;
	GtkWidget *repeatMb;
	GtkWidget *fileMenu;	/* File Menu */
	GtkWidget *fileMb;
	GtkWidget *openFileMb;
	GtkWidget *openDirMb;
	GtkWidget *toolMenu;	/* Tools */
	GtkWidget *toolMb;
	GtkWidget *sleepMenu;	// sleep timer
	GtkWidget *sleeptimer;
	GtkWidget *one;
	GtkWidget *five;
	GtkWidget *ten;
	GtkWidget *fifteen;
	GtkWidget *disable;
	GtkWidget *darkmode;	// dark mode
	GtkWidget *helpMenu;	/* Help Menu */
	GtkWidget *helpMb;
	GtkWidget *aboutMb;
	GtkWidget *playlistMb;

	// Pop-up menu
	GtkWidget *ebox;
	GtkWidget *pmenu;
	GtkWidget *hideMi;
	GtkWidget *quitPu;
	GtkWidget *sepA;
	GtkWidget *stopPu;
	GtkWidget *play_pausePu;
	GtkWidget *sepB;
	GtkWidget *seek_backPu;
	GtkWidget *seek_forPu;
	GtkWidget *nextPu;
	GtkWidget *previousPu;

} gui_elem;

#endif /* HEADER_GUI_ELEMENTS_H_ */
