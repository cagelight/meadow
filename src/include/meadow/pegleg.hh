#pragma once

#include <cstdint>
#include <unordered_map>
#include <variant>

namespace meadow::pegleg {
	
	using Variable = std::variant<std::monostate, int64_t>;
	
	struct Scope {
		
		std::unordered_map<std::string, Variable> vars;
		
	};
	
	struct State {
		
		Scope root_scope;
		
	};
	
}
