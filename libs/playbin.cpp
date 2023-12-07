#include <iostream>
#include <string>
#include <gst/gst.h>
#include <gtk/gtk.h>
#include <gst/video/videooverlay.h>
#include <gdk/gdk.h>
#include <glib.h>

#if defined (GDK_WINDOWING_X11)
#include <gdk/gdkx.h>
#elif defined (GDK_WINDOWING_WIN32)
#include <gdk/gdkwin32.h>
#elif defined (GDK_WINDOWING_QUARTZ)
#include <gdk//gdkquartz.h>
#endif

#include "../header/videobin.h"
#include "../header/audiobin.h"
#include "../header/playbin.h"
#include "../header/features.h"
#include "../header/custom_data.h"
#include "../header/application_state.h"

/* --------------------------------------------------------------------
 * Here the constructor is initialising all the elements are being
 * to the member variables of the class "mediapipe".
 * --------------------------------------------------------------------
 */

void for_toggle(app_state *state);

mediapipe::mediapipe()
{
	//	gtk_init(nullptr, nullptr);
	//
	//	data.main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	gst_init(nullptr, nullptr);

	data.decodebin = gst_element_factory_make("decodebin", "decodebin");
	data.pipeline = nullptr;
}

/*----------------------------------------------------------------------------
 *
 * The caller member function here is configures/connects the elements
 * to set the pipeline to the play mode.
 *
 * It also handles the key-press events based on which it sets the pipeline
 * is stopped, paused, run or closed

 *
 * argument 1:
 * 		Name: argv
 * 		purpose : takes in the path for the media we want to play.
 *
 * return -1: when there is a problem with the elements
 * return 0: when there is no problem with the configuration of the pipeline
 * ---------------------------------------------------------------------------
 */

