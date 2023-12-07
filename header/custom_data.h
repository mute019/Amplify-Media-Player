#include <gst/gst.h>
#include <gtk/gtk.h>
#include <string>
#include <vector>
#include "audiobin.h"
#include "videobin.h"

#ifndef HEADER_CUSTOM_DATA_H_
#define HEADER_CUSTOM_DATA_H_

typedef struct stream_flags
{
	gboolean *video_flag;
} sfs;


/*--------------------------------------------------------
 *
 * This alias structure will store the reference to the
 * elements pipeline, filesrc and decodebin
 *
 * -------------------------------------------------------
 */

typedef struct pipedata {

	GstElement* pipeline;
	GstElement* source;
	GstElement* decodebin;
	GstElement* tee;
	GstElement* visualiser;
	GstElement* videobin;
	GstElement* audiobin;
	GstElement* audio_conv;
	GstElement* queue_1;
	GstElement* queue_2;

	audio_bin* audio_ob = nullptr;
	video_bin* video_ob = nullptr;

	sfs* req_flag = new sfs;

	GstState state;

	gboolean terminate;

} CustomData;

#endif
