#pragma once

#include "meadow/strop.hh"

#include <random>

#define tlog meadow::streamlogger {}

template <typename T>
static inline T rndnum(T min, T max) {
	static std::mt19937_64 rng {std::random_device{}()};
	return std::uniform_int_distribution<T> {min, max} (rng);
}

#define TEST(expr) { if (!(expr)) { tlog << meadow::strf("TEST FAILURE [%s]: TEST EXPRESSION: (%s)", _here, #expr); std::exit(1); }}

void test_aeon();
void test_buffer();