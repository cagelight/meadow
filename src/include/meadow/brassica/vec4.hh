#pragma once

#include "common.hh"

template <typename T> struct meadow::brassica::vec4 {

	// ================================================================
	// DATA
	// ================================================================

	using value_type = T;
	using data_type = std::array<T, 4>;	
	data_type data;

	// ================================================================
	// CONSTRUCTION
	// ================================================================
	
	inline constexpr vec4() = default;
	inline constexpr vec4(data_type v) : data { v } {};
	inline constexpr vec4(T x, T y, T z, T w) : data { x, y, z, w } {};
	
	explicit inline constexpr vec4(T const & v) : data { v, v, v, v } {};
	
	inline constexpr vec4(vec4 const &) = default;
	inline constexpr vec4(vec4 &&) = default;
	
	template <typename U> 
	explicit inline constexpr vec4(vec4<U> const & v) : data { static_cast<T>(v[0]), static_cast<T>(v[1]), static_cast<T>(v[2]), static_cast<T>(v[3]) } {}
	
	// ================================================================
	// GENERAL
	// ================================================================
	
	inline constexpr bool is_real() const {
		return 
			!std::isnan(data[0]) &&
			!std::isnan(data[1]) &&
			!std::isnan(data[2]) &&
			!std::isnan(data[3]) &&
			!std::isinf(data[0]) &&
			!std::isinf(data[1]) &&
			!std::isinf(data[2]) &&
			!std::isinf(data[3]);
	}
	
	// ================================================================
	// LINEAR ALGEBRA
	// ================================================================
	
	[[nodiscard]]
	inline constexpr T magnitude_squared() const {
		return dot(*this);
	}
	
	[[nodiscard]]
	inline T magnitude() const {
		return static_cast<T>( std::sqrt(magnitude_squared()) );
	}
	
	inline T & normalize() { 
		T mag = magnitude();
		operator /= (mag);
		return mag;
	}
	
	[[nodiscard]]
	inline vec4 normalized() const {
		T mag = magnitude();
		return operator / (mag);
	}
	
	[[nodiscard]]
	static inline constexpr T dot(vec4 const & A, vec4 const & B) {
		return A.data[0] * B.data[0] + A.data[1] * B.data[1] + A.data[2] * B.data[2] + A.data[3] * B.data[3];
	}
	[[nodiscard]]
	inline constexpr T dot(vec4 const & other) const {
		return dot(*this, other);
	}
	
	// ================================================================
	// OPERATORS
	// ================================================================
	
	// ASSIGNMENT
	inline constexpr vec4 & operator = (vec4 const &) = default;
	inline constexpr vec4 & operator = (vec4 &&) = default;
	
	// EQUIVALENCY
	[[nodiscard]]
	inline constexpr bool operator == (vec4 const & other) const { return data == other.data; }
	[[nodiscard]]
	inline constexpr bool operator != (vec4 const & other) const { return data != other.data; }
	
	// ARITHMETIC
	[[nodiscard]]
	inline constexpr vec4 operator + (T const & v) const { return { data[0] + v, data[1] + v, data[2] + v, data[3] + v }; }
	[[nodiscard]]
	inline constexpr vec4 operator + (vec4 const & v) const { return { data[0] + v.data[0], data[1] + v.data[1], data[2] + v.data[2], data[3] + v.data[3] }; }
	[[nodiscard]]
	inline constexpr vec4 operator - (T const & v) const { return { data[0] - v, data[1] - v, data[2] - v, data[3] - v }; }
	[[nodiscard]]
	inline constexpr vec4 operator - (vec4 const & v) const { return { data[0] - v.data[0], data[1] - v.data[1], data[2] - v.data[2], data[3] - v.data[3] }; }
	[[nodiscard]]
	inline constexpr vec4 operator * (T const & v) const { return { data[0] * v, data[1] * v, data[2] * v, data[3] * v }; }
	[[nodiscard]]
	inline constexpr vec4 operator / (T const & v) const { return { data[0] / v, data[1] / v, data[2] / v, data[3] / v }; }
	
	inline constexpr vec4 & operator += (T const & v) { data[0] += v; data[1] += v; data[2] += v; data[3] += v; return *this; }
	inline constexpr vec4 & operator += (vec4 const & v) { data[0] += v.data[0]; data[1] += v.data[1]; data[2] += v.data[2]; data[3] += v.data[3]; return *this; }
	inline constexpr vec4 & operator -= (T const & v) { data[0] -= v; data[1] -= v; data[2] -= v; data[3] -= v; return *this; }
	inline constexpr vec4 & operator -= (vec4 const & v) { data[0] -= v.data[0]; data[1] -= v.data[1]; data[2] -= v.data[2]; data[3] -= v.data[3]; return *this; }
	inline constexpr vec4 & operator *= (T const & v) { data[0] *= v; data[1] *= v; data[2] *= v; data[3] *= v; return *this; }
	inline constexpr vec4 & operator /= (T const & v) { data[0] /= v; data[1] /= v; data[2] /= v; data[3] /= v; return *this; }
	
	[[nodiscard]]
	inline constexpr vec4 operator - () const { return { -data[0], -data[1], -data[2], -data[3] }; }
	
	// ACCESSING
	[[nodiscard]]
	inline constexpr T & operator [] (size_t i) { return data[i]; }
	[[nodiscard]]
	inline constexpr T const & operator [] (size_t i) const { return data[i]; }
	
	// ================================================================
	// CONSTSTANTS
	// ================================================================
	
	static inline constexpr vec4<T> origin { 0, 0, 0, 0 };
};

// ================================================================
// COMMUTATIVE OPERATORS
// ================================================================

namespace meadow::brassica {
	template <typename T> [[nodiscard]]
	inline constexpr vec4<T> operator + (T const & A, vec4<T> const & B) {
		return B + A;
	}
	
	template <typename T> [[nodiscard]]
	inline constexpr vec4<T> operator * (T const & A, vec4<T> const & B) {
		return B * A;
	}
}

// ================================================================
// STL SPECIALIZATIONS
// ================================================================

template <typename T> struct std::hash<meadow::brassica::vec4<T>> {
	size_t operator () (meadow::brassica::vec4<T> const & v) const {
		return hash_combine(hash_combine(std::hash(v[0]), std::hash(v[1])), hash_combine(std::hash(v[2]), std::hash(v[3])));
	}
};
