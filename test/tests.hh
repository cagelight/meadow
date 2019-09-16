#pragma once

#include "../src/meadow/strop.hh"

#include <random>

#define log meadow::streamlogger {}

template <typename T>
static inline T rndnum(T min, T max) {
	static std::mt19937_64 rng {std::random_device{}()};
	return std::uniform_int_distribution<T> {min, max} (rng);
}

void test_aeon();
