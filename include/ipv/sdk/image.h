#pragma once

// ipv::image implements image_i and also data_i, so it can be used in a plugin
// and passed to the ipv framework
#include "../api/image_i.h"

// when this header is included, the preceding macro configuration should
// control the functionality image IO
#ifdef IPV_USE_IMAGE_IO
#include "image_io.h"
#endif

// ipv::image uses vector for pixel buffer (can have local implementation)
// local header is included to depend conditionally on std::vector or ipv::vector
// DONE: forget it, just use std::vector
#include <vector>

// for type conversion helpers
#include <type_traits>

// ipv::image template is defined here, inline! (header only)
// not shared nor passed over dll boundaries
// optionally, can use local ipv::vector definition

namespace ipv
{

	// typical specification of common image formats
	template<typename> struct yaimage;
	typedef yaimage<bool> mask;
	typedef yaimage<char> image8;
	typedef yaimage<short> image16;
	typedef yaimage<int> image32;
	typedef yaimage<float> image32f;

	// helper templates defining image scalar types
	template<typename> struct yaimage_types;

	// helper templates for switching between types
	template<typename> struct yaimage_types;
	template<depth_t> struct depth_to_type;

	// helper function to get depth enum from type
	template<typename> depth_t type2depth();

	template<typename T>
	struct yaimage : image_i, std::vector<typename yaimage_types<T>::type>
	{

		yaimage(size_t width    = 0, size_t height = 1, 
			size_t channels = 1, const T& t    = T())
			: vector(width*height*channels, t)
			, _width(width)
			, _channels(channels)
		{}

		// change image size and channel count
		// leaving channels as 0 keeps previous count
		void resize(size_t width, size_t height, size_t channels = 0)
		{
			_width = width; 
			if (channels) _channels = channels;
			vector::resize(width*height*channels);
		}

		// copy pixel values to buffer without conversion,
		// doesn't resize or check count <= size()
		template <typename U>
		void copy_n(U first, size_t count)
		{
			for (auto it = begin(); s; ++first, ++it) *it = *first;
		}
		// copy pixel values to buffer without conversion,
		template <typename U, typename Fn>
		void copy_n(U first, size_t count, Fn fn)
		{
			for (auto it = begin(); s; ++first, ++it) *it = Fn(*first);
		}

		// access pixels as value arrays
		// doesn't check index, throws out_of_range
		T* pixel(size_t i, size_t j) {return &at(_channels*(i + j*_width));}

		// access pixels or a specific channel
		// doesn't check index, throws out_of_range
		T* operator ()(size_t i, size_t j) { return pixel(i, j);}
		T& operator ()(size_t i, size_t j, size_t c) { return pixel(i, j)[c];}

		bool convert(const image_i& im)
		{
#			ifdef IPV_USE_IMAGE_IO
			return convert_image(im, *this);
#			endif

			return false;
		}

		//
		// image_i implementation
		//
		size_t channels() const {return _channels;}
		size_t width() const { return _width; }
		size_t height() const { return _width ? size() / _width : 0;}
		depth_t depth() const { return type2depth<T>(); }
		bool copy(const image_i& im)
		{
			// adjust target size 
			resize(im.width(), im.height(), im.channels());
			if (empty()) return true; // annihilation is a success

			// simple case of image update
			if (im.bytes() == bytes() && im.depth() == depth())
				return data(im.data(), im.bytes());

#			ifdef IPV_USE_IMAGE_IO
			// use pixel converter (part of IPV API)
			return convert_image(im, *this, &front());
#			endif
			return false;
		}

		//
		// data_i implementation
		//

		// may return excessive bytes after last row
		virtual size_t bytes() const { return size()*sizeof(T);}
		virtual const void* data() const { return empty() ? 0 : &front(); }
		virtual bool data(const void* ptr, size_t bytes_)
		{
			// doesn't check input
			return !!memcpy_s(&front(), bytes(), ptr, bytes_);
		}

		virtual id_t type_id() const
		{
			return ipv::type_id<image_i>();
		}


	protected: //members

		size_t _width, _channels;
	};

	template<typename T>
	depth_t type2depth()
	{
		switch (sizeof(T))
		{
		case 1: return std::is_same<T, bool>::value ? depth1bit : depth8bit;
		case 2: return depth16bit;
		case 4: return std::is_floating_point<T>::value ? depth32f : depth32bit;
		default: return unknown;
		}
	}

	template<> struct depth_to_type<depth1bit> {typedef bool type;};
	template<> struct depth_to_type<depth8bit> {typedef char type;};
	template<> struct depth_to_type<depth16bit> {typedef short type;};
	template<> struct depth_to_type<depth32bit> {typedef int type;};
	template<> struct depth_to_type<depth32f> {typedef float type;};

	// don't use vector<bool> - no buffer access
	template<> struct yaimage_types<bool> { typedef char type; };
	template<typename T> struct yaimage_types { typedef T type; };

	void yaimage_test();
}
