#include <iostream>
#include <bits/stdc++.h>
#include <gtk/gtk.h>
#include <gst/gst.h>
#include <vector>
#include <string>
#include <random>
#include <chrono>
#include <algorithm>

#include "../header/playback.h"
#include "../header/custom_data.h"
#include "../header/application_state.h"
#include "../header/gtk_gui.h"

using namespace std;

/*
 * fun:start(pstarts the pipeline to playing state)
 *
 * arg1:GstElement pipeline
 *
 */


//setting the toggle button when help window is closed
gboolean on_windows_closed(GtkWidget* widget, GdkEvent* event, gpointer data)
{
	GtkToggleButton* button = GTK_TOGGLE_BUTTON(data);
	gtk_toggle_button_set_active(button, FALSE);
	return FALSE;
}

void start(GtkWidget* widget, app_state* status)
{
	GstState state;
	gst_element_get_state(status->pipeline, &state, NULL, GST_CLOCK_TIME_NONE);

	if (state == GST_STATE_PLAYING) {
		// Pause the playbin and display the play icon
		gst_element_set_state(status->pipeline, GST_STATE_PAUSED);
		GtkWidget *play_icon = gtk_image_new_from_icon_name("media-playback-start",
				GTK_ICON_SIZE_BUTTON);
		gtk_button_set_image(GTK_BUTTON(widget), play_icon);
	    gtk_menu_item_set_label(GTK_MENU_ITEM(status->play_pauseMb), "Play");
	} else {
		// Start playing the playbin and display the pause icon
		gst_element_set_state(status->pipeline, GST_STATE_PLAYING);
		GtkWidget *pause_icon = gtk_image_new_from_icon_name("media-playback-pause",
				GTK_ICON_SIZE_BUTTON);
		gtk_button_set_image(GTK_BUTTON(widget), pause_icon);
	    gtk_menu_item_set_label(GTK_MENU_ITEM(status->play_pauseMb), "Pause");
	}
}

/*
 * fun:pause(pauses the pipeline from playing state)
 *
 * arg1:GstElement pipeline
 *
 */
void halt(GtkWidget* widget, app_state* status)
{
	if(status->pipeline_state == GST_STATE_READY || status->pipeline_state == GST_STATE_NULL)
	{
		return;
	}
	g_print("Pausing.........");
	gst_element_set_state (status->pipeline, GST_STATE_PAUSED);
}

/*
 * fun:resume(resumes the pipeline)
 *
 * arg1:GstElement pipeline
 *
 */

void resume(GtkWidget* widget, GstElement *pipeline)
{
	g_print("Playing.........");
	gst_element_set_state (pipeline, GST_STATE_PLAYING);
}

/*
 * fun:stop
 *
 * arg1:GstElement pipeline
 *
 */

void stop(GtkWidget* widget, app_state* data)
{
	g_print("Stream Stopped! \n");

	gtk_range_set_value(GTK_RANGE(data->temp_range), (gdouble)(1.0));
	GtkWidget *play_icon = gtk_image_new_from_icon_name("media-playback-start",
					GTK_ICON_SIZE_BUTTON);
	gtk_button_set_image(GTK_BUTTON(data->play_pauseButton), play_icon);

	gst_element_set_state (data->pipeline, GST_STATE_READY);

	gtk_widget_queue_draw(data->video_window);
}

gint64 duration(GstElement *pipeline)
{
	gint64 duration;
	GstStateChangeReturn ret = gst_element_set_state(pipeline, GST_STATE_PAUSED);
	if (ret == GST_STATE_CHANGE_FAILURE)
	{
		g_printerr("Failed to set pipeline to paused state.\n");
		exit(0);
	}
	// Wait until the pipeline is prerolled(buffering)
	ret = gst_element_get_state(pipeline, NULL, NULL, GST_CLOCK_TIME_NONE);
	if (ret == GST_STATE_CHANGE_FAILURE)
	{
		g_printerr("Failed to preroll the pipeline.\n");
		exit(0);
	}
	//checking the duration of the file
	if(!gst_element_query_duration (pipeline, GST_FORMAT_TIME, &duration))
	{
		cout<<"unable to query duration "<<endl;
	}
	return duration;
}

/*
 * fun: Repeating the currently playing song once
 *
 * arg1: GtkButton
 * arg2: app_state *data(structure containing elements)
 *
 */

void repeat_cb(GtkButton *button, app_state *data) {
	if(data->repeat == FALSE){
		gtk_button_set_image(GTK_BUTTON(button), gtk_image_new_from_icon_name("media-playlist-repeat-one", GTK_ICON_SIZE_BUTTON));
		gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(data->repeatMb), TRUE);
		data->repeat = TRUE;
	}
	else {
		gtk_button_set_image(GTK_BUTTON(button), gtk_image_new_from_icon_name("media-playlist-repeat", GTK_ICON_SIZE_BUTTON));
		gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(data->repeatMb), FALSE);
		data->repeat = FALSE;
	}
}

