#pragma once

// static IPV functions available as a library

#include "config.h"

#include "image_i.h"
#include "xml_i.h"

namespace ipv
{
	IPV_API image_i* create_image(size_t w, size_t h, size_t c, depth_t = unknown);
	IPV_API image_i* load_image(const IPV_CHAR path[], depth_t = unknown);

	IPV_API bool load_image(const IPV_CHAR path[], image_i&);
	IPV_API bool save_image(const IPV_CHAR path[], const image_i&);

	IPV_API xml_i* create_xml();
	IPV_API xml_i* load_xml(const IPV_CHAR path[]);

	IPV_API bool   save_xml(const IPV_CHAR path[], const xml_i&);
}
