#include <iostream>
#include <unistd.h>

#include "../header/videobin.h"

/*
 * fun: constructor with no arguments
 *
 * Initalizing all the class elements
 */
video_bin::video_bin(GstElement** bin)
{
	gst_init(nullptr, nullptr);

	queue = gst_element_factory_make("queue", "video-queue");

	videoconvert = gst_element_factory_make("videoconvert", "videoconvert");
	sink = gst_element_factory_make("xvimagesink", "sink");

	*bin = gst_bin_new("video_bin");
	gst_bin_add_many(GST_BIN(*bin), queue, videoconvert, sink, NULL);
}

/*
 * fun: linking of GstElements
 *
 */

int video_bin::linking()
{
	if(!gst_element_link_many(queue, videoconvert, sink,NULL))
	{
		cout<<"error in linking" << std::endl;
		return -1;
	}
	g_object_set(G_OBJECT(sink), "async", FALSE, NULL);
	return 0;
}
