#pragma once

#include <string>

namespace meadow {
	
	std::string strf( char const * fmt, ... ) noexcept;
	char const * vas( char const * fmt, ... ) noexcept;
	
}

#define MEADOW_HEREC ::meadow::vas("%s, in %s, line %u", __PRETTY_FUNCTION__, __FILE__,  __LINE__)
