#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace meadow::aeon {
	struct object;
	
	using nul_t = std::monostate;
	using int_t = int_fast64_t;
	using flt_t = double;
	using str_t = std::string;
	using ary_t = std::vector<object>;
	using map_t = std::unordered_map<str_t, object>;
}

struct meadow::aeon::object final {
private:
	using variant_t = std::variant<nul_t, bool, int_t, flt_t, str_t, ary_t, map_t>;
public:
	std::unique_ptr<variant_t> value;
	
	// ================================================================
	// CONSTRUCTORS
	// ================================================================
	
	inline object() : value(std::make_unique<variant_t>()) {}
	template <typename T, std::enable_if_t<std::is_same<T, bool>::value, int> = 0>
	inline object(bool const & v) : value(std::make_unique<variant_t>(v)) {}
	template <typename T, std::enable_if_t<std::is_integral<T>::value, int> = 0>
	inline object(T const & v) : value(std::make_unique<variant_t>(static_cast<int_t>(v))) {}
	template <typename T, std::enable_if_t<std::is_floating_point<T>::value, int> = 0>
	inline object(T const & v) : value(std::make_unique<variant_t>(static_cast<flt_t>(v))) {}
	inline object(str_t const & v) : value(std::make_unique<variant_t>(v)) {}
	inline object(ary_t const & v) : value(std::make_unique<variant_t>(v)) {}
	inline object(map_t const & v) : value(std::make_unique<variant_t>(v)) {}
	
	inline object(object const & other) : value(std::make_unique<variant_t>(*other.value)) {}
	inline object(object && other) : value(std::make_unique<variant_t>(std::move(*other.value))) {}
	
	// ================================================================
	// GENERAL
	// ================================================================
	
	[[nodiscard]] size_t size() const;
	
	// ================================================================
	// TYPE CHECKS
	// ================================================================
	
	[[nodiscard]] inline bool is_null()    const { return holds<nul_t>(); }
	[[nodiscard]] inline bool is_bool()    const { return holds<bool>(); } 
	[[nodiscard]] inline bool is_integer() const { return holds<int_t>(); }
	[[nodiscard]] inline bool is_floating()   const { return holds<flt_t>(); }
	[[nodiscard]] inline bool is_string()  const { return holds<str_t>(); }
	[[nodiscard]] inline bool is_array()   const { return holds<ary_t>(); }
	[[nodiscard]] inline bool is_map()     const { return holds<map_t>(); }
	
	// ================================================================
	// TYPE SETTERS AND UNDERLYING ACCESS
	// ================================================================
	
	// Type mismatches produce a default fixed value in const calls, otherwise new type emplacement without conversion
	
	bool & boolean();
	[[nodiscard]] bool const & boolean() const;
	int_t & integer();
	[[nodiscard]] int_t const & integer() const;
	flt_t & floating();
	[[nodiscard]] flt_t const & floating() const;
	str_t & string();
	[[nodiscard]] str_t const & string() const;
	ary_t & array();
	[[nodiscard]] ary_t const & array() const;
	map_t & map();
	[[nodiscard]] map_t const & map() const;
	
	// ================================================================
	// TYPE CONVERSIONS
	// ================================================================
	
	// Unlike above, type mismatches can convert values in reasonable cases
	
	[[nodiscard]] bool as_boolean() const;
	[[nodiscard]] int_t as_integer() const;
	[[nodiscard]] flt_t as_floating() const;
	[[nodiscard]] str_t as_string() const;
	
	// ================================================================
	// SERIALIZATION
	// ================================================================
	
	struct deserialize_exception : public std::exception {
		deserialize_exception() = delete;
		deserialize_exception(std::string const & msg) : m_what(msg) {}
		virtual char const * what() const noexcept override { return m_what.c_str(); }
	private:
		std::string m_what;
	};
	
	[[nodiscard]] static std::string jsonify_string(std::string const & str);
	
	[[nodiscard]] std::string serialize_json() const;
	[[nodiscard]] static object deserialize_json(char const * begin, char const * end);
	
	[[nodiscard]] inline static object deserialize_json(std::string const & str) { return deserialize_json(str.c_str(), str.c_str() + str.size()); }
	
	// ================================================================
	// OPERATORS
	// ================================================================
	
	inline object & operator = (object const & other) { *value = *other.value; return *this; }
	inline object & operator = (object && other) { *value = std::move(*other.value); return *this; }
	
	template <typename T, std::enable_if_t<std::is_same<T, bool>::value, int> = 0>
	inline object & operator = (bool const & v) { boolean() = v; return *this; }
	template <typename T, std::enable_if_t<std::is_integral<T>::value, int> = 0>
	inline object & operator = (T const & v) { integer() = static_cast<int_t>(v); return *this; }
	template <typename T, std::enable_if_t<std::is_floating_point<T>::value, int> = 0>
	inline object & operator = (T const & v) { floating() = static_cast<flt_t>(v); return *this; }
	inline object & operator = (str_t const & v) { string() = v; return *this; }
	
	[[nodiscard]] object & operator [] (size_t);
	[[nodiscard]] object const & operator[] (size_t) const;
	
	[[nodiscard]] object & operator [] (str_t const &);
	[[nodiscard]] object const & operator [] (str_t const &) const;
	
	[[nodiscard]] bool operator == (object const & other) const { return *value == *other.value; }
	
	template <typename T, std::enable_if_t<std::is_same<T, bool>::value, int> = 0>
	[[nodiscard]] bool operator == (bool const & v) const { return is_bool() ? v == get<bool>() : false; }
	template <typename T, std::enable_if_t<std::is_integral<T>::value, int> = 0>
	[[nodiscard]] bool operator == (T const & v) const {
		if (is_integer()) return static_cast<int_t>(v) == get<int_t>();
		else if (is_floating()) return static_cast<flt_t>(v) == get<flt_t>();
		else return false;
	}
	template <typename T, std::enable_if_t<std::is_floating_point<T>::value, int> = 0>
	[[nodiscard]] bool operator == (T const & v) const {
		if (is_floating()) return static_cast<flt_t>(v) == get<flt_t>();
		else if (is_integer()) return static_cast<flt_t>(v) == static_cast<flt_t>(get<int_t>());
		else return false;
	}
	[[nodiscard]] bool operator == (str_t const & v) const { return is_string() ? v == get<str_t>() : false; }
	
private:
	template <typename T>
	[[nodiscard]] inline bool holds() const { return std::holds_alternative<T>(*value); }
	template <typename T>
	[[nodiscard]] inline T & get() { return std::get<T>(*value); }
	template <typename T>
	[[nodiscard]] inline T const & get() const { return std::get<T>(*value); }
};