int mediapipe::caller(gui_elem** widgets, app_state** state)
{

	// This is where the messages generated from the pipeline will be initially stored.
	GstBus* bus;

	//This will be used to store the message to be processed.
	GstMessage* message;

	// This is used to indicate whether there was change in state.
	GstStateChangeReturn ret;

	// It was declared for the termination of the while loop.
	data.terminate = FALSE;

	// No argument is passed during the initialisation of the gstreamer.

	data.req_flag->video_flag = &((*state)->video_flag);

	data.pipeline = (*state)->pipeline;

	data.source = (*state)->filesrc;


	// checking successful creation of the pipeline elements.
	if (
			!data.pipeline ||
			!data.source ||
			!data.decodebin

	) {
		g_printerr("Not all elements can be created!");
		return -1;
	}

	// all the required elements one by one added to the pipeline.

	gst_bin_add_many(

			GST_BIN (data.pipeline),
			data.source,
			data.decodebin,
			NULL

	);

	// linking the elements which have already been added to the the pipeline

	if (!gst_element_link_many(data.source, data.decodebin, NULL))
	{
		g_printerr("Elements could not be linked!");
		gst_object_unref(data.pipeline);
		return -1;
	}

	auto uri = (*((*state)->play_pointer)).c_str();

	// file://<path>
	g_object_set(data.source, "location", uri, NULL);


	(*state)->pipeline = data.pipeline;

	// the callback function is bound to the "pad-added" signal which will be invoked as soon as the signal is detected

	g_signal_connect(data.decodebin, "pad-added", G_CALLBACK (pad_added_handler), &data);
	g_signal_connect(data.decodebin, "pad-removed", G_CALLBACK (pad_removed_handler), &data);


	//	// retrieve the audio-volume element
	//		GstElement* audio_bin = gst_bin_get_by_name(GST_BIN(data.pipeline), "audio_bin");
	//		GstElement* audio_volume = gst_bin_get_by_name(GST_BIN(audio_bin), "audio-volume");
	//		audio_volume = (*state)->volume;

	//now we are setting the pipeline to the playing state
	ret = gst_element_set_state(data.pipeline, GST_STATE_PLAYING);

	if (ret == GST_STATE_CHANGE_FAILURE)
	{
		g_printerr ("Unable to set the pipeline to the playing state.\n");
		gst_object_unref (data.pipeline);
		return -1;
	}

	// A bus element is also created to the monitor the messages

	bus = gst_element_get_bus(data.pipeline);

	do
	{
		// Here we are filtering the messages we are looking for
		message = gst_bus_timed_pop_filtered(
				bus,
				GST_CLOCK_TIME_NONE,

				// Currently we are filtering only the change of state, error that arises, and end of stream messages.
				(GstMessageType) (GST_MESSAGE_STATE_CHANGED | GST_MESSAGE_ERROR | GST_MESSAGE_EOS)
		);

		if (message != nullptr)
		{
			switch(GST_MESSAGE_TYPE(message))
			{
			case GST_MESSAGE_ERROR:

				/*--------------------------------------------------------------
				 * Here we are printing the debugging information to the console
				 * -------------------------------------------------------------
				 */
				GError* gerror;
				gchar* debg_info;
				gst_message_parse_error(message, &gerror, &debg_info);
				g_printerr("Error received from the element %s : %s \n", GST_OBJECT_NAME(message->src), gerror->message);
				g_printerr("Debugging information: %s \n", debg_info? debg_info : "none");
				g_clear_error(&gerror);
				g_free(debg_info);
				break;

			case GST_MESSAGE_EOS:

				// Here we are setting the "terminate" to "TRUE" end the while loop
				g_print("EOS received from element '%s'\n", GST_OBJECT_NAME(message->src));

				g_print("Stream ended! \n");
				if((*state)->repeat== TRUE)
				{
					gst_element_set_state(data.pipeline, GST_STATE_READY);
					//						g_object_set((*state)->filesrc, "location", (*(*state)->play_pointer).c_str() , NULL);
					gst_element_set_state(data.pipeline, GST_STATE_PLAYING);
					continue;
				}
				//					data.terminate = TRUE;
				(*state)->play_pointer++;
				if((*state)->play_pointer == (*state)->playlist_vector.end())
				{
					(*state)->play_pointer = (*state)->playlist_vector.begin();
				}
				if (
						(*(*state)->play_pointer).find(".mp3") != std::string::npos ||
						(*(*state)->play_pointer).find(".wav") != std::string::npos ||
						(*(*state)->play_pointer).find(".aiff") != std::string::npos ||
						(*(*state)->play_pointer).find(".flac") != std::string::npos ||
						(*(*state)->play_pointer).find(".aac") != std::string::npos ||
						(*(*state)->play_pointer).find(".ogg") != std::string::npos ||
						(*(*state)->play_pointer).find(".wma") != std::string::npos ||
						(*(*state)->play_pointer).find(".mp4a") != std::string::npos ||
						(*(*state)->play_pointer).find(".ac3") != std::string::npos ||
						(*(*state)->play_pointer).find(".webm") != std::string::npos ||
						(*(*state)->play_pointer).find(".amr") != std::string::npos

				) {
					(*state)->video_flag = FALSE;
				} else
				{
					(*state)->video_flag = TRUE;
				}
				gst_element_set_state(data.pipeline, GST_STATE_READY);
				g_object_set((*state)->filesrc, "location", (*(*state)->play_pointer).c_str() , NULL);
				gst_element_set_state(data.pipeline, GST_STATE_PLAYING);
				for_toggle((*state));
				break;

			case GST_MESSAGE_STATE_CHANGED:

				// Here we are monitoring the change of state of the pipeline

				if (GST_MESSAGE_SRC(message) == GST_OBJECT(data.pipeline))
				{
					GstState oldstate, newstate, pendingstate;
					gst_message_parse_state_changed(message, &oldstate, &newstate, &pendingstate);
					g_print ("Pipeline state changed from %s to %s:\n",
							gst_element_state_get_name (oldstate), gst_element_state_get_name (newstate));

					(*state)->pipeline_state = newstate;

					//						g_signal_connect(data.video_window, "realize", G_CALLBACK (realize_window), (CustomData *)&data);
					//						g_signal_connect(data.video_window, "draw", G_CALLBACK (draw_window), (CustomData *)&data);
				}

				break;

			default:
				g_printerr("Unexpected Error!!\n");
				break;
			}
			gst_message_unref(message);
		}

	} while(!data.terminate);

	// As soon as thaud_bine while loop ends, we clear the memory by unrefing all the memory references
	//	gst_object_unref(bus);
	gst_element_set_state (data.pipeline, GST_STATE_NULL);
	//	gst_bin_remove_many(GST_BIN(data.pipeline), data.decodebin, data.source);
	//	gst_object_unref (data.pipeline);

	return 0;
}

void mediapipe::play(gui_elem* widgets, app_state* status)
{
	caller(&widgets, &status);
}

