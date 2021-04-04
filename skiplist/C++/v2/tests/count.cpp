//
// Created by Isa on 4/2/21.
//

#include "test_utils.h"

#define tag "[count]"

TEST_CASE("count", tag)
{
	SECTION("count exists")
	{
		my_map<int, string> const act = init_list_L;
		std_map<int, string> const exp = init_list_L;

		size_t count1 = act.count(init_list_L.begin()->first);
		size_t count2 = exp.count(init_list_L.begin()->first);

		REQUIRE(count1 == count2);
	}
	SECTION("count not exists")
	{
		my_map<int, string> const act = init_list_L;
		std_map<int, string> const exp = init_list_L;

		size_t count1 = act.count(-1);
		size_t count2 = exp.count(-1);

		REQUIRE(count1 == count2);
	}
	SECTION("count not exists empty")
	{
		my_map<int, string> const act;
		std_map<int, string> const exp;

		size_t count1 = act.count(rand());
		size_t count2 = exp.count(rand());

		REQUIRE(count1 == count2);
	}
}