#include <iostream>
#include <string>
#include <vector>
#include <thread>
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

#include "../header/gtk_gui.h"
#include "../header/gui_elements.h"
#include "../header/navigator.h"
#include "../header/playback.h"
#include "../header/features.h"
#include "../header/metadata.h"


/*
 * fun: This one initializes all our widgets, initial state of our application, and gstreamer pipeline
 * */

gtkgui::gtkgui()
{
	gtk_init(nullptr, nullptr);
	gst_init(nullptr, nullptr);
	status = new app_state;
	status->pipeline = gst_pipeline_new("test-pipeline");
	status->filesrc = gst_element_factory_make("filesrc", "src");
	status->pipeline_state = GST_STATE_NULL;
	widget_configs = new gui_elem;

	widget_configs->main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	widget_configs->video_window = gtk_drawing_area_new();

	status->video_window = widget_configs->video_window;

	// Menubar widgets
	widget_configs->menubar = gtk_menu_bar_new();
	widget_configs->playbackMenu = gtk_menu_new();
	widget_configs->playbackMb = gtk_menu_item_new_with_label("Playback");
	widget_configs->play_pauseMb = gtk_menu_item_new_with_label("Pause");
	widget_configs->stopMb = gtk_menu_item_new_with_label("Stop");
	widget_configs->sepMb1 = gtk_separator_menu_item_new();
	widget_configs->quitMb = gtk_menu_item_new_with_label("Quit");
	widget_configs->seek_forMb = gtk_menu_item_new_with_label("Seek +5s");
	widget_configs->seek_backMb = gtk_menu_item_new_with_label("Seek -5s");
	widget_configs->seek_toMb = gtk_menu_item_new_with_label("Seek to specific");
	widget_configs->nextMb = gtk_menu_item_new_with_label("Next");
	widget_configs->prevMb = gtk_menu_item_new_with_label("Previous");
	widget_configs->sepMb1 = gtk_separator_menu_item_new();
	widget_configs->repeatMb = gtk_check_menu_item_new_with_label("Repeat");
	gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(widget_configs->repeatMb), FALSE);
	widget_configs->fileMenu = gtk_menu_new();
	widget_configs->fileMb = gtk_menu_item_new_with_label("File");
	widget_configs->openFileMb = gtk_menu_item_new_with_label("Open File");
	widget_configs->openDirMb = gtk_menu_item_new_with_label("Open Directory");

	widget_configs->toolMenu = gtk_menu_new();
	widget_configs->toolMb = gtk_menu_item_new_with_label("Tools");
	widget_configs->sleepMenu = gtk_menu_new();
	widget_configs->sleeptimer = gtk_menu_item_new_with_label("Sleep Timer");
	widget_configs->one = gtk_menu_item_new_with_label("1min");
	widget_configs->five=gtk_menu_item_new_with_label("5min");
	widget_configs->ten=gtk_menu_item_new_with_label("10min");
	widget_configs->fifteen=gtk_menu_item_new_with_label("15min");
	widget_configs->disable=gtk_menu_item_new_with_label("disable");
	widget_configs->darkmode = gtk_menu_item_new_with_label("Dark mode");

	widget_configs->helpMenu = gtk_menu_new();
	widget_configs->helpMb = gtk_menu_item_new_with_label("Help");
	widget_configs->aboutMb = gtk_menu_item_new_with_label("About");
	widget_configs->playlistMb = gtk_menu_item_new_with_label("Playlist");

	// Pop-up menu
	widget_configs->ebox = gtk_event_box_new();
	widget_configs->pmenu = gtk_menu_new();
	widget_configs->hideMi = gtk_menu_item_new_with_label("Minimize");
	widget_configs->quitPu = gtk_menu_item_new_with_label("Quit");
	widget_configs->sepA = gtk_separator_menu_item_new();
	widget_configs->play_pausePu = gtk_menu_item_new_with_label("Pause");
	widget_configs->stopPu = gtk_menu_item_new_with_label("Stop");
	widget_configs->sepB = gtk_separator_menu_item_new();
	widget_configs->seek_forPu = gtk_menu_item_new_with_label("Seek +5s");
	widget_configs->seek_backPu = gtk_menu_item_new_with_label("Seek -5s");
	widget_configs->nextPu = gtk_menu_item_new_with_label("Next");
	widget_configs->previousPu = gtk_menu_item_new_with_label("Previous");


	widget_configs->main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	//Media Playback Buttons
	widget_configs->play = gtk_button_new_from_icon_name ("media-playback-pause", GTK_ICON_SIZE_SMALL_TOOLBAR);
	gtk_widget_set_tooltip_text(widget_configs->play, "Play-Pause");
	widget_configs->stop = gtk_button_new_from_icon_name ("media-playback-stop", GTK_ICON_SIZE_SMALL_TOOLBAR);
	gtk_widget_set_tooltip_text(widget_configs->stop, "Stop");
	widget_configs->repeat=gtk_toggle_button_new();
	gtk_button_set_image(GTK_BUTTON(widget_configs->repeat), gtk_image_new_from_icon_name("media-playlist-repeat", GTK_ICON_SIZE_BUTTON));
	gtk_widget_set_tooltip_text(widget_configs->repeat, "Repeat");
	widget_configs->shuffle=gtk_toggle_button_new();
	gtk_button_set_image(GTK_BUTTON(widget_configs->shuffle), gtk_image_new_from_icon_name("media-playlist-shuffle-symbolic", GTK_ICON_SIZE_BUTTON));
	gtk_widget_set_tooltip_text(widget_configs->shuffle, "Shuffle");
	widget_configs->rewind=gtk_toggle_button_new();
	gtk_button_set_image(GTK_BUTTON(widget_configs->rewind), gtk_image_new_from_icon_name("go-previous", GTK_ICON_SIZE_SMALL_TOOLBAR));
	gtk_widget_set_tooltip_text(widget_configs->rewind, "Rewind");

	widget_configs->next = gtk_button_new_from_icon_name ("media-skip-forward", GTK_ICON_SIZE_BUTTON);
	gtk_widget_set_tooltip_text(widget_configs->next, "Next-Song");
	widget_configs->prev = gtk_button_new_from_icon_name ("media-skip-backward", GTK_ICON_SIZE_BUTTON);
	gtk_widget_set_tooltip_text(widget_configs->prev, "Previous-Song");
	widget_configs->meta_data_display=gtk_toggle_button_new();
	gtk_button_set_image(GTK_BUTTON(widget_configs->meta_data_display), gtk_image_new_from_icon_name("dialog-information", GTK_ICON_SIZE_BUTTON));
	gtk_widget_set_tooltip_text(widget_configs->meta_data_display, "Details");

	//seekers
	widget_configs->seek_back = gtk_button_new_from_icon_name ("media-seek-backward", GTK_ICON_SIZE_BUTTON);
	gtk_widget_set_tooltip_text(widget_configs->seek_back, "Seek-back-5seconds");
	widget_configs->seek_for = gtk_button_new_from_icon_name ("media-seek-forward", GTK_ICON_SIZE_BUTTON);
	gtk_widget_set_tooltip_text(widget_configs->seek_for, "Seek-forward-5seconds");

	//time duration
	widget_configs->current_time = gtk_label_new("00:00:00");
	widget_configs->total_time = gtk_label_new("00:00:00");

	//sliders
	widget_configs->seek_slider = gtk_scale_new_with_range (GTK_ORIENTATION_HORIZONTAL, 0, 100, 1);
	gtk_scale_set_draw_value (GTK_SCALE (widget_configs->seek_slider), 0);

	widget_configs->volume_slider = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0, 1, 0.1);
	gtk_widget_set_size_request(widget_configs->volume_slider, 150, -1);
	gtk_range_set_value(GTK_RANGE(widget_configs->volume_slider), 1);

	// Utitlity for playlist
	widget_configs->open_dir = gtk_button_new_from_icon_name ("list-add", GTK_ICON_SIZE_SMALL_TOOLBAR);
	gtk_widget_set_tooltip_text(widget_configs->open_dir, "Open-Directory");
	widget_configs->view_playlist=gtk_toggle_button_new();
	gtk_button_set_image(GTK_BUTTON(widget_configs->view_playlist), gtk_image_new_from_icon_name("view-list-symbolic", GTK_ICON_SIZE_BUTTON));
	gtk_widget_set_tooltip_text(widget_configs->view_playlist, "Playlist-View");

	widget_configs->about=gtk_toggle_button_new();
	gtk_button_set_image(GTK_BUTTON(widget_configs->about), gtk_image_new_from_icon_name("avatar-default", GTK_ICON_SIZE_BUTTON));
	gtk_widget_set_tooltip_text(widget_configs->about, "About");

	widget_configs->favorite=gtk_toggle_button_new();
	gtk_button_set_image(GTK_BUTTON(widget_configs->favorite), gtk_image_new_from_icon_name("emblem-favorite", GTK_ICON_SIZE_SMALL_TOOLBAR));
	gtk_widget_set_tooltip_text(widget_configs->favorite, "Favorite");

	widget_configs->mute=gtk_toggle_button_new();
	gtk_button_set_image(GTK_BUTTON(widget_configs->mute), gtk_image_new_from_icon_name("audio-volume-muted-symbolic", GTK_ICON_SIZE_BUTTON));
	gtk_widget_set_tooltip_text(widget_configs->mute, "Mute");

	widget_configs->help_button=gtk_toggle_button_new();
	gtk_button_set_image(GTK_BUTTON(widget_configs->help_button), gtk_image_new_from_icon_name("gtk-help", GTK_ICON_SIZE_SMALL_TOOLBAR));
	gtk_widget_set_tooltip_text(widget_configs->help_button, "Help");

	//Container Initialization
	widget_configs->main_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	widget_configs->control_box_1 = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
	widget_configs->control_box_2 = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
}


