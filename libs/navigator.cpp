#include <gst/gst.h>
#include <gtk/gtk.h>
#include <vector>
#include <string>

#include "../header/custom_data.h"
#include "../header/application_state.h"
#include "../header/gtk_gui.h"
#include "../header/navigator.h"
/*
 * fun: playing the next song in playlist
 *
 * arg1: GtkWidget
 * arg2: application status of my application which tracks all the information regarding our application
 *
 */
void play_next_song(GtkWidget* widget, app_state *data)
{
	GtkWidget *play_icon = gtk_image_new_from_icon_name("media-playback-pause",
			GTK_ICON_SIZE_BUTTON);
	gtk_button_set_image(GTK_BUTTON(data->play_pauseButton), play_icon);
    gtk_menu_item_set_label(GTK_MENU_ITEM(data->play_pauseMb), "Pause");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(data->rewind),false);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(data->temp_mute),false);
	gtk_range_set_value(GTK_RANGE(data->temp_range), (gdouble)(1.0));
	g_printerr("Next Button Clicked! \n");

	if (!data->playlist_vector.size())
	{
		g_printerr("Playlist is empty!! \n");
		return;
	}
	// Get the next song in the vector
	data->play_pointer++;
	if (data->playlist_vector.end() == data->play_pointer)
	{
		data->play_pointer = data->playlist_vector.begin(); // start from the beginning if we reached the end of the vector
	}

	if (
			(*data->play_pointer).find(".mp3") != std::string::npos ||
			(*data->play_pointer).find(".wav") != std::string::npos ||
			(*data->play_pointer).find(".aiff") != std::string::npos ||
			(*data->play_pointer).find(".flac") != std::string::npos ||
			(*data->play_pointer).find(".aac") != std::string::npos ||
			(*data->play_pointer).find(".ogg") != std::string::npos ||
			(*data->play_pointer).find(".wma") != std::string::npos ||
			(*data->play_pointer).find(".mp4a") != std::string::npos ||
			(*data->play_pointer).find(".ac3") != std::string::npos ||
			(*data->play_pointer).find(".webm") != std::string::npos ||
			(*data->play_pointer).find(".amr") != std::string::npos

	) {
		data->video_flag = FALSE;
	}
	else
	{
		data->video_flag = TRUE;
	}

	gst_element_set_state(data->pipeline, GST_STATE_READY);

	std::string uri = (*data->play_pointer);
	const char* title = g_strdup_printf("AMPLIFY Media-Player\t%s",uri.c_str());
	gtk_window_set_title(GTK_WINDOW(data->temp_main_window),title);
	g_object_set(G_OBJECT(data->filesrc), "location", uri.c_str(), nullptr);
	bool ispresent=false;
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(data->temp_fav)))
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
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(data->temp_fav),false);
		}
	}
	else
	{
		for(auto i:data->fav_list_vector)
		{
			if(!strcmp(i.c_str(),(*data->play_pointer).c_str()))
			{
				g_print("Present\n");
				ispresent=true;
				break;
			}
		}
		if(ispresent==true)
		{
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(data->temp_fav),true);
		}
	}
	gst_element_set_state(data->pipeline, GST_STATE_PLAYING);
}


/*
 * fun: playing the previous song in playlist
 *
 * arg1: GtkWidget
 * arg2: application status of my application which tracks all the information regarding our application
 *
 */
void play_previous_song(GtkWidget* widget, app_state *data)
{
	GtkWidget *play_icon = gtk_image_new_from_icon_name("media-playback-pause",
			GTK_ICON_SIZE_BUTTON);
	gtk_button_set_image(GTK_BUTTON(data->play_pauseButton), play_icon);
    gtk_menu_item_set_label(GTK_MENU_ITEM(data->play_pauseMb), "Pause");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(data->rewind),false);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(data->temp_mute),false);
	gtk_range_set_value(GTK_RANGE(data->temp_range), (gdouble)(1.0));

	g_printerr("Next Button Clicked! \n");

	if (!data->playlist_vector.size())
	{
		g_printerr("Playlist is empty!! \n");
		return;
	}

	// Get the next song in the vector
	if (data->playlist_vector.begin() == data->play_pointer)
	{
		data->play_pointer = std::prev(data->playlist_vector.end()); // start from the beginning if we reached the end of the vector
	}
	else
	{
		data->play_pointer--;
	}

	if (
			(*data->play_pointer).find(".mp3") != std::string::npos ||
			(*data->play_pointer).find(".wav") != std::string::npos ||
			(*data->play_pointer).find(".aiff") != std::string::npos ||
			(*data->play_pointer).find(".flac") != std::string::npos ||
			(*data->play_pointer).find(".aac") != std::string::npos ||
			(*data->play_pointer).find(".ogg") != std::string::npos ||
			(*data->play_pointer).find(".wma") != std::string::npos ||
			(*data->play_pointer).find(".mp4a") != std::string::npos ||
			(*data->play_pointer).find(".ac3") != std::string::npos ||
			(*data->play_pointer).find(".webm") != std::string::npos ||
			(*data->play_pointer).find(".amr") != std::string::npos

	) {
		data->video_flag = FALSE;
	}
	else
	{
		data->video_flag = TRUE;
	}

	gst_element_set_state(data->pipeline, GST_STATE_READY);
	std::string uri = (*data->play_pointer);
	const char* title = g_strdup_printf("AMPLIFY Media-Player\t%s",uri.c_str());
	gtk_window_set_title(GTK_WINDOW(data->temp_main_window),title);
	g_object_set(G_OBJECT(data->filesrc), "location", uri.c_str(), nullptr);

	bool ispresent=false;
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(data->temp_fav)))
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
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(data->temp_fav),false);
		}
	}
	else
	{
		for(auto i:data->fav_list_vector)
		{
			if(!strcmp(i.c_str(),(*data->play_pointer).c_str()))
			{
				g_print("Present\n");
				ispresent=true;
				break;
			}
		}
		if(ispresent==true)
		{
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(data->temp_fav),true);
		}
	}

	gst_element_set_state(data->pipeline, GST_STATE_PLAYING);
}
