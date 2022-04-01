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
#include <vector>

namespace meadow {
	
	// ================================
	// SPLITTING
	// ================================
	
	template <typename T, typename V = T::value_type>
	std::vector<T> split(T const & dat, V const & delim, bool cull_empty = true) {
		std::vector<T> out;
		
		auto b = dat.begin(), e = b;
		
		for (;e != dat.end();e++) {
			if (*e == delim) {
				if (!cull_empty || e > b)
					out.emplace_back(b, e);
				b = e + 1;
			}
		}
		
		if (e > b)
			out.emplace_back(b, e);
		else if (!cull_empty) {
			out.emplace_back(e, e);
		}
		
		return out;
	}
	
	// ================================
	// MONSTROSITIES
	// ================================
	
	struct stringbuilder {
		inline stringbuilder & operator << ( std::string_view const & other ) { m_ss << other; return *this; }
		
		template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0> 
		inline stringbuilder & operator << ( T const & other ) { m_ss << std::to_string(other); return *this; }
		
		inline std::string str() const { return m_ss.str(); }
		operator std::string () const { return m_ss.str(); }
	private:
		std::ostringstream m_ss;
	};
	
	#define MEADOW_HERE (meadow::stringbuilder {} << __PRETTY_FUNCTION__ << ", in " << __FILE__ << ", line " << __LINE__).str()
	
	struct streamlogger {
		using logger_cb = std::function<void(std::string_view, std::string_view)>;
		logger_cb cb;
		
		inline streamlogger(std::string here_str = "", logger_cb cb = [](std::string_view here, std::string_view str){
			if (here.size()) {
				std::cout << "[" << here << "] -- " << str << std::endl;
			} else
				std::cout << str << std::endl;
		}) : cb(cb), m_ss(), m_here(here_str) { }
		
		inline ~streamlogger() { cb(m_here, m_ss.str()); }
		
		inline streamlogger & operator << ( std::string_view other ) { m_ss << other; return *this; }
		
		template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0> 
		inline streamlogger & operator << ( T const & other ) { m_ss << std::to_string(other); return *this; }
		
		inline std::string str() const { return m_ss.str(); }
		operator std::string () const { return m_ss.str(); }
	private:
		std::ostringstream m_ss;
		std::string m_here;
	};
}
