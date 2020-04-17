#include "meadow/aeon.hh"

#include <iomanip>
#include <locale>
#include <sstream>

using namespace meadow;

static aeon const null_aeon;

// ================================================================
// GENERAL
// ================================================================

size_t aeon::size() const {
	struct conversion_visitor {
		constexpr size_t operator () (nul_t const &  ) { return 0; }
		constexpr size_t operator () (bool  const &  ) { return 1; }
		constexpr size_t operator () (int_t const &  ) { return 1; }
		constexpr size_t operator () (flt_t const &  ) { return 1; }
		inline    size_t operator () (str_t const & v) { return v.size(); }
		inline    size_t operator () (ary_t const & v) { return v.size(); }
		inline    size_t operator () (map_t const & v) { return v.size(); }
	};
	
	return std::visit(conversion_visitor {}, *value);
}

// ================================================================
// TYPE SETTERS AND UNDERLYING ACCESS
// ================================================================

// ----------------
// BOOLEAN
// ----------------
bool & aeon::boolean() {
	if (is_bool()) return get<bool>();
	else return value->emplace<bool>();
}

bool const & aeon::boolean() const {
	static constexpr bool default_value = false;
	if (is_bool()) return get<bool>();
	else return default_value;
}

// ----------------
// INTEGER
// ----------------
aeon::int_t & aeon::integer() {
	if (is_integer()) return get<int_t>();
	else return value->emplace<int_t>();
}

aeon::int_t const & aeon::integer() const {
	static constexpr int_t default_value = 0;
	if (is_integer()) return get<int_t>();
	else return default_value;
}

// ----------------
// FLOATING
// ----------------
aeon::flt_t & aeon::floating() {
	if (is_floating()) return get<flt_t>();
	else return value->emplace<flt_t>();
}

aeon::flt_t const & aeon::floating() const {
	static constexpr flt_t default_value = 0.0;
	if (is_floating()) return get<flt_t>();
	else return default_value;
}

// ----------------
// FLOATING
// ----------------
aeon::str_t & aeon::string() {
	if (is_string()) return get<str_t>();
	else return value->emplace<str_t>();
}

aeon::str_t const & aeon::string() const {
	static str_t const default_value = "";
	if (is_string()) return get<str_t>();
	else return default_value;
}

// ----------------
// ARRAY
// ----------------
aeon::ary_t & aeon::array() {
	if (is_array()) return get<ary_t>();
	else return value->emplace<ary_t>();
}

aeon::ary_t const & aeon::array() const {
	static ary_t const default_value;
	if (is_array()) return get<ary_t>();
	else return default_value;
}

// ----------------
// ARRAY
// ----------------
aeon::map_t & aeon::map() {
	if (is_map()) return get<map_t>();
	else return value->emplace<map_t>();
}

aeon::map_t const & aeon::map() const {
	static map_t const default_value;
	if (is_map()) return get<map_t>();
	else return default_value;
}

// ================================================================
// TYPE CONVERSIONS
// ================================================================

// ----------------
// BOOLEAN
// ----------------
bool aeon::as_boolean() const {
	struct conversion_visitor {
		constexpr bool operator () (nul_t const &  ) { return false; }
		constexpr bool operator () (bool  const & v) { return v; }
		constexpr bool operator () (int_t const & v) { return v; }
		constexpr bool operator () (flt_t const & v) { return v; }
		inline    bool operator () (str_t const & v) { return !v.empty() && v != "false"; }
		inline    bool operator () (ary_t const & v) { return v.size(); }
		inline    bool operator () (map_t const & v) { return v.size(); }
	};
	
	return std::visit(conversion_visitor {}, *value);
}

// ----------------
// INTEGER
// ----------------
aeon::int_t aeon::as_integer() const {
	struct conversion_visitor {
		constexpr int_t operator () (nul_t const &  ) { return 0; }
		constexpr int_t operator () (bool  const & v) { return v; }
		constexpr int_t operator () (int_t const & v) { return v; }
		constexpr int_t operator () (flt_t const & v) { return v; }
		inline    int_t operator () (str_t const & v) { return std::strtoll(v.c_str(), nullptr, 10); }
		inline    int_t operator () (ary_t const & v) { return v.size(); }
		inline    int_t operator () (map_t const & v) { return v.size(); }
	};
	
	return std::visit(conversion_visitor {}, *value);
}

