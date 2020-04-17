#pragma once

/*
██╗  ██╗███████╗ █████╗ ██████╗ ███████╗██████╗      ██████╗ ███╗   ██╗██╗  ██╗   ██╗
██║  ██║██╔════╝██╔══██╗██╔══██╗██╔════╝██╔══██╗    ██╔═══██╗████╗  ██║██║  ╚██╗ ██╔╝
███████║█████╗  ███████║██║  ██║█████╗  ██████╔╝    ██║   ██║██╔██╗ ██║██║   ╚████╔╝ 
██╔══██║██╔══╝  ██╔══██║██║  ██║██╔══╝  ██╔══██╗    ██║   ██║██║╚██╗██║██║    ╚██╔╝  
██║  ██║███████╗██║  ██║██████╔╝███████╗██║  ██║    ╚██████╔╝██║ ╚████║███████╗██║   
╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝╚═════╝ ╚══════╝╚═╝  ╚═╝     ╚═════╝ ╚═╝  ╚═══╝╚══════╝╚═╝   
*/

#include <iostream>
#include <functional>
#include <sstream>
#include <string>

namespace meadow {
	
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
