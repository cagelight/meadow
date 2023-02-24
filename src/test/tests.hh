#pragma once

#include "meadow/strop.hh"
#include "meadow/stropc.hh"

#include <benchmark/benchmark.h>

#include <random>

#define tlog meadow::streamlogger {}

template <typename T>
static inline T rndnum(T min, T max) {
	static std::mt19937_64 rng {std::random_device{}()};
	return std::uniform_int_distribution<T> {min, max} (rng);
}

#define TEST(expr) { if (!(expr)) { meadow::streamlogger {MEADOW_HERE, [](std::string_view here, std::string_view str){ \
		std::cout << "TEST FAILURE [" << here << "]\n\t" << str << std::endl;\
	}} << meadow::strf("TEST EXPRESSION: (%s)", #expr); std::exit(1); }}
	
#define TEST_MSG(expr, msg) { if (!(expr)) { meadow::streamlogger {MEADOW_HERE, [](std::string_view here, std::string_view str){ \
		std::cout << "TEST FAILURE [" << here << "]\n\t" << str << "\n\t" << msg << std::endl;\
	}} << meadow::strf("TEST EXPRESSION: (%s)", #expr); std::exit(1); }}

void test_aeon();
void test_buffer();
void test_brassica();
void test_bspacker();
void test_noise();
void test_oceanus();
void test_pegleg();
void test_strop();
void test_undae();