void repeat_in_menu (GtkWidget* widget, app_state *data) {
	if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget))) {
 		gtk_button_set_image(GTK_BUTTON(data->repeatButton), gtk_image_new_from_icon_name("media-playlist-repeat-one", GTK_ICON_SIZE_BUTTON));
		 data->repeat = TRUE;
		  gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(widget), TRUE);
	  } else {
	 		gtk_button_set_image(GTK_BUTTON(data->repeatButton), gtk_image_new_from_icon_name("media-playlist-repeat", GTK_ICON_SIZE_BUTTON));
		  data->repeat = FALSE;
		  gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(widget), FALSE);
	  }
}
/*
 * fun: Shuffling the song in the playist
 *
 * arg1: GstElement pipeline
 * arg2: vector containing paths of the media
 *
 */
void shuffle_cb(GtkToggleButton* button, app_state *data)
{
	if (gtk_toggle_button_get_active(button))
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		gen.seed(std::chrono::system_clock::now().time_since_epoch().count());

		auto value = *(data->play_pointer);

		data->shuffled_playlist_vector.assign(data->playlist_vector.begin(), data->playlist_vector.end());
		std::shuffle(data->playlist_vector.begin(), data->playlist_vector.end(), gen);

		data->play_pointer = data->playlist_vector.begin();

		auto finder = std::find(data->playlist_vector.begin(), data->playlist_vector.end(), value);

		int index = std::distance(data->playlist_vector.begin(), finder);

		std::advance(data->play_pointer, index);

	}
	else
	{
		auto value = *data->play_pointer;

		data->playlist_vector.assign(data->shuffled_playlist_vector.begin(), data->shuffled_playlist_vector.end());

		auto finder = std::find(data->playlist_vector.begin(), data->playlist_vector.end(), value);

		int index = std::distance(data->playlist_vector.begin(), finder);

		data->play_pointer = data->playlist_vector.begin();

		std::advance(data->play_pointer, index);
	}
}

// Callback function for the volume scale
/*
 * fun: Volume callback function
 *
 * arg1: Gtk Range
 * arg2: GstElement Pipeline
 *
 */
void volume_scale_changed(GtkRange *range, app_state *status)
{
	// retrieve the audio-volume element
	GstElement* audio_bin = gst_bin_get_by_name(GST_BIN(status->pipeline), "audio_bin");
	GstElement* audio_volume = gst_bin_get_by_name(GST_BIN(audio_bin), "audio-volume");

	gdouble value = gtk_range_get_value(range);
	g_object_set(G_OBJECT(audio_volume), "volume", value, NULL);
	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(status->temp_mute))&&value>=0.1)
	{
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(status->temp_mute), FALSE);
	}
}

/*
 * fun: Seek Backward for 5 second
 *
 * arg1: Gtk Widget
 * arg2: GstElement Pipeline
 *
 */
void seek_back_5_cb (GtkWidget* widget, GstElement* pipeline)
{
	gint64 current = -1;
	/* Query the current position of the stream */
	if (!gst_element_query_position (pipeline, GST_FORMAT_TIME, &current))
	{
		g_printerr ("Could not query current position.\n");
	}

	/* Converting the current time to seconds */
	float res=(float)(current/GST_SECOND);

	/* Corner case for backward seek */
	if (res < 5)
	{
		res = 5;
	}

	/* Seek simple funtion */
	gst_element_seek_simple (pipeline, GST_FORMAT_TIME,
			static_cast<GstSeekFlags>(GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_KEY_UNIT), (res - 5) * GST_SECOND);

	cout << "seeking back 5 secs" << endl;
	gst_element_set_state (pipeline, GST_STATE_PLAYING);
}

/*
 * fun: Seek Forward for 5 second
 *
 * arg1: Gtk Widget
 * arg2: GstElement Pipeline
 * arg3: gboolean terminate
 *
 */
void seek_forward_5_cb (GtkWidget* widget,GstElement *pipeline, gboolean &terminate)
{
	gint64 current = -1;
	gint64 Duration=0;
	Duration = duration(pipeline);

	/* Query the current position of the stream */
	if (!gst_element_query_position (pipeline, GST_FORMAT_TIME, &current))
	{
		g_printerr ("Could not query current position.\n");
	}

	/* Converting the current time to seconds */
	float res=(float)(current/GST_SECOND);

	/* Corner case for forward seek */
	if (((Duration - current)/GST_SECOND) < 5)
	{
		res = (float)(Duration/GST_SECOND) - 6;
		terminate = TRUE;
	}

	/* Seek simple funtion */
	gst_element_seek_simple (pipeline, GST_FORMAT_TIME,
			static_cast<GstSeekFlags>(GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_KEY_UNIT), (res + 5) * GST_SECOND);

	cout << "seeking forward 5 secs" << endl;
	gst_element_set_state (pipeline, GST_STATE_PLAYING);
}

