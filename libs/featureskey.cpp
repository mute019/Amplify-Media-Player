#include "../header/featureskey.h"
#include <iostream>
#include<bits/stdc++.h>

/*
 * fun:start(pstarts the pipeline to playing state)
 *
 * arg1:GstElement pipeline
 *
 */
void startk(GstElement *pipeline)
{
	g_print("Start Playing.........");
	gst_element_set_state (pipeline, GST_STATE_PLAYING);
}

/*
 * fun:pause(pauses the pipeline from playing state)
 *
 * arg1:GstElement pipeline
 *
 */
void pausek(GstElement *pipeline)
{
	g_print("Pausing.........");
	gst_element_set_state (pipeline, GST_STATE_PAUSED);
}

/*
 * fun:resume(resumes the pipeline)
 *
 * arg1:GstElement pipeline
 *
 */

void resumek(GstElement *pipeline)
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

void stopk(GstElement *pipeline)
{
	g_print("Stoping.........");
	gst_element_set_state (pipeline, GST_STATE_READY);
}

/*
 * fun:quit
 *
 * arg1:GstElement pipeline
 *
 */
void Help(){
	g_print("\nFEATURES\n");
	g_print("b\t:Start Playing\np\t:Pause\nr\t:Resume\ns\t:Stop");
	g_print("\n/\t:Advance\n|\t:Previous\n,.\t:Backward/forward 5 Seconds\n");
	g_print("z\t:Rewind\n");
	g_print("+\t:Increase Volume\n-\t:Decrease Volume\n");
	g_print("[]\t:Backward/Forward 60 Seconds\n{}\t:Backward/Forward 10%% of Media\nq\t:Quit");
	g_print("\nh\t:Help/Exit\nd\t:View Play List\n");

}

/*
 * fun:duration
 *
 * arg1:GstElement pipeline
 *
 * output:returns total duration of the playing media
 */


/*
 * fun:its seeks forward by 10% for the total duration of the playing media
 *
 * arg1:GstElement pipeline
 * arg2:gboolean terminate
 * arg3:gint64 duration(toatal duration of the playing media)
 *
 */

void seek_forward_10percent(GstElement *pipeline,gboolean &terminate,gint64 duration)
{
	gst_element_set_state(pipeline, GST_STATE_PLAYING);
	float playedSec=0,percent;
	gint64 current;
	float res=(float)(duration/GST_SECOND);
	percent=0.1*res;
	cout<<percent<<endl;

	//checking the current position of the video
	gst_element_query_position (pipeline, GST_FORMAT_TIME, &current);
	playedSec=(float)(current/GST_SECOND);

	if(playedSec>=(res-percent))
	{
		gst_element_seek_simple (pipeline, GST_FORMAT_TIME,(GstSeekFlags)(GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_KEY_UNIT),(res-1) * GST_SECOND);
		terminate=TRUE;
	}
	g_print ("Position %" GST_TIME_FORMAT " / %" GST_TIME_FORMAT "\r", GST_TIME_ARGS (current), GST_TIME_ARGS (duration));

	//seeking the video
	gst_element_seek_simple (pipeline, GST_FORMAT_TIME,(GstSeekFlags)(GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_KEY_UNIT),( percent+playedSec) * GST_SECOND);

}

/*
 * fun:its seeks backward by 10% for the total duration of the playing media
 *
 * arg1:GstElement pipeline
 * arg2:gint64 duration(toatal duration of the playing media)
 *
 */

void seek_backward_10percent(GstElement *pipeline, gint64 duration)
{
	gst_element_set_state(pipeline, GST_STATE_PLAYING);
	gint64 current=0;
	float playedSec=0,percent;
	float res=(float)(duration/GST_SECOND);
	percent=0.1*res;  //calculated value for 10% for the entire duration of the video

	//checking the current position of the video
	gst_element_query_position (pipeline, GST_FORMAT_TIME, &current);
	playedSec=(float)(current/GST_SECOND);
	if( playedSec<=percent)
	{
		gst_element_seek_simple (pipeline, GST_FORMAT_TIME,(GstSeekFlags)(GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_KEY_UNIT),(0) * GST_SECOND);
	}
	else
	{
		//seeking the video
		gst_element_seek_simple (pipeline, GST_FORMAT_TIME,(GstSeekFlags)(GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_KEY_UNIT),(playedSec-percent) * GST_SECOND);
	}

	g_print ("Position %" GST_TIME_FORMAT " / %" GST_TIME_FORMAT "\r", GST_TIME_ARGS (current), GST_TIME_ARGS (duration));
}

