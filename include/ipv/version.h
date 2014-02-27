#pragma once

// Framework version
// plugins automatically export their build versions
// major version number is incremented when framework interface changes
// minor number is incremented otherwise

#define IPV_VERSION_MINOR 2
#define IPV_VERSION_MAJOR 0


#define _IPV_VERSION_SHIFT 0x100
#define IPV_VERSION (IPV_VERSION_MAJOR*_IPV_VERSION_SHIFT + IPV_VERSION_MINOR)