/*
 * fun:opening the directory and loading all the playable media into a vector
 *
 * arg1:Gtk Buttonn
 * arg2:app_state object which contains the vector
 */
void open_directory_cb(GtkButton *button,app_state *status)
{
	GDir *dir;
	gint response;
	//status->playlist_vector.clear();
	const gchar *filename;
	gchar *file_path;
	gchar *dir_path;
	// Create a new file chooser dialog
	GtkWidget *dialog = gtk_file_chooser_dialog_new("Open File",NULL,
			GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
			"_Cancel", GTK_RESPONSE_CANCEL,
			"_Open", GTK_RESPONSE_ACCEPT,
			NULL);

	// Show the file chooser dialog and wait for a response
	response = gtk_dialog_run(GTK_DIALOG(dialog));

	std::string str=*status->play_pointer;

	if (response == GTK_RESPONSE_ACCEPT)
	{
		// Get the selected file path
		dir_path = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
		dir = g_dir_open(dir_path, 0, NULL);
		if (dir)
		{
			while ((filename = g_dir_read_name(dir)) != NULL)
			{
				if (g_str_has_suffix(filename, ".mp3") ||
						g_str_has_suffix(filename, ".mp4") ||
						g_str_has_suffix(filename, ".avi") ||
						g_str_has_suffix(filename, ".mkv") ||
						g_str_has_suffix(filename, ".webm"))
				{
					file_path=g_build_filename(dir_path, filename, NULL);

					auto finder = std::find(status->playlist_vector.begin(), status->playlist_vector.end(), std::string(file_path));

					if (finder != status->playlist_vector.end())
					{
						continue;
					}

					status->playlist_vector.push_back(std::string(file_path));
					std::cout<<endl;
				}
			}
			g_dir_close(dir);
		}
	}
	auto address = std::find(status->playlist_vector.begin(), status->playlist_vector.end(), str);
	status->play_pointer = address;
	gtk_widget_destroy(dialog);
}

/*
 * fun:function to seek the pipeline when the slider is moved by the user
 *
 * arg1:GstWidget
 * arg2:app_state *data(structure containing elements)
 *
 */

void on_seek_slider_changed(GtkWidget *widget,app_state *status)
{
	//GstElement *playbin = (GstElement *) data;
	gdouble value = gtk_range_get_value (GTK_RANGE (widget));
	gst_element_seek_simple (status->pipeline, GST_FORMAT_TIME, (GstSeekFlags)(GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_KEY_UNIT),(gint64)(value * GST_SECOND));
}

/*
 * fun:function to update the slider position when the pipeline is playning and gets updated every second
 *
 * arg1:GstWidget
 * arg2:app_state *data(structure containing elements)
 *
 */
gboolean refresh_slider_position(app_state *status)
{
	GstState state,pending;
	GstClockTime timeout = 1 * GST_SECOND;
	gst_element_get_state(status->pipeline, &state, &pending, timeout);
	if (state == GST_STATE_PLAYING)
	{
		gint64 current,duration;
		/* Query the current position and duration of the pipeline */
		if (gst_element_query_position(status->pipeline, GST_FORMAT_TIME, &current) && gst_element_query_duration(status->pipeline, GST_FORMAT_TIME, &duration)) {
			/* Convert the position and duration to seconds */
			gint current_sec = current / GST_SECOND;
			gint duration_sec = duration / GST_SECOND;
			gchar *time_str = g_strdup_printf("%02d:%02d:%02d",(gint)(current_sec/ 3600),(gint)(current_sec / 60 % 60),(gint)(current_sec % 60));
			gchar *duration_str = g_strdup_printf("%02d:%02d:%02d ",(gint)(duration_sec/ 3600),(gint)(duration_sec / 60 % 60),(gint)(duration_sec % 60));
			gtk_label_set_text(GTK_LABEL(status->ref_current), time_str);
			gtk_label_set_text(GTK_LABEL(status->ref_duration), duration_str);
			//blocking the slider callback signal
			g_signal_handler_block (status->seek_slider, status->slider_update_signal_id);
			//setting the current position of the slider
			gtk_range_set_value(GTK_RANGE(status->seek_slider), (gdouble) current / GST_SECOND);
			/* Update the slider range */
			gtk_range_set_range(GTK_RANGE(status->seek_slider), 0, duration_sec);
			//unblocking slider call back signal
			g_signal_handler_unblock (status->seek_slider, status->slider_update_signal_id);
		}
	}
	return TRUE;
}

/*
 * fun: Print PlayList
 *
 * arg1: Gtk Buffer
 * arg2:app_state *data(structure containing elements)
 *
 */


