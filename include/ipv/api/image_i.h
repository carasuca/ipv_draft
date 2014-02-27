#pragma once

#include "data_i.h"

// image depth defined by bits per channel
enum depth_t: size_t 
{unknown, depth1bit, depth8bit = 8, depth16bit = 16, depth32bit = 32, depth32f};

namespace ipv
{
	__interface image_i : data_i
	{
		size_t channels() const;
		size_t width() const;
		size_t height() const;
		depth_t depth() const;
		// resize and copy data
		bool copy(const image_i&);
		//void resize(size_t w, size_t h, size_t c = 0);
	};

}
