#include <gst/gst.h>
#include <iostream>

#ifndef VIDEOBIN_H_
#define VIDEOBIN_H_

using namespace std;

class video_bin
{
	GstElement *queue, *videoconvert, *sink;
public:
	video_bin(GstElement** );
	int linking();
};

#endif /* VIDEOBIN_H_ */