void print_playlist(GtkTextBuffer *buffer, app_state** data )
{
	stringstream ss;
	for (const auto& i : (*data)->playlist_vector)
	{
		size_t pos = i.find_last_of("/\\");
		bool is_playing = (i == (*(*data)->play_pointer));
		bool is_favorite = (std::find((*data)->fav_list_vector.begin(), (*data)->fav_list_vector.end(), i) != (*data)->fav_list_vector.end());
		ss << i.substr(pos+1) << (is_playing ? "🎵" : "") << (is_favorite ? "❤️" : "") <<"\n";
	}
	// Print the contents of the stringstream to the text buffer with markup
	gtk_text_buffer_set_text(buffer, ss.str().c_str(), -1);
}

/*
 * fun: PlayList callback function
 *
 * arg1: Gtk Buffer
 * arg2:app_state *data(structure containing elements)
 *
 */
void on_playlist_button_clicked(GtkToggleButton* button, app_state *data)
{
	// Get the main window from the user data
	static GtkWidget* playlist_window = NULL;

	if (gtk_toggle_button_get_active(button))
	{
		// Create a new window to display the playlist

		playlist_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
		gtk_window_set_title(GTK_WINDOW(playlist_window), "Playlist");
		gtk_container_set_border_width(GTK_CONTAINER(playlist_window), 10);
		gtk_window_set_default_size(GTK_WINDOW(playlist_window), 300, 300);
		g_signal_connect(G_OBJECT(playlist_window), "delete-event", G_CALLBACK(on_windows_closed), button);

		// Create a new text view widget to display the playlist
		GtkWidget *playlist_text_view = gtk_text_view_new();
		gtk_text_view_set_editable(GTK_TEXT_VIEW(playlist_text_view), FALSE);
		gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(playlist_text_view), FALSE);
		// Add the text view widget to a scrolled window
		GtkWidget *playlist_scroll_window = gtk_scrolled_window_new(NULL, NULL);
		gtk_container_add(GTK_CONTAINER(playlist_scroll_window), playlist_text_view);
		gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(playlist_scroll_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
		// Add the scrolled window to the playlist window
		gtk_container_add(GTK_CONTAINER(playlist_window), playlist_scroll_window);
		// Show all the widgets in the playlist window
		gtk_widget_show_all(playlist_window);
		// Get the text buffer associated with the text view widget
		GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(playlist_text_view));
		print_playlist(buffer,&data);
	}
	else
	{
		// If the button is no longer active, destroy the metadata window
		gtk_widget_destroy(playlist_window);
	}
}

/*
 * fun: Print About
 *
 * arg1: Gtk Button
 * arg2:app_state *data(structure containing elements)
 *
 */
void about_cb(GtkToggleButton *button, app_state *data)
{
	static GtkWidget* about_window = NULL;
		if (gtk_toggle_button_get_active(button)) {
			// Create a new window to display the metadata
			about_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
			gtk_window_set_title(GTK_WINDOW(about_window), "ABOUT");
			gtk_container_set_border_width(GTK_CONTAINER(about_window), 10);
			gtk_window_set_default_size(GTK_WINDOW(about_window), 300, 300);
			g_signal_connect(G_OBJECT(about_window), "delete-event", G_CALLBACK(on_windows_closed), button);
			// Create a new text view widget to display the metadata
			GtkTextBuffer *text_buffer = gtk_text_buffer_new(NULL);
			GtkWidget *text_view = gtk_text_view_new_with_buffer(text_buffer);
			gtk_container_add(GTK_CONTAINER(about_window), text_view);



			GtkTextTag *bold_tag ,*size_tag;
			bold_tag = gtk_text_tag_new("bold");
			if (bold_tag) {
				PangoWeight weight = PANGO_WEIGHT_BOLD;
				g_object_set(G_OBJECT(bold_tag), "weight", weight, NULL);
			}
			size_tag = gtk_text_tag_new("size");
			if (size_tag) {
				int font_size = 14;
				g_object_set(G_OBJECT(size_tag), "size", font_size * PANGO_SCALE, NULL);
			}
			GtkTextTagTable *tag_table =gtk_text_buffer_get_tag_table(text_buffer);
			gtk_text_tag_table_add(tag_table,bold_tag);
			gtk_text_tag_table_add(tag_table, size_tag);
			// Insert some bold text into the buffer
			GtkTextIter start_iter, end_iter;
			gtk_text_buffer_get_bounds(text_buffer, &start_iter, &end_iter);gtk_text_buffer_insert_with_tags_by_name(text_buffer, &start_iter, "🎵🎶 AMPLIFY MEDIA PLAYER 🎶🎵 \n\n", -1, "bold","size", NULL);
			gtk_text_buffer_get_bounds(text_buffer, &start_iter, &end_iter);gtk_text_buffer_insert_with_tags_by_name(text_buffer, &start_iter, "Version 2.0\n", -1, "bold","size", NULL);


		std::stringstream ss;
		ss << "🎙️TEAM-8\t" << "Coach\t" << "Sanchit Gharat\n\n";
		ss << "  Emp ID\t" << "Employee Name\t" << "Work Contribution\n";
		ss << "🎸212507\t" << "Soujanya Kalal\t" << "\tIntegrating Features\n";
		ss << "🎺212522\t" << "Vedavathi B Shriyan\t"<< "Features\n";
		ss << "🎹212531\t" << "Girish R\t"<< "\t\tFeatures\n";
		ss << "🎼212576\t" << "K Sumanth\t"<< "\t\tFeatures\n";
		ss << "🎻212778\t" << "Bijoy Das\t" << "\t\tIntegrating Features\n";
		ss << "💽213120\t" << "Vikas D\t"<< "\t\t\tFeatures\n";
		ss << "🥁213082\t" << "Jalaja D G\t"<< "\t\tFeatures\n";
		ss << "🎙️213040\t" << "Yashas Pathange R\t"<< "Features\n";
		ss << "🎛️212843\t" << "Pratiksha Shetty\t"<< "Features\n";

		std::string text = ss.str();
		gtk_text_buffer_insert_at_cursor(text_buffer, text.c_str(), text.length());
		gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
		gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(text_view), FALSE);
		gtk_widget_show_all(about_window);
	}
	else
	{
		// Deactivate the button
		gtk_toggle_button_set_active(button, FALSE);
		// Destroy the help window if it exists
		if (about_window != NULL)
		{
			gtk_widget_destroy(about_window);
			about_window = NULL;
		}
	}
}

