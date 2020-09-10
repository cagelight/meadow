#include "tests.hh"

#include "meadow/brassica.hh"
namespace b = meadow::brassica;

template <typename T>
bool EXPECT(T res, T exp) {
	static constexpr T test_epsilon = static_cast<T>(0.005);
	if (res > exp + test_epsilon) return false;
	if (res < exp - test_epsilon) return false;
	return true;
}

void test_brassica() {
	
	float pi = b::pi<float>;
	float pi2 = b::pi_m<float>(2);

	TEST(b::min(20, 40) == 20);	
	TEST(b::max(20, 40) == 40);	
	
	{
		int a = 10, b = 15;
		b::min(a, b)++;
		TEST(a == 11);
	}
	
	{
		TEST(b::clamped(999, 10, 20) == 20);
		TEST(b::clamped(0, 10, 20) == 10);
		int test = 50;
		TEST(b::clamp(test, 0, 100) == 50 && test == 50)
		TEST(b::clamp(test, 60, 100) == 60 && test == 60)
		TEST(b::clamp(test, 0, 10) == 10 && test == 10)
	}
	
	{
		TEST(b::lerp(100, 200, 0.5) == 150);
		TEST(b::lerp(0, 100, 0.9) == 90);
	}
	
	{
		static_assert(std::is_trivial<b::vec2d>::value);
		static_assert(std::is_standard_layout<b::vec2d>::value);
		static_assert(sizeof(b::vec2d) == 16);
		
		b::vec2d a;
		b::vec2d b {};
		b::vec2d c { 0 };
		b::vec2d d { 5 };
		b::vec2d e { 3, 4 };
		
		TEST(b[0] == 0);
		TEST(b[1] == 0);
		TEST(c[0] == 0);
		TEST(c[1] == 0);
		TEST(d[0] == 5);
		TEST(d[1] == 5);
		TEST(e[0] == 3);
		TEST(e[1] == 4);
		
		TEST(b::vec2d::origin == c);
		
		TEST(b::vec2d(2, 6) + 1 == b::vec2d(3, 7));
		TEST(b::vec2d(2, 6) - 1 == b::vec2d(1, 5));
		TEST(b::vec2d(2, 6) * 2 == b::vec2d(4, 12));
		TEST(b::vec2d(2, 6) / 2 == b::vec2d(1, 3));
		
		e += 1;
		e -= 2;
		e *= 5;
		TEST(e == b::vec2d(10,15));
		
		TEST(b::lerp(b::vec2d(10, 100), b::vec2d(50, 200), 0.5) == b::vec2d(30, 150));
		TEST(b::vec2d(0, 10).magnitude() == 10);
		
		TEST(EXPECT(b::vec2d(4, 7).magnitude(), 8.06226));
		b::vec2d f1 = b::vec2d(4, 7);
		b::vec2d f2 = f1.normalized();
		f1.normalize();
		TEST(f1 == f2);
		TEST(EXPECT(f1[0], 0.496139));
		TEST(EXPECT(f1[1], 0.868243));
	}
	
	{
		static_assert(std::is_trivial<b::vec2f>::value);
		static_assert(std::is_standard_layout<b::vec2f>::value);
		
		b::vec2f a;
		b::vec2f b {};
		b::vec2f c { 0 };
		b::vec2f d { 5 };
		b::vec2f e { 3, 4 };
		
		TEST(b[0] == 0);
		TEST(b[1] == 0);
		TEST(c[0] == 0);
		TEST(c[1] == 0);
		TEST(d[0] == 5);
		TEST(d[1] == 5);
		TEST(e[0] == 3);
		TEST(e[1] == 4);
		
		TEST(b::vec2f::origin == c);
		
		TEST(b::vec2f(2, 6) + 1 == b::vec2f(3, 7));
		TEST(b::vec2f(2, 6) - 1 == b::vec2f(1, 5));
		TEST(b::vec2f(2, 6) * 2 == b::vec2f(4, 12));
		TEST(b::vec2f(2, 6) / 2 == b::vec2f(1, 3));
		
		e += 1;
		e -= 2;
		e *= 5;
		TEST(e == b::vec2f(10,15));
		
		TEST(b::lerp(b::vec2f(10, 100), b::vec2f(50, 200), 0.5f) == b::vec2f(30, 150));
		TEST(b::vec2f(0, 10).magnitude() == 10);
		
		TEST(EXPECT(b::vec2f(4, 7).magnitude(), 8.06226f));
		b::vec2f f1 = b::vec2f(4, 7);
		b::vec2f f2 = f1.normalized();
		f1.normalize();
		TEST(f1 == f2);
		TEST(EXPECT(f1[0], 0.496139f));
		TEST(EXPECT(f1[1], 0.868243f));
	}
	
	{
		using test_t = b::vec3<double>;
		static_assert(std::is_trivial<test_t>::value);
		static_assert(std::is_standard_layout<test_t>::value);
		
		test_t a;
		test_t b {};
		test_t c { 0 };
		test_t d { 5 };
		test_t e { 3, 4, 5 };
		
		TEST(b[0] == 0);
		TEST(b[1] == 0);
		TEST(b[2] == 0);
		TEST(c[0] == 0);
		TEST(c[1] == 0);
		TEST(c[2] == 0);
		TEST(d[0] == 5);
		TEST(d[1] == 5);
		TEST(d[2] == 5);
		TEST(e[0] == 3);
		TEST(e[1] == 4);
		TEST(e[2] == 5);
		
		TEST(test_t::origin == c);
		
		TEST(test_t(2, 6, 8) + 1 == test_t(3, 7, 9));
		TEST(test_t(2, 6, 8) - 1 == test_t(1, 5, 7));
		TEST(test_t(2, 6, 8) * 2 == test_t(4, 12, 16));
		TEST(test_t(2, 6, 8) / 2 == test_t(1, 3, 4));
	}
	
	{
		using vec_t = b::vec3<double>;
		using plane_t = b::plane<double>;
		
		vec_t t1, t2, t3;
		t1 = { 0, 4, 0 };
		t2 = { 5, 0, 0 };
		t3 = { 0, 0, 0 };
		
		plane_t p1 { t1, t2, t3 };
		auto v1 = plane_t::intersection(p1, p1, p1);
	}
}
