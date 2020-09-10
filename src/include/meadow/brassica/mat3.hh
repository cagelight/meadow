#pragma once

#include "common.hh"
#include "vec3.hh"

template <typename T> struct meadow::brassica::mat3 {

	// ================================================================
	// DATA
	// ================================================================

	using value_type = T;
	using row_type = vec3<T>;
	using data_type = std::array<row_type, 3>;
	data_type data;
	
	// ================================================================
	// CONSTRUCTION
	// ================================================================
	
	inline constexpr mat3() = default;
	inline constexpr mat3(T const & v) : data {
		v, 0, 0,
		0, v, 0,
		0, 0, v
	} {}
	inline constexpr mat3(
		row_type const & v0,
		row_type const & v1,
		row_type const & v2
	) : data { v0, v1, v2 } {}
	
	inline constexpr mat3(
		T const & v00, T const & v01, T const & v02,
		T const & v10, T const & v11, T const & v12,
		T const & v20, T const & v21, T const & v22
	) : data {
		row_type {v00, v01, v02},
		row_type {v10, v11, v12},
		row_type {v20, v21, v22}
	} {}
	
	template <typename U> 
	explicit inline constexpr mat3(mat3<U> const & v) : data {
		row_type { static_cast<T>( v[0][0] ), static_cast<T>( v[0][1] ), static_cast<T>( v[0][2] ) },
		row_type { static_cast<T>( v[1][0] ), static_cast<T>( v[1][1] ), static_cast<T>( v[1][2] ) },
		row_type { static_cast<T>( v[2][0] ), static_cast<T>( v[2][1] ), static_cast<T>( v[2][2] ) },
	} {}
	
	inline constexpr mat3(mat3 const &) = default;
	inline constexpr mat3(mat3 &&) = default;
	
	explicit inline constexpr mat3(quat<T> const & v) : mat3(static_cast<T>(1)){
		T sqx = v.data[0] * v.data[0];
		T sqy = v.data[1] * v.data[1];
		T sqz = v.data[2] * v.data[2];
		T sqw = v.data[3] * v.data[3];
		data[0][0] =  sqx - sqy - sqz + sqw;
		data[1][1] = -sqx + sqy - sqz + sqw;
		data[2][2] = -sqx - sqy + sqz + sqw;
		T t1 = v.data[0] * v.data[1];
		T t2 = v.data[2] * v.data[3];
		data[1][0] = ((T)2) * (t1 + t2);
		data[0][1] = ((T)2) * (t1 - t2);
		t1 = v.data[0] * v.data[2];
		t2 = v.data[1] * v.data[3];
		data[2][0] = ((T)2) * (t1 - t2);
		data[0][2] = ((T)2) * (t1 + t2);
		t1 = v.data[1] * v.data[2];
		t2 = v.data[0] * v.data[3];
		data[2][1] = ((T)2) * (t1 + t2);
		data[1][2] = ((T)2) * (t1 - t2);
	}
	
	// ================================================================
	// LINEAR ALGEBRA (FACTORIES)
	// ================================================================
	
	[[nodiscard]]
	static inline constexpr mat3 scale(T const & s) {
		mat3 w = identity;
		w[0][0] = s;
		w[1][1] = s;
		w[2][2] = s;
		return w;
	}
	
	[[nodiscard]]
	static inline constexpr mat3 scale(T const & x, T const & y, T const & z) {
		mat3 w = identity;
		w[0][0] = x;
		w[1][1] = y;
		w[2][2] = z;
		return w;
	}
	
	[[nodiscard]]
	static inline constexpr mat3 scale(vec3<T> const & v) {
		mat3 w = identity;
		w[0][0] = v.data[0];
		w[1][1] = v.data[1];
		w[2][2] = v.data[2];
		return w;
	}
	
	// ================================================================
	// LINEAR ALGEBRA (OPERATIONS)
	// ================================================================
	
	[[nodiscard]]
	static constexpr mat3 multiply(mat3 const & B, mat3 const & A) {
		return {
			A[0][0] * B[0][0] + A[0][1] * B[1][0] + A[0][2] * B[2][0],
			A[0][0] * B[0][1] + A[0][1] * B[1][1] + A[0][2] * B[2][1],
			A[0][0] * B[0][2] + A[0][1] * B[1][2] + A[0][2] * B[2][2],
			A[1][0] * B[0][0] + A[1][1] * B[1][0] + A[1][2] * B[2][0],
			A[1][0] * B[0][1] + A[1][1] * B[1][1] + A[1][2] * B[2][1],
			A[1][0] * B[0][2] + A[1][1] * B[1][2] + A[1][2] * B[2][2],
			A[2][0] * B[0][0] + A[2][1] * B[1][0] + A[2][2] * B[2][0],
			A[2][0] * B[0][1] + A[2][1] * B[1][1] + A[2][2] * B[2][1],
			A[2][0] * B[0][2] + A[2][1] * B[1][2] + A[2][2] * B[2][2],
		};
	}
	
	[[nodiscard]]
	inline constexpr mat3 inverse() const {

		T det = data[0][0] * (data[2][2] * data[1][1] - data[2][1] * data[1][2]) -
		        data[0][1] * (data[2][2] * data[1][0] - data[2][0] * data[1][2]) +
		        data[0][2] * (data[2][1] * data[1][0] - data[2][0] * data[1][1]);
		
		return {
			(data[2][2] * data[1][1] - data[2][1] * data[1][2]) / det,
			(data[2][0] * data[1][2] - data[2][2] * data[1][0]) / det,
			(data[2][1] * data[1][0] - data[2][0] * data[1][1]) / det,
			(data[0][2] * data[2][1] - data[0][1] * data[2][2]) / det,
			(data[0][0] * data[2][2] - data[0][2] * data[2][0]) / det,
			(data[0][1] * data[2][0] - data[0][0] * data[2][1]) / det,
			(data[1][2] * data[0][1] - data[1][1] * data[0][2]) / det,
			(data[1][0] * data[0][2] - data[1][2] * data[0][0]) / det,
			(data[1][1] * data[0][0] - data[1][0] * data[0][1]) / det
		};
	}
	
	// ================================================================
	// OPERATORS
	// ================================================================
	
	// ASSIGNMENT
	inline constexpr mat3 & operator = (mat3 const &) = default;
	inline constexpr mat3 & operator = (mat3 &&) = default;
	
	// ARITHMETIC
	[[nodiscard]]
	inline constexpr mat3 operator * (mat3 const & other) const {
		return multiply(*this, other);
	}
	
	inline constexpr mat3 & operator *= (mat3 const & other) {
		*this = multiply(*this, other);
		return *this;
	}
	
	// ================================================================
	// CONSTSTANTS
	// ================================================================
	
	static inline constexpr mat3<T> identity { static_cast<T>(1) };
	
	[[nodiscard]]
	inline constexpr row_type & operator [] (size_t i) { return data[i]; }
	[[nodiscard]]
	inline constexpr row_type const & operator [] (size_t i) const { return data[i]; }
};

// ================================================================
// CROSS-CLASS OPERATORS
// ================================================================

namespace meadow::brassica {
	template <typename T> [[nodiscard]]
	inline constexpr vec3<T> operator * (mat3<T> const & m, vec3<T> const & v) {
		return vec3<T> {
			v[0] * m[0][0] + v[1] * m[1][0] + v[2] * m [2][0],
			v[0] * m[0][1] + v[1] * m[1][1] + v[2] * m [2][1],
			v[0] * m[0][2] + v[1] * m[1][2] + v[2] * m [2][2],
		};
	}
	
	template <typename T> [[nodiscard]]
	inline vec3<T> operator * (vec3<T> const & v, mat3<T> const & m) {
		return vec3<T> {
			v[0] * m[0][0] + v[1] * m[0][1] + v[2] * m [0][2],
			v[0] * m[1][0] + v[1] * m[1][1] + v[2] * m [1][2],
			v[0] * m[2][0] + v[1] * m[2][1] + v[2] * m [2][2],
		};
	}
}
