#pragma once

#include "config.h"
#include "plugin.h"

namespace ipv
{
	// extended framework interface, 
	// e.g. allows communication between instances
	__interface framework_impl_i : framework_i
	{
		virtual int run(int, IPV_CHAR*[]) = 0;
	};

	// lazy initialization of the current framework
	// used by instance() in plugins to get global reference
	// overwritten by back-end (cli/gui/web) implementation
	enum inject_op {inject_read, inject_push, inject_pop};
	IPV_API framework_impl_i* inject(inject_op op = inject_read, framework_impl_i* = 0);

	struct runtime_exception : std::exception
	{
		runtime_exception(result r) : std::exception(str(r), r) {}

		// common way to throw exceptions around
		static void raise(result r = Error)  //throw(runtime_exception)
		{
			throw runtime_exception(r);
		}
	};

	struct framework_impl_base : framework_impl_i
	{
		framework_impl_base() {inject(inject_push, this);}
		~framework_impl_base() {inject(inject_pop);}
	};
}