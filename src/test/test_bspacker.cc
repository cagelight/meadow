#include "tests.hh"

#include "meadow/bspacker.hh"

using namespace meadow;

void test_bspacker() {
	
	BSPacker<uint64_t, int> bsp;
	
	auto c1 = bsp.pack(40, 30);
	TEST(c1.x == 0 && c1.y == 0);
	TEST(bsp.width() == 40);
	TEST(bsp.height() == 30);
	
	auto c2 = bsp.pack(40, 30);
	TEST(c2.x == 0 && c2.y == 30);
	TEST(bsp.width() == 40);
	TEST(bsp.height() == 60);
}
