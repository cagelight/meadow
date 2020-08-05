#pragma once

#include "common.hh"

template <typename T> struct meadow::brassica::mat4 {

	// ================================================================
	// DATA
	// ================================================================

	using value_type = T;
	using row_type = std::array<T, 4>;
	using data_type = std::array<row_type, 4>;	
	data_type data;
	
	// ================================================================
	// CONSTRUCTION
	// ================================================================
	
	inline constexpr mat4() = default;
	inline constexpr mat4(T const & v) : data {
		v, 0, 0, 0,
		0, v, 0, 0,
		0, 0, v, 0,
		0, 0, 0, v
	} {}
	inline constexpr mat4(
		data_type const & v0,
		data_type const & v1,
		data_type const & v2,
		data_type const & v3
	) : data { v0, v1, v2, v3 } {}
	inline constexpr mat4(
		T const & v00, T const & v01, T const & v02, T const & v03,
		T const & v10, T const & v11, T const & v12, T const & v13,
		T const & v20, T const & v21, T const & v22, T const & v23,
		T const & v30, T const & v31, T const & v32, T const & v33
	) : data {
		row_type {v00, v01, v02, v03},
		row_type {v10, v11, v12, v13},
		row_type {v20, v21, v22, v23},
		row_type {v30, v31, v32, v33},
	} {}
	
	template <typename U> 
	explicit inline constexpr mat4(mat4<U> const & v) : data {
		row_type { static_cast<T>( v[0][0] ), static_cast<T>( v[0][1] ), static_cast<T>( v[0][2] ), static_cast<T>( v[0][3] ) },
		row_type { static_cast<T>( v[1][0] ), static_cast<T>( v[1][1] ), static_cast<T>( v[1][2] ), static_cast<T>( v[1][3] ) },
		row_type { static_cast<T>( v[2][0] ), static_cast<T>( v[2][1] ), static_cast<T>( v[2][2] ), static_cast<T>( v[2][3] ) },
		row_type { static_cast<T>( v[3][0] ), static_cast<T>( v[3][1] ), static_cast<T>( v[3][2] ), static_cast<T>( v[3][3] ) },
	} {}
	
	inline constexpr mat4(mat4 const &) = default;
	inline constexpr mat4(mat4 &&) = default;
	
	explicit inline constexpr mat4(quat<T> const & v) : mat4(static_cast<T>(1)){
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
	static inline constexpr mat4 ortho(T b, T t, T l, T r, T n, T f) {
		mat4 w = identity;
		w[0][0] = static_cast<T>( 2) / (r - l);
		w[1][1] = static_cast<T>( 2) / (t - b);
		w[2][2] = static_cast<T>(-2) / (f - n);
		w[3][0] = - (r + l) / (r - l);
		w[3][1] = - (t + b) / (t - b);
		w[3][2] = - (f + n) / (f - n);
		return w;
	}
	
	[[nodiscard]]
	static inline mat4 perspective(T vfov, T width, T height, T near, T far) {
		mat4 w = identity;
		T t1 = static_cast<T>(1) / std::tan(static_cast<T>(0.5) * vfov);
		w[0][0] = t1 * (height / width);
		w[1][1] = t1;
		w[2][2] = (far + near) / (far - near);
		w[2][3] = static_cast<T>(1);
		w[3][2] = -( static_cast<T>(2) * far * near ) / (far - near);
		w[3][3] = static_cast<T>(0);
		return w;
	}
	
	[[nodiscard]]
	static inline constexpr mat4 scale(T const & s) {
		mat4 w = identity;
		w[0][0] = s;
		w[1][1] = s;
		w[2][2] = s;
		return w;
	}
	
	[[nodiscard]]
	static inline constexpr mat4 scale(T const & x, T const & y, T const & z) {
		mat4 w = identity;
		w[0][0] = x;
		w[1][1] = y;
		w[2][2] = z;
		return w;
	}
	
	[[nodiscard]]
	static inline constexpr mat4 scale(vec3<T> const & v) {
		mat4 w = identity;
		w[0][0] = v.data[0];
		w[1][1] = v.data[1];
		w[2][2] = v.data[2];
		return w;
	}
	
	[[nodiscard]]
	static inline constexpr mat4 translate(vec3<T> const & v) {
		mat4 w = identity;
		w[3][0] = v.data[0];
		w[3][1] = v.data[1];
		w[3][2] = v.data[2];
		return w;
	}
	
	// ================================================================
	// LINEAR ALGEBRA (OPERATIONS)
	// ================================================================
	
	[[nodiscard]]
	static constexpr mat4 multiply(mat4 const & B, mat4 const & A) {
		return {
			A[0][0] * B[0][0] + A[0][1] * B[1][0] + A[0][2] * B[2][0] + A[0][3] * B[3][0],
			A[0][0] * B[0][1] + A[0][1] * B[1][1] + A[0][2] * B[2][1] + A[0][3] * B[3][1],
			A[0][0] * B[0][2] + A[0][1] * B[1][2] + A[0][2] * B[2][2] + A[0][3] * B[3][2],
			A[0][0] * B[0][3] + A[0][1] * B[1][3] + A[0][2] * B[2][3] + A[0][3] * B[3][3],
			A[1][0] * B[0][0] + A[1][1] * B[1][0] + A[1][2] * B[2][0] + A[1][3] * B[3][0],
			A[1][0] * B[0][1] + A[1][1] * B[1][1] + A[1][2] * B[2][1] + A[1][3] * B[3][1],
			A[1][0] * B[0][2] + A[1][1] * B[1][2] + A[1][2] * B[2][2] + A[1][3] * B[3][2],
			A[1][0] * B[0][3] + A[1][1] * B[1][3] + A[1][2] * B[2][3] + A[1][3] * B[3][3],
			A[2][0] * B[0][0] + A[2][1] * B[1][0] + A[2][2] * B[2][0] + A[2][3] * B[3][0],
			A[2][0] * B[0][1] + A[2][1] * B[1][1] + A[2][2] * B[2][1] + A[2][3] * B[3][1],
			A[2][0] * B[0][2] + A[2][1] * B[1][2] + A[2][2] * B[2][2] + A[2][3] * B[3][2],
			A[2][0] * B[0][3] + A[2][1] * B[1][3] + A[2][2] * B[2][3] + A[2][3] * B[3][3],
			A[3][0] * B[0][0] + A[3][1] * B[1][0] + A[3][2] * B[2][0] + A[3][3] * B[3][0],
			A[3][0] * B[0][1] + A[3][1] * B[1][1] + A[3][2] * B[2][1] + A[3][3] * B[3][1],
			A[3][0] * B[0][2] + A[3][1] * B[1][2] + A[3][2] * B[2][2] + A[3][3] * B[3][2],
			A[3][0] * B[0][3] + A[3][1] * B[1][3] + A[3][2] * B[2][3] + A[3][3] * B[3][3],
		};
	}
	
	[[nodiscard]]
	inline constexpr mat4 inverse() const {

		T const A2323 = data[2][2] * data[3][3] - data[2][3] * data[3][2] ;
		T const A1323 = data[2][1] * data[3][3] - data[2][3] * data[3][1] ;
		T const A1223 = data[2][1] * data[3][2] - data[2][2] * data[3][1] ;
		T const A0323 = data[2][0] * data[3][3] - data[2][3] * data[3][0] ;
		T const A0223 = data[2][0] * data[3][2] - data[2][2] * data[3][0] ;
		T const A0123 = data[2][0] * data[3][1] - data[2][1] * data[3][0] ;
		T const A2313 = data[1][2] * data[3][3] - data[1][3] * data[3][2] ;
		T const A1313 = data[1][1] * data[3][3] - data[1][3] * data[3][1] ;
		T const A1213 = data[1][1] * data[3][2] - data[1][2] * data[3][1] ;
		T const A2312 = data[1][2] * data[2][3] - data[1][3] * data[2][2] ;
		T const A1312 = data[1][1] * data[2][3] - data[1][3] * data[2][1] ;
		T const A1212 = data[1][1] * data[2][2] - data[1][2] * data[2][1] ;
		T const A0313 = data[1][0] * data[3][3] - data[1][3] * data[3][0] ;
		T const A0213 = data[1][0] * data[3][2] - data[1][2] * data[3][0] ;
		T const A0312 = data[1][0] * data[2][3] - data[1][3] * data[2][0] ;
		T const A0212 = data[1][0] * data[2][2] - data[1][2] * data[2][0] ;
		T const A0113 = data[1][0] * data[3][1] - data[1][1] * data[3][0] ;
		T const A0112 = data[1][0] * data[2][1] - data[1][1] * data[2][0] ;

		T const det = static_cast<T>(1) / (
			  data[0][0] * ( data[1][1] * A2323 - data[1][2] * A1323 + data[1][3] * A1223 ) 
			- data[0][1] * ( data[1][0] * A2323 - data[1][2] * A0323 + data[1][3] * A0223 ) 
			+ data[0][2] * ( data[1][0] * A1323 - data[1][1] * A0323 + data[1][3] * A0123 ) 
			- data[0][3] * ( data[1][0] * A1223 - data[1][1] * A0223 + data[1][2] * A0123 )
		);

		return {
			det *   ( data[1][1] * A2323 - data[1][2] * A1323 + data[1][3] * A1223 ),
			det * - ( data[0][1] * A2323 - data[0][2] * A1323 + data[0][3] * A1223 ),
			det *   ( data[0][1] * A2313 - data[0][2] * A1313 + data[0][3] * A1213 ),
			det * - ( data[0][1] * A2312 - data[0][2] * A1312 + data[0][3] * A1212 ),
			det * - ( data[1][0] * A2323 - data[1][2] * A0323 + data[1][3] * A0223 ),
			det *   ( data[0][0] * A2323 - data[0][2] * A0323 + data[0][3] * A0223 ),
			det * - ( data[0][0] * A2313 - data[0][2] * A0313 + data[0][3] * A0213 ),
			det *   ( data[0][0] * A2312 - data[0][2] * A0312 + data[0][3] * A0212 ),
			det *   ( data[1][0] * A1323 - data[1][1] * A0323 + data[1][3] * A0123 ),
			det * - ( data[0][0] * A1323 - data[0][1] * A0323 + data[0][3] * A0123 ),
			det *   ( data[0][0] * A1313 - data[0][1] * A0313 + data[0][3] * A0113 ),
			det * - ( data[0][0] * A1312 - data[0][1] * A0312 + data[0][3] * A0112 ),
			det * - ( data[1][0] * A1223 - data[1][1] * A0223 + data[1][2] * A0123 ),
			det *   ( data[0][0] * A1223 - data[0][1] * A0223 + data[0][2] * A0123 ),
			det * - ( data[0][0] * A1213 - data[0][1] * A0213 + data[0][2] * A0113 ),
			det *   ( data[0][0] * A1212 - data[0][1] * A0212 + data[0][2] * A0112 ),
		};
	}
	
	// ================================================================
	// OPERATORS
	// ================================================================
	
	// ASSIGNMENT
	inline constexpr mat4 & operator = (mat4 const &) = default;
	inline constexpr mat4 & operator = (mat4 &&) = default;
	
	// ARITHMETIC
	[[nodiscard]]
	inline constexpr mat4 operator * (mat4 const & other) const {
		return multiply(*this, other);
	}
	
	inline constexpr mat4 & operator *= (mat4 const & other) {
		*this = multiply(*this, other);
		return *this;
	}
	
	// ================================================================
	// CONSTSTANTS
	// ================================================================
	
	static inline constexpr mat4<T> identity { static_cast<T>(1) };
	
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
	inline constexpr vec4<T> operator * (mat4<T> const & m, vec4<T> const & v) {
		return vec4<T> {
			v[0] * m[0][0] + v[1] * m[1][0] + v[2] * m [2][0] + v[3] * m[3][0],
			v[0] * m[0][1] + v[1] * m[1][1] + v[2] * m [2][1] + v[3] * m[3][1],
			v[0] * m[0][2] + v[1] * m[1][2] + v[2] * m [2][2] + v[3] * m[3][2],
			v[0] * m[0][3] + v[1] * m[1][3] + v[2] * m [2][3] + v[3] * m[3][3],
		};
	}
	
	template <typename T> [[nodiscard]]
	inline vec4<T> operator * (vec4<T> const & v, mat4<T> const & m) {
		return vec4<T> {
			v[0] * m[0][0] + v[1] * m[0][1] + v[2] * m [0][2] + v[3] * m[0][3],
			v[0] * m[1][0] + v[1] * m[1][1] + v[2] * m [1][2] + v[3] * m[1][3],
			v[0] * m[2][0] + v[1] * m[2][1] + v[2] * m [2][2] + v[3] * m[2][3],
			v[0] * m[3][0] + v[1] * m[3][1] + v[2] * m [3][2] + v[3] * m[3][3],
		};
	}
}