// ----------------
// FLOAT
// ----------------
aeon::flt_t aeon::as_floating() const {
	struct conversion_visitor {
		constexpr flt_t operator () (nul_t const &  ) { return 0; }
		constexpr flt_t operator () (bool  const & v) { return v; }
		constexpr flt_t operator () (int_t const & v) { return v; }
		constexpr flt_t operator () (flt_t const & v) { return v; }
		inline    flt_t operator () (str_t const & v) { return std::strtod(v.c_str(), nullptr); }
		inline    flt_t operator () (ary_t const & v) { return v.size(); }
		inline    flt_t operator () (map_t const & v) { return v.size(); }
	};
	
	return std::visit(conversion_visitor {}, *value);
}

// ----------------
// STRING
// ----------------
aeon::str_t aeon::as_string() const {
	struct conversion_visitor {
		inline str_t operator () (nul_t const &  ) { return "null"; }
		inline str_t operator () (bool  const & v) { return v ? "true" : "false"; }
		inline str_t operator () (int_t const & v) { return std::to_string(v); }
		inline str_t operator () (flt_t const & v) { return std::to_string(v); }
		inline str_t operator () (str_t const & v) { return v; }
		inline str_t operator () (ary_t const &  ) { return "[array]"; }
		inline str_t operator () (map_t const &  ) { return "[map]"; }
	};
	
	return std::visit(conversion_visitor {}, *value);
}

// ================================================================
// SERIALIZATION
// ================================================================

// ----------------
// SERIALIZE JSON
// ----------------

std::string aeon::jsonify_string(str_t const & str) {
	std::ostringstream ss;
	
	ss << '"';
	for (auto c : str) {
		switch(c) {
			case '"': ss << "\\\""; break;
			case '\\': ss << "\\\\"; break;
			case '\b': ss << "\\b"; break;
			case '\f': ss << "\\f"; break;
			case '\n': ss << "\\n"; break;
			case '\r': ss << "\\r"; break;
			case '\t': ss << "\\t"; break;
			default: {
				if (c < 32) ss << "\\u" << std::hex << std::setw(4) << std::setfill('0') << static_cast<int32_t>(c);
				else ss << c;
			}
		}
	}
	ss << '"';
	return ss.str();
}

std::string aeon::serialize_json() const {
	struct conversion_visitor {
		inline std::string operator () (nul_t const &  ) { return "null"; }
		inline std::string operator () (bool  const & v) { return v ? "true" : "false"; }
		inline std::string operator () (int_t const & v) { return std::to_string(v); }
		inline std::string operator () (flt_t const & v) { std::ostringstream ss; ss << v; return ss.str(); }
		inline std::string operator () (str_t const & v) { return jsonify_string(v); }
		inline std::string operator () (ary_t const & v) {
			std::ostringstream ss;
			ss << "[";
			for (size_t i = 0; i < v.size(); i++) {
				if (i) ss << ",";
				ss << v[i].serialize_json();
			}
			ss << "]";
			return ss.str();
		}
		inline std::string operator () (map_t const & v) {
			std::ostringstream ss;
			ss << "{";
			bool first = true;
			for (auto const & [key, value] : v) {
				if (first) first = false;
				else ss << ",";
				ss << jsonify_string(key);
				ss << ':';
				ss << value.serialize_json();
			}
			ss << "}";
			return ss.str();
		}
	};
	
	return std::visit(conversion_visitor {}, *value);
}

// ----------------
// DESERIALIZE JSON
// ----------------

#define throw_eoi throw aeon::deserialize_exception {"unexpected end of input"}
#define throw_ii throw aeon::deserialize_exception {"invalid input"}

static inline void json_skip_irrelevant(char const * & cur, char const * end) {
	for (;cur < end; cur++) switch (*cur) {
		case ' ': case '\r': case '\n': case '\t':
		case ',': case ':':
			continue;
		default: return;
	}
	throw_eoi;
}

static inline aeon::str_t json_parse_string(char const * & cur, char const * end) {
	cur++;
	char const * begin = cur;
	bool escaped = false;
	
	std::ostringstream ss;
	
	for (;cur < end; cur++) {
		char c = *cur;
		if (c < 32) throw_ii;
		switch (c) {
			case 'b': if (escaped)  { ss << '\b'; escaped = false; } else ss << 'b'; continue;
			case 'f': if (escaped)  { ss << '\f'; escaped = false; } else ss << 'f'; continue;
			case 'n': if (escaped)  { ss << '\n'; escaped = false; } else ss << 'n'; continue;
			case 'r': if (escaped)  { ss << '\r'; escaped = false; } else ss << 'r'; continue;
			case 't': if (escaped)  { ss << '\t'; escaped = false; } else ss << 't'; continue;
			case '\\': if (escaped) { ss << '\\'; escaped = false; } else escaped = true; continue;
			case '"': if (escaped)  { ss << '"';  escaped = false; continue; } else break;
			default: ss << c; continue;
		}
		break;
	}
	if (cur >= end || cur == begin || escaped) throw_eoi;
	cur++;
	
	return ss.str();
};