/*
 * fun: This packs all our widgets
 * */
void gtkgui::create_gui() {
	//packing of widgets in their respective containers
	const char* title = g_strdup_printf("AMPLIFY Media-Player\t%s",(*status->play_pointer).c_str());
	gtk_window_set_title(GTK_WINDOW(widget_configs->main_window),title);
	gtk_window_set_default_size(GTK_WINDOW(widget_configs->main_window), 1280, 720);

	gtk_box_pack_start (GTK_BOX (widget_configs->main_hbox), widget_configs->ebox, TRUE, TRUE, 0);

	gtk_container_add(GTK_CONTAINER(widget_configs->main_window), widget_configs->main_box);

	// Menubar
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(widget_configs->fileMb), widget_configs->fileMenu);
	gtk_menu_shell_append(GTK_MENU_SHELL(widget_configs->fileMenu), widget_configs->openFileMb);
	gtk_menu_shell_append(GTK_MENU_SHELL(widget_configs->fileMenu), widget_configs->openDirMb);
	gtk_menu_shell_append(GTK_MENU_SHELL(widget_configs->menubar), widget_configs->fileMb);

	gtk_menu_item_set_submenu(GTK_MENU_ITEM(widget_configs->playbackMb), widget_configs->playbackMenu);
	gtk_menu_shell_append(GTK_MENU_SHELL(widget_configs->playbackMenu), widget_configs->play_pauseMb);
	gtk_menu_shell_append(GTK_MENU_SHELL(widget_configs->playbackMenu), widget_configs->stopMb);
	gtk_menu_shell_append(GTK_MENU_SHELL(widget_configs->playbackMenu), widget_configs->seek_forMb);
	gtk_menu_shell_append(GTK_MENU_SHELL(widget_configs->playbackMenu), widget_configs->seek_backMb);
	gtk_menu_shell_append(GTK_MENU_SHELL(widget_configs->playbackMenu), widget_configs->seek_toMb);
	gtk_menu_shell_append(GTK_MENU_SHELL(widget_configs->playbackMenu), widget_configs->repeatMb);
	gtk_menu_shell_append(GTK_MENU_SHELL(widget_configs->playbackMenu), widget_configs->sepMb1);
	gtk_menu_shell_append(GTK_MENU_SHELL(widget_configs->playbackMenu), widget_configs->quitMb);
	gtk_menu_shell_append(GTK_MENU_SHELL(widget_configs->menubar), widget_configs->playbackMb);

	gtk_menu_item_set_submenu(GTK_MENU_ITEM(widget_configs->toolMb), widget_configs->toolMenu);
	gtk_menu_shell_append(GTK_MENU_SHELL(widget_configs->toolMenu), widget_configs->sleeptimer);
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(widget_configs->sleeptimer), widget_configs->sleepMenu);
	gtk_menu_shell_append(GTK_MENU_SHELL(widget_configs->sleepMenu), widget_configs->one);
	gtk_menu_shell_append(GTK_MENU_SHELL(widget_configs->sleepMenu), widget_configs->five);
	gtk_menu_shell_append(GTK_MENU_SHELL(widget_configs->sleepMenu), widget_configs->ten);
	gtk_menu_shell_append(GTK_MENU_SHELL(widget_configs->sleepMenu), widget_configs->fifteen);
	gtk_menu_shell_append(GTK_MENU_SHELL(widget_configs->sleepMenu), widget_configs->disable);
	gtk_menu_shell_append(GTK_MENU_SHELL(widget_configs->toolMenu), widget_configs->darkmode);
	gtk_menu_shell_append(GTK_MENU_SHELL(widget_configs->menubar), widget_configs->toolMb);

