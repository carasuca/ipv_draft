// ipv.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "ipv/internal.h"

#include <stack> // framework injection stack

using namespace std;

// TODO: move to internal.h or inernal_mt.h
#ifdef IPV_MT_SYNCHRONIZE
#define IPV_MT_DEADLOCK_TIMEOUT 2000

namespace ipv
{
	struct lock 
	{ // TODO: simplify things by allowing multiple calls
		// detect deadlocks by timeout
		lock() {}
		bool get(bool exclusive = true, int ms = 0) {}
		bool release() {}
	};
	struct guard {guard(lock&){}};
}
#	define IPV_MT_LOCK(LockName) ipv::lock LockName
// does something like: ipv::guard __ipv_mt_guard42(lock_obj, additional_params)
#	define IPV_MT_GUARD(LockName, ...) ipv::guard __ipv_mt_guard ## __LINE__(LockName, __VA_ARGS__)
#else
#	define IPV_MT_LOCK(...)
#	define IPV_MT_GUARD(...)
#endif

namespace ipv
{
	typedef IPV_CHAR char_t;

	framework_i& get_dummy()
	{
		static struct : framework_i
		{
			bool set(const data_i&, const data_i* = 0) {return false;}
			bool get(data_i&, const data_i* = 0) {return false;}
		} dummy;
		return dummy;
	}

#ifdef IPV_USE_CLI_AS_DEFAULT
	__declspec(dllimport) framework_i& get_cli();
#endif

	framework_i& get_default()
	{
#ifdef IPV_USE_CLI_AS_DEFAULT
		return get_cli();
#else
		return get_dummy();
#endif
	}

	framework_impl_i* inject(inject_op op, framework_impl_i* new_framework)
	{
		static std::stack<framework_impl_i*> impl_stack;
		switch (op)
		{
			// TODO: check if it is actually popping the correct framework
		case inject_pop: impl_stack.pop(); break;
		case inject_push: impl_stack.push(new_framework); break;
		}

		return impl_stack.empty() ? 0 : impl_stack.top();

		// DONE: convert to stack
		// TODO: add lock guard for multithreaded version
		// watch out for possible deadlocks!
		//static framework_impl* current = new_framework;
		//if (overwrite) current = new_framework;
		//return current;
	}

	framework_i& instance()
	{
		framework_i* current = inject();
		return current ? *current : get_default();
	}

	const char* str(result r)
	{
		// TODO: add nice messages
		switch (r)
		{
		case OK: return "Success";
		}

		if (r > 0) return "Some success";
		if (r < 0) return "Some error";
		return "No idea";
	}

	runtime_exception::runtime_exception(result r) : std::exception(str(r), r) {}

	void runtime_exception::raise(result r)
	{
		throw runtime_exception(r);
	}

}

