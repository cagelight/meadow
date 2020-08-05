#pragma once

#include "common.hh"

template <typename T> struct meadow::brassica::quat {

	// ================================================================
	// DATA
	// ================================================================

	using value_type = T;
	using data_type = std::array<T, 4>;
	data_type data;
	
	// ================================================================
	// CONSTRUCTION
	// ================================================================
	
	inline constexpr quat() = default;
	inline constexpr quat(data_type v) : data { v } {};
	inline constexpr quat(T x, T y, T z, T w) : data { x, y, z, w } {};
	
	inline constexpr quat(quat const &) = default;
	inline constexpr quat(quat &&) = default;
	
	template <typename U> 
	explicit inline constexpr quat(quat<U> const & v) : data { static_cast<T>(v[0]), static_cast<T>(v[1]), static_cast<T>(v[2]), static_cast<T>(v[3]) } {}
	
	// ================================================================
	// LINEAR ALGEBRA (FACTORIES)
	// ================================================================
	
	[[nodiscard]]
	static inline quat multiply(quat const & A, quat const & B) {
		return quat {
			(A.data[3] * B.data[0] + A.data[0] * B.data[3] + A.data[1] * B.data[2] - A.data[2] * B.data[1]),
			(A.data[3] * B.data[1] - A.data[0] * B.data[2] + A.data[1] * B.data[3] + A.data[2] * B.data[0]),
			(A.data[3] * B.data[2] + A.data[0] * B.data[1] - A.data[1] * B.data[0] + A.data[2] * B.data[3]),
			(A.data[3] * B.data[3] - A.data[0] * B.data[0] - A.data[1] * B.data[1] - A.data[2] * B.data[2])
		} .normalized();
	}
	
	template <typename RNG>
	[[nodiscard]]
	static inline quat random(RNG & rng) noexcept {
		static std::uniform_real_distribution<T> dist {0, 1};
		T u = dist(rng), v = dist(rng), w = dist(rng);
		return {
			std::sqrt(1 - u) * std::sin(2 * pi<T> * v),
			std::sqrt(1 - u) * std::cos(2 * pi<T> * v),
			std::sqrt(u)     * std::sin(2 * pi<T> * w),
			std::sqrt(u)     * std::cos(2 * pi<T> * w)
		};
	}
	
	[[nodiscard]]
	static inline quat axis(vec3<T> const & axis, T const & angle) {
		T a = angle / static_cast<T>(2);
		T s = std::sin(a);
		quat q {
			axis[0] * s,
			axis[1] * s,
			axis[2] * s,
			std::cos(a)
		};
		q.normalize();
		return q;
	}
	
	[[nodiscard]]
	static inline quat vector_delta(vec3<T> const & from, vec3<T> const & to, T lerp = static_cast<T>(1)) {
		static constexpr T one = static_cast<T>(1);
		static constexpr T negative_one = static_cast<T>(-1);
		
		T dot = vec3<T>::dot(to, from);
		if (dot > one) dot = one;
		if (dot < negative_one) dot = negative_one;
		if (dot == one) return identity;
		if (dot == negative_one) return quat { 0, 0, 1, 0 };
		
		vec3<T> rotaxis = vec3<T>::cross(to, from);
		rotaxis.normalize();
		
		T rot_val = std::acos(dot);
		return quat { rotaxis, rot_val * lerp };
	}
	
	[[nodiscard]]
	static inline quat slerp (quat const & A, quat const & B, T value) {
		
		if (value == 0) return A;
		if (value == 1) return B;
		
		T dot = A[0] * B[0] + A[1] * B[1] + A[2] * B[2] + A[3] * B[3];
		if (dot > 1) dot = 1;
		if (dot < -1) dot = -1;
		if (dot == 1) return A;
		T angle = std::acos(dot);
		T sqi = std::sqrt(static_cast<T>(1) - dot * dot);
		T vA = std::sin((static_cast<T>(1) - value) * angle) / sqi;
		T vB = std::sin(value * angle) / sqi;
		
		quat ret {
			A[0] * vA + B[0] * vB,
			A[1] * vA + B[1] * vB,
			A[2] * vA + B[2] * vB,
			A[3] * vA + B[3] * vB,
		};
		
		ret.normalize();
		return ret;
	}
	
	// ================================================================
	// LINEAR ALGEBRA
	// ================================================================
	
	[[nodiscard]]
	inline constexpr T magnitude_squared() const {
		return (data[0] * data[0]) + (data[1] * data[1]) + (data[2] * data[2]) + (data[3] * data[3]);
	}
	
	[[nodiscard]]
	inline T magnitude() const {
		return static_cast<T>( std::sqrt(magnitude_squared()) );
	}
	
	inline T & normalize() { 
		T mag = magnitude();
		data[0] /= mag;
		data[1] /= mag;
		data[2] /= mag;
		data[3] /= mag;
		return mag;
	}
	
	[[nodiscard]]
	inline quat normalized() const {
		T mag = magnitude();
		return {
			data[0] / mag,
			data[1] / mag,
			data[2] / mag,
			data[3] / mag,
		};
	}
	
	[[nodiscard]]
	inline constexpr quat conjugate() const {
		return { -data[0], -data[1], -data[2], data[3] };
	}
	
	[[nodiscard]]
	inline quat reciprocal() const {
		T m = magnitude();
		return { -data[0] / m, -data[1] / m, -data[2] / m, data[3] / m };
	}
	
	quat look_at(vec3<T> const & origin, vec3<T> const & target, T lerp = static_cast<T>(1)) const {
		vec3<T> front_to = vec3<T> { target - origin } .normalized() * conjugate();
		return vector_delta(vec3<T> { 0, 0, 1 }, front_to, lerp);
	}
	
	// ================================================================
	// OPERATORS
	// ================================================================
	
	// ASSIGNMENT
	inline constexpr quat & operator = (quat const &) = default;
	inline constexpr quat & operator = (quat &&) = default;
	
	// EQUIVALENCY
	[[nodiscard]]
	inline constexpr bool operator == (quat const & other) const { return data == other.data; }
	[[nodiscard]]
	inline constexpr bool operator != (quat const & other) const { return data != other.data; }
	
	// ARITHMETIC
	inline quat operator * (quat const & other) const {
		return quat::multiply(*this, other);
	}
	
	// ACCESSING
	[[nodiscard]]
	inline constexpr T & operator [] (size_t i) { return data[i]; }
	[[nodiscard]]
	inline constexpr T const & operator [] (size_t i) const { return data[i]; }
	
	// ================================================================
	// CONSTSTANTS
	// ================================================================
	
	static inline constexpr quat<T> identity { 0, 0, 0, 1 };
};

// ================================================================
// CROSS-CLASS OPERATORS
// ================================================================

namespace meadow::brassica {
	template <typename T> 
	[[nodiscard]]
	inline constexpr vec3<T> operator * (quat<T> const & q, vec3<T> const & v) {
		vec3<T> qv { q[0], q[1], q[2] };
		vec3<T> w1 = vec3<T>::cross(v, qv) * static_cast<T>(2);
		return v + w1 * q[3] + vec3<T>::cross(w1, qv);
	}

	template <typename T>
	[[nodiscard]]
	inline constexpr vec3<T> operator * (vec3<T> const & v, quat<T> const & q) {
		return q * v;
	}

	template <typename T>
	[[nodiscard]]
	inline constexpr vec3<T> & operator *= (vec3<T> & v, quat<T> const & q) {
		v = q * v;
	}
}