static inline aeon deserialize_json_r(char const * & cur, char const * end) {
	if (cur == end) throw_eoi;
	
	static constexpr auto validate_null = [](char const * & cur, char const * end) -> aeon {
		if (cur+3 >= end) throw_eoi;
		if (*(cur+1) != 'u' || *(cur+2) != 'l' || *(cur+3) != 'l') throw_ii;
		cur += 4;
		return {};
	};
	
	static constexpr auto validate_true = [](char const * & cur, char const * end) -> aeon {
		if (cur+3 >= end) throw_eoi;
		if (*(cur+1) != 'r' || *(cur+2) != 'u' || *(cur+3) != 'e') throw_ii;
		cur += 4;
		return true;
	};
	
	static constexpr auto validate_false = [](char const * & cur, char const * end) -> aeon {
		if (cur+4 >= end) throw_eoi;
		if (*(cur+1) != 'a' || *(cur+2) != 'l' || *(cur+3) != 's' || *(cur+4) != 'e') throw_ii;
		cur += 5;
		return false;
	};
	
	static constexpr auto parse_numerical = [](char const * & cur, char const * end) -> aeon {
		char const * begin = cur;
		char const * exp = nullptr;
		bool is_floating = false;
		
		for (;cur != end; cur++) {
			switch (*cur) {
				case '.': 
					if (is_floating) throw_ii;
					is_floating = true;
					continue;
				case 'e': case 'E':
					if (exp) throw_ii;
					exp = cur;
					continue;
				case '+': case '-': 
					if (cur != begin && !(exp && cur == exp + 1)) throw_ii;
					[[fallthrough]];
				case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
					continue;
				default: break;
			}
			break;
		}
		if (cur == begin) throw_eoi;

		std::string numstr {begin, cur};
		if (is_floating || exp) return std::strtod(numstr.c_str(), nullptr);
		else return std::strtoll(numstr.c_str(), nullptr, 10);
	};
	
	static constexpr auto parse_array = [](char const * & cur, char const * end) -> aeon {
		cur++;
		aeon ret; ret.array();
		for (;cur < end;) {
			json_skip_irrelevant(cur, end);
			if (*cur == ']') {
				cur++;
				return ret;
			}
			ret.array().push_back(deserialize_json_r(cur, end));
		}
		throw_eoi;
	};
	
	static constexpr auto parse_map = [](char const * & cur, char const * end) -> aeon {
		cur++;
		aeon ret; ret.map();
		for (;cur < end;) {
			json_skip_irrelevant(cur, end);
			if (*cur == '}') {
				cur++;
				return ret;
			}
			aeon::str_t str = json_parse_string(cur, end);
			json_skip_irrelevant(cur, end);
			ret[str] = deserialize_json_r(cur, end);
		}
		throw_eoi;
	};
	
	json_skip_irrelevant(cur, end);
	for (;cur < end; cur++) switch(*cur) {
		case 'n': return validate_null(cur, end);
		case 't': return validate_true(cur, end);
		case 'f': return validate_false(cur, end);
		
		case '-':
		case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
		return parse_numerical(cur, end);
		
		case '"': return json_parse_string(cur, end);
		case '[': return parse_array(cur, end);
		case '{': return parse_map(cur, end);
		
		default: throw_ii;
	}
	
	throw_eoi;
}

aeon aeon::deserialize_json(char const * cur, char const * end) {
	return deserialize_json_r(cur, end);
}

// ================================================================
// OPERATORS
// ================================================================

// ----------------
// ARRAY
// ----------------
aeon & aeon::operator [] (size_t idx) {
	auto & ary = array();
	if (idx >= ary.size()) ary.resize(idx + 1);
	return ary[idx];
}

aeon const & aeon::operator [] (size_t idx) const {
	auto const & ary = array();
	if (idx >= ary.size()) return null_aeon;
	return ary[idx];
}

// ----------------
// MAP
// ----------------
aeon & aeon::operator [] (str_t const & key) {
	return map()[key];
}

aeon const & aeon::operator [] (str_t const & key) const {
	auto const & val = map();
	auto i = val.find(key);
	if (i == val.end()) return null_aeon;
	else return i->second;
}

// ================================================================
