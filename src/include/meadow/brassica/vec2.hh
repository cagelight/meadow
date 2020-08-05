#pragma once

#include "common.hh"

template <typename T> struct meadow::brassica::vec2 {

	// ================================================================
	// DATA
	// ================================================================

	using value_type = T;
	using data_type = std::array<T, 2>;	
	data_type data;

	// ================================================================
	// CONSTRUCTION
	// ================================================================
	
	inline constexpr vec2() = default;
	inline constexpr vec2(data_type v) : data { v } {};
	inline constexpr vec2(T x, T y) : data { x, y } {};
	
	explicit inline constexpr vec2(T const & v) : data { v, v } {};
	
	inline constexpr vec2(vec2 const &) = default;
	inline constexpr vec2(vec2 &&) = default;
	
	template <typename U> 
	explicit inline constexpr vec2(vec2<U> const & v) : data { static_cast<T>(v[0]), static_cast<T>(v[1]) } {}
	
	// ================================================================
	// LINEAR ALGEBRA
	// ================================================================
	
	[[nodiscard]]
	static inline constexpr T dot(vec2 const & A, vec2 const & B) {
		return A.data[0] * B.data[0] + A.data[1] * B.data[1];
	}
	[[nodiscard]]
	inline constexpr T dot(vec2 const & other) const {
		return dot(*this, other);
	}
	
	[[nodiscard]]
	inline T magnitude() const {
		return static_cast<T>( std::sqrt((data[0] * data[0]) + (data[1] * data[1])) );
	}
	
	inline T & normalize() { 
		T mag = magnitude();
		operator /= (mag);
		return mag;
	}
	
	[[nodiscard]]
	inline vec2 normalized() const {
		T mag = magnitude();
		return operator / (mag);
	}
	
	// ================================================================
	// OPERATORS
	// ================================================================
	
	// ASSIGNMENT
	inline constexpr vec2 & operator = (vec2 const &) = default;
	inline constexpr vec2 & operator = (vec2 &&) = default;
	
	// EQUIVALENCY
	[[nodiscard]]
	inline constexpr bool operator == (vec2 const & other) const { return data == other.data; }
	[[nodiscard]]
	inline constexpr bool operator != (vec2 const & other) const { return data != other.data; }
	
	// ARITHMETIC
	[[nodiscard]]
	inline constexpr vec2 operator + (T const & v) const { return { data[0] + v, data[1] + v }; }
	[[nodiscard]]
	inline constexpr vec2 operator + (vec2 const & v) const { return { data[0] + v.data[0], data[1] + v.data[1] }; }
	[[nodiscard]]
	inline constexpr vec2 operator - (T const & v) const { return { data[0] - v, data[1] - v }; }
	[[nodiscard]]
	inline constexpr vec2 operator - (vec2 const & v) const { return { data[0] - v.data[0], data[1] - v.data[1] }; }
	[[nodiscard]]
	inline constexpr vec2 operator * (T const & v) const { return { data[0] * v, data[1] * v }; }
	[[nodiscard]]
	inline constexpr vec2 operator / (T const & v) const { return { data[0] / v, data[1] / v }; }
	
	inline constexpr vec2 & operator += (T const & v) { data[0] += v; data[1] += v; return *this; }
	inline constexpr vec2 & operator += (vec2 const & v) { data[0] += v.data[0]; data[1] += v.data[1]; return *this; }
	inline constexpr vec2 & operator -= (T const & v) { data[0] -= v; data[1] -= v; return *this; }
	inline constexpr vec2 & operator -= (vec2 const & v) { data[0] -= v.data[0]; data[1] -= v.data[1]; return *this; }
	inline constexpr vec2 & operator *= (T const & v) { data[0] *= v; data[1] *= v; return *this; }
	inline constexpr vec2 & operator /= (T const & v) { data[0] /= v; data[1] /= v; return *this; }
	
	[[nodiscard]]
	inline constexpr vec2 operator - () const { return { -data[0], -data[1] }; }
	
	// ACCESSING
	[[nodiscard]]
	inline constexpr T & operator [] (size_t i) { return data[i]; }
	[[nodiscard]]
	inline constexpr T const & operator [] (size_t i) const { return data[i]; }
	
	// ================================================================
	// CONSTSTANTS
	// ================================================================
	
	static inline constexpr vec2<T> origin { 0, 0 };
};

// ================================================================
// COMMUTATIVE OPERATORS
// ================================================================

namespace meadow::brassica {
	template <typename T> [[nodiscard]]
	inline constexpr vec2<T> operator + (T const & A, vec2<T> const & B) {
		return B + A;
	}
	
	template <typename T> [[nodiscard]]
	inline constexpr vec2<T> operator * (T const & A, vec2<T> const & B) {
		return B * A;
	}
}

// ================================================================
// STL SPECIALIZATIONS
// ================================================================

template <typename T> struct std::hash<meadow::brassica::vec2<T>> {
	size_t operator () (meadow::brassica::vec2<T> const & v) const {
		return hash_combine(std::hash(v[0]), std::hash(v[1]));
	}
};