//	gtk_menu_item_set_submenu(GTK_MENU_ITEM(widget_configs->helpMb), widget_configs->helpMenu);
//	gtk_menu_shell_append(GTK_MENU_SHELL(widget_configs->helpMenu), widget_configs->aboutMb);
//	gtk_menu_shell_append(GTK_MENU_SHELL(widget_configs->helpMenu), widget_configs->playlistMb);
//	gtk_menu_shell_append(GTK_MENU_SHELL(widget_configs->menubar), widget_configs->helpMb);

	gtk_box_pack_start(GTK_BOX(widget_configs->main_box), widget_configs->menubar, FALSE, FALSE, 0);

	// Pop-up Menu
	gtk_widget_show(widget_configs->play_pausePu);
	gtk_widget_show(widget_configs->stopPu);
	gtk_widget_show(widget_configs->sepA);
	gtk_widget_show(widget_configs->hideMi);
	gtk_widget_show(widget_configs->quitPu);
	gtk_widget_show(widget_configs->sepB);
	gtk_widget_show(widget_configs->seek_forPu);
	gtk_widget_show(widget_configs->seek_backPu);
	gtk_widget_show(widget_configs->nextPu);
	gtk_widget_show(widget_configs->previousPu);
	gtk_menu_shell_append(GTK_MENU_SHELL(widget_configs->pmenu), widget_configs->play_pausePu);
	gtk_menu_shell_append(GTK_MENU_SHELL(widget_configs->pmenu), widget_configs->stopPu);
	gtk_menu_shell_append(GTK_MENU_SHELL(widget_configs->pmenu), widget_configs->sepA);
	gtk_menu_shell_append(GTK_MENU_SHELL(widget_configs->pmenu), widget_configs->seek_forPu);
	gtk_menu_shell_append(GTK_MENU_SHELL(widget_configs->pmenu), widget_configs->seek_backPu);
	gtk_menu_shell_append(GTK_MENU_SHELL(widget_configs->pmenu), widget_configs->nextPu);
	gtk_menu_shell_append(GTK_MENU_SHELL(widget_configs->pmenu), widget_configs->previousPu);
	gtk_menu_shell_append(GTK_MENU_SHELL(widget_configs->pmenu), widget_configs->sepB);
	gtk_menu_shell_append(GTK_MENU_SHELL(widget_configs->pmenu), widget_configs->hideMi);
	gtk_menu_shell_append(GTK_MENU_SHELL(widget_configs->pmenu), widget_configs->quitPu);

	//Control Containers

	//Control box 1 configuration
	GtkWidget* controls_0 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_pack_start (GTK_BOX (controls_0), widget_configs->current_time, FALSE, FALSE, 2);
	gtk_box_pack_start (GTK_BOX (controls_0), widget_configs->seek_slider, TRUE, TRUE, 2);
	gtk_box_pack_start (GTK_BOX (controls_0), widget_configs->total_time, FALSE, FALSE, 2);

	GtkWidget* controls_middle = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_pack_start(GTK_BOX(controls_middle), widget_configs->view_playlist, FALSE, FALSE, 2);
	gtk_box_pack_start (GTK_BOX (controls_middle), widget_configs->repeat, FALSE, FALSE, 2);
	gtk_box_pack_start (GTK_BOX (controls_middle), widget_configs->prev, FALSE, FALSE, 2);
	gtk_box_pack_start (GTK_BOX (controls_middle), widget_configs->play, FALSE, FALSE, 2);
	gtk_box_pack_start (GTK_BOX (controls_middle), widget_configs->next, FALSE, FALSE, 2);
	gtk_box_pack_start (GTK_BOX (controls_middle), widget_configs->shuffle, FALSE, FALSE, 2);
	gtk_box_pack_start(GTK_BOX(controls_middle), widget_configs->rewind, FALSE, FALSE, 2);


	GtkWidget *controls_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	// Create a box widget to center the controls_middle widget
	GtkWidget *center_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_pack_start(GTK_BOX(center_box), controls_middle, FALSE, FALSE, 0);

	// Center the center_box widget horizontally and vertically
	gtk_widget_set_halign(center_box, GTK_ALIGN_CENTER);
	gtk_widget_set_valign(center_box, GTK_ALIGN_CENTER);

	gtk_box_pack_start(GTK_BOX(controls_box), widget_configs->meta_data_display, FALSE, FALSE, 2);
	gtk_box_pack_start(GTK_BOX(controls_box), widget_configs->favorite, FALSE, FALSE, 2);
	gtk_box_pack_start(GTK_BOX(controls_box), widget_configs->about, FALSE, FALSE, 2);
	gtk_box_pack_start(GTK_BOX(controls_box), center_box, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(controls_box), widget_configs->mute, FALSE, FALSE, 2);
	gtk_box_pack_start(GTK_BOX(controls_box), widget_configs->volume_slider, FALSE, FALSE, 2);
	gtk_box_pack_end(GTK_BOX(controls_box), widget_configs->help_button, FALSE, FALSE, 2);


	gtk_box_pack_start (GTK_BOX (widget_configs->control_box_1), controls_0, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (widget_configs->control_box_1), controls_box, FALSE, FALSE, 0);

	// Now containing all subs containers which contains the controls
	gtk_box_pack_start (GTK_BOX (widget_configs->main_box), widget_configs->main_hbox, TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (widget_configs->main_box), widget_configs->control_box_1, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (widget_configs->main_box), widget_configs->control_box_2, FALSE, FALSE, 0);

	// Add the container to the window
	gtk_container_add(GTK_CONTAINER(widget_configs->ebox), widget_configs->video_window);

	g_signal_connect(widget_configs->video_window, "realize", G_CALLBACK (realize_window), status);
}