/*
 * fun: Print Help
 *
 * arg1: GtkToggle Button
 * arg2:app_state *data(structure containing elements)
 *
 */
void on_help_button_toggled(GtkToggleButton* button, app_state *data)
{
	static GtkWidget* help_window = NULL;
	if (gtk_toggle_button_get_active(button))
	{
		if (help_window == NULL) {
			stringstream ss;
			string t;
			// Create the help window
			help_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
			gtk_window_set_title(GTK_WINDOW(help_window), "Help");
			gtk_window_set_default_size(GTK_WINDOW(help_window), 500, 300);
			// Connect the signal handler to the "delete-event" signal/
			g_signal_connect(G_OBJECT(help_window), "delete-event", G_CALLBACK(on_windows_closed), button);
			// Add some content to the window
			// Create a new text view widget to display the playlist
			GtkWidget *playlist_text_view = gtk_text_view_new();
			gtk_text_view_set_editable(GTK_TEXT_VIEW(playlist_text_view), FALSE);
			gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(playlist_text_view), FALSE);
			// Add the text view widget to a scrolled window
			GtkWidget *playlist_scroll_window = gtk_scrolled_window_new(NULL, NULL);
			gtk_container_add(GTK_CONTAINER(playlist_scroll_window), playlist_text_view);
			gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(playlist_scroll_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
			// Add the scrolled window to the playlist window
			gtk_container_add(GTK_CONTAINER(help_window), playlist_scroll_window);
			ss<<"\t KEYBOARD FEATURES\n";
			ss<<"\tb    :Start Playing\n";
			ss<<"\tp    :Pause\n";
			ss<<"\tr    :Resume\n";
			ss<<"\ts    :Stop\n";
			ss<<"\t\\    :Advance\n";
			ss<<"\t|    :Previous\n";
			ss<<"\t,.    :Backward/forward 5 Seconds\n";
			ss<<"\t[]    :Backward/Forward 60 Seconds\n";
			ss<<"\t{}    :Backward/Forward 10% of Media\n";
			ss<<"\tz    :Rewind\n";
			ss<<"\t+    :Increase Volume\n";
			ss<<"\t-    :Decrease Volume\n";
			ss<<"\tQ    :Quit\n";
			ss<<"\th    :Help/Exit\n";
			ss<<"\td    :View Play List\n";
			//Get the text buffer associated with the text view widget
			GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(playlist_text_view));
			t = ss.str();
			gtk_text_buffer_set_text(buffer,t.c_str(),-1);
			// Show all the widgets in the playlist window
			gtk_widget_show_all(help_window);
		}
	}
	else
	{
		// Deactivate the button
		gtk_toggle_button_set_active(button, FALSE);
		// Destroy the help window if it exists
		if (help_window != NULL)
		{
			gtk_widget_destroy(help_window);
			help_window = NULL;
		}
	}
}

/*
 * fun: Function to Mute
 *
 * arg1: GtkToggle Button
 * arg2:app_state *data(structure containing elements)
 *
 */
