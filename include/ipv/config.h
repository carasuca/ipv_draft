#pragma once

// IPV configuration header
// modify it only if you are building ipv library from source
// define the following macros to change compilation conditions

// don't define basic char type
#define IPV_NO_CHAR
#undef IPV_NO_CHAR

// don't automatically set linker dependency
//#define IPV_NO_LIB_DEPENDENCY
//#undef IPV_NO_LIB_DEPENDENCY


#ifndef IPV_NO_CHAR
#	if defined(TCHAR)
#		define IPV_CHAR TCHAR
#	elif defined(UNICODE)
#		define IPV_CHAR wchar_t
#	else 
#		define IPV_CHAR char
#	endif // TCHAR
#endif //IPV_NO_CHAR

#define IPV_TXT(STR) L##STR
#define IPV_UNICODE

// indicates that plugins can execute in a multithreaded environment
// additional synchronization will be used in framework implementations
#define IPV_MULTITHREADED

// when ipv.lib is linked, it can provide default cli interface
// you can change this not defining the following macro
// TODO: fix mutual dependency
//#define IPV_USE_CLI_AS_DEFAULT

// prevent automatic exports of main/version/...
// (useful for building libs)
#define IPV_DONT_PLUG_ME_IN
#undef IPV_DONT_PLUG_ME_IN


#ifndef TCHAR
#	define TCHAR wchar_t
#endif

#ifndef IPV_STATIC_LIB
#	ifdef IPV_EXPORTS
#		define IPV_API __declspec(dllexport)
#	else
#		define IPV_API __declspec(dllimport)
#	endif
#else // IPV_STATIC_LIB
#define IPV_API
#endif
