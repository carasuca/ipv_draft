#pragma once

#include "data_i.h"
#include "config.h"

namespace ipv
{
	// general data interface
	__interface param_i : data_i
	{
		id_t value_type() const;				
	};
}