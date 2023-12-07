#include <gst/gst.h>
#include <gtk/gtk.h>
#include <vector>
#include <string>

#include "custom_data.h"
#include "application_state.h"
#include "gtk_gui.h"

#ifndef HEADER_NAVIGATOR_H_
#define HEADER_NAVIGATOR_H_

void play_previous_song(GtkWidget*, app_state* );
void play_next_song(GtkWidget* ,app_state* );

#endif /* HEADER_NAVIGATOR_H_ */