void mute_cb(GtkToggleButton *button,app_state *data)
{
	if (gtk_toggle_button_get_active(button))
	{
		GstElement* audio_bin = gst_bin_get_by_name(GST_BIN(data->pipeline), "audio_bin");
		GstElement* audio_volume = gst_bin_get_by_name(GST_BIN(audio_bin), "audio-volume");
		g_object_get(G_OBJECT(audio_volume), "volume", &(data->temp_vol), NULL);
		gdouble value = 0;
		g_object_set(G_OBJECT(audio_volume), "volume", value, NULL);
		g_printerr("Muted\n");
		g_print("Old Volume : %f\nNew volume : %f\n",data->temp_vol,value);
		gtk_range_set_value(GTK_RANGE(data->temp_range), (gdouble)(0));

	}
	else {
		GstElement* audio_bin = gst_bin_get_by_name(GST_BIN(data->pipeline), "audio_bin");
		GstElement* audio_volume = gst_bin_get_by_name(GST_BIN(audio_bin), "audio-volume");
		gdouble temp;
		g_object_get(G_OBJECT(audio_volume), "volume", &temp, NULL);
		g_object_set(G_OBJECT(audio_volume), "volume", data->temp_vol, NULL);
		g_printerr("Unmuted\n");
		g_print("Old Volume : %f\nNew volume : %f\n",temp,data->temp_vol);
		gtk_range_set_value(GTK_RANGE(data->temp_range), data->temp_vol);
	}
}

/*
 * fun: Function to add song to Favorite list
 *
 * arg1: GtkToggle Button
 * arg2:app_state *data(structure containing elements)
 *
 */
void fav_lists(GtkToggleButton *button,app_state *data)
{
	//	g_print("%s\n",(*data->play_pointer).c_str());
	bool ispresent=false;
	if (gtk_toggle_button_get_active(button))
	{
		for(auto i:data->fav_list_vector)
		{
			if(!strcmp(i.c_str(),(*data->play_pointer).c_str()))
			{
				g_print("Already Present\n");
				ispresent=true;
				break;
			}
		}
		if(ispresent==false)
		{
			data->fav_list_vector.push_back((*data->play_pointer).c_str());
			g_print("ADDED\t%s to the Vector\n",(*data->play_pointer).c_str());
		}
	}
	else
	{
		int len=0;
		for(auto i:data->fav_list_vector)
		{
			if(!strcmp(i.c_str(),(*data->play_pointer).c_str()))
			{
				g_print("Found in Favorite Now Removing\n");
				data->fav_list_vector.erase(data->fav_list_vector.begin() + len);
				break;
			}
			len++;
		}
	}
	g_printerr("\nFAV LIST\n");
	for(auto i:data->fav_list_vector)
	{
		g_printerr("%s\n",i.c_str());
	}
	g_printerr("END\n");
}

/*
 * fun: Function to Rewind the video
 *
 * arg1: GtkToggle Button
 * arg2:app_state *data(structure containing elements)
 *
 */
gdouble rate1 = 1;
void Rewind_cb(GtkToggleButton* button, app_state *data){
	rate1 = rate1 * -1;
	gint64 position;
	GstEvent *seek_event;

	if (!gst_element_query_position(data->pipeline, GST_FORMAT_TIME, &position))
	{
		g_printerr("Unable to retrieve current position.\n");
	}

	if (rate1 > 0)
	{
		data->duration = duration(data->pipeline);
		gst_element_set_state(data->pipeline, GST_STATE_PLAYING);
		seek_forward_5(data->pipeline,data->Terminate,data->duration);
//		gst_element_seek_simple (data->pipeline, GST_FORMAT_TIME,static_cast<GstSeekFlags>(GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_KEY_UNIT), 1 * GST_SECOND);
		seek_event =gst_event_new_seek(rate1, GST_FORMAT_TIME,(GstSeekFlags)(GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_ACCURATE), GST_SEEK_TYPE_SET,position, GST_SEEK_TYPE_END, 0);
	}
	else
	{
		seek_event = gst_event_new_seek(rate1, GST_FORMAT_TIME,(GstSeekFlags)(GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_ACCURATE), GST_SEEK_TYPE_SET, 0, GST_SEEK_TYPE_SET, position);
	}
	gst_element_send_event(data->pipeline, seek_event);
	g_print("Current rate: %g\n", rate1);

}

bool clicked = FALSE;

int show_popup(GtkWidget *widget, GdkEvent *event) {

  const gint RIGHT_CLICK = 3;

  if (event->type == GDK_BUTTON_PRESS) {

      GdkEventButton *bevent = (GdkEventButton *) event;

      if (bevent->button == RIGHT_CLICK) {

          gtk_menu_popup(GTK_MENU(widget), NULL, NULL, NULL, NULL,
              bevent->button, bevent->time);
          }

      return TRUE;
  }

  return FALSE;
}