/*
 *  fun: This one is our main_loop which creates a thread for our gstreamer pipeline and draws all our widgets in the screen.
 *
 *  arg1: contains the playlist extracted from the caller
 * */


void gtkgui::main_loop(std::vector<std::string>& playlist)
{

	status->playlist_vector = playlist;
	status->play_pointer = status->playlist_vector.begin();
	create_gui();

	//checking whether the media is playable

	if (
			(*status->playlist_vector.begin()).find(".mp3") != std::string::npos ||
			(*status->playlist_vector.begin()).find(".wav") != std::string::npos ||
			(*status->playlist_vector.begin()).find(".aiff") != std::string::npos ||
			(*status->playlist_vector.begin()).find(".flac") != std::string::npos ||
			(*status->playlist_vector.begin()).find(".aac") != std::string::npos ||
			(*status->playlist_vector.begin()).find(".ogg") != std::string::npos ||
			(*status->playlist_vector.begin()).find(".wma") != std::string::npos ||
			(*status->playlist_vector.begin()).find(".mp4a") != std::string::npos ||
			(*status->playlist_vector.begin()).find(".ac3") != std::string::npos ||
			(*status->playlist_vector.begin()).find(".webm") != std::string::npos ||
			(*status->playlist_vector.begin()).find(".amr") != std::string::npos

	) {
		status->video_flag = FALSE;
	} else {
		status->video_flag = TRUE;
	}

	// GUI Widget Signal Handlers
	g_signal_connect (G_OBJECT (widget_configs->play), "clicked", G_CALLBACK (start), status);
	g_signal_connect (G_OBJECT (widget_configs->stop), "clicked", G_CALLBACK (stop), status);
	g_signal_connect (G_OBJECT (widget_configs->next), "clicked", G_CALLBACK (play_next_song), status);
	g_signal_connect (G_OBJECT (widget_configs->prev), "clicked", G_CALLBACK (play_previous_song), status);
	g_signal_connect (G_OBJECT (widget_configs->repeat), "toggled", G_CALLBACK (repeat_cb), status);
	g_signal_connect (G_OBJECT (widget_configs->shuffle), "toggled", G_CALLBACK (shuffle_cb), status);
	g_signal_connect (G_OBJECT (widget_configs->seek_back), "clicked", G_CALLBACK (seek_back_5_cb), status->pipeline);
	g_signal_connect (G_OBJECT (widget_configs->seek_for), "clicked", G_CALLBACK (seek_forward_5_cb), status->pipeline);
	g_signal_connect(widget_configs->volume_slider, "value-changed", G_CALLBACK(volume_scale_changed), status);
	g_signal_connect(G_OBJECT(widget_configs->meta_data_display), "toggled", G_CALLBACK(metawindow),status);
	g_signal_connect(G_OBJECT(widget_configs->view_playlist), "toggled", G_CALLBACK(on_playlist_button_clicked), status);
	g_signal_connect(widget_configs->open_dir, "clicked", G_CALLBACK(open_directory_cb), status);
	g_signal_connect(G_OBJECT(widget_configs->about), "toggled", G_CALLBACK(about_cb),status);
	g_signal_connect(G_OBJECT(widget_configs->mute), "toggled", G_CALLBACK(mute_cb), status);
	g_signal_connect(G_OBJECT(widget_configs->favorite), "toggled", G_CALLBACK(fav_lists), status);
	g_signal_connect(G_OBJECT(widget_configs->help_button), "toggled", G_CALLBACK(on_help_button_toggled), status);
	g_signal_connect(G_OBJECT(widget_configs->rewind), "toggled", G_CALLBACK(Rewind_cb),status);
	// Connect to the key-press-event signal
	g_signal_connect(G_OBJECT(widget_configs->main_window), "key-press-event", G_CALLBACK(on_key_press), status);

	// Menubar
	g_signal_connect(G_OBJECT(widget_configs->quitMb), "activate", G_CALLBACK(gtk_main_quit), nullptr);
//	g_signal_connect (G_OBJECT (widget_configs->playMb), "activate", G_CALLBACK (start), status);
	g_signal_connect (G_OBJECT (widget_configs->stopMb), "activate", G_CALLBACK (stop), status);
	g_signal_connect (G_OBJECT (widget_configs->play_pauseMb), "activate", G_CALLBACK (play_pause_cb), status);
	g_signal_connect (G_OBJECT (widget_configs->seek_backMb), "activate", G_CALLBACK (seek_back_5_cb), status->pipeline);
	g_signal_connect (G_OBJECT (widget_configs->seek_forMb), "activate", G_CALLBACK (seek_forward_5_cb), status->pipeline);
	g_signal_connect (G_OBJECT (widget_configs->seek_toMb), "activate", G_CALLBACK (seek_to_specific_time), status);
	g_signal_connect (G_OBJECT (widget_configs->repeatMb), "activate", G_CALLBACK (repeat_in_menu), status);
//	g_signal_connect (G_OBJECT (widget_configs->shuffleMb), "activate", G_CALLBACK (shuffle_in_menu), status);

	g_signal_connect(widget_configs->openDirMb, "activate", G_CALLBACK(open_directory_cb), status);
	g_signal_connect(widget_configs->openFileMb, "activate", G_CALLBACK(play_selected_file), status);

	g_signal_connect(G_OBJECT(widget_configs->one), "activate", G_CALLBACK(timings), NULL);
	g_signal_connect(G_OBJECT(widget_configs->five), "activate", G_CALLBACK(timings), NULL);
	g_signal_connect(G_OBJECT(widget_configs->ten), "activate", G_CALLBACK(timings), NULL);
	g_signal_connect(G_OBJECT(widget_configs->fifteen), "activate", G_CALLBACK(timings), NULL);
	g_signal_connect(G_OBJECT(widget_configs->disable), "activate", G_CALLBACK(timings), NULL);

	g_signal_connect(G_OBJECT(widget_configs->darkmode), "activate", G_CALLBACK(on_dark_mode), NULL);

	// Pop-up Menu
	g_signal_connect_swapped(G_OBJECT(widget_configs->hideMi), "activate", G_CALLBACK(gtk_window_iconify), GTK_WINDOW(widget_configs->main_window));
	g_signal_connect(G_OBJECT(widget_configs->quitPu), "activate", G_CALLBACK(gtk_main_quit), nullptr);
	g_signal_connect_swapped(G_OBJECT(widget_configs->ebox), "button-press-event", G_CALLBACK(show_popup), widget_configs->pmenu);
	g_signal_connect (G_OBJECT (widget_configs->stopPu), "activate", G_CALLBACK (stop), status);
	g_signal_connect (G_OBJECT (widget_configs->play_pausePu), "activate", G_CALLBACK (play_pause_cb), status);
	g_signal_connect (G_OBJECT (widget_configs->seek_backPu), "activate", G_CALLBACK (seek_back_5_cb), status->pipeline);
	g_signal_connect (G_OBJECT (widget_configs->seek_forPu), "activate", G_CALLBACK (seek_forward_5_cb), status->pipeline);
	g_signal_connect (G_OBJECT (widget_configs->nextPu), "activate", G_CALLBACK (play_next_song), status);
	g_signal_connect (G_OBJECT (widget_configs->previousPu), "activate", G_CALLBACK (play_previous_song), status);


	// Window Signals
	g_signal_connect(widget_configs->main_window, "destroy", G_CALLBACK (gtk_main_quit), nullptr);

	g_signal_connect(G_OBJECT (widget_configs->video_window), "draw", G_CALLBACK (draw_window), status);

	g_signal_connect(status->pipeline, "child-added", G_CALLBACK (separate_thread), &status);

	// Showing widgets
	gtk_widget_show_all(widget_configs->main_window);

	//separation of gstreamer thread
	status->thread = g_thread_new("gtk_thread", (GThreadFunc)pipeline_init, this);

	status->seek_slider=widget_configs->seek_slider;
	status->ref_current=widget_configs->current_time;
	status->ref_duration = widget_configs->total_time;

	status->temp_range=widget_configs->volume_slider;
	status->temp_mute=widget_configs->mute;
	status->temp_fav=widget_configs->favorite;
	status->temp_main_window = widget_configs->main_window;

	status->repeatMb = widget_configs->repeatMb;
	status->repeatButton = widget_configs->repeat;
	status->play_pauseMb = widget_configs->play_pauseMb;
	status->play_pauseButton = widget_configs->play;
	status->darkmode = widget_configs->darkmode;

	status->rewind = widget_configs->rewind;

//	GstElement* audio_bin = gst_bin_get_by_name(GST_BIN(status->pipeline), "audio_bin");
//	GstElement* audio_volume = gst_bin_get_by_name(GST_BIN(audio_bin), "audio-volume");
//	g_object_get(G_OBJECT(audio_volume), "volume", &(status->temp_vol), NULL);

	// Connect the "value-changed" signal to update the playback position
	status->slider_update_signal_id=g_signal_connect(widget_configs->seek_slider, "value-changed", G_CALLBACK(on_seek_slider_changed), status);
	g_timeout_add_seconds(1, (GSourceFunc)refresh_slider_position, status);

	if (!status->thread)
	{
		g_printerr("GTK_MAIN thread failed");
	}
	gtk_main();
}


