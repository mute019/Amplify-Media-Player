#include <iostream>
#include <gtk/gtk.h>
#include <gst/gst.h>

#include "../header/features.h"
#include "../header/featureskey.h"
#include "../header/custom_data.h"
#include "../header/application_state.h"
#include "../header/gtk_gui.h"

/*
 * fun:quit
 *
 * arg1:GstElement pipeline
 *
 */
void quit(GstElement *pipeline)
{
	g_print("Quiting.........");
	exit(0);
}

/*
 * fun:Keyboard Handling
 *
 * arg1: Gtk widget
 * arg2: GdkEvent key
 * arg3: app_state *data(structure containing elements)
 *
 */

gboolean on_key_press(GtkWidget *widget, GdkEventKey *event, app_state *data)
{
	// Check if the pressed key is a printable ASCII character
	if (gdk_keyval_is_lower(event->keyval) || gdk_keyval_is_upper(event->keyval)) {
		char c = gdk_keyval_to_unicode(event->keyval);
		// Check if the "enter" key was pressed
		switch(c){
		case '/':
			play_next_songk(*data,data->playlist_vector);
			break;
		case '|':
			play_previous_songk(*data,data->playlist_vector);
			break;
		case 'b':
			startk(data->pipeline);
			std::cout << c << " was pressed."<< std::endl;
			break;
		case 's':
			stopk(data->pipeline);
			std::cout << c << " was pressed."<< std::endl;
			break;
		case 'p':
			pausek(data->pipeline);
			std::cout << c << " was pressed."<< std::endl;
			break;
		case 'r':
			resumek(data->pipeline);
			std::cout << c << " was pressed."<< std::endl;
			break;
		case 'q':
			quit(data->pipeline);
			std::cout << c << " was pressed."<< std::endl;
			break;
		case '.':
			seek_forward_5 (data->pipeline, data->Terminate, data->duration);
			break;
		case']':
			seek_forward_60 (data->pipeline, data->Terminate, data->duration);
			break;
		case ',':
			seek_back_5 (data->pipeline, data->duration);
			break;
		case '[':
			seek_back_60 (data->pipeline, data->duration);
			break;
		case '}':
			data->duration =duration(data->pipeline);
			seek_forward_10percent(data->pipeline,data->Terminate, data->duration);
			break;
		case '{':
			data->duration =duration(data->pipeline);
			seek_backward_10percent(data->pipeline, data->duration);
			break;
		case '+':
			increase_volume(*data);
			break;
		case '-':
			decrease_volume(*data);
			break;
		case 'd':
			DisplayPlayList(data->playlist_vector);
			break;
		case 'h':
			Help();
			break;
		case 'z':
			Rewind(data->pipeline);
			break;
		}
	}
	return TRUE;
}