/*
 * fun: seeking forward 5 seconds
 *
 * arg1: GstElement pipeline
 * arg2: gboolean terminate (to terminate the pipeline after the meadia reaches end)
 * arg3: gint64 duration (total duration of the playing media)
 *
 */

void seek_forward_5 (GstElement *pipeline, gboolean &terminate, gint64 duration) {
	gint64 current = -1;

	/* Query the current position of the stream */
	if (!gst_element_query_position (pipeline, GST_FORMAT_TIME, &current)) {
		g_printerr ("Could not query current position.\n");
	}

	/* Converting the current time to seconds */
	float res=(float)(current/GST_SECOND);

	/* Corner case for forward seek */
	if (((duration - current)/GST_SECOND) < 5) {
		res = (float)(duration/GST_SECOND) - 6;
		terminate = TRUE;
	}

	/* Seek simple funtion */
	gst_element_seek_simple (pipeline, GST_FORMAT_TIME,
			static_cast<GstSeekFlags>(GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_KEY_UNIT), (res + 5) * GST_SECOND);

	cout << "seeking forward 5 secs" << endl;
}

/*
 * fun: seeking forward 60 seconds
 *
 * arg1: GstElement pipeline
 * arg2: gboolean terminate (to terminate the pipeline after the meadia reaches end)
 * arg3: gint64 duration (total duration of the playing media)
 *
 */

void seek_forward_60 (GstElement *pipeline, gboolean &terminate, gint64 duration) {
	gint64 current = -1;

	/* Query the current position of the stream */
	if (!gst_element_query_position (pipeline, GST_FORMAT_TIME, &current)) {
		g_printerr ("Could not query current position.\n");
	}

	/* Converting the current time to seconds */
	float res=(float)(current/GST_SECOND);

	/* Corner case for forward seek */
	if (((duration - current)/GST_SECOND) < 60) {
		res = (float)(duration/GST_SECOND) - 61;
		terminate = TRUE;
	}

	/* Seek simple funtion */
	gst_element_seek_simple (pipeline, GST_FORMAT_TIME,
			static_cast<GstSeekFlags>(GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_KEY_UNIT), (res + 60) * GST_SECOND);

	cout << "seeking forward 60 secs" << endl;
}

/*
 * fun: seeking backward 5 seconds
 *
 * arg1: GstElement pipeline
 * arg2: gint64 duration (total duration of the playing media)
 *
 */

void seek_back_5 (GstElement *pipeline, gint64 duration) {
	gint64 current = -1;

	/* Query the current position of the stream */
	if (!gst_element_query_position (pipeline, GST_FORMAT_TIME, &current)) {
		g_printerr ("Could not query current position.\n");
	}

	/* Converting the current time to seconds */
	float res=(float)(current/GST_SECOND);

	/* Corner case for backward seek */
	if (res < 5) {
		res = 5;
	}

	/* Seek simple funtion */
	gst_element_seek_simple (pipeline, GST_FORMAT_TIME,
			static_cast<GstSeekFlags>(GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_KEY_UNIT), (res - 5) * GST_SECOND);

	cout << "seeking back 5 secs" << endl;
}

/*
 * fun: seeking backward 60 seconds
 *
 * arg1: GstElement pipeline
 * arg2: gint64 duration (total duration of the playing media)
 *
 */

