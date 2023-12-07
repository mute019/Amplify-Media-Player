#include <gst/gst.h>
#include <gtk/gtk.h>
#include <vector>
#include <string>

#include "../header/featureskey.h"
#include "custom_data.h"
#include "application_state.h"
#include "gtk_gui.h"

#ifndef HEADER_PLAYBACK_H_
#define HEADER_PLAYBACK_H_
/*--------------------------------------------------------------------------------------------------------------
 *
 * This contains the declarations of all the
 *
 * --------------------------------------------------------------------------------------------------------------
 *
 * */

void start(GtkWidget* ,app_state* );
void halt(GtkWidget* ,app_state *);
void resume(GtkWidget* ,GstElement *pipeline);
void stop(GtkWidget* ,app_state* );
void quit(GstElement *pipeline);
gint64 duration(GstElement *pipeline);
void shuffle_cb(GtkToggleButton *button, app_state *data);
void repeat_cb(GtkButton *button, app_state *data);
void open_directory_cb(GtkButton *button,app_state *status);//opening directory call back function
void seek_back_5_cb (GtkWidget* , GstElement* pipeline);
void seek_forward_5_cb (GtkWidget* ,GstElement *pipeline, gboolean &terminate);
void volume_scale_changed(GtkRange *range, app_state *data);
void on_seek_slider_changed(GtkWidget *widget,app_state *status);
gboolean refresh_slider_position(app_state *status);
void on_metadata_button_toggled(GtkToggleButton *button, app_state *data);
void print_playlist(GtkTextBuffer *buffer,app_state** data);
void on_playlist_button_clicked(GtkToggleButton* button, app_state *data);
void about_cb(GtkToggleButton *button, app_state *data);
void on_help_button_toggled(GtkToggleButton* button, app_state *data);
void mute_cb(GtkToggleButton *button, app_state *data);
void fav_lists(GtkToggleButton *button,app_state *data);
void Rewind_cb(GtkToggleButton* button, app_state *data);

int show_popup(GtkWidget *widget, GdkEvent *event);
int click_pause_play(GtkWidget *widget, GdkEvent *event, app_state *data);
void repeat_in_menu (GtkWidget* widget, app_state *data);
void play_selected_file(GtkWidget *widget,app_state *status);
void seek_to_specific_time (GtkWidget* widget, app_state *data);
void play_pause_cb(GtkWidget *widget, app_state *data);
void timings(GtkWidget *menu_item, gpointer data);
void on_dark_mode(GtkWidget *menu_item, app_state *data);

#endif /* HEADER_PLAYBACK_H_ */
