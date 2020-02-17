#include "tests.hh"

#include "../src/meadow/buffer.hh"

using namespace meadow;

void test_buffer() {
	
	{
		buffer b;
		TEST(!b.data() && !b.size() && !b.capacity())
		b.reserve(512);
		TEST(b.data()) TEST(!b.size()) TEST(b.capacity() == 512)
		b.resize(64);
		b[14] = 87;
		TEST(b.data()) TEST(b.size() == 64) TEST(b.capacity() == 512) TEST(b[14] == 87)
		b.resize(1024);
		TEST(b.data()) TEST(b.size() == 1024) TEST(b.capacity() == 1024) TEST(b[14] == 87)
		b.resize(32);
		TEST(b.data()) TEST(b.size() == 32) TEST(b.capacity() == 1024) TEST(b[14] == 87)
		b.shrink_to_fit();
		TEST(b.data()) TEST(b.size() == 32) TEST(b.capacity() == 32) TEST(b[14] == 87)
		b.reserve(8192);
		TEST(b.data()) TEST(b.size() == 32) TEST(b.capacity() == 8192) TEST(b[14] == 87)
	}
	
	{
		buffer b {32};
		TEST(b.data()) TEST(!b.size()) TEST(b.capacity() == 32)
		b.resize(4);
		TEST(b.data()) TEST(b.size() == 4) TEST(b.capacity() == 32)
		b[0] = 0xDE;
		b[1] = 0xAD;
		b[2] = 0xBE;
		b[3] = 0xEF;
		TEST(b.hex() == "DEADBEEF")
		TEST(b.hex(true) == "deadbeef")
		
		buffer q {b};
		TEST(b == q)
		buffer j;
		j.resize(32);
		j = q;
		TEST(b == j)
		TEST(j.hex() == "DEADBEEF")
		j.clear();
		buffer y;
		TEST(j == y)
		
		y << '\xDE' << '\xAD' << '\xBE' << '\xEF';
		TEST(y == b)
		y.clear();
		y << 0xEFBEADDE;
		TEST(y == b)
	}
}