void seek_back_60 (GstElement *pipeline, gint64 duration) {
	gint64 current = -1;

	/* Query the current position of the stream */
	if (!gst_element_query_position (pipeline, GST_FORMAT_TIME, &current)) {
		g_printerr ("Could not query current position.\n");
	}

	/* Converting the current time to seconds */
	float res=(float)(current/GST_SECOND);

	/* Corner case for backward seek */
	if (res < 60) {
		res = 60;
	}

	/* Seek simple funtion */
	gst_element_seek_simple (pipeline, GST_FORMAT_TIME,
			static_cast<GstSeekFlags>(GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_KEY_UNIT), (res - 60) * GST_SECOND);

	cout << "seeking back 60 secs" << endl;
}

/*
 * fun: playing the next song in playlist
 *
 * arg1: GstElement pipeline
 * arg2: vector containing paths of the media
 *
 */
static vector<string>::size_type current_song_index = 0; //current song variable is set to 0 and declared global.

void play_next_songk(app_state& data, vector <string> filepaths) {
	// Stop the current song
	gst_element_set_state(data.pipeline, GST_STATE_NULL);


	// Get the next song in the vector
	current_song_index++;
	if (current_song_index >= filepaths.size()) {
		current_song_index = 0; // start from the beginning if we reached the end of the vector
	}
	std::string nextsong = filepaths[current_song_index];



	if (
			nextsong.find(".mp3") != std::string::npos ||
			nextsong.find(".wav") != std::string::npos ||
			nextsong.find(".aiff") != std::string::npos ||
			nextsong.find(".flac") != std::string::npos ||
			nextsong.find(".aac") != std::string::npos ||
			nextsong.find(".ogg") != std::string::npos ||
			nextsong.find(".wma") != std::string::npos ||
			nextsong.find(".mp4a") != std::string::npos ||
			nextsong.find(".ac3") != std::string::npos ||
			nextsong.find(".webm") != std::string::npos ||
			nextsong.find(".amr") != std::string::npos

		) {
			data.video_flag = FALSE;
		} else {
			data.video_flag = TRUE;
		}
	// Update the file source element to read the next song
	g_object_set(G_OBJECT(data.filesrc), "location", nextsong.c_str(), nullptr);
	const char* title = g_strdup_printf("AMPLIFY Media-Player\t%s",nextsong.c_str());
	gtk_window_set_title(GTK_WINDOW(data.temp_main_window),title);
	data.duration = duration(data.pipeline);


	// Start playing the next song
	gst_element_set_state(data.pipeline, GST_STATE_PLAYING);

}

/*
 * fun: playing the previous song in playlist
 *
 * arg1: GstElement pipeline
 * arg2: vector containing paths of the media
 *
 */

void play_previous_songk(app_state& data, vector <string> filepaths)
{
	// Stop the current song
	gst_element_set_state(data.pipeline, GST_STATE_NULL);
	// Get the previous song in the vector, only if it's not empty
	if (current_song_index == 0) {
		current_song_index = filepaths.size() - 1; // start from the end if we reached the beginning of the vector
	}
	else
	{
		current_song_index--;
	}
	std::string previous_song = filepaths[current_song_index];

	if (
			previous_song.find(".mp3") != std::string::npos ||
			previous_song.find(".wav") != std::string::npos ||
			previous_song.find(".aiff") != std::string::npos ||
			previous_song.find(".flac") != std::string::npos ||
			previous_song.find(".aac") != std::string::npos ||
			previous_song.find(".ogg") != std::string::npos ||
			previous_song.find(".wma") != std::string::npos ||
			previous_song.find(".mp4a") != std::string::npos ||
			previous_song.find(".ac3") != std::string::npos ||
			previous_song.find(".webm") != std::string::npos ||
			previous_song.find(".amr") != std::string::npos

		) {
			data.video_flag = FALSE;
		} else {
			data.video_flag = TRUE;
		}
	// Update the file source element to read the previous song
	g_object_set(G_OBJECT(data.filesrc), "location",previous_song.c_str(), nullptr);
	const char* title = g_strdup_printf("AMPLIFY Media-Player\t%s", previous_song.c_str());
	gtk_window_set_title(GTK_WINDOW(data.temp_main_window),title);
	data.duration = duration(data.pipeline);



	// Start playing the previous song
	gst_element_set_state(data.pipeline, GST_STATE_PLAYING);

}

