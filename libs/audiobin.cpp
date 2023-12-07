#include <unistd.h>
#include "../header/audiobin.h"

//constructor of audio_bin
audio_bin::audio_bin(GstElement** bin)
{
	gst_init(nullptr, nullptr);
	audioconvert = gst_element_factory_make("audioconvert", "audio-convert");
	volume = gst_element_factory_make("volume", "audio-volume");
	audioresample = gst_element_factory_make("audioresample", "audio-resample");
	autoaudiosink = gst_element_factory_make("autoaudiosink", "auto-audio-sink");
	if (!audioconvert || !volume || !audioresample || !autoaudiosink)
	{
		g_printerr("Failed to create one or more elements. Exiting.\n");
	}
	*bin = gst_bin_new("audio_bin");
	gst_bin_add_many(GST_BIN(*bin), audioconvert, volume, audioresample, autoaudiosink, nullptr);
}

//configure function to connect all the elements
int audio_bin::configure()
{
	if (!gst_element_link_many(audioconvert, volume, audioresample, autoaudiosink, nullptr))
	{
		g_printerr("Failed to link one or more elements. Exiting.\n");
		return -1;
	}
	gst_element_set_state(autoaudiosink, GST_STATE_PAUSED);
	return 0;
}