/*
 * fun: This function is invoked as separate thread to initiate the gstreamer pipeline
 *
 * arg1: gtkgui (a instance of the gtkgui class is passed)
 *
 * */

void gtkgui::pipeline_init(gtkgui* gui_inst)
{
	//	g_print("The pipeline is %s", GST_ELEMENT_NAME((*app_stat)->pipeline));
	mediapipe *pipeline = new mediapipe();
	pipeline->play(gui_inst->widgets_getter(), gui_inst->app_state_getter());
	delete(pipeline);
}

/*
 * fun: This function is invoked as soon as any child is added to the gstreamer pipeline
 *
 * arg1: GstChildProxy
 * arg2: GObject (reference to the child)
 * arg3: gchar * (Name of the child added)
 * arg4: app_state** (reference to out application)
 *
 * */



void gtkgui::separate_thread(GstChildProxy *childproxy, GObject *child, gchar *name, app_state** data)
{
	if (GST_IS_BIN(child) && (static_cast<std::string>(name) == "video_bin")) {

		GstElement* sink= gst_bin_get_by_name(GST_BIN(GST_ELEMENT(child)),"sink");
		g_print("The Element is: %s \n", GST_ELEMENT_NAME(sink));

		(*data)->overlay = GST_VIDEO_OVERLAY(sink);

		gst_video_overlay_set_window_handle((*data)->overlay, (*data)->window_handle);
	}
}