void play_selected_file(GtkWidget *widget,app_state *data)
{
	GtkWidget *dialog;
	GtkFileFilter *filter;
	gint result;
	gst_element_set_state(data->pipeline, GST_STATE_NULL);
	dialog = gtk_file_chooser_dialog_new("Open File", NULL,
										 GTK_FILE_CHOOSER_ACTION_OPEN,
										 "Cancel", GTK_RESPONSE_CANCEL,
										 "Open", GTK_RESPONSE_ACCEPT,
										 NULL);
	filter = gtk_file_filter_new();
	gtk_file_filter_add_pattern(filter, "*.mp3");
	gtk_file_filter_add_pattern(filter, "*.mp4");
	gtk_file_filter_add_pattern(filter, "*.avi");
	gtk_file_filter_set_name(filter, "Playable_media");
	gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);
	result = gtk_dialog_run(GTK_DIALOG(dialog));
	if (result == GTK_RESPONSE_ACCEPT) {
		char *filename;
		GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
		filename = gtk_file_chooser_get_filename(chooser);
		string file_path = (string)filename;
		if (
					file_path.find(".mp3") != std::string::npos ||
					file_path.find(".wav") != std::string::npos ||
					file_path.find(".aiff") != std::string::npos ||
					file_path.find(".flac") != std::string::npos ||
					file_path.find(".aac") != std::string::npos ||
					file_path.find(".ogg") != std::string::npos ||
					file_path.find(".wma") != std::string::npos ||
					file_path.find(".mp4a") != std::string::npos ||
					file_path.find(".ac3") != std::string::npos ||
					file_path.find(".webm") != std::string::npos ||
					file_path.find(".amr") != std::string::npos

			) {
				data->video_flag = FALSE;
			} else {
				data->video_flag = TRUE;
			}
		// Do something with the selected file here
		g_object_set(G_OBJECT(data->filesrc), "location", file_path.c_str(), nullptr);
		gst_element_set_state(data->pipeline, GST_STATE_PLAYING);
		auto finder = std::find(data->playlist_vector.begin(), data->playlist_vector.end(), std::string(file_path));

		if (finder == data->playlist_vector.end()) {
			data->playlist_vector.push_back(std::string(file_path));
		}
		data->play_pointer=std::prev(data->playlist_vector.end());
		g_free(filename);
	}
	gtk_widget_destroy(dialog);
}

void seek_time_setter(GtkWidget *widget, app_state *data)
{
	GtkSpinButton *spin_button = GTK_SPIN_BUTTON(data->entry);
	gint value = gtk_spin_button_get_value_as_int(spin_button);
	/* Seek simple funtion */
	gst_element_seek_simple (data->pipeline, GST_FORMAT_TIME,
			static_cast<GstSeekFlags>(GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_KEY_UNIT), value * GST_SECOND);
	gst_element_set_state (data->pipeline, GST_STATE_PLAYING);
	gtk_widget_destroy(data->seekwindow);

}

void destroy (GtkWidget *widget, app_state *data) {
	gst_element_set_state (data->pipeline, GST_STATE_PLAYING);
}

void seek_to_specific_time (GtkWidget* widget, app_state *data) {
	data->seekwindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	gtk_window_set_title(GTK_WINDOW(data->seekwindow), "Seek");
	gtk_container_set_border_width(GTK_CONTAINER(data->seekwindow), 10);

	GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	gtk_container_add(GTK_CONTAINER(data->seekwindow), box);

	GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
	gtk_box_pack_end(GTK_BOX(box), hbox, FALSE, FALSE, 0);

	GtkWidget *label1 = gtk_label_new("Select seek position: 0 to");
	gtk_box_pack_start(GTK_BOX(hbox), label1, FALSE, FALSE, 0);

	int res=(int)(duration(data->pipeline)/GST_SECOND);
	std::string str = std::to_string(res);
	const char* cstr = str.c_str();
	GtkWidget *label2 = gtk_label_new(cstr);
	gtk_box_pack_start(GTK_BOX(hbox), label2, FALSE, FALSE, 0);

	GtkWidget *label3 = gtk_label_new("secs");
	gtk_box_pack_start(GTK_BOX(hbox), label3, FALSE, FALSE, 0);

	GtkWidget *spin = gtk_spin_button_new_with_range(0, res, 1);
	gtk_box_pack_start(GTK_BOX(box), spin, FALSE, FALSE, 0);

	data->entry = spin;
	GtkWidget *button = gtk_button_new_with_label("OK");
	g_signal_connect(button, "clicked", G_CALLBACK(seek_time_setter), data);
	g_signal_connect(data->seekwindow, "destroy", G_CALLBACK(destroy), data);

	gtk_box_pack_end(GTK_BOX(hbox), button, TRUE, TRUE, 0);

	gtk_widget_show_all(data->seekwindow);
}

