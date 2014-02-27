#include <ipv/sdk/plugin.h>

int wmain(int, wchar_t* [])
{
	int rad = 3;
	float scale = 0.5;
	ipv::get(rad);
	ipv::get(scale);

	for (int i = 0, max = 1000000; i != max && ipv::update(i*100/max); ++i);

	return ipv::OK;
}

