#pragma once

#include "../api/param_i.h"
#include "auto_str.h"

namespace ipv
{
	template<typename T>
	struct param : param_i, auto_str
	{
		param(const T& t) : _cptr(&t), _ptr(0), auto_str(t) {}
		param(T& t) : _ptr(&t), _cptr(&t), auto_str(t) {}

		virtual id_t value_type() const
		{
			return ipv::type_id<T>();
		}

		virtual bool data(const void* p, size_t s)
		{
			if (_ptr && p && s == sizeof(T))
			{
				*_ptr = *reinterpret_cast<const T*>(p);
				return true;
			}

			return false;
		}

		virtual const void* data() const
		{
			return c_str();
		}

		virtual size_t bytes() const
		{
			return length()*sizeof(wchar_t);
		}

		virtual id_t type_id() const
		{
			return ipv::type_id<param_i>();
		}


		id_t _value_type;

		T* _ptr;
		const T* _cptr;
	};
}