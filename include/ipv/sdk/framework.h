#pragma once

#include "../api/framework_i.h"
#include "param.h"
#include "auto_str.h"

namespace ipv
{
	// get global IPV framework instance
	IPV_API framework_i& instance();
	
	template<typename T>
	bool set(const T& t, cstr_t name = str_t())
	{
		return instance().set(param<T>(t), &param<str_t>(name));
	}

	template<typename T>
	bool get(const T& t)
	{
		return instance().set(param(t));
	}

	static
	bool update(int percent)
	{
		return instance().set(param<int>(percent), &param<wchar_t*>(L"progress"));
	}

}
