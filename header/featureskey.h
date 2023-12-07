#ifndef HEADER_FEATURESKEY_H_
#define HEADER_FEATURESKEY_H_

#include <gst/gst.h>
#include <vector>
#include <string>
#include "application_state.h"
#include "featureskey.h"
#include "gtk_gui.h"
#include "gui_elements.h"
using namespace std;

void startk(GstElement *pipeline);
void pausek(GstElement *pipeline);
void resumek(GstElement *pipeline);
void stopk(GstElement *pipeline);
void quit(GstElement *pipeline);
void Help();
void print_one_tag(const GstTagList * list, const gchar * tag, gpointer user_data);
void print_metadata(GstElement *pipeline);
gint64 duration(GstElement *pipeline);
void seek_forward_10percent(GstElement *pipeline, gboolean &terminate, gint64 duration);
void seek_backward_10percent(GstElement *pipeline, gint64 duration);
void seek_forward_5 (GstElement *pipeline, gboolean &terminate, gint64 duration);
void seek_forward_60 (GstElement *pipeline, gboolean &terminate, gint64 duration);
void seek_back_5 (GstElement *pipeline, gint64 duration);
void seek_back_60 (GstElement *pipeline, gint64 duration);
void play_previous_songk(app_state& data, vector<string> filepaths);
void play_next_songk(app_state& data, vector<string> filepaths);
void decrease_volume(app_state& data);
void increase_volume(app_state& data);
void increase_volume(GstElement *pipeline,  GstElement *volume);
void decrease_volume(GstElement *pipeline,  GstElement *volume);
void DisplayPlayList(vector<string> filepaths);
void Rewind(GstElement *pipeline);

#endif /* HEADER_FEATURESKEY_H_ */
