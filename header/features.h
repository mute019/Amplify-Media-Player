#include <gst/gst.h>
#include <gtk/gtk.h>
#include <vector>
#include <string>
#include "custom_data.h"
#include "application_state.h"
#include "gtk_gui.h"

using namespace std;

#ifndef FEATURES_HPP_
#define FEATURES_HPP_

gboolean on_key_press(GtkWidget *widget, GdkEventKey *event, app_state *data);

gint64 duration(GstElement *pipeline);
void seek_forward_10percent(GstElement *pipeline, gboolean &terminate, gint64 duration);
void seek_backward_10percent(GstElement *pipeline, gint64 duration);
void seek_forward_5 (GstElement *pipeline, gboolean &terminate, gint64 duration);
void seek_forward_60 (GstElement *pipeline, gboolean &terminate, gint64 duration);
void seek_back_5 (GstElement *pipeline, gint64 duration);
void seek_back_60 (GstElement *pipeline, gint64 duration);

#endif /* FEATURES_HPP_ */
