#pragma once

#include "data_i.h" // basic data type for framework communication

namespace ipv
{

	// API definition for ImProVise framework
	__interface framework_i
	{
		// access parameter values, images and other data
		bool set(const data_i&, const data_i* = 0);
		bool get(data_i&, const data_i* = 0);

	};
}