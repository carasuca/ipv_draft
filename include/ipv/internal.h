#include "config.h"
#include "api/framework_i.h"
#include "sdk/plugin.h"

namespace ipv
{
	// extended framework interface, 
	// e.g. allows communication between instances
	__interface framework_impl_i : framework_i
	{
		int run(int, IPV_CHAR*[]);
	};

	enum inject_op {inject_read, inject_push, inject_pop};

	// lazy initialization of the current framework
	// used by instance() in plugins to get global reference
	// overwritten by back-end (cli/gui/web) implementation
	IPV_API framework_impl_i* inject(inject_op op = inject_read, framework_impl_i* = 0);

	struct runtime_exception : std::exception
	{
		runtime_exception(result r);

		// common way to throw exceptions around
		IPV_API static void raise(result r = Error);  //throw(runtime_exception)
	};

	struct framework_impl_base : framework_impl_i
	{
		framework_impl_base() {inject(inject_push, this);}
		~framework_impl_base() {inject(inject_pop);}
	};
}