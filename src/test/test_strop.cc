#include "tests.hh"

#include "meadow/strop.hh"

#include <span>

void test_strop() {
	
	std::string t1 { "hello\nfuck you" };
	auto t1r = meadow::split<std::string_view>(t1, '\n');
	TEST(t1r.size() == 2)
	TEST(t1r[0] == "hello")
	TEST(t1r[1] == "fuck you")
	
	std::string t2 { "\n\n\nhello\n\nfuck you\n\n\n\n" };
	auto t2r = meadow::split<std::string_view>(t2, '\n');
	TEST(t2r.size() == 2)
	TEST(t2r[0] == "hello")
	TEST(t2r[1] == "fuck you")
	
	std::string t3 { "hello\n\nfuck you" };
	auto t3r = meadow::split<std::string_view>(t3, '\n', false);
	TEST(t3r.size() == 3)
	TEST(t3r[0] == "hello")
	TEST(t3r[1] == "")
	TEST(t3r[2] == "fuck you")
	
	std::string t4 { "\n\n\nhello\n\nfuck you\n\n\n\n" };
	auto t4r = meadow::split<std::string_view>(t4, '\n', false);
	TEST(t4r.size() == 10)
	TEST(t4r[0] == "")
	TEST(t4r[1] == "")
	TEST(t4r[2] == "")
	TEST(t4r[3] == "hello")
	TEST(t4r[4] == "")
	TEST(t4r[5] == "fuck you")
	TEST(t4r[6] == "")
	TEST(t4r[7] == "")
	TEST(t4r[8] == "")
	TEST(t4r[9] == "")
	
	std::vector<int32_t> t5 { 50, 23, 92, 65, 23, 47, 23, 90 };
	auto t5r = meadow::split<std::span<int32_t>>(t5, 23);
	TEST(t5r.size() == 4)
	TEST(t5r[0][0] == 50);
	TEST(t5r[1][0] == 92);
	TEST(t5r[1][1] == 65);
	TEST(t5r[2][0] == 47);
	TEST(t5r[3][0] == 90);
}
