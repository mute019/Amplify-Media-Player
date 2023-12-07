#include <iostream>
#include <string>
#include <vector>
#include <gst/gst.h>
#include <gtk-3.0/gtk/gtk.h>
#include <gtk-3.0/gdk/gdk.h>
#include "header/gtk_gui.h"

std::vector<std::string> vec;

int directory_main(const char argv[])
{
	DIR *dir;
	struct dirent *ent;
	const char *dir_path = argv;// replace with actual directory path
	char file_path[1024];
	int num_files = 0;
	// initialize GStreamer
	gst_init(NULL,NULL);

	// open directory
	if ((dir = opendir(dir_path)) != NULL)
	{
		// read all files in directory
		while ((ent = readdir(dir)) != NULL && num_files < 15)
		{
			if (g_str_has_suffix( ent->d_name, ".mp3") ||
					g_str_has_suffix(ent->d_name, ".mp4") ||
					g_str_has_suffix(ent->d_name, ".avi"))
			{
				snprintf(file_path, sizeof(file_path), "%s%s", dir_path, ent->d_name);
				printf("Reading file: %s\n", file_path);
				vec.push_back (file_path);
			}
			num_files++;
		}
		closedir(dir);
	}
	else
	{
		printf("Failed to open directory: %s\n", dir_path);
		return 1;
	}
	printf("List of songs in playlist\n");
	for(auto i:vec)
	{
		g_print("%s\n",i.c_str());
	}
	return 0;
}

int main(int argc, char** argv)
{
	int len = strlen(argv[1]);
	if(len > 0 && argv[1][len-1] != '/')
	{
		std::string str = argv[1];
		vec.push_back (str);
		gtkgui *gui = new gtkgui();
		gui->main_loop(vec);
	}
	else
	{
		g_print("%s",argv[1]);
		directory_main(argv[1]);
		gtkgui *gui = new gtkgui();
		gui->main_loop(vec);
	}
	return 0;
}