/*
 * fun: This sets up the overlay from our pipeline and connects with the window handlers
 *
 * arg1: GtkWidget
 * arg2: Instance of our gtkgui class
 * */

void gtkgui::realize_window(GtkWidget* video, app_state* data)
{
	if (!GTK_IS_WIDGET(video))
	{
		g_print("Widget not found! \n");
		return;
	}
	data->gd_window = gtk_widget_get_window (video);

	if (!gdk_window_ensure_native (data->gd_window))
		g_error ("Couldn't create native window needed for GstVideoOverlay!");

	/* Retrieve window handler from GDK */
#if defined (GDK_WINDOWING_WIN32)
	data->window_handle = (guintptr)GDK_WINDOW_HWND (gd_window);
#elif defined (GDK_WINDOWING_QUARTZ)
	data->window_handle = gdk_quartz_window_get_nsview (gd_window);
#elif defined (GDK_WINDOWING_X11)
	data->window_handle = GDK_WINDOW_XID (data->gd_window);
#endif
}


/*
 * fun: This executes when our GUI has nothing to draw in the video window
 *
 * arg1: GtkWidget
 * arg2: cairo_t
 * arg3: Instance of our gtkgui class
 * */
gboolean gtkgui::draw_window(GtkWidget* widget, cairo_t* cr, app_state* data)
{
	if (data->pipeline_state < GST_STATE_PAUSED)
	{
		GtkAllocation allocation;
		/* Cairo is a 2D graphics library which we use here to clean the video window.
		 * It is used by GStreamer for other reasons, so it will always be available to us. */
		gtk_widget_get_allocation (widget, &allocation);
		cairo_set_source_rgb (cr, 0, 0, 0);
		cairo_rectangle (cr, 0, 0, allocation.width, allocation.height);
		cairo_fill (cr);
	}
	return FALSE;
}
/*
 * fun: This gets the instance to our application status
 *
 *
 * */
app_state* gtkgui::app_state_getter()
{
	return status;
}

/*
 * fun: This gets the instance to our widgets
 *
 *
 * */
gui_elem* gtkgui::widgets_getter()
{
	return widget_configs;
}