void play_pause_cb(GtkWidget *widget, app_state *data)
{
		if (data->pipeline_state == GST_STATE_PLAYING) {
			// Pause the playbin and display the play icon
			gst_element_set_state(data->pipeline, GST_STATE_PAUSED);
		    gtk_menu_item_set_label(GTK_MENU_ITEM(widget), "Play");
		    GtkWidget *play_icon = gtk_image_new_from_icon_name("media-playback-start",
					GTK_ICON_SIZE_BUTTON);
			gtk_button_set_image(GTK_BUTTON(data->play_pauseButton), play_icon);
		} else {
			// Start playing the playbin and display the pause icon
			gst_element_set_state(data->pipeline, GST_STATE_PLAYING);
		    gtk_menu_item_set_label(GTK_MENU_ITEM(widget), "Pause");
		    GtkWidget *play_icon = gtk_image_new_from_icon_name("media-playback-pause",
					GTK_ICON_SIZE_BUTTON);
			gtk_button_set_image(GTK_BUTTON(data->play_pauseButton), play_icon);

		}
}

static gboolean sleep_timer_enabled = true;
static guint sleep_timer_id = 0;

void timings(GtkWidget *menu_item, gpointer data) {
    static GtkWidget *prev_item = NULL;
    const gchar* label = gtk_menu_item_get_label(GTK_MENU_ITEM(menu_item));

    if (prev_item != NULL) {
        GdkRGBA white = {1, 1, 1, 1}; // white background color
        gtk_widget_override_background_color(prev_item, GTK_STATE_FLAG_NORMAL, &white);
    }

    // highlight the newly selected item
    GdkRGBA grey = {0.8, 0.8, 0.8, 1}; // light grey background color
    gtk_widget_override_background_color(menu_item, GTK_STATE_FLAG_NORMAL, &grey);

    if (g_strcmp0(label, "disable") == 0) {
		if (sleep_timer_enabled) {
			if (prev_item != NULL) {
				g_print("Disabling sleep timer\n");
				g_source_remove(sleep_timer_id);
			}
			sleep_timer_id = 0;
			sleep_timer_enabled = false;
		}
    }
    else {
    	g_print("%sute selected\n", label);
		gint num = g_ascii_strtoll(label, NULL, 10);
		sleep_timer_id = g_timeout_add_seconds(num * 60, (GSourceFunc)gtk_main_quit,data);
		sleep_timer_enabled = true;
    }

    // update the previously selected item
	prev_item = menu_item;
}

/* Dark Mode */

static GtkCssProvider *dark_css_provider = NULL;

void set_dark_mode() {
    if (!dark_css_provider) {
        // Set the dark mode CSS style
   	 const char *dark_style = " \
   	     * { \
   	         background-color: #1c1c1c; \
   	         color: #f0f0f0; \
   	     } \
   	     button { \
   	         color: #ffffff; \
   	     } \
   	     button:hover { \
   	         background-color: #444444; \
   	     } \
   	     menuitem { \
   	         background-color: #1c1c1c; \
   	     } \
   	     menuitem:checked { \
   	         color: #ffffff; \
   	     } \
   	     menubar { \
   	         background-color: #1c1c1c; \
   	     } \
   	     menu { \
   	         color: #ffffff; \
   	     } \
   		 GtkWidget { \
   	         background-color: #1c1c1c; \
   	     } \
   	     /* Add more CSS styles for your application here */ \
   	 ";

        dark_css_provider = gtk_css_provider_new();
        gtk_css_provider_load_from_data(dark_css_provider, dark_style, -1, NULL);
    }

    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                               GTK_STYLE_PROVIDER(dark_css_provider),
                                               GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

void unset_dark_mode() {
    if (dark_css_provider) {
        // Remove the dark mode CSS style
        gtk_style_context_remove_provider_for_screen(gdk_screen_get_default(),
                                                      GTK_STYLE_PROVIDER(dark_css_provider));
    }
}

void on_switch_toggled(GtkSwitch *switch_button, gpointer data)
{
    if (gtk_switch_get_active(switch_button)) {
        g_print("Switch is on\n");
        set_dark_mode();
    } else {
        g_print("Switch is off\n");
        unset_dark_mode();
    }
}

void on_dark_mode(GtkWidget *menu_item, app_state *data) {
	 static gboolean is_dark_mode = FALSE;
	    if (!is_dark_mode) {
	        set_dark_mode();
		    gtk_menu_item_set_label(GTK_MENU_ITEM(menu_item), "Light Mode");
	        is_dark_mode = TRUE;
	    } else {
	        unset_dark_mode();
		    gtk_menu_item_set_label(GTK_MENU_ITEM(menu_item), "Dark Mode");
	        is_dark_mode = FALSE;
	    }
}