/*----------------------------------------------------------------------------
 *
 * fun: This functions creates all the appropriate elements which are needed to play our media
 *
 * argument 1:
 * 		Name: src
 * 		purpose : gets the reference to the decodebin
 *
 * argument 2:
 * 		Name: new_pad
 * 		purpose: gets the reference to the src pad of the decodebin
 *
 * argument 3:
 * 		Name: data
 * 		purpose: gets the reference to the custom data we are passing
 * return : void
 *
 * ---------------------------------------------------------------------------
 */

void mediapipe::pad_added_handler(GstElement *src, GstPad* new_pad, CustomData* data)
{ // @suppress("Unused static function")
	std::cout << "\n\n\n";
	GstPadLinkReturn ret;

	GstCaps *new_pad_cap = NULL;
	GstStructure *new_pad_struct = NULL;
	const gchar *new_pad_type = NULL;

	GstPad* ghost_pad = nullptr;
	GstPad* sink_pad = nullptr;

	g_print("Received new pad '%s' from '%s':\n", GST_PAD_NAME(new_pad), GST_ELEMENT_NAME(src));

	new_pad_cap = gst_pad_get_current_caps (new_pad);
	new_pad_struct = gst_caps_get_structure (new_pad_cap, 0);
	new_pad_type = gst_structure_get_name (new_pad_struct);

	// If video stream is detected then we can then this part of the function is executed
	if (g_str_has_prefix (new_pad_type, "video/x-raw"))
	{
		data->video_ob = new video_bin(&data->videobin);
		if (!data->video_ob)
		{
			g_print("Audiobin object failed! \n");
			return;
		}

		data->video_ob->linking();

		gst_bin_add(GST_BIN(data->pipeline), data->videobin);
		GstElement *text_overlay = gst_bin_get_by_name(GST_BIN(data->videobin), "video-queue");
		sink_pad = gst_element_get_static_pad(text_overlay, "sink");
		g_print("Received new pad '%s' from '%s':\n", GST_PAD_NAME(sink_pad), GST_ELEMENT_NAME(text_overlay));
		ghost_pad = gst_ghost_pad_new("sink", sink_pad);
		gst_pad_set_active (ghost_pad, TRUE);
		gst_element_add_pad (data->videobin, ghost_pad);
		gst_element_set_state(data->videobin, GST_STATE_PLAYING);
		g_print ("It has type '%s'.\n", new_pad_type);
	}
	else if (g_str_has_prefix (new_pad_type, "audio/x-raw"))
	{
		// This part is invoked when there is no videostream and we need to add audio-visualizer
		if ((*(data->req_flag->video_flag)) == FALSE)
		{
			data->visualiser = gst_element_factory_make("goom", "audio-visualiser");
			data->tee = gst_element_factory_make("tee", "audio-splitter");
			sink_pad = gst_element_get_static_pad(data->tee, "sink");
			data->queue_1 = gst_element_factory_make("queue", "audio-queue-1");
			data->queue_2 = gst_element_factory_make("queue", "audio-queue-2");
			data->audio_conv = gst_element_factory_make("audioconvert", "aud-conv");

			if (
					!data->visualiser ||
					!data->tee ||
					!data->queue_1 ||
					!data->queue_2 ||
					!data->audio_conv
			) {
				g_print("Queue 1, Queue 2, Audiovisualiser and Tee element could not be created! \n");
				return;
			}

			gst_bin_add_many(
					GST_BIN(data->pipeline),
					data->tee,
					data->queue_1,
					data->queue_2,
					data->visualiser,
					data->audio_conv,
					NULL
			);

			// Tee and Queue connections

			GstPad* req_pad_1 = gst_element_get_request_pad(data->tee, "src_%u");
			GstPad* req_pad_2 = gst_element_get_request_pad(data->tee, "src_%u");

			GstPad* queue1_pad = gst_element_get_static_pad(data->queue_1, "sink");
			GstPad* queue2_pad = gst_element_get_static_pad(data->queue_2, "sink");

			if (
					gst_pad_link (req_pad_1, queue1_pad) != GST_PAD_LINK_OK

			) {
				g_printerr("Audio tee pads failed to link with queue 1! \n");
			}

			if (
					gst_pad_link (req_pad_2, queue2_pad) != GST_PAD_LINK_OK

			) {
				g_printerr("Audio tee pads failed to link with queue 2! \n");
			}

			if (!gst_element_link_many(data->queue_1, data->audio_conv, data->visualiser, NULL))
			{
				g_print("Could not link queue_1 and wavescope! \n");
			}

			// Creation of the video sink
			data->video_ob = new video_bin(&data->videobin);

			if (!data->video_ob)
			{
				g_print("Audiobin object failed! \n");
				return;
			}

			gst_bin_add(GST_BIN(data->pipeline), data->videobin);

			data->video_ob->linking();
			GstElement *text_overlay = gst_bin_get_by_name(GST_BIN(data->videobin), "video-queue");

			// Creation of the ghostpad

			GstPad* sink_pad_conv = gst_element_get_static_pad(text_overlay, "sink");
			GstPad* src_pad_audivis = gst_element_get_static_pad(data->visualiser, "src");
			GstPad* ghost_pad_1 = gst_ghost_pad_new("sink", sink_pad_conv);
			gst_pad_set_active (ghost_pad_1, TRUE);
			gst_element_add_pad (data->videobin, ghost_pad_1);

			if (
					gst_pad_link (src_pad_audivis, ghost_pad_1) != GST_PAD_LINK_OK

			) {
				g_print("Could not link visualiser and videoconvert! \n");
			}

			//linking between audio_bin (audio-convert specifically) and queue2

			// creation of audio sink
			data->audio_ob = new audio_bin(&data->audiobin);

			if (!data->audio_ob)
			{
				g_print("Audiobin object failed! \n");
				return;
			}

			data->audio_ob->configure();
			gst_bin_add(GST_BIN(data->pipeline), data->audiobin);

			//creation of the ghost pad of the audiobin

			GstPad* queue_2_src_pad = gst_element_get_static_pad(data->queue_2, "src");

			GstElement *ad_conv = gst_bin_get_by_name(GST_BIN(data->audiobin), "audio-convert");

			GstPad* audio_bin_pad = gst_element_get_static_pad(ad_conv, "sink");

			GstPad* ghost_pad_2 = gst_ghost_pad_new("sink", audio_bin_pad);

			gst_pad_set_active (ghost_pad_2, TRUE);
			gst_element_add_pad (data->audiobin, ghost_pad_2);

			if (
					gst_pad_link (queue_2_src_pad, ghost_pad_2) != GST_PAD_LINK_OK

			) {
				g_print("Could not link queue and audioconvert! \n");
			}

		}
		else
		{
			// This part is executed when there is video stream and we also need a audiosink for the audio stream
			if ((*(data->req_flag->video_flag)) == TRUE)
			{
				data->audio_ob = new audio_bin(&data->audiobin);

				if (!data->audio_ob)
				{
					g_print("Audiobin object failed! \n");
					return;
				}

				data->audio_ob->configure();

				gst_bin_add(GST_BIN(data->pipeline), data->audiobin);

				GstElement *audio_conv = gst_bin_get_by_name(GST_BIN(data->audiobin), "audio-convert");

				if (!audio_conv)
				{
					g_printerr("Audio-convert not found! \n");
				}
				//adding a queue
				data->queue_2 = gst_element_factory_make("queue", "audio-queue");
				gst_bin_add(GST_BIN(data->pipeline), data->queue_2);

				// creation of the ghost pad
				ghost_pad = gst_element_get_static_pad(data->queue_2, "sink");
				g_print("Received new pad '%s' from '%s':\n", GST_PAD_NAME(ghost_pad), GST_ELEMENT_NAME(data->queue_2));

				GstPad* audio_convert_pad = gst_element_get_static_pad(audio_conv, "sink");
				GstPad* queue_src_pad = gst_element_get_static_pad(data->queue_2, "src");

				GstPad* ghost_pad_1 = gst_ghost_pad_new("sink", audio_convert_pad);
				gst_pad_set_active (ghost_pad_1, TRUE);
				gst_element_add_pad (data->audiobin, ghost_pad_1);

				if (gst_pad_link(queue_src_pad, ghost_pad_1) != GST_PAD_LINK_OK)
				{
					g_printerr("Queue and Audio Bin could not be added! \n");
				}
			}
			g_print ("It has type '%s'.\n", new_pad_type);
		}
	}
	else
	{
		g_print ("It has type '%s' which is not raw audio. Ignoring.\n", new_pad_type);
	}

	if (*(data->req_flag->video_flag))
	{
		ret = gst_pad_link (new_pad, ghost_pad);
	}
	else
	{
		ret = gst_pad_link (new_pad, sink_pad);
	}

	if (GST_PAD_LINK_FAILED (ret))
	{
		g_print ("Type is '%s' but link failed.\n", new_pad_type);
	}
	else
	{
		g_print ("Link succeeded (type '%s').\n", new_pad_type);
	}

	if (new_pad_cap != NULL)
		gst_caps_unref (new_pad_cap);
	if (sink_pad)
	{
		gst_object_unref (sink_pad);
	}
	return;
}