/*
 * fun:Increase volume of the song in playlist
 *
 * arg1: GstElement pipeline
 * arg2: GstElement volume
 * arg3: volume variable which is set to 0.1 before calling this function.
 *
 */

void increase_volume(app_state& data) {
	// retrieve the audio-volume element
	GstElement* audio_bin = gst_bin_get_by_name(GST_BIN(data.pipeline), "audio_bin");
	GstElement* audio_volume = gst_bin_get_by_name(GST_BIN(audio_bin), "audio-volume");

	// retrieve the current volume value
	gdouble volume;
	g_object_get(G_OBJECT(audio_volume), "volume", &volume, NULL);

	volume += 0.1;
	if (volume > 1.0) {
		volume = 1.0;
	}
	// set the new volume value
	g_object_set(G_OBJECT(audio_volume), "volume", volume, NULL);
	gtk_range_set_value(GTK_RANGE(data.temp_range), volume);

	g_print("New volume value: %f\n", volume);
}

/*
 * fun:Decrease volume of the song in playlist
 *
 * arg1: GstElement pipeline
 * arg2: GstElement volume
 * arg3: volume variable which is set to 0.1 before calling this function.
 *
 */

void decrease_volume(app_state& data) {
	GstElement* audio_bin = gst_bin_get_by_name(GST_BIN(data.pipeline), "audio_bin");
	GstElement* audio_volume = gst_bin_get_by_name(GST_BIN(audio_bin), "audio-volume");

	// retrieve the current volume value
	gdouble volume;
	g_object_get(G_OBJECT(audio_volume), "volume", &volume, NULL);

	//decrease the volume by 0.1 (10%)
	volume -= 0.1;
	if (volume < 0.0) {
		volume = 0.0;
	}

	g_object_set(G_OBJECT(audio_volume), "volume", volume, NULL);
	gtk_range_set_value(GTK_RANGE(data.temp_range), volume);

	g_print("New volume value: %f\n", volume);
}

/*
 * fun: Print all the songs in playlist
 *
 * arg1: vector containing song names
 *
 */

void DisplayPlayList(vector<string> filepaths){
	stringstream ss;
	ss<<"PLAYLIST\n";
	for(const auto& i:filepaths)
	{
		size_t pos = i.find_last_of("/\\");
		ss<<i.substr(pos+1)<<"\n";
	}
	string t = ss.str();
	g_print("%s",t.c_str());
}

/*
 * fun: Function to Rewind the video
 *
 * arg1: GstElement pipeline
 *
 */
gdouble rate = 1;
void Rewind(GstElement *pipeline){
	rate = rate * -1;
	gint64 position;
	GstEvent *seek_event;

	if (!gst_element_query_position(pipeline, GST_FORMAT_TIME, &position))
	{
		g_printerr("Unable to retrieve current position.\n");
	}

	if (rate > 0)
	{
		gint64 current = -1;
		if (!gst_element_query_position (pipeline, GST_FORMAT_TIME, &current)) {
			g_printerr ("Could not query current position.\n");
		}

		/* Converting the current time to seconds */
		float res=(float)(current/GST_SECOND);

		gst_element_seek_simple (pipeline, GST_FORMAT_TIME,static_cast<GstSeekFlags>(GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_KEY_UNIT), res * GST_SECOND);
		seek_event =gst_event_new_seek(rate, GST_FORMAT_TIME,(GstSeekFlags)(GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_ACCURATE), GST_SEEK_TYPE_SET,position, GST_SEEK_TYPE_END, 0);
	}
	else
	{
		seek_event = gst_event_new_seek(rate, GST_FORMAT_TIME,(GstSeekFlags)(GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_ACCURATE), GST_SEEK_TYPE_SET, 0, GST_SEEK_TYPE_SET, position);
	}

	gst_element_send_event(pipeline, seek_event);
	g_print("Current rate: %g\n", rate);

}
