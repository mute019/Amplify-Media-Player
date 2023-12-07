#include <gst/gst.h>
#include <gst/pbutils/gstdiscoverer.h>
#include <gst/controller/controller.h>
#include <string>

#include "./application_state.h"

#ifndef HEADER_METADATA_H_
#define HEADER_METADATA_H_

void print_metadata(const GstTagList*, const gchar*, GtkTextBuffer*);

void metadata(GtkTextBuffer**, app_state** );

void metawindow (GtkWidget*, app_state* );

#endif /* HEADER_METADATA_H_ */
