#pragma once

#include "common.hh"

template <typename T> struct meadow::brassica::vec3 {

	// ================================================================
	// DATA
	// ================================================================

	using value_type = T;
	using data_type = std::array<T, 3>;	
	data_type data;

	// ================================================================
	// CONSTRUCTION
	// ================================================================
	
	inline constexpr vec3() = default;
	inline constexpr vec3(data_type v) : data { v } {};
	inline constexpr vec3(T x, T y, T z) : data { x, y, z } {};
	
	explicit inline constexpr vec3(T const & v) : data { v, v, v } {};
	
	inline constexpr vec3(vec3 const &) = default;
	inline constexpr vec3(vec3 &&) = default;
	
	template <typename U> 
	explicit inline constexpr vec3(vec3<U> const & v) : data { static_cast<T>(v[0]), static_cast<T>(v[1]), static_cast<T>(v[2]) } {}
	
	// ================================================================
	// GENERAL
	// ================================================================
	
	inline constexpr bool is_real() const {
		return 
			!std::isnan(data[0]) &&
			!std::isnan(data[1]) &&
			!std::isnan(data[2]) && 
			!std::isinf(data[0]) &&
			!std::isinf(data[1]) &&
			!std::isinf(data[2]);
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
	inline vec3 normalized() const {
		T mag = magnitude();
		return operator / (mag);
	}
	
	[[nodiscard]]
	static inline constexpr T dot(vec3 const & A, vec3 const & B) {
		return A.data[0] * B.data[0] + A.data[1] * B.data[1] + A.data[2] * B.data[2];
	}
	[[nodiscard]]
	inline constexpr T dot(vec3 const & other) const {
		return dot(*this, other);
	}
	
	[[nodiscard]]
	static inline constexpr vec3 cross(vec3 const & A, vec3 const & B) {
		return {
			A.data[1] * B.data[2] - A.data[2] * B.data[1],
			A.data[2] * B.data[0] - A.data[0] * B.data[2],
			A.data[0] * B.data[1] - A.data[1] * B.data[0],
		};
	}
	[[nodiscard]]
	inline constexpr vec3 cross(vec3 const & other) const {
		return cross(*this, other);
	}
	
	// ================================================================
	// OPERATORS
	// ================================================================
	
	// ASSIGNMENT
	inline constexpr vec3 & operator = (vec3 const &) = default;
	inline constexpr vec3 & operator = (vec3 &&) = default;
	
	// EQUIVALENCY
	[[nodiscard]]
	inline constexpr bool operator == (vec3 const & other) const { return data == other.data; }
	[[nodiscard]]
	inline constexpr bool operator != (vec3 const & other) const { return data != other.data; }
	
	// ARITHMETIC
	[[nodiscard]]
	inline constexpr vec3 operator + (T const & v) const { return { data[0] + v, data[1] + v, data[2] + v }; }
	[[nodiscard]]
	inline constexpr vec3 operator + (vec3 const & v) const { return { data[0] + v.data[0], data[1] + v.data[1], data[2] + v.data[2] }; }
	[[nodiscard]]
	inline constexpr vec3 operator - (T const & v) const { return { data[0] - v, data[1] - v, data[2] - v }; }
	[[nodiscard]]
	inline constexpr vec3 operator - (vec3 const & v) const { return { data[0] - v.data[0], data[1] - v.data[1], data[2] - v.data[2] }; }
	[[nodiscard]]
	inline constexpr vec3 operator * (T const & v) const { return { data[0] * v, data[1] * v, data[2] * v }; }
	[[nodiscard]]
	inline constexpr vec3 operator / (T const & v) const { return { data[0] / v, data[1] / v, data[2] / v }; }
	
	inline constexpr vec3 & operator += (T const & v) { data[0] += v; data[1] += v; data[2] += v; return *this; }
	inline constexpr vec3 & operator += (vec3 const & v) { data[0] += v.data[0]; data[1] += v.data[1]; data[2] += v.data[2]; return *this; }
	inline constexpr vec3 & operator -= (T const & v) { data[0] -= v; data[1] -= v; data[2] -= v; return *this; }
	inline constexpr vec3 & operator -= (vec3 const & v) { data[0] -= v.data[0]; data[1] -= v.data[1]; data[2] -= v.data[2]; return *this; }
	inline constexpr vec3 & operator *= (T const & v) { data[0] *= v; data[1] *= v; data[2] *= v; return *this; }
	inline constexpr vec3 & operator /= (T const & v) { data[0] /= v; data[1] /= v; data[2] /= v; return *this; }
	
	[[nodiscard]]
	inline constexpr vec3 operator - () const { return { -data[0], -data[1], -data[2] }; }
	
	// ACCESSING
	[[nodiscard]]
	inline constexpr T & operator [] (size_t i) { return data[i]; }
	[[nodiscard]]
	inline constexpr T const & operator [] (size_t i) const { return data[i]; }
	
	// ================================================================
	// CONSTSTANTS
	// ================================================================
	
	static inline constexpr vec3<T> origin { 0, 0, 0 };
};

// ================================================================
// COMMUTATIVE OPERATORS
// ================================================================

namespace meadow::brassica {
	template <typename T> [[nodiscard]]
	inline constexpr vec3<T> operator + (T const & A, vec3<T> const & B) {
		return B + A;
	}
	
	template <typename T> [[nodiscard]]
	inline constexpr vec3<T> operator * (T const & A, vec3<T> const & B) {
		return B * A;
	}
}

// ================================================================
// STL SPECIALIZATIONS
// ================================================================

template <typename T> struct std::hash<meadow::brassica::vec3<T>> {
	size_t operator () (meadow::brassica::vec3<T> const & v) const {
		return hash_combine(hash_combine(std::hash(v[0]), std::hash(v[1])), std::hash(v[2]));
	}
};
