#include <gst/gst.h>
#include <gst/pbutils/gstdiscoverer.h>
#include <gst/controller/controller.h>
#include <string>
#include "../header/metadata.h"

bool titleexecuted=false;
bool artistexecuted=false;

//setting the toggle button when help window is closed
gboolean on_window_closed(GtkWidget* widget, GdkEvent* event, gpointer data)
{
	GtkToggleButton* button = GTK_TOGGLE_BUTTON(data);
	gtk_toggle_button_set_active(button, FALSE);
	return FALSE;
}

/*
 * fun:Function to print meta data
 *
 * arg1: GstTagList
 * arg2: Tag
 * arg3: Buffer
 *
 */
void print_metadata(const GstTagList *list, const gchar *tag, GtkTextBuffer *buffer)
{
	int i, num;
	num = gst_tag_list_get_tag_size (list, tag);
	for (i = 0; i < num; ++i)
	{
		const GValue *val;
		val = gst_tag_list_get_value_index (list, tag, i);
		if (G_VALUE_HOLDS_STRING (val))
		{
			if((!strcmp(tag,"title"))&&titleexecuted==false)
			{
				for(int i=0;i<(int)strlen(tag);i++)
				{
					titleexecuted=true;
					char ch=tag[i];
					g_print("%c",toupper(ch));
				}
				g_print ("\t: %s\t\t\t",g_value_get_string (val));
				gtk_text_buffer_insert_at_cursor(buffer,"Title\t\t: ",-1);
				gtk_text_buffer_insert_at_cursor(buffer,g_value_get_string (val),-1);
			}
			if((!strcmp(tag,"artist"))&&artistexecuted==false)
			{
				for(int i=0;i<(int)strlen(tag);i++)
				{
					artistexecuted=true;
					char ch=tag[i];
					g_print("%c",toupper(ch));
				}
				g_print ("\t: %s\t\t",g_value_get_string (val));
				gtk_text_buffer_insert_at_cursor(buffer,"\nArtist\t\t: ",-1);
				gtk_text_buffer_insert_at_cursor(buffer,g_value_get_string (val),-1);
			}
		}
	}
}

/*
 * fun:Meta Data callback function
 *
 * arg1: Buffer
 * arg2:app_state *data(structure containing elements)
 *
 */
void metadata(GtkTextBuffer **buffer, app_state** data)
{
	GstDiscoverer *discoverer;
	GError *error = NULL;
	GstDiscovererInfo *info;
	const GstTagList *tags;
	guint64 duration;

	/* Initialize GStreamer */
	gst_init(nullptr, nullptr);

	/* Create the discoverer instance */
	discoverer = gst_discoverer_new(5 * GST_SECOND, &error);
	if (!discoverer)
	{
		g_print("Failed to create discoverer instance: %s\n", error->message);
		g_clear_error(&error);
		return;
	}

	/* Parse the URI passed as argument */
	std::string newuri= "file:"+(*((*data)->play_pointer));

	info = gst_discoverer_discover_uri(discoverer, newuri.c_str(), &error);

	if (info)
	{
		tags = gst_discoverer_info_get_tags(info);
		if (tags)
		{
			gst_tag_list_foreach(tags, (GstTagForeachFunc)print_metadata, *buffer);
		}
	}

	duration = gst_discoverer_info_get_duration(info);
	g_print("\tDURATION: %lu Seconds\n", duration/GST_SECOND);

	gtk_text_buffer_insert_at_cursor(*buffer,"\nDuration\t: ",-1);

	std::string str = std::to_string(duration/GST_SECOND)+" Seconds";
	gtk_text_buffer_insert_at_cursor(*buffer,str.c_str(),-1);
	titleexecuted=false;
	artistexecuted=false;

	gst_discoverer_info_unref(info);
	g_object_unref(discoverer);
	return;
}

void metawindow (GtkWidget* button, app_state* data)
{
	gboolean active = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(button));

	// Get the main window from the user data
	//GtkWindow *main_window = GTK_WINDOW(data);
	if (!active)
	{
		gtk_widget_destroy(data->meta_window);
		data->meta_window = nullptr;
		return;
	}

	data->meta_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(data->meta_window), "Media_Info");

	gtk_container_set_border_width(GTK_CONTAINER(data->meta_window), 10);
	gtk_window_set_default_size(GTK_WINDOW(data->meta_window), 300, 300);
	g_signal_connect(G_OBJECT(data->meta_window), "delete-event", G_CALLBACK(on_window_closed), button);

	// Create a new text view widget to display the playlist
	GtkWidget *metadata_text_view = gtk_text_view_new();
	gtk_text_view_set_editable(GTK_TEXT_VIEW(metadata_text_view), FALSE);
	gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(metadata_text_view), FALSE);

	// Add the text view widget to a scrolled window
	GtkWidget *playlist_scroll_window = gtk_scrolled_window_new(NULL, NULL);
	gtk_container_add(GTK_CONTAINER(playlist_scroll_window), metadata_text_view);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(playlist_scroll_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

	// Get the text buffer associated with the text view widget
	GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(metadata_text_view));

	// Generate the content and update the text buffer
	metadata(&buffer,&data);

	// Add the scrolled window to the playlist window
	gtk_container_add(GTK_CONTAINER(data->meta_window), playlist_scroll_window);

	// Show all the widgets in the playlist window
	gtk_widget_show_all(data->meta_window);

	return;
}





