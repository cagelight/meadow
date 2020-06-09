#pragma once

/*
██╗  ██╗███████╗ █████╗ ██████╗ ███████╗██████╗      ██████╗ ███╗   ██╗██╗  ██╗   ██╗
██║  ██║██╔════╝██╔══██╗██╔══██╗██╔════╝██╔══██╗    ██╔═══██╗████╗  ██║██║  ╚██╗ ██╔╝
███████║█████╗  ███████║██║  ██║█████╗  ██████╔╝    ██║   ██║██╔██╗ ██║██║   ╚████╔╝ 
██╔══██║██╔══╝  ██╔══██║██║  ██║██╔══╝  ██╔══██╗    ██║   ██║██║╚██╗██║██║    ╚██╔╝  
██║  ██║███████╗██║  ██║██████╔╝███████╗██║  ██║    ╚██████╔╝██║ ╚████║███████╗██║   
╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝╚═════╝ ╚══════╝╚═╝  ╚═╝     ╚═════╝ ╚═╝  ╚═══╝╚══════╝╚═╝   
*/

#include <ostream>
#include <sstream>
#include <string>
#include <string_view>

namespace meadow {

	struct insensitive_char_traits : public std::char_traits<char> {
		
		static bool eq(char c1, char c2) {
			return std::toupper(c1) == std::toupper(c2);
		}
		static bool lt(char c1, char c2) {
			return std::toupper(c1) <  std::toupper(c2);
		}
		static int compare(const char* s1, const char* s2, size_t n) {
			while ( n-- != 0 ) {
				if ( std::toupper(*s1) < std::toupper(*s2) ) return -1;
				if ( std::toupper(*s1) > std::toupper(*s2) ) return 1;
				++s1; ++s2;
			}
			return 0;
		}
		static char const * find(const char* s, int n, char a) {
			auto const ua (std::toupper(a));
			while ( n-- != 0 ) 
			{
				if (std::toupper(*s) == ua)
					return s;
				s++;
			}
			return nullptr;
		}
	};

	using istring = std::basic_string<char, insensitive_char_traits>;
	using istring_view = std::basic_string_view<char, insensitive_char_traits>;
	using istringstream = std::basic_stringstream<char, insensitive_char_traits>;

	static inline std::string i2s(istring const & str) { return std::string { str.data(), str.size() }; }
	static inline std::string i2s(istring_view const & str) { return std::string { str.data(), str.size() }; }
	static inline std::string_view i2sv(istring const & str) { return std::string_view { str.data(), str.size() }; }
	static inline std::string_view i2sv(istring_view const & str) { return std::string_view { str.data(), str.size() }; }
	static inline istring s2i(std::string const & str) { return istring { str.data(), str.size() }; }
	static inline istring s2i(std::string_view const & str) { return istring { str.data(), str.size() }; }
	static inline istring_view s2iv(std::string const & str) { return istring_view { str.data(), str.size() }; }
	static inline istring_view s2iv(std::string_view const & str) { return istring_view { str.data(), str.size() }; }
}

namespace std {
	
	template <> struct hash<meadow::istring> {
		inline size_t operator() (meadow::istring const & str) const {
			size_t h = 14695981039346656037UL;
			for (char c : str) {
				h ^= std::toupper(c);
				h *= 1099511628211UL;
			}
			return h;
		}
	};
	
	template <> struct hash<meadow::istring_view> {
		inline size_t operator() (meadow::istring_view const & str) const {
			size_t h = 14695981039346656037UL;
			for (char c : str) {
				h ^= std::toupper(c);
				h *= 1099511628211UL;
			}
			return h;
		}
	};
	
	static inline ostream & operator << (ostream & os, meadow::istring const & str) {
		os << std::string_view { str.data(), str.size() };
		return os;
	}
	
	static inline ostream & operator << (ostream & os, meadow::istring_view const & str) {
		os << std::string_view { str.data(), str.size() };
		return os;
	}
}
