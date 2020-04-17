#pragma once

/*
██╗  ██╗███████╗ █████╗ ██████╗ ███████╗██████╗      ██████╗ ███╗   ██╗██╗  ██╗   ██╗
██║  ██║██╔════╝██╔══██╗██╔══██╗██╔════╝██╔══██╗    ██╔═══██╗████╗  ██║██║  ╚██╗ ██╔╝
███████║█████╗  ███████║██║  ██║█████╗  ██████╔╝    ██║   ██║██╔██╗ ██║██║   ╚████╔╝ 
██╔══██║██╔══╝  ██╔══██║██║  ██║██╔══╝  ██╔══██╗    ██║   ██║██║╚██╗██║██║    ╚██╔╝  
██║  ██║███████╗██║  ██║██████╔╝███████╗██║  ██║    ╚██████╔╝██║ ╚████║███████╗██║   
╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝╚═════╝ ╚══════╝╚═╝  ╚═╝     ╚═════╝ ╚═╝  ╚═══╝╚══════╝╚═╝   
*/

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

}

namespace std {
	
	template <> struct hash<meadow::istring> {
		size_t operator() (meadow::istring const & str) const {
			size_t h = 14695981039346656037UL;
			for (char c : str) {
				h ^= std::toupper(c);
				h *= 1099511628211UL;
			}
			return h;
		}
	};
	
	template <> struct hash<meadow::istring_view> {
		size_t operator() (meadow::istring_view const & str) const {
			size_t h = 14695981039346656037UL;
			for (char c : str) {
				h ^= std::toupper(c);
				h *= 1099511628211UL;
			}
			return h;
		}
	};
	
}
