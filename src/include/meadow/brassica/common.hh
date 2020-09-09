#pragma once

#include <array>
#include <cstdint>
#include <ctgmath>
#include <random>

namespace meadow::brassica {

	template <typename T> inline constexpr T pi = static_cast<T>( 3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067982148086513282306647093844609550582231725359408128481117450284102701938521105559644622948954930381964428810975665933446128475648233786783165271201909145649L );
	template <typename T> [[nodiscard]] inline consteval T pi_m(T mult) { return mult * pi<T>; }

	template <typename T> [[nodiscard]] inline constexpr T radians(T const & v) { return v * pi<T> / static_cast<T>(180); }
	template <typename T> [[nodiscard]] inline constexpr T degrees(T const & v) { return v / pi<T> * static_cast<T>(180); }

	template <typename T> [[nodiscard]] inline constexpr T const & min(T const & A, T const & B) { return A < B ? A : B; }
	template <typename T> [[nodiscard]] inline constexpr T const & max(T const & A, T const & B) { return A > B ? A : B; }
	template <typename T> [[nodiscard]] inline constexpr T & min(T & A, T & B) { return A < B ? A : B; }
	template <typename T> [[nodiscard]] inline constexpr T & max(T & A, T & B) { return A > B ? A : B; }

	template <typename T> [[nodiscard]] inline constexpr T const & clamped(T const & value, T const & min, T const & max) { return (value < min) ? min : (value > max) ? max : value; }
	template <typename T> inline constexpr T & clamp(T & value, T const & min, T const & max) { value = clamped<T>(value, min, max); return value; }

	template <typename T, typename L> inline constexpr T lerp(T const & A, T const & B, L const & v) {
		return static_cast<T>((static_cast<L>(1) - v) * A) + static_cast<T>(v * B);
	}

	template <typename T, typename F = double> [[nodiscard]] static T square_up(F in) {
		return static_cast<T>(2) << static_cast<T>(std::floor(std::log2(in - static_cast<F>(1))));
	}

	[[nodiscard]] inline constexpr size_t hash_combine(size_t A, size_t B) {
		return A + 0x9e3779b9 + (B << 6) + (B >> 2);
	}

	template <typename T> struct vec2;
	template <typename T> struct vec3;
	template <typename T> struct vec4;
	template <typename T> struct mat4;
	template <typename T> struct quat;
}
