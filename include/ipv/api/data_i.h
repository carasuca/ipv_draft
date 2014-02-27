#pragma once

#include "config.h"

namespace ipv
{
	typedef size_t id_t;

	// general data interface
	__interface data_i
	{
		bool data(const void*, size_t); // copy data
		const void* data() const;
		size_t bytes() const;
		id_t type_id() const;
	};

	// helper base data class
	template<typename T>
	struct data_t : data_i
	{
		virtual const void* data() const {return NULL;}
		virtual bool data(const void*, size_t) const {return false;}
		virtual size_t bytes() const {return 0;}
		virtual size_t type_id() {return ipv::type_id<T>();}
	};

	// used for identifying data_i derived classes
	//template<typename T> size_t type_id();
	// BUG: arrgh, rtti dummy
	template<typename T> size_t type_id() {return typeid(T).hash_code();}
}