/*----------------------------------------------------------------------------
 *
 * fun: This functions clean up our pipeline of all the elements we are adding dynamically
 *
 * argument 1:
 * 		Name: src
 * 		purpose : gets the reference to the decodebin
 *
 * argument 2:
 * 		Name: new_pad
 * 		purpose: gets the reference to the src pad of the decodebin
 *
 * argument 3:
 * 		Name: data
 * 		purpose: gets the reference to the custom data we are passing
 * return : void
 *
 * ---------------------------------------------------------------------------
 */


void mediapipe::pad_removed_handler(GstElement *src, GstPad* new_pad, CustomData* data)
{
	if (data->audiobin)
	{
		gst_element_set_state(data->audiobin, GST_STATE_NULL);
		gst_bin_remove(GST_BIN(data->pipeline), data->audiobin);
		//		gst_object_unref(data->audiobin);
		delete(data->audio_ob);
		g_print("Audiobin removed! \n");
		data->audio_ob = nullptr;
		data->audiobin = nullptr;
	}

	if (data->videobin)
	{
		gst_element_set_state(data->videobin, GST_STATE_NULL);
		gst_bin_remove(GST_BIN(data->pipeline), data->videobin);
		//		gst_object_unref(data->videobin);
		delete(data->video_ob);
		data->video_ob = nullptr;
		data->videobin = nullptr;
		g_print("Videobin removed! \n");
	}

	if (*(data->req_flag->video_flag) == FALSE)
	{
		if (data->queue_1 || data->audio_conv || data->visualiser)
		{
			gst_element_unlink_many(data->queue_1, data->audio_conv, data->visualiser, nullptr);

			gst_element_set_state(data->queue_1, GST_STATE_NULL);
			gst_bin_remove(GST_BIN(data->pipeline), data->queue_1);

			gst_element_set_state(data->audio_conv, GST_STATE_NULL);
			gst_bin_remove(GST_BIN(data->pipeline), data->audio_conv);

			gst_element_set_state(data->visualiser, GST_STATE_NULL);
			gst_bin_remove(GST_BIN(data->pipeline), data->visualiser);

			data->queue_1 = nullptr;
			data->audio_conv = nullptr;
			data->visualiser = nullptr;
		}
	}

	if (data->tee)
	{
		gst_element_set_state(data->tee, GST_STATE_NULL);
		gst_bin_remove(GST_BIN(data->pipeline), data->tee);
		//		gst_object_unref(data->tee);
		g_print("Tee is removed! \n");
		data->tee = nullptr;
	}

	if (data->queue_2)
	{
		gst_element_set_state(data->queue_2, GST_STATE_NULL);
		gst_bin_remove(GST_BIN(data->pipeline), data->queue_2);
		//		gst_object_unref(data->queue_2);
		g_print("Queue 2 is removed! \n");
		data->queue_2 = nullptr;
	}
}


void for_toggle(app_state *state)
{
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(state->rewind),false);
	const char* title = g_strdup_printf("AMPLIFY Media-Player\t%s",(*state->play_pointer).c_str());
	gtk_window_set_title(GTK_WINDOW(state->temp_main_window),title);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(state->temp_mute),false);
	gtk_range_set_value(GTK_RANGE(state->temp_range), (gdouble)(1.0));
	bool ispresent=false;
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(state->temp_fav)))
	{
		for(auto i:state->fav_list_vector)
		{
			if(!strcmp(i.c_str(),(*state->play_pointer).c_str()))
			{
				g_print("Already Present\n");
				ispresent=true;
				break;
			}
		}
		if(ispresent==false)
		{
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(state->temp_fav),false);
		}
	}
	else
	{
		for(auto i:state->fav_list_vector)
		{
			if(!strcmp(i.c_str(),(*state->play_pointer).c_str()))
			{
				g_print("Present\n");
				ispresent=true;
				break;
			}
		}
		if(ispresent==true)
		{
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(state->temp_fav),true);
		}
	}
	g_printerr("CALLED\n");
}
