#include "tests.hh"

#include "meadow/aeon.hh"

#include <utility>

using aeon = meadow::aeon;

void test_aeon() {
	
	TEST(aeon::jsonify_string("TEST") == "\"TEST\"")
	TEST(aeon::jsonify_string("\t\n\\") == "\"\\t\\n\\\\\"")
	TEST(aeon::jsonify_string("\x07") == "\"\\u0007\"")
	
	aeon test;
	
	{
		test = true;
		TEST(test == true)
		TEST(test.as_boolean())
		test = false;
		TEST(test == false)
		TEST(!test.as_boolean())
		test = "true";
		TEST(test.as_boolean())
		test = "false";
		TEST(!test.as_boolean())
	}
	
	{
		test = 0;
		TEST(test.is_integer())
		TEST(!test.as_boolean())
		TEST(test == 0)
		TEST(test.serialize_json() == "0")
	}
	
	{
		test = 5;
		TEST(test.is_integer())
		TEST(test.as_boolean())
		TEST(test == 5)
		TEST(test == 5.0)
		TEST(test.serialize_json() == "5")
	}
	
	{
		test = 7.2;
		TEST(test.is_floating())
		TEST(test.as_boolean())
		TEST(test == 7.2)
		TEST(test.serialize_json() == "7.2")
	}
	
	{
		test = "";
		TEST(test.is_string())
		TEST(test == "");
		TEST(test.serialize_json() == "\"\"")
		TEST(test.deserialize_json("\"\"") == "")
	}
	
	{
		test = "test lawl";
		TEST(test.is_string())
		TEST(test == "test lawl");
		TEST(test.serialize_json() == "\"test lawl\"")
		TEST(test.deserialize_json("\"test lawl\"") == "test lawl")
	}
	
	{
		test[0] = 5;
		test[1] = 2.3;
		test[2] = "test lawl";
		TEST(test.size() == 3)
		test[5] = 99;
		TEST(test.size() == 6)
		TEST(test.serialize_json() == "[5,2.3,\"test lawl\",null,null,99]")
	}
	
	{
		test["1"] = 5;
		test["test"] = "lawl";
		test["foo"] = "bar";
		TEST(test.size() == 3)
	}
	
	{
		test = "500";
		TEST(test.as_integer() == 500)
		test = "2.125";
		TEST(test.as_floating() == 2.125)
	}
	
	{
		TEST(aeon::deserialize_json("null") == aeon {})
		TEST(aeon::deserialize_json("false") == false)
		TEST(aeon::deserialize_json("true") == true)
		TEST(aeon::deserialize_json("-45623") == -45623)
		TEST(aeon::deserialize_json("12.56") == 12.56)
		TEST(aeon::deserialize_json("12.5e+3") == 12500)
		TEST(aeon::deserialize_json("\"TEST\"") == "TEST")
		TEST(aeon::deserialize_json("\"\\nwat\\\"\\\\\"") == "\nwat\"\\")
		//TEST(aeon::deserialize_json("\"<\\u0047>\"") == "<G>") // FIXME
	}
	
	{
		test = aeon::deserialize_json("   [  40, 20.5, null  \n\n  , \"FOO\"\n\n , [5,    \r\n \"bar\", \"22.22\"]]   \r\n");
		auto const & testc = std::as_const(test);
		TEST(testc[0] == 40)
		TEST(testc[1] == 20.5)
		TEST(testc[2] == aeon {})
		TEST(testc[3] == "FOO")
		TEST(testc[4][0] == 5)
		TEST(testc[4][1] == "bar")
		TEST(testc[4][2].as_floating() == 22.22)
		TEST(testc == aeon::deserialize_json(testc.serialize_json()))
	}
	
	{
		test = aeon::deserialize_json("{\"test\":\"lawl\",\"num\":700,   \"ary\"  :   [null, null, 92]   }");
		auto const & testc = std::as_const(test);
		TEST(testc[5][2][2][9] == aeon {})
		TEST(testc["test"] == "lawl")
		TEST(testc["num"] == 700)
		TEST(testc["ary"][2] == 92)
		TEST(testc["num"].as_string() == "700")
		TEST(testc == aeon::deserialize_json(testc.serialize_json()))
	}
	
	// BRUTE FORCE SEGFAULT TESTING
	{
		constexpr char gen_chars [] = {"abcdefg0123456789\"\r\n ,:.[][][][][][]{}{}{}{}{}{}{}{}{}{}{}"};
		for (size_t i = 0; i < 1000000; i++) {
			std::array<char, 32> str;
			for (size_t j = 0; j < 32; j++) {
				str[j] = gen_chars[rndnum<size_t>(0, sizeof(gen_chars) - 1)];
			}
			try {
				auto meh = aeon::deserialize_json(std::string_view(str.begin(), 32));
			} catch (...) {}
		}
	}
	
	test.array() = {4, 4, 3};
}
