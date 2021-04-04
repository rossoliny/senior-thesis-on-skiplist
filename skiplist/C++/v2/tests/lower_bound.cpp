//
// Created by Isa on 4/2/21.
//

#include "test_utils.h"

#define tag "[lower_bound]"

TEST_CASE("lower bound search", tag)
{
	SECTION("not found at all")
	{
		my_map<string, int> const act;
		std_map<string, int> const exp;

		auto it1 = act.lower_bound("lower_bound");
		auto it2 = exp.lower_bound("lower_bound");

		REQUIRE(it1 == act.end());
		REQUIRE(it2 == exp.end());
		MAPS_REQUIRE_EQUAL(act, exp);
	}
	SECTION("found next")
	{
		my_map<string, int> const act(vec_pairs.begin(), vec_pairs.end());
		std_map<string, int> const exp(vec_pairs.begin(), vec_pairs.end());

		auto it1 = act.lower_bound("abb");
		auto it2 = exp.lower_bound("abb");

		REQUIRE(*it1 == vec_pairs[3]);
		REQUIRE(*it1 == *it2);
		MAPS_REQUIRE_EQUAL(act, exp);
	}
	SECTION("found exact")
	{
		my_map<string, int> const act(vec_pairs.begin(), vec_pairs.end());
		std_map<string, int> const exp(vec_pairs.begin(), vec_pairs.end());

		auto it1 = act.lower_bound("ab");
		auto it2 = exp.lower_bound("ab");

		REQUIRE(*it1 == *it2);
		MAPS_REQUIRE_EQUAL(act, exp);
	}
}