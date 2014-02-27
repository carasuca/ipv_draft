#pragma once

//#include "config.h"
#include "config.h"   // defines version number
#include "framework.h" // look here for API definition
#include "image.h"     // image type used by plugins
#include "walker.h"
#include "auto_str.h"

#ifdef UNICODE
#	define IPV_TMAIN wmain
#	define IPV_CHAR wchar_t
#else // !UNICODE
#	define IPV_TMAIN main
#	define IPV_CHAR char
#endif

#define IPV_PLUGIN_MAIN __declspec(dllexport) int IPV_MAIN(int, const IPV_CHAR*[])
#define IPV_PLUGIN(...)

#ifndef IPV_NO_PLUGIN_MAIN
// export main of wmain
IPV_PLUGIN_MAIN;

#endif

namespace ipv
{
	enum result : int 
	{
		OK, Success = OK,
		Error = int(0x80000000), // errors are negative values
		Missing,
		HadToExit,
		General = -1
	};

	IPV_API const char* str(result r);

	// not used as of IPV 0.1, reserved for future use
	__interface plugin_i : data_i
	{
		result run(int = 0, TCHAR*[] = 0);
	};

	typedef result plugin_fn(int, IPV_CHAR*[]);
	typedef plugin_fn* plugin_fn_ptr;

	typedef auto_str str_t;
	typedef const auto_str& cstr;

	//str_t str(const std::string& s) { return s;}
	//str_t str(const std::wstring& s) { return s;}


	//template<typename T>
	//str_t str(const point_t<T>& p)
	//{
	//	return (oss_t() << p[0] << IPV_CHAR(' ') << p[1]);
	//}

	// convert your type to string
	template<typename T>
	str_t str(const T& t)
	{
		return str_t(t);
	}

	// convert string to your type
	template<typename T>
	str_t str(T& t, cstr s)
	{
		// ostringstream
	}

	//template<typename T>
	//bool set(const T& t, cstr name = str_t())
	//{
	//	return instance().set(str(t), name);
	//}

	template<typename T>
	bool get(T& t, cstr name = str_t())
	{
		//str_t(t)
		return instance().get(param<T>(t), &param<str_t>(name));
	}

	//template<typename T>
	//bool has(T& t, cstr name = str_t())
	//{
	//	//str_t(t)
	//	return instance().get(NULL, name);
	//}

}

// tell linker to use ipv.lib
#if !defined(IPV_NO_LIB) && !defined(IPV_EXPORTS)
#	pragma comment(lib, "ipv")
#endif
