#pragma once

#include "strop_ho.hh"

namespace meadow {
	
	std::string strf( char const * fmt, ... ) noexcept;
	char const * vas( char const * fmt, ... ) noexcept;
	
	#define _here ::meadow::vas("%s, in %s, line %u", __PRETTY_FUNCTION__, __FILE__,  __LINE__)
	#define _herestr ::meadow::strf("%s, in %s, line %u", __PRETTY_FUNCTION__, __FILE__,  __LINE__)
	
}
