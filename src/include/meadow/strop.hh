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
		using logger_cb = std::function<void(std::string_view)>;
		logger_cb cb;
		
		inline streamlogger(logger_cb cb = [](std::string_view str){ std::cout << str << std::endl; }) : cb(cb), m_ss() { }
		inline ~streamlogger() { cb(m_ss.str()); }
		
		inline streamlogger & operator << ( std::string const & other ) { m_ss << other; return *this; }
		inline streamlogger & operator << ( char const * other ) { m_ss << other; return *this; }
		
		template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0> 
		inline streamlogger & operator << ( T const & other ) { m_ss << std::to_string(other); return *this; }
		
		inline std::string str() const { return m_ss.str(); }
		operator std::string () const { return m_ss.str(); }
	private:
		std::ostringstream m_ss;
	};
	
	struct stringbuilder {
		inline stringbuilder & operator << ( std::string const & other ) { m_ss << other; return *this; }
		inline stringbuilder & operator << ( char const * other ) { m_ss << other; return *this; }
		
		template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0> 
		inline stringbuilder & operator << ( T const & other ) { m_ss << std::to_string(other); return *this; }
		
		inline std::string str() const { return m_ss.str(); }
		operator std::string () const { return m_ss.str(); }
	private:
		std::ostringstream m_ss;
	};
}

#define MEADOW_HERE (meadow::stringbuilder {} << __PRETTY_FUNCTION__ << ", in " << __FILE__ << ", line " << __LINE__).str()
