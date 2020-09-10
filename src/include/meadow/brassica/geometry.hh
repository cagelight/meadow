#pragma once

#include "common.hh"

template <typename T> struct meadow::brassica::plane {
	
	// ================================================================
	// DATA
	// ================================================================
	
	using value_type = T;
	using norm_type = vec3<T>;
	using dist_type = T;
	
	norm_type normal;
	dist_type dist;
	
	// ================================================================
	// CONSTRUCTION
	// ================================================================
	
	inline constexpr plane() = default;
	inline constexpr plane(norm_type const & norm, dist_type const & dist)
		: normal { norm },
		  dist { dist }
	{}
	
	inline constexpr plane(plane const &) = default;
	inline constexpr plane(plane &&) = default;
	
	template <typename U> 
	explicit inline constexpr plane(plane<U> const & v)
		: normal { static_cast<norm_type>(v.normal) }, 
		  dist { static_cast<dist_type>(v.dist) } {}
	
	inline constexpr plane(vec3<T> const & p1, vec3<T> const & p2, vec3<T> const & p3)
		: normal { (p2 - p1).cross(p3 - p1).normalized() },
		  dist { -p1.dot(normal) }
	{}
	
	// ================================================================
	// LINEAR ALGEBRA
	// ================================================================
	
	static inline constexpr vec3<T> intersection(plane const & p1, plane const & p2, plane const & p3) {
		return mat3<T> { p1.normal, p2.normal, p3.normal } .inverse() * vec3<T> { p1.dist, p2.dist, p3.dist };
	}
};
