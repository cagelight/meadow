#pragma once

#include <iostream>
#include <functional>
#include <sstream>
#include <string>

namespace meadow {
	
	std::string strf( char const * fmt, ... ) noexcept;
	char const * vas( char const * fmt, ... ) noexcept;
	
	#define _here ::meadow::vas("%s, in %s, line %u", __PRETTY_FUNCTION__, __FILE__,  __LINE__)
	#define _herestr ::meadow::strf("%s, in %s, line %u", __PRETTY_FUNCTION__, __FILE__,  __LINE__)
	
	struct streamlogger {
		using logger_cb = std::function<void(std::string const &)>;
		logger_cb cb;
		
		inline streamlogger(std::string const & init = "", logger_cb cb = [](std::string const & str){ std::cout << str << std::endl; }) : cb(cb), str() { str << init; }
		inline ~streamlogger() { cb(str.str()); }
		
		inline streamlogger & operator << ( std::string const & other ) { str << other; return *this; }
		inline streamlogger & operator << ( char const * other ) { str << other; return *this; }
		template <typename T> inline streamlogger & operator << ( T const & other ) { str << std::to_string(other); return *this; }
	private:
		std::ostringstream str;
	};
	
	struct stringbuilder {
		inline stringbuilder & operator << ( std::string const & other ) { str << other; return *this; }
		inline stringbuilder & operator << ( char const * other ) { str << other; return *this; }
		template <typename T> inline stringbuilder & operator << ( T const & other ) { str << std::to_string(other); return *this; }
		
		operator std::string () { return str.str(); }
	private:
		std::ostringstream str;
	};
}
