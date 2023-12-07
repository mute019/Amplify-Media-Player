#include <gst/gst.h>
#include <gtk/gtk.h>
#include <string>

#include "custom_data.h"
#include "application_state.h"
#include "gui_elements.h"

/*------------------------------------------------------------------
 *
 * Declaration of the class "mediapipe"
 *
 * Member variable 1: CustomData
 *
 * Member Functions 1: a
 *
 * -----------------------------------------------------------------
 */
#ifndef HEADER_PLAYBIN_H_
#define HEADER_PLAYBIN_H_


class mediapipe
{
	CustomData data;
	static void pad_added_handler(GstElement *, GstPad* , CustomData* );
	static void pad_removed_handler(GstElement *, GstPad* , CustomData* );
	static void quit_window(GtkWidget* , CustomData** );
	static gboolean draw_window(GtkWidget* , cairo_t* , CustomData** );
	int caller(gui_elem**, app_state**);
public:
	mediapipe();
	void play(gui_elem* , app_state* );
};

#endif /* HEADER_PLAYBIN_H_ */
