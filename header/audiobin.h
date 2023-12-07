#ifndef AUDIOBIN_H_
#define AUDIOBIN_H_

#include <gst/gst.h>

class audio_bin
{
	GstElement* audioconvert;
	GstElement* volume;
	GstElement* audioresample;
	GstElement* autoaudiosink;
public:
	audio_bin(GstElement** );
	int configure();
};

#endif /* AUDIO_H_ */
