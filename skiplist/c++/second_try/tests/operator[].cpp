//
// Created by Isa on 4/2/21.
//

#include "test_utils.h"

#define tag "[operator[]]"

TEST_CASE("operator[]", tag)
{
	SECTION("find arg rvalue")
	{
		my_map<string, string> act;
		std_map<string, string> exp;

		string key1 = "skiplist";
		string key2 = key1;
		auto r1 = act[std::move(key1)];
		auto r2 = exp[std::move(key2)];

		REQUIRE(key1 == key2);
		REQUIRE(r1 == r2);

		MAPS_REQUIRE_EQUAL(act, exp);
	}
	SECTION("find arg lvalue")
	{
		my_map<string, string> act;
		std_map<string, string> exp;

		string key = "skiplist";

		auto r1 = act[key];
		auto r2 = exp[key];

		REQUIRE(key == "skiplist");
		REQUIRE(r1 == r2);

		MAPS_REQUIRE_EQUAL(act, exp);
	}

